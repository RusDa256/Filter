#pragma once
#include <QCoreApplication>
#include <QImage>
#include"Filter.h"
using namespace std;

class Kernel
{
protected:

    unique_ptr<float[]> data;
    size_t radius;

    size_t getLen() const
    {
        return getSize() * getSize();
    }

public:

    Kernel(size_t _radius)
    {
        radius = _radius;
        data = make_unique<float[]>(getLen());
    }

    Kernel(const Kernel& other) : Kernel(other.radius)
    {
        copy(other.data.get(), other.data.get() + getLen(), data.get());
    }

    size_t getRadius() const { return radius; }
    size_t getSize() const { return 2 * radius + 1; }
    float operator[](size_t id) const { return data[id]; }
    float& operator[](size_t id) { return data[id]; }
};


class MatrixFilter : public Filter
{
protected:
    Kernel mKernel;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
    MatrixFilter(const Kernel& kernel) : mKernel(kernel) {};
    virtual ~MatrixFilter() = default;
};


QColor MatrixFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    int size = mKernel.getSize();
    int radius = mKernel.getRadius();
    for (int i = -radius; i <= radius; i++)
    {
        for (int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;
            QColor color = img.pixelColor(myclamp(x + j, img.width() - 1, 0), myclamp(y + i, img.height() - 1, 0));

            returnR += color.red() * mKernel[idx];
            returnG += color.green() * mKernel[idx];
            returnB += color.blue() * mKernel[idx];
        }
    }

    return QColor(myclamp(abs(returnR), 255.f, 0.f), myclamp(returnG, 255.f, 0.f), myclamp(returnB, 255.f, 0.f));
}