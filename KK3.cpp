#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

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
    split(complex_img, plane);
    fftshift(plane[0], plane[1]);
    //int cnt = 0;
    //Mat re_1d = Mat::zeros(1, plane[0].cols*plane[0].rows, CV_32F);
    //Mat im_1d = Mat::zeros(1, plane[0].cols * plane[0].rows, CV_32F);
    //for (int i = 0; i < plane[0].cols; ++i)
    //{
    //    for (int j = 0; j < plane[0].rows; ++j)
    //    {
    //        re_1d.at<float>(0, cnt) = plane[0].at<float>(i,j);
    //        im_1d.at<float>(0, cnt) = plane[1].at<float>(i, j);
    //        cnt++;
    //    }
    //}
    // 计算实部和虚部的Hilbert变换
    Mat hilbert_re, hilbert_im;

    // 创建Hilbert核
    cv:Mat hilbert_kernel_c = Mat::zeros(1, input_img.cols, CV_32F);
    int nc = hilbert_kernel_c.cols;
    for (int i = 0; i < nc; i++) {
        if (i == nc / 2) {
            hilbert_kernel_c.at<float>(0, i) = 1;
        }
        else
        {
            float valc = 1.0 / (CV_PI * (i - nc / 2));
            hilbert_kernel_c.at<float>(0, i) = valc;
        }
    }

    cv::Mat hilbert_kernel_r = Mat::zeros(input_img.rows, 1, CV_32F);
    //hilbert_kernel_r.at<float>(0, 0) = 1;
    int nr = hilbert_kernel_r.rows;
    for (int i = 0; i < nr; i++) {
        if (i == nr / 2)
        {
            hilbert_kernel_r.at<float>(i, 0) = 1;
        }
        else
        {
            float val = 1.0 / (CV_PI * (i - nr / 2));
            hilbert_kernel_r.at<float>(i, 0) = val;
        }
    }

    cv::Mat hilbert_kernel = Mat::zeros(input_img.size(), CV_32F);
    int nx = hilbert_kernel.rows;
    int ny = hilbert_kernel.cols;
    double x, y;
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            if (i == nx/2  && j == ny/2) {
                hilbert_kernel.at<float>(i, j) = 0;
            }
            else {
                if (i == nx/2 ){
                    x = 1;
                }
                else {
                    x = i - nx/2 ;
                }
                if (j == ny/2 ) {
                    y = 1;
                }
                else {
                    y = j - ny/2 ;
                }
                double distance = x*y;
                hilbert_kernel.at<float>(i, j) = 1 / (CV_PI * distance);
            }
        }
    }
    // 对实部和虚部分别应用Hilbert核
    filter2D(plane[0], hilbert_im, -1, hilbert_kernel);
    filter2D(plane[1], hilbert_re, -1, hilbert_kernel);
    for (int i = 0; i < nx; i++)
    {
        printf("%d: %f\n", i, hilbert_kernel.at<float>(i,1));
    }
    fftshift(hilbert_re,hilbert_im);
    Mat hilbert_v[] = { hilbert_re , -hilbert_im };
    //hilbert_im = -hilbert_im;
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