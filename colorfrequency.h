//created by Heitor Adao Junior
#ifndef COLORFREQUENCY_H
#define COLORFREQUENCY_H

#include <QColor>

class ColorFrequency
{
public:
    ColorFrequency() {}

    ColorFrequency(const QColor &newColor)
        :color(newColor),frequency(1)
    {}

    ColorFrequency(const ColorFrequency &newCF) {
        color = newCF.color;
        frequency = newCF.frequency;
    }

    ColorFrequency operator=(const ColorFrequency &cf) {
        color = cf.color;
        frequency = cf.frequency;
        return *this;
    }
    bool operator==(const ColorFrequency &cf) {
        return cf.color == color;
    }

    QColor color;
    int frequency;
};

bool operator<(const ColorFrequency &CF1, const ColorFrequency &CF2) {
    return CF1.frequency < CF2.frequency;
}

#endif // COLORFREQUENCY_H
