#pragma once
#include <QCoreApplication>
#include <QImage>
#include <string>



class NewMorphoKernel
{
protected:

    QColor calcNewPixelColor(const QImage& img, int x, int y) const;

public:

    int h;
    int w;
    double** arr;

    NewMorphoKernel() = default;
    NewMorphoKernel(int _h, int _w, double** _arr)
    {
        h = _h;
        w = _w;
        arr = new double* [h];

        for (int i = 0; i < h; i++)
        {
            arr[i] = new double[w];

            for (int j = 0; j < w; j++)
            {
                arr[i][j] = _arr[i][j];
            }
        }
    }

    QImage process(const QImage& img) const
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

    int getH() { return h; }
    int getW() { return w; }
};



 /////
 /// Dilation
 /////
class Dilation : public NewMorphoKernel
{
protected:

    QColor calcNewPixelColor(const QImage& img, int x, int y);

public:

    Dilation()
    {
        w = 3;
        h = 3;
        arr = new double* [h];

        arr[0] = new double[w] {0, 1, 0};
        arr[1] = new double[w] {1, 1, 1};
        arr[2] = new double[w] {0, 1, 0};
    }

    Dilation(NewMorphoKernel tmp) : NewMorphoKernel(tmp) {};
};

QColor Dilation::calcNewPixelColor(const QImage& img, int x, int y)
{
    float returnR = 0;
    float returnB = 0;
    float returnG = 0;

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            QColor color = img.pixelColor(myclamp<float>(x + j, img.width() - 1, 0),
                myclamp<float>(y + i, img.height() - 1, 0));

            if (arr[i][j])
            {
                if (color.red() > returnR)
                    returnR = color.red();
                if (color.green() > returnG)
                    returnG = color.green();
                if (color.blue() > returnB)
                    returnB = color.blue();
            }

        }
    return QColor(returnR, returnG, returnB);
}


/////
/// Erosion
/////
class Erosion : public NewMorphoKernel
{
    Erosion()
    {
        w = 3;
        h = 3;
        arr = new double* [h];

        arr[0] = new double[w] {0, 1, 0};
        arr[1] = new double[w] {1, 1, 1};
        arr[2] = new double[w] {0, 1, 0};
    }

    Erosion(NewMorphoKernel tmp) : NewMorphoKernel(tmp) {};
    QColor calcNewPixelColor(const QImage& img, int x, int y) const;

};

QColor Erosion::calcNewPixelColor(const QImage& img, int x, int y) const
{
    float returnR = 0;
    float returnB = 0;
    float returnG = 0;

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            QColor color = img.pixelColor(myclamp<float>(x + j, img.width() - 1, 0),
                myclamp<float>(y + i, img.height() - 1, 0));

            if (arr[i][j])
            {
                if (color.red() < returnR)
                    returnR = color.red();
                if (color.green() < returnG)
                    returnG = color.green();
                if (color.blue() < returnB)
                    returnB = color.blue();
            }

        }
    return QColor(returnR, returnG, returnB);
}