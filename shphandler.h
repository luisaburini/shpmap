#ifndef SHPHANDLER_H
#define SHPHANDLER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QVector2D>
#include <QDebug>

class SHPHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int totalNumPoints READ totalNumPoints WRITE setTotalNumPoints NOTIFY totalNumPointsChanged)
    Q_PROPERTY(int numPoints READ numPoints WRITE setNumPoints NOTIFY numPointsChanged)
    Q_PROPERTY(int numParts READ numParts WRITE setNumParts NOTIFY numPartsChanged)

    Q_PROPERTY(double latitudeY READ latitudeY WRITE setLatitudeY NOTIFY latitudeYChanged)
    Q_PROPERTY(double longitudeX READ longitudeX WRITE setLongitudeX NOTIFY longitudeXChanged)
    Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(double xmin READ xmin WRITE setXmin NOTIFY xminChanged)
    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(double ymin READ ymin WRITE setYmin NOTIFY yminChanged)

    Q_PROPERTY(int indexPoints READ indexPoints WRITE setIndexPoints NOTIFY indexPointsChanged)
    Q_PROPERTY(int indexParts READ indexParts WRITE setIndexParts NOTIFY indexPartsChanged)


public:
    explicit SHPHandler(QObject * parent=nullptr);

    double latitudeY();
    double longitudeX();
    void setLatitudeY(double latitude);
    void setLongitudeX(double longitude);
    int numParts();
    int numPoints();
    int totalNumPoints();
    int indexParts();
    int indexPoints();
    void setIndexPoints(int indexPoints);
    void setIndexParts(int indexParts);
    FILE *fp;
    struct polygon *pl;
    double width();
    double xmin();
    double height();
    double ymin();
    void setTotalNumPoints(int totalNumPoints);
    void setNumParts(int numParts);
    void setNumPoints(int numPoints);
    void setHeight(double height);
    void setYmin(double ymin);
    void setWidth(double width);
    void setXmin(double xmin);
    QVector<double> parts;
    QVector<double> xPoints;
    QVector<double> yPoints;
    QVector<double> bbox;
    void parseShapefileHeader();
    int readPolygon(FILE *fp);

signals:
    void latitudeYChanged();
    void longitudeXChanged();
    void widthChanged();
    void xminChanged();
    void heightChanged();
    void yminChanged();
    void indexPartsChanged();
    void indexPointsChanged();
    void numPointsChanged();
    void totalNumPointsChanged();
    void numPartsChanged();

private:
    double m_latitudeY;
    double m_longitudeX;
    double m_width;
    double m_xmin;
    double m_height;
    double m_ymin;
    int m_indexPoints;
    int m_indexParts;
    int m_numParts;
    int m_numPoints;
    int m_totalNumPoints;
};

#endif // SHPHANDLER_H
