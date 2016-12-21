#ifndef CRYPTOHELPER_H
#define CRYPTOHELPER_H

#include <QString>
#include <QChar>
#include <QList>
#include <QTime>
#include <QStringList>
#include <QCryptographicHash>
#include <QQmlEngine>
#include <qmath.h>

#include <QObject>

class CryptoHelper : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY(CryptoHelper)
    CryptoHelper() {}

public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new CryptoHelper();
    }

    Q_INVOKABLE QString hmacSha1Base64(const QString & key, const QString & data);

signals:

public slots:
};

#endif // CRYPTOHELPER_H
