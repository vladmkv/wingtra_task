#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QDebug>

#include "GeoFeatureParser.h"

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    QFile csvResourceFile("://GeoFeatures.csv");
    QString absoluteFilePath = QCoreApplication::applicationDirPath() + "/GeoFeatures.csv";
    if (!csvResourceFile.copy(absoluteFilePath)) {
        qDebug() << "Can't copy to" << absoluteFilePath << csvResourceFile.errorString();
    }
    GeoFeatureParser* geoFeatureParser = new GeoFeatureParser(absoluteFilePath);
    engine.rootContext()->setContextProperty("geoFeatureParser", geoFeatureParser);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
    &app, [url](QObject * obj, const QUrl & objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
