#pragma once
#include <QCoreApplication>
#include <QImage>
#include"Filter.h"

using namespace std;

class InvertFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

QColor InvertFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
    return color;
}


class GrayScaleFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const
    {
        QColor color = img.pixelColor(x, y);
        float clr = color.red() * 0.299 + 0.587 * color.green() + 0.144 * color.blue();
        color.setRgb(clr, clr, clr);
        return color;
    }
};

class SepiaFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const
    {
        int k = 25;
        QColor color = img.pixelColor(x, y);
        color.setRgb(myclamp<float>(color.red() + 2 * k, 255.f, 0.f), myclamp<float>(color.green() + 0.5 * k, 255.f, 0.f), myclamp<float>(color.blue() - k, 255.f, 0.f));
        return color;
    }
};

class BrightnessFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const
    {
        int k = 50;
        QColor color = img.pixelColor(x, y);
        color.setRgb(myclamp<float>(color.red() + k, 255.f, 0.f), myclamp<float>(color.green() + k, 255.f, 0.f), myclamp<float>(color.blue() + k, 255.f, 0.f));
        return color;
    }
};


class Otr : public Filter
{
public:
    int MaxR = 0;
    int MaxG = 0;
    int MaxB = 0;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
    void GetMax(const QImage& img);
    QImage process(const QImage& img);
};

QColor Otr::calcNewPixelColor(const QImage& img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    color.setRgb(myclamp<float>(color.red() * 255 / MaxR, 255.f, 0.f), myclamp<float>(color.green() * 255 / MaxG, 255.f, 0.f), myclamp<float>(color.blue() * 255 / MaxB, 255.f, 0.f));
    return color;
}

void Otr::GetMax(const QImage& img)
{
    for (int i = 0; i < img.height(); i++)
    {
        for (int j = 0; j < img.width(); j++)
        {
            QColor color = img.pixelColor(j, i);

            if (color.red() > MaxR)
            {
                MaxR = color.red();
            }
            if (color.green() > MaxG)
            {
                MaxG = color.green();
            }
            if (color.blue() > MaxB)
            {
                MaxB = color.blue();
            }
        }
    }
}

QImage Otr::process(const QImage& img)
{
    QImage result(img);
    GetMax(img);
    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }
    return result;
}


class TurnFilter : public Filter
{
public:

    int t;
    TurnFilter(int _t = 0)
    {
        t = _t;
    }

    QColor calcNewPixelColor(const QImage& img, int x, int y) const
    {
        QColor color = img.pixelColor(x + t, y);
        color.setRgb(myclamp<float>(color.red(), 255.f, 0.f), myclamp<float>(color.green(), 255.f, 0.f), myclamp<float>(color.blue(), 255.f, 0.f));
        return color;
    }
};

class GlassFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const
    {
        QColor color = img.pixelColor(x, y);
        color.setRgb(myclamp<float>(color.red(), 255.f, 0.f), myclamp<float>(color.green(), 255.f, 0.f), myclamp<float>(color.blue(), 255.f, 0.f));
        return color;
    }
public:

    QImage process_glass(const QImage& img)
    {
        QImage result(img);

        for (int x = 0; x < img.width(); x++)
            for (int y = 0; y < img.height(); y++)
            {
                QColor color = calcNewPixelColor(img, x, y);
                result.setPixelColor(x + (rand() % 1 - 0.5) * 10, y + (rand() % 1 - 0.5) * 10, color);
            }
        return result;
    }
};


class LinearCorrectionFilter : public Filter
{
protected:
    int mutable max = 0;
    int mutable min = 255;

    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;

public:
    QImage process(const QImage& img) const override;
};

QColor LinearCorrectionFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    int val = 0;
    if (max - min)
        val = (color.value() - min) * (255.0 / (max - min));
    return QColor::fromHsv(color.hue(), color.saturation(), val);
}

QImage LinearCorrectionFilter::process(const QImage& img) const
{
    QImage result(img);

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = img.pixelColor(x, y);

            if (color.value() > max)
                max = color.value();
            if (color.value() < min)
                min = color.value();
        }

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }
    return result;
}


class MedianFilter : public Filter
{
protected:
    int radius;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
    MedianFilter(int _r) : radius(_r) {}
};

QColor MedianFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    int returnR = 0;
    int returnG = 0;
    int returnB = 0;
    short int size = 2 * radius + 1;

    short int** data = new short int* [3];
    for (int i = 0; i < 3; i++)
        data[i] = new short int[size * size];


    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;
            data[0][idx] = img.pixelColor(myclamp<float>(x + j, img.width() - 1, 0), myclamp<float>(y + i, img.height() - 1, 0)).red();
            data[1][idx] = img.pixelColor(myclamp<float>(x + j, img.width() - 1, 0), myclamp<float>(y + i, img.height() - 1, 0)).green();
            data[2][idx] = img.pixelColor(myclamp<float>(x + j, img.width() - 1, 0), myclamp<float>(y + i, img.height() - 1, 0)).blue();
        }

    sort(data[0], data[0] + size * size);
    returnR = data[0][(size * size - 1) / 2];

    sort(data[1], data[1] + size * size);
    returnG = data[1][(size * size - 1) / 2];

    sort(data[2], data[2] + size * size);
    returnB = data[2][(size * size - 1) / 2];

    return QColor(
        myclamp<float>(returnR, 255, 0),
        myclamp<float>(returnG, 255, 0),
        myclamp<float>(returnB, 255, 0));
}