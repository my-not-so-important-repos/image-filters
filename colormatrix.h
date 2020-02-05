// created by Heitor Adao Junior
#ifndef COLORMATRIX_H
#define COLORMATRIX_H

#include <QColor>
#include <QPixmap>
#include <QPoint>
#include <QImage>


class ColorMatrix
{
public:
    ColorMatrix();
    ColorMatrix(int order, int *matrix);
    QColor getPixel(const QImage &image, const QPoint &point) const;
    void setWeights(int order, int *matrix);
private:
    int order;
    int *weight;
    int sumWeights;
};

#endif // COLORMATRIX_H
