#include <QDebug>
#include <QFile>
#include <QColor>
#include <QLineF>
#include <QPolygonF>

#include <optional>

#include "GeoFeatureParser.h"
#include "GeoDataModel.h"

GeoFeatureParser::GeoFeatureParser(const QString& filePath, QObject *parent)
    : QObject(parent)
    , _filePath(filePath)
{
    connect(this, &GeoFeatureParser::filePathChanged, this, &GeoFeatureParser::_parseGeoFeatureCSVFile);

    _parseGeoFeatureCSVFile();
}


void GeoFeatureParser::setFilePath(const QString &filePath)
{
    if (_filePath != filePath) {
        _filePath = filePath;
        qDebug() << "Geographic features file parser path set to" << _filePath;
        emit filePathChanged();
    }
}

QPointF GeoFeatureParser::_parsePoint(const QString &string)
{
    auto pointCoordinates = string.split(COORDINATE_DELIMITER);
    if (pointCoordinates.count() != 2)
        return QPointF();

    auto x = pointCoordinates[0].trimmed().toDouble();
    auto y = pointCoordinates[1].trimmed().toDouble();
    return QPointF(x, y);
}

QVector<QPointF> GeoFeatureParser::_parsePoints(const QString &string)
{
    auto pointStrings = string.split(POINT_DELIMITER);
    auto points = QVector<QPointF>();

    for (auto &pointString : pointStrings)
    {
        auto point = _parsePoint(pointString);

        if (!point.isNull())
        {
            points.push_back(point);
        }
    }

    return points;
}

std::optional<GeoItem> GeoFeatureParser::_parseGeoItem(const QByteArray &line)
{
    QStringList lineChunkStrings;
    QList<QByteArray> lineChunks = line.split(';');
    for (const QByteArray& chunk : lineChunks) {
        lineChunkStrings.append(chunk.trimmed());
    }

    if (lineChunkStrings.count() != COLUMNS)
        return std::nullopt;

    auto it = lineChunkStrings.cbegin();

    GeoItem item;
    item._name = *(it++);
    item._type = *(it++);
    item._color = *(it++);

    QString geometryString = *(it);
    if (item._type == TYPE_POINT)
    {
        auto point = _parsePoint(geometryString);

        if (point.isNull())
            return std::nullopt;

        item._geometry = QVariant(point);
    }
    else if (item._type == TYPE_LINE || item._type == TYPE_POLYGON)
    {
        auto points = _parsePoints(geometryString);

        if (item._type == TYPE_LINE)
        {
            if (points.count() != 2)
            {
                return std::nullopt;
            }

            auto line = QLineF(points[0], points[1]);

            item._geometry = QVariant(line);
        }
        else if (item._type == TYPE_POLYGON)
        {
            if (points.count() < 3)
            {
                return std::nullopt;
            }

            auto polygon = QPolygonF(points);

            item._geometry = QVariant(polygon);
        }
    }

    return item;
}

void GeoFeatureParser::_parseGeoFeatureCSVFile()
{
    // TODO 1: parse a CSV file and extract all contained geographic features

    // Read in the csv file from the path saved in _filePath using either Qt functionality or pure C++
    // Tip: if you prefer working with pure C++:
    // use _filePath.toStdString() to get an std::string from a QString

    // Each line contains a geographic feature, which is in the shape of either a polygon, a point or a line
    // Apart from this shape type, each geographic feature also has a name, a color and of course
    // coordinates describing its shape

    // Find all geographic features contained in the CSV file and save them to an appropriate data structure  with all
    // their corresponding properties for later manipulations like sorting

    // At the end fill _geoFeatures with strings of the form "Name, shape type, color, [(longitude, latitude),..., (longitude, latitude)]",
    // e.g. "TestLine1, Line, red, [(48.5677, 8.5678), (48.5678, 8.5668)]
    // Tip: if you prefer working with pure C++:
    // You can use _geoFeatures.clear() to empty the _geoFeatures list and start filling it again.
    // Then you can use QString::fromStdString() to get a QString type from an std::string and finally
    // _geoFeatures.append() to append all the QStrings to the list

    qDebug() << "Parsing file" << _filePath;

    _geoFeatures.clear();

    QFile file(_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }

    QVector<QStringList> lineList;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();

        if (line.startsWith('#'))
            continue;

        auto item = _parseGeoItem(line);

        if (item)
        {
            _geoFeatures.append(*item);
        }
    }

    _printGeoFeatures();
    emit geoFeaturesChanged();
}

void GeoFeatureParser::_printGeoFeatures()
{
    qDebug() << "Parsed geographic features:";
    for (const QString& geoFeature : _geoFeatures) {
        qDebug() << geoFeature;
    }
}

// This function is called by a button in the UI.
void GeoFeatureParser::sortByName()
{
    // TODO 2: sort all geographic features by name in acending order
    // In the end fill _geoFeatures again with strings of the form "Name, shape type, color, [(longitude, latitude),..., (longitude, latitude)]",
    // in the correct order

    _printGeoFeatures();
    emit geoFeaturesChanged();
}

// This function is called by a button in the UI.
void GeoFeatureParser::sortByType()
{
    // TODO 3: sort all geographic features by type, i.e. in the order line, polygon, then point
    // In the end fill _geoFeatures again with strings of the form "Name, shape type, color, [(longitude, latitude),..., (longitude, latitude)]",
    // in the correct order

    _printGeoFeatures();
    emit geoFeaturesChanged();
}

// This function is called by a button in the UI.
void GeoFeatureParser::sortByLocation()
{
    // TODO 4: for all geographic features, first compute the distance to Zurich (8.5392, 47.3686).
    // Then sort all geographic features by location, starting with the feature closest to Zurich (8.5392, 47.3686) and ending with the one furthest away
    // This time, in the end fill _geoFeatures with strings of the form "Name, shape type, color, distanceToZurich [in km] [(longitude, latitude),..., (longitude, latitude)]",
    // e.g. "TestLine1, Line, red, 10.34 km, [(48.5677, 8.5678), (48.5678, 8.5668)]
    // in the correct order, i.e. sorted by location

    _printGeoFeatures();
    emit geoFeaturesChanged();
}
