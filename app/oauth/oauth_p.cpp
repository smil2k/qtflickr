#include "oauth_p.h"
#include "oauth.h"
#include "oauth_helper.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>

#include <QDebug>

OAuthPrivate::OAuthPrivate(OAuth * publicAPI) : multipartBound(QString("---------------------------") + Helper::identifier(24)), q_ptr(publicAPI)
{
    nam = new QNetworkAccessManager(this);
    config = new QSslConfiguration(QSslConfiguration::defaultConfiguration());

/*    QList<QSslCertificate> cert = config->caCertificates();
    cert.append(QSslCertificate::fromPath(":/cacert.pem"));
    config->setCaCertificates(cert);*/
}

QString OAuthPrivate::paramsString(const Params &params){

    QString paramStr;
    Params::const_iterator i = params.constBegin();
    while (i != params.constEnd()){
        paramStr += QUrl::toPercentEncoding(i.key()) + "=" + QUrl::toPercentEncoding(i.value().toString()) + "&";
        ++i;
    }
    return paramStr.mid(0, paramStr.length() - 1);
}

QString OAuthPrivate::baseString(const QString & method, const QString & url, const QString & paramStr){
    QString baseStr = method.toUpper() + "&" + QUrl::toPercentEncoding(url);
    baseStr += "&" + QUrl::toPercentEncoding(paramStr);
    return baseStr;
}

QString OAuthPrivate::signingKey(const QString & consumerSecretKey, const QString & oauthTokenSecretKey){
    return QUrl::toPercentEncoding(consumerSecretKey) + "&" + QUrl::toPercentEncoding(oauthTokenSecretKey);
}

QString OAuthPrivate::signature(const QString signingKeyStr, const QString & baseStr){
    return Helper::hmacSha1Base64(signingKeyStr, baseStr);
}

QString OAuthPrivate::buildAuthHeader(const QString & method, const QString & url, const Params & data, const QString & verifier)
{
    Params params;

    params.insert("oauth_consumer_key", consumerKey.toLocal8Bit());
    params.insert("oauth_nonce", Helper::identifier(42));
    params.insert("oauth_signature_method", "HMAC-SHA1");
    params.insert("oauth_timestamp", QString("%1").arg(Helper::timestamp()));
    params.insert("oauth_version", "1.0");

    if(!verifier.isEmpty())
        params.insert("oauth_verifier", verifier);

    if(!oauthToken.isEmpty())
        params.insert("oauth_token", oauthToken.toLocal8Bit());

    if(!callbackUrl.isEmpty())
        params.insert("oauth_callback", callbackUrl.toLocal8Bit());


    params.unite(data);

    QString authStr;
    Params::const_iterator i = params.constBegin();

    params.insert("oauth_signature", signature(signingKey(consumerSecret, oauthTokenSecret), baseString(method, url, paramsString(params))));

    while (i != params.constEnd()){
        authStr += QUrl::toPercentEncoding(i.key()) + "=\"" + QUrl::toPercentEncoding(i.value().toString()) + "\", ";
        ++i;
    }

    return authStr.mid(0, authStr.length() - 2);
}

QPair<QNetworkRequest, QString> OAuthPrivate::buildRequest(const QString & method, const QString & url, const QString authStr, const Params & data)
{
    QNetworkRequest request;
    request.setUrl(url);

    Params::const_iterator j = data.constBegin();

    QString bodyStr;

    while (j != data.constEnd()){
        bodyStr += "" + QUrl::toPercentEncoding(j.key()) + "=" + QUrl::toPercentEncoding(j.value().toString()) + "&";
        ++j;
    }

    if(method.toUpper() == "POST" || method.toUpper()== "PUT"){
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        request.setRawHeader("Content-Length", QString("%1").arg(bodyStr.mid(0, bodyStr.length() - 1).toLocal8Bit().length()).toLocal8Bit());
    }else{
        if(bodyStr.length() > 0)
            request.setUrl(url + "?" + bodyStr.mid(0, bodyStr.length() - 1));
        else
            request.setUrl(url);
    }

    request.setRawHeader("Authorization",("OAuth " + authStr.toLocal8Bit()));
    return QPair<QNetworkRequest, QString>(request, bodyStr.mid(0, bodyStr.length() - 1));
}

void OAuthPrivate::requestToken(const QString & method, const QString & url, const Params & params ){

    QNetworkRequest request = buildRequest(method, url, buildAuthHeader(method, url, params)).first;
    request.setAttribute(QNetworkRequest::User, RequestToken);

    QNetworkReply * reply;
    if (method.toUpper() == "GET") {
        reply = nam->get(request);
    } else {
        reply = nam->post(request, QByteArray());
    }

    reply->setSslConfiguration(* config);
    connect(reply, SIGNAL(finished()), SLOT(reply()));
}


void OAuthPrivate::accessToken(const QString & method, const QString & url, const QString & verifier){

    QNetworkRequest request = buildRequest(method, url, buildAuthHeader(method, url, Params(), verifier)).first;
    request.setAttribute(QNetworkRequest::User, AccessToken);

    QNetworkReply * reply;
    if (method.toUpper() == "GET") {
        reply = nam->get(request);
    } else {
        reply = nam->post(request, QByteArray());
    }

    reply->setSslConfiguration(* config);
    connect(reply, SIGNAL(finished()), SLOT(reply()));
}

void OAuthPrivate::resource(const QString &url, const QString &method, const Params &params, const QByteArray &rawParams)
{

    qDebug() << "header";

    QPair<QNetworkRequest, QString> r = buildRequest(method, url, buildAuthHeader(method, url, params), params);
    QNetworkRequest request = r.first;
    request.setAttribute(QNetworkRequest::User, Resource);

    QNetworkReply *oauthReply = 0;

    if(method.toUpper() == "POST"){

        if(rawParams.length()){
            if(rawParams.contains("Content-Disposition")){
                request.setRawHeader(QString("Content-Type").toLocal8Bit(),QString("multipart/form-data; boundary=" + multipartBound).toLocal8Bit());
            }

            request.setRawHeader(QString("Content-Length").toLocal8Bit(), QString::number(rawParams.length()).toLocal8Bit());
            oauthReply = nam->post(request, rawParams);

        }else{
            oauthReply = nam->post(request, r.second.toLocal8Bit());
        }

    }else if(method.toUpper() == "PUT"){
        oauthReply = nam->put(request, r.second.toLocal8Bit());
    }else if(method.toUpper() == "DELETE"){
        oauthReply = nam->deleteResource(request);
    }else{
        oauthReply = nam->get(request);
    }

    if(oauthReply){
        oauthReply->setSslConfiguration(* config);
        connect(oauthReply, SIGNAL(finished()), SLOT(reply()));
    }
}


void OAuthPrivate::reply(){
    QNetworkReply * r = qobject_cast<QNetworkReply * >(sender());
    QString data = r->readAll();

    qDebug() << "data: " << data;
    qDebug() << "error: " << r->error();
    qDebug() << "errorstring: " << r->errorString();

    Q_Q(OAuth);

    if(r->error() == QNetworkReply::NoError){
        if(r->request().attribute(QNetworkRequest::User).toInt() == RequestToken){
            emit q->requestTokenReceived(Helper::qsToMap(data), data);
        }else if(r->request().attribute(QNetworkRequest::User).toInt() == AccessToken){
            emit q->accessTokenReceived(Helper::qsToMap(data), data);
        }else{
            emit q->resourceReceived(data);
        }
    }else{
        emit q->errorOccurred(data);
    }

    r->deleteLater();
 }
