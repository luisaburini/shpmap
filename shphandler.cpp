#include "shphandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <string.h>
#include<math.h>
#include <errno.h>
#include <inttypes.h>
#include <QFile>

struct polygon {
    double box[4];
    int32_t numParts, numPoints;
    int32_t *parts;
    double *points_x;
    double *points_y;
};


SHPHandler::SHPHandler(QObject *parent):
    QObject(parent)
{
}

int SHPHandler::indexPoints(){
    return m_indexPoints;
}

int SHPHandler::numParts(){
    return m_numParts;
}

int SHPHandler::indexParts(){
    return m_indexParts;
}

int SHPHandler::numPoints(){
    m_numPoints = parts.at(indexParts());
    return m_numPoints;
}

int SHPHandler::totalNumPoints(){
    return m_totalNumPoints;
}

double SHPHandler::latitudeY(){
    m_latitudeY = yPoints.at(indexPoints());
    return m_latitudeY;
}

double SHPHandler::longitudeX(){
    m_longitudeX = xPoints.at(indexPoints());
    return m_longitudeX;
}

double SHPHandler::xmin(){
    if(bbox.empty())
        parseShapefileHeader();
    m_xmin = bbox.at(0);
    return m_xmin;
}

double SHPHandler::ymin(){
    if(bbox.empty())
        parseShapefileHeader();
    m_ymin = bbox.at(1);
    return m_ymin;
}

double SHPHandler::width(){
    if(bbox.empty())
        parseShapefileHeader();
    m_width = abs(round(xmin() + abs(bbox.at(2))));
    return m_width+100;
}

double SHPHandler::height(){
    if(bbox.empty())
        parseShapefileHeader();
    m_height = (round(abs(bbox.at(3)) - ymin() )) ;
    return m_height+100;
}

void SHPHandler::setTotalNumPoints(int totalNumPoints){
    if(m_totalNumPoints == totalNumPoints)
        return;
    m_totalNumPoints = totalNumPoints;
    emit totalNumPointsChanged();
}

void SHPHandler::setIndexPoints(int indexPoints){
    if(m_indexPoints == indexPoints)
        return;
    m_indexPoints = indexPoints;
    emit indexPointsChanged();
}

void SHPHandler::setIndexParts(int indexParts){
    if(indexParts == m_indexParts)
        return;
    m_indexParts = indexParts;
    emit indexPartsChanged();
}

void SHPHandler::setLatitudeY(double latitude){
    if(latitude == m_latitudeY)
        return;
    m_latitudeY = latitude;
    emit latitudeYChanged();
}

void SHPHandler::setNumParts(int numParts){
    if(numParts == m_numParts)
        return;
    m_numParts = numParts;
    emit numPartsChanged();
}

void SHPHandler::setNumPoints(int numPoints){
    if(numPoints == m_numPoints)
        return;
    m_numPoints = numPoints;
    emit numPointsChanged();
}

void SHPHandler::setLongitudeX(double longitude){
    if(longitude == m_longitudeX)
        return;
    m_longitudeX = longitude;
    emit longitudeXChanged();

}

void SHPHandler::setWidth(double width){
    if(width == m_width)
        return;
    m_width = width;
    emit widthChanged();
}

void SHPHandler::setXmin(double xmin){
    if(xmin == m_xmin)
        return;
    m_xmin = xmin;
    emit xminChanged();
}

void SHPHandler::setHeight(double height){
    if(height == m_height)
        return;
    m_height = height;
    emit heightChanged();
}

void SHPHandler::setYmin(double ymin){
    if(ymin == m_ymin)
        return;
    m_ymin = ymin;
    emit yminChanged();
}

void SHPHandler::parseShapefileHeader(){

    int32_t header_begin[9];
    double header_end[8];
    int32_t prec_num = 0;

    //fp = fopen ("/home/lburini/python-tutorials/Shapefiles/PRY_adm/PRY_adm0.shp", "rb");
    //fp = fopen ("/home/root/PRY_adm0.shp", "rb");
    //fp = fopen ("/home/root/BRA_adm0.shp", "rb");
    fp = fopen ("/home/lburini/qt-tutorials/SHPMap/shapefile/sample_shapefiles/BRA_adm0.shp", "rb");
    fread(header_begin, sizeof(header_begin), 1, fp);
    fread(header_end, sizeof(header_end), 1, fp);
    // convert endianess
    for(int i=0; i<8; i++)
        header_begin[i] = be32toh(header_begin[i]);

    for(int i=0; i<4; i++)
    {
        bbox.append(header_end[i]);
        qDebug() << bbox[i];
    }

    prec_num = readPolygon(fp);
}

