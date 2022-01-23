#ifndef GeoFeatureParser_H
#define GeoFeatureParser_H

#include <QObject>
#include <optional>

#include "GeoItem.h"

class GeoFeatureParser : public QObject
{
    Q_OBJECT
public:
    GeoFeatureParser(const QString& filePath, QObject* parent = nullptr);
    ~GeoFeatureParser() = default;

    Q_PROPERTY(QString      filePath    READ filePath       WRITE setFilePath   NOTIFY filePathChanged)
    Q_PROPERTY(QStringList  geoFeatures READ geoFeatures                        NOTIFY geoFeaturesChanged)

    Q_INVOKABLE void sortByType();
    Q_INVOKABLE void sortByName();
    Q_INVOKABLE void sortByLocation();

    QString filePath() { return _filePath; }
    void setFilePath(const QString& filePath);

    QStringList geoFeatures() { return _geoFeatures; }

signals:
    void filePathChanged();
    void geoFeaturesChanged();

private:
    void _parseGeoFeatureCSVFile();
    void _printGeoFeatures();

    QPointF _parsePoint(const QString &string);
    QVector<QPointF> _parsePoints(const QString &geometryString);
    std::optional<GeoItem> _parseGeoItem(const QByteArray &line);


    QString             _filePath;
    QStringList         _geoFeatures;
};

#endif // GeoFeatureParser_H
