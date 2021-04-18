#pragma once
#include <QCoreApplication>
#include <QImage>


class Filter
{
protected:
    virtual QColor calcNewPixelColor(const QImage& img, int x, int y) const = 0;
public:
    virtual ~Filter() = default;
    virtual QImage process(const QImage& img) const;
};

template <class T>
T myclamp(T value, T max, T min)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

QImage Filter::process(const QImage& img) const
{
    QImage result(img);

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }
    return result;
}