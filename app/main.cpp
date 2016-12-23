#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebView/QtWebView>

#include "oauth/oauth.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtWebView::initialize();

    QQmlApplicationEngine engine;

    qmlRegisterType<OAuth>("Social", 1, 0, "OAuth");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
