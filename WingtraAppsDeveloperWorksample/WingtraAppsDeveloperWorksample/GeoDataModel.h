#ifndef GEODATAMODEL_H
#define GEODATAMODEL_H

#include <QAbstractTableModel>
#include <QColor>

class GeoDataModel : public QAbstractTableModel
{
public:
    GeoDataModel();
};

struct GeoItem
{
    QString _name;
    QString _type;
    QString _color;
    QVariant _geometry;

    operator QString () const
    {
        return QString("%1, %2, %3").arg(_name, _type, _color);
    }
};

#endif // GEODATAMODEL_H
