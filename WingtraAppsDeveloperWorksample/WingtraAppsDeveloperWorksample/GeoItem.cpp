#include <QGeoPolygon>
#include <QGeoPath>

#include "GeoItem.h"

QPointF GeoItem::_parsePoint(const QString &string)
{
    auto pointCoordinates = string.split(COORDINATE_DELIMITER);
    if (pointCoordinates.count() != 2)
        return QPointF();

    auto x = pointCoordinates[0].trimmed().toDouble();
    auto y = pointCoordinates[1].trimmed().toDouble();
    return QPointF(x, y);
}

QVector<QPointF> GeoItem::_parsePoints(const QString &string)
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

std::optional<GeoItem> GeoItem::parseGeoItem(const QByteArray &line)
{
    QStringList lineChunkStrings;
    QList<QByteArray> lineChunks = line.split(';');
    for (const QByteArray& chunk : lineChunks) {
        lineChunkStrings.append(chunk.trimmed());
    }

    if (lineChunkStrings.count() != COLUMNS)
        return std::nullopt;

    auto it = lineChunkStrings.cbegin();

    QString name = *(it++);
    QString type = *(it++);
    QString color = *(it++);
    QVector<QPointF> points;

    QString geometryString = *(it);

    if (type == TYPE_POINT)
    {
        points.push_back(_parsePoint(geometryString));
    }
    else if (type == TYPE_LINE || type == TYPE_POLYGON)
    {
        points = _parsePoints(geometryString);
    }

    auto item = GeoItem(name, type, color, points);

    return item;
}

QString GeoItem::_pointsToString() const
{
    QStringList pointTexts;

    for (const auto &point : _points)
    {
        pointTexts.append(QString("(%1, %2)").arg(point.x()).arg(point.y()));
    }

    return QString("[%1]").arg(pointTexts.join(", "));
}

// Qt class accepts flipped coordinates.
QGeoCoordinate fixFlippedCoordinate(const QPointF &point)
{
    return QGeoCoordinate(point.y(), point.x());
}

QList<QGeoCoordinate> GeoItem::_getCoordinateList() const
{
    auto list = QList<QGeoCoordinate>();

    for(const auto & point : _points)
    {
        list.push_back(fixFlippedCoordinate(point));
    }

    return list;
}

QGeoCoordinate GeoItem::_getCenter() const
{
    if (_type == TYPE_POINT)
    {
        return QGeoCoordinate(fixFlippedCoordinate(_points[0]));
    }
    else if (_type == TYPE_LINE && _points.count() == 2)
    {
        auto geoLine = QGeoPath(_getCoordinateList());
        return geoLine.center();
    }
    else if (_type == TYPE_POLYGON && _points.count() > 2)
    {
        auto geoPolygon = QGeoPolygon(_getCoordinateList());
        return geoPolygon.center();
    }

    return QGeoCoordinate();
}

