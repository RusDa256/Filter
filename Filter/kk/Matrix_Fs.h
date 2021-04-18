#pragma once
#include <QCoreApplication>
#include <QImage>
#include"Kernel_and_MatrixF.h"

class BlurKernel : public Kernel
{
public:
    using Kernel::Kernel;
    BlurKernel(std::size_t radius = 1) : Kernel(radius)
    {
        for (std::size_t i = 0; i < getLen(); i++)
            data[i] = 1.0f / getLen();
    }
};
class BlurFilter : public MatrixFilter
{
public:
    BlurFilter(std::size_t radius = 1) : MatrixFilter(BlurKernel(radius)) {}
};

class GaussianKernel : public Kernel
{
public:
    using Kernel::Kernel;
    GaussianKernel(size_t radius = 3, float sigma = 2.f) : Kernel(radius)
    {
        float norm = 0;
        int signed_radius = static_cast<int>(radius);

        for (int x = -signed_radius; x <= signed_radius; x++)
        {
            for (int y = -signed_radius; y <= signed_radius; y++)
            {
                size_t idx = (x + radius) * getSize() + (y + radius);
                data[idx] = exp(-(x * x + y * y) / (sigma * sigma));
                norm += data[idx];
            }
        }

        for (size_t i = 0; i < getLen(); i++)
        {
            data[i] /= norm;
        }
    }
};

class GaussianFilter : public MatrixFilter
{
public:
    GaussianFilter(std::size_t radius = 2) : MatrixFilter(GaussianKernel(radius)) {}

};

class SharpnessKernel : public Kernel
{
public:
    using Kernel::Kernel;
    SharpnessKernel(std::size_t radius = 1) : Kernel(radius)
    {
        data[0] = 0;
        data[1] = -1;
        data[2] = 0;
        data[3] = -1;
        data[4] = 5;
        data[5] = -1;
        data[6] = 0;
        data[7] = -1;
        data[8] = 0;
    }
};
class SharpnessFilter : public MatrixFilter
{
public:
    SharpnessFilter(std::size_t radius = 1) : MatrixFilter(SharpnessKernel(radius)) {}
};

class SobelKernel : public Kernel
{
public:
    using Kernel::Kernel;
    SobelKernel(int ind, size_t radius = 1) : Kernel(radius)
    {
        if (ind == 0) // y
        {
            data[0] = -1; data[1] = -2; data[2] = -1;
            data[3] = 0; data[4] = 0; data[5] = 0;
            data[6] = 1; data[7] = 2; data[8] = 1;
        }
        if (ind == 1) // x
        {
            data[0] = -1; data[1] = 0; data[2] = 1;
            data[3] = -2; data[4] = 0; data[5] = 2;
            data[6] = -1; data[7] = 0; data[8] = 1;
        }
    }
};

class SobelFilter : public MatrixFilter
{
public:
    SobelFilter(int ind, size_t radius = 1) : MatrixFilter(SobelKernel(ind, radius)) {}
};