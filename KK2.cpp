#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <memory.h>
#include "ConstParam.h"

using namespace std;
using namespace cv;


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

    // 计算实部和虚部的Hilbert变换
    Mat vector_r=plane[0].reshape(0, 1);
    Mat vector_i=plane[1].reshape(0, 1);
    Mat vector[] = { vector_r,vector_i };

    // 创建Hilbert核
    cv::Mat hilbert_kernel_c = Mat::zeros(1, input_img.cols * input_img.rows, CV_32F);
    hilbert_kernel_c.at<float>(0, 0) = 1;
    int n = hilbert_kernel_c.cols;
    for (int i = 1; i < n; i++) {
        if (i == n / 2)
        {
            hilbert_kernel_c.at<float>(0, i) = 1;
        }
        else if (i < n / 2)
        {
            hilbert_kernel_c.at<float>(0, i) = 2;
        }
        else
        {
            hilbert_kernel_c.at<float>(0, i) = 0;
        }
    }


    cv::Mat hilbert_kernel_r = Mat::zeros(input_img.rows , 1, CV_32F);
    hilbert_kernel_r.at<float>(0, 0) = 1;
    int nr = hilbert_kernel_r.rows;
    for (int i = 1; i < nr; i++) {
        if (i < nr / 2)
        {
            hilbert_kernel_r.at<float>(i, 0) = 2;
        }
        else if (i == nr / 2)
        {
            hilbert_kernel_r.at<float>(i, 0) = 1;
        }
        else
        {
            hilbert_kernel_r.at<float>(i, 0) = 0;
        }
    }

cv:Mat hilbert_kernel = Mat::zeros(input_img.size(), CV_32F);
    int nx = hilbert_kernel.rows;
    int ny = hilbert_kernel.cols;
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            if (i == 0 || i == nx / 2) {
                if (j == 0 || j == ny / 2) {
                    hilbert_kernel.at<float>(i, j) = 1;
                }
                else if (j < ny / 2) {
                    hilbert_kernel.at<float>(i, j) = 2;
                }
            }
            else if ((i < nx / 2) && (j <= ny / 2)) {
                hilbert_kernel.at<float>(i, j) = 2;
            }
        }
    }


    // 对实部和虚部分别应用Hilbert核
    //Mat hilbert_re = Mat::zeros(input_img.size(), CV_32F);
    //Mat hilbert_im = Mat::zeros(input_img.size(), CV_32F);
    //for (int i = 0; i < nr; i++)
    //{
    //    Mat row_r = plane[0].row(i);
    //    Mat row_i = plane[1].row(i);
    //    Mat new_row_im = hilbert_im.row(i);
    //    Mat new_row_re = hilbert_re.row(i);
    //    multiply(row_r, hilbert_kernel_c, new_row_re);
    //    multiply(row_i, hilbert_kernel_c, new_row_im);
    //}
    Mat hilbert_re, hilbert_im;
    multiply(vector[0], hilbert_kernel_c, hilbert_re);
    multiply(vector[1], hilbert_kernel_c, hilbert_im);
    Mat plane_re = hilbert_re.reshape(0, input_img.rows);
    Mat plane_im = hilbert_im.reshape(0, input_img.rows);
    Mat hilbert_v[] = { plane_re, plane[1] };
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
    Mat input_img = imread("beauty.jpg", 0);
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