int SHPHandler::readPolygon(FILE *fp){
/*
        - Polygon has not a fixed length
        - Rings are always closed

        Double[4] bounding box
        Integer num parts
        Integer num points
        Integer[num parts] parts
        Integer[num points] points
*/
    //qDebug() << "entrei na readPoly";
    struct polygon *pl = (struct polygon*)malloc(sizeof(struct polygon*));
    pl->parts = (int32_t *) malloc(sizeof(int32_t));
    pl->points_x = (double *) malloc(sizeof(double));
    pl->points_y = (double *) malloc(sizeof(double));

    int i;

    int32_t record_number, content_length;
    int32_t shape_type;
    double *xycoord;

    printf("Part index: %" PRIu32 "\n\n", pl->parts[0]);
    fread(&record_number, sizeof(record_number), 1, fp);
    fread(&content_length, sizeof(content_length), 1, fp);

    record_number = be32toh(record_number);
    content_length = be32toh(content_length);

    printf("Record number: %" PRIu32 "\n", record_number);
    printf("Content length: %" PRIu32 "\n", content_length);

    /*if(prev_record_number != (record_number - 1))
        return -1;*/

    printf("Sizeof box: %lu\n", sizeof(pl->box));
    printf("Sizeof numParts: %lu\n", sizeof(pl->numParts));
    printf("Sizeof numPoints: %lu\n", sizeof(pl->numPoints));

    fread(&shape_type, sizeof(shape_type), 1, fp);
    fread(pl->box, sizeof(pl->box), 1, fp);
    fread(&pl->numParts, sizeof(pl->numParts), 1, fp);
    fread(&pl->numPoints, sizeof(pl->numPoints), 1, fp);

    printf("Shape type: %d\n", shape_type);
    printf("Number of parts: %" PRIu32 "\n", pl->numParts);
    printf("Number of points: %" PRIu32 "\n", pl->numPoints);

    pl->parts = (int32_t *) realloc(pl->parts, pl->numParts * sizeof(int32_t));
    printf("Allocated memory to parts: %lu\n", pl->numParts * sizeof(int32_t));
    fread(pl->parts, pl->numParts * sizeof(int32_t), 1, fp);
    for(i = 0; i < pl->numParts; i++){
        //printf("Part[%d] index: %" PRIu32 "\n", i, pl->parts[i]);
        parts.append(pl->parts[i]);
        //qDebug() << "Parts[" << i << "]= " << parts.at(i);
    }
    parts.append(pl->numPoints/2);

    // two times number of points since it is a X,Y pair of coordinates
    xycoord = (double *) malloc(2 * pl->numPoints * sizeof(double));
    pl->points_x = (double *) realloc(pl->points_x, pl->numPoints * sizeof(double));
    pl->points_y = (double *) realloc(pl->points_y, pl->numPoints * sizeof(double));
    printf("Allocated memory to points: %lu\n", 2 * pl->numPoints * sizeof(double));


    fread(xycoord, 2 * pl->numPoints * sizeof(double), 1, fp);
    for(i = 0; i < 2*pl->numPoints; i++){ //split read X,Y in X and Y vectors
        //(i % 2 == 0) ? (pl->points_x[i/2] = xycoord[i]) : (pl->points_y[i/2] = xycoord[i]);
        (i % 2 == 0) ? (xPoints.append((xycoord[i] + 80))) : (yPoints.append((xycoord[i]) + 42.5));
     }
    printf("ESCREVI OS POINTS NO QVECTOR\n");
    qDebug() << xPoints.size();
    qDebug() << yPoints.size();
/*
    QString filename = "/home/lburini/qt-tutorials/SHPMap/coordinatesQVector.txt";
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite)){
        for(i=0; i< 2*pl->numPoints; i++){
            QTextStream stream(&file);
            (i % 2 == 0) ? (stream << "x_lon = array([" << xPoints.at(i/2)-70 << "])\n") : ((stream << "y_lat =array([" << yPoints.at(i/2)-40 << "])\n"));
        }
    }*/

    qDebug() << "SIZE OF QVECTOR<DOUBLE> YPOINTS" << yPoints.size();
    qDebug() << "SIZE OF QVECTOR<DOUBLE> XPOINTS" << xPoints.size();

    setNumParts(pl->numParts);

    qDebug() << "Diretamente do C++: PARTS";
    qDebug() << pl->numParts;

    setTotalNumPoints(pl->numPoints);
    qDebug() << "Diretamente do C++: POINTS";
    qDebug() << totalNumPoints() ;
    //deallocate memory
    free(xycoord);
    xycoord = NULL;

    return record_number;

}
