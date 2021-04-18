#include <QCoreApplication>
#include <QImage>
#include <string>
#include<iostream>
#include"PointFilter.h"
#include"Morpho.h"
#include"Matrix_Fs.h"
#include"NewKernel.h"
using namespace std;


int main(int argc, char* argv[])
{
    string s;
    QImage img;

    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
        {
            s = argv[i + 1];
        }
    }

    img.load(QString(s.c_str()));


    //InvertFilter invert;
    //invert.process(img).save("invert_img.png");

    //GrayScaleFilter grayimg;
    //grayimg.process(img).save("gray_img.png");

    //SepiaFilter sep;
    //sep.process(img).save("sep.png");

    //BrightnessFilter br;
    //br.process(img).save("br.png");

    //BlurFilter blur;
    //blur.process(img).save("blur_img.png");

    //GaussianFilter gaus;
    //gaus.process(img).save("gaus_img.png");

    //SharpnessFilter Sn;
    //Sn.process(img).save("Sn_img.png");

    //SobelFilter Sf_x(1);
    //SobelFilter Sf_y(0);
    //Sf_x.process(img).save("Sf_x_img.png");
    //Sf_y.process(img).save("Sf_y_img.png");

    //Otr Otr; 
    //Otr.process(img).save("Otr_img.png");

    //TurnFilter Turn(100);
    //Turn.process(img).save("Turn_img.png");

    //GlassFilter glass; 
    //glass.process_glass(img).save("glass_img.png");


    //MedianFilter Mf(2);
    //Mf.process(img).save("Mf_img.png");

    //LinearCorrectionFilter LF;
    //LF.process(img).save("Lf_img.png");

    int h = 4;
    int w = 4;

    double** arr = new double*[h];

    arr[0] = new double[w] { 0, 1, 1, 0 };
    arr[1] = new double[w] { 1, 1, 1, 1 };
    arr[2] = new double[w] { 1, 1, 1, 1 };
    arr[3] = new double[w] { 1, 1, 1, 1 };

    NewMorphoKernel MK(h, w, arr);

    Dilation DL(MK);
    DL.process(img).save("DL.png");
}
