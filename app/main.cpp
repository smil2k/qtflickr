#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cryptohelper.h"
#include "oauth.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<OAuth>("Social", 1, 0, "OAuth");
    qmlRegisterSingletonType<CryptoHelper>("FlickrHelper", 1, 0, "CryptoHelper", &CryptoHelper::qmlInstance);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
