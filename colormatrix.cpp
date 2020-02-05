// created by Heitor Adao Junior
#include "colormatrix.h"
#include <cmath>
using std::floor;

ColorMatrix::ColorMatrix()
    :order(0)
    ,sumWeights(0)
    ,weight(0)
{
    int weightLowPass[9] = {1,2,1,
                            2,5,2,
                            1,2,1};
    setWeights(3, weightLowPass);
    Q_ASSERT(sumWeights > 0);
}

ColorMatrix::ColorMatrix(int order, int *matrix)
    :order(0)
    ,sumWeights(0)
    ,weight(0)
{
    setWeights(order, matrix);
    Q_ASSERT(sumWeights > 0);
}

QColor ColorMatrix::getPixel(const QImage &image, const QPoint &point) const
{
    QColor *colorComponents = new QColor[order * order];

#if 1
    for (int y = 0; y < order; ++y) {
        int y2 = point.y() + y - floor(order / 2.0);
        for (int x = 0; x < order; ++x) {
            int x2 = point.x() + x - floor(order / 2.0);           // no bug because
            colorComponents[y * order + x] = image.pixel(x2, y2); // <= this method
        }                                                        // take care of boundaries of image
    }
#else
    colorComponents[0] = image.pixel(point.x()-1, point.y()-1);
    colorComponents[1] = image.pixel(point.x()  , point.y()-1);
    colorComponents[2] = image.pixel(point.x()+1, point.y()-1);

    colorComponents[3] = image.pixel(point.x()-1, point.y());
    colorComponents[4] = image.pixel(point.x()  , point.y());
    colorComponents[5] = image.pixel(point.x()+1, point.y());

    colorComponents[6] = image.pixel(point.x()-1, point.y()+1);
    colorComponents[7] = image.pixel(point.x()  , point.y()+1);
    colorComponents[8] = image.pixel(point.x()+1, point.y()+1);
#endif

    int red = 0;
    int green = 0;
    int blue = 0;
    for (int i = 0; i < order * order; ++i) {
        red += colorComponents[i].red() * weight[i];
        green += colorComponents[i].green() * weight[i];
        blue += colorComponents[i].blue() * weight[i];
    }
    delete colorComponents;
    colorComponents = NULL;
    Q_ASSERT(sumWeights);
    red /= sumWeights;
    green /= sumWeights;
    blue /= sumWeights;

    return QColor(red, green, blue);
}

void ColorMatrix::setWeights(int order, int *matrix)
{
    // check if order is odd and size matches order
    if (order % 2) {
        //if (sizeof(*matrix) >= sizeof(int) * order * order) {
        if (matrix) {
            this->order = order;
            sumWeights = 0;
            delete weight;
            weight = new int[order * order];
            for (int i = 0; i < order * order; ++i) {
                weight[i] = matrix[i];
                sumWeights += weight[i];
            }
        }
    }
}
