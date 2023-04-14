#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <memory.h>
#include "ConstParam.h"

using namespace std;
using namespace cv;

void fftshift(cv::Mat& plane0, cv::Mat& plane1);

void kramers_kronig(Mat& input_img, Mat& output_img)
{
    // 将输入图像转换为浮点型复数矩阵
    Mat complex_img;
    input_img.convertTo(complex_img, CV_32FC2);

    // 进行傅里叶变换，获取复数矩阵
    dft(complex_img, complex_img, DFT_COMPLEX_OUTPUT);

    // 分离实部和虚部
    Mat re, im;
    Mat plane[] = { re, im };
    cv::split(complex_img, plane);
    fftshift(plane[0], plane[1]);
    // 计算实部和虚部的Hilbert变换


    // 创建Hilbert核
    cv::Mat hilbert_kernel_c = Mat::zeros(input_img.rows, input_img.cols, CV_32F);
    int ncx = hilbert_kernel_c.rows;
    int ncy = hilbert_kernel_c.cols;
    for (int i = 0; i < ncx; i++) {
        for (int j = 0; j < ncy; j++) {
            if (i == 0||i==ncx/2) {
                hilbert_kernel_c.at<float>(i, j) = 1;
            }
            else if (i < ncx / 2) {
                hilbert_kernel_c.at<float>(i, j) = 2;
            }
            else {
                hilbert_kernel_c.at<float>(i, j) = 0;
            }
        }
    }


    cv::Mat hilbert_kernel_r = Mat::zeros(input_img.rows,input_img.cols, CV_32F);
    int nrx = hilbert_kernel_r.rows;
    int nry = hilbert_kernel_r.cols;
    for (int i = 0; i < nrx; i++) {
        for (int j = 0; j < nry; j++) {
            if (j == 0 || j == nry / 2) {
                hilbert_kernel_r.at<float>(i, j) = 1;
            }
            else if (j < nry / 2) {
                hilbert_kernel_r.at<float>(i, j) = 2;
            }
            else {
                hilbert_kernel_r.at<float>(i, j) = 0;
            }
        }
    }
    //imshow("t", hilbert_kernel_r);
    //waitKey(0);
    //system("pause");
    cv:Mat hilbert_kernel = Mat::zeros(input_img.size(), CV_32F);
    hilbert_kernel = (hilbert_kernel_c + hilbert_kernel_r)/2;

    //imshow("c", hilbert_kernel / 4);
    //waitKey(0);
    //system("pause");

    // 对实部和虚部分别应用Hilbert核
    Mat hilbert_re = Mat::zeros(input_img.size(), CV_32F);
    Mat hilbert_im = Mat::zeros(input_img.size(), CV_32F);
    multiply(plane[0], hilbert_kernel_c, hilbert_re);
    multiply(plane[1], hilbert_kernel_c, hilbert_im);
    //hilbert_re = plane[0]* hilbert_kernel;
    //hilbert_im = plane[1]* hilbert_kernel;
    fftshift(hilbert_re, hilbert_im);

    Mat hilbert_v[] = { hilbert_re, hilbert_im };
    cv::Mat BLUR;
    cv::merge(hilbert_v, 2, BLUR);
    cv::idft(BLUR, BLUR, DFT_COMPLEX_OUTPUT);
    BLUR = BLUR / BLUR.rows / BLUR.cols;
    cv::split(BLUR, plane);
    imshow("0", plane[0] / 255);
    imshow("1", plane[1] / 255);
}

int main()
{
    // 读取输入图像
    Mat input_img = imread("lena.jpg", 0);
    //input_img.convertTo(input_img, CV_32FC1);
    //Mat output_img = Mat::zeros(input_img.size(), CV_32F);
    Mat output_img;
    kramers_kronig(input_img, output_img);
    imshow("original", input_img);
    //imshow("result", output_img);
    waitKey(0);
    system("pause");
    return 0;
}


// fft变换后进行频谱搬移
void fftshift(cv::Mat& plane0, cv::Mat& plane1)
{
    // 以下的操作是移动图像  (零频移到中心)
    int cx = plane0.cols / 2;
    int cy = plane0.rows / 2;
    cv::Mat part1_r(plane0, cv::Rect(0, 0, cx, cy));  // 元素坐标表示为(cx, cy)
    cv::Mat part2_r(plane0, cv::Rect(cx, 0, cx, cy));
    cv::Mat part3_r(plane0, cv::Rect(0, cy, cx, cy));
    cv::Mat part4_r(plane0, cv::Rect(cx, cy, cx, cy));

    cv::Mat temp;
    part1_r.copyTo(temp);  //左上与右下交换位置(实部)
    part4_r.copyTo(part1_r);
    temp.copyTo(part4_r);

    part2_r.copyTo(temp);  //右上与左下交换位置(实部)
    part3_r.copyTo(part2_r);
    temp.copyTo(part3_r);

    cv::Mat part1_i(plane1, cv::Rect(0, 0, cx, cy));  //元素坐标(cx,cy)
    cv::Mat part2_i(plane1, cv::Rect(cx, 0, cx, cy));
    cv::Mat part3_i(plane1, cv::Rect(0, cy, cx, cy));
    cv::Mat part4_i(plane1, cv::Rect(cx, cy, cx, cy));

    part1_i.copyTo(temp);  //左上与右下交换位置(虚部)
    part4_i.copyTo(part1_i);
    temp.copyTo(part4_i);

    part2_i.copyTo(temp);  //右上与左下交换位置(虚部)
    part3_i.copyTo(part2_i);
    temp.copyTo(part3_i);
}