#include <QDebug>
#include <QFile>

#include "GeoFeatureParser.h"
#include "GeoItem.h"

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

    while (!file.atEnd()) {
        QByteArray line = file.readLine();

        if (line.startsWith('#'))
            continue;

        auto item = GeoItem::parseGeoItem(line);

        if (item)
        {
            // Collect string representations
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
