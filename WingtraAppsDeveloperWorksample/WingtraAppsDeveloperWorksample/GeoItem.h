#pragma once

#include <QAbstractTableModel>
#include <QColor>

#include <optional>

class GeoItem
{
public:
    static std::optional<GeoItem> parseGeoItem(const QByteArray &line);

    static inline bool caseInsensitiveNameLessThan(const GeoItem &item1, const GeoItem &item2)
    {
        return item1._name.toLower() < item2._name.toLower();
    }

    static inline bool typeLessThan(const GeoItem &item1, const GeoItem &item2)
    {
        // line, polygon, then point
        static QList<QString> _typeOrder = {TYPE_LINE, TYPE_POLYGON, TYPE_POINT};

        return _typeOrder.indexOf(item1._type) < _typeOrder.indexOf(item2._type);
    }

public:
    GeoItem()
    {
    }

    GeoItem(const QString &name,
        const QString &type,
        const QString &color,
        const QVector<QPointF> &points) :
            _name(name),
            _type(type),
            _color(color),
            _points(points)
    {
    }

    inline operator QString () const
    {
        return QString("%1, %2, %3, %4").arg(_name, _type, _color, pointsToString());
    }

    QVariant createGeometry() const;

private:
    constexpr static const int COLUMNS = 4;
    constexpr static const char TYPE_POINT[] = "point";
    constexpr static const char TYPE_LINE[] = "line";
    constexpr static const char TYPE_POLYGON[] = "polygon";
    constexpr static const char COORDINATE_DELIMITER = ',';
    constexpr static const char POINT_DELIMITER = ' ';
    const static QList<QString> _typeOrder;

private:
    static QPointF _parsePoint(const QString &string);
    static QVector<QPointF> _parsePoints(const QString &string);

    QString pointsToString() const;

private:
    QString _name;
    QString _type;
    QString _color;
    QVariant _geometry;
    QVector<QPointF> _points;
};

