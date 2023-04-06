#include<iostream>
#include<opencv2/opencv.hpp>
#include<ctime>
using namespace std;
using namespace cv;

void fftshift(cv::Mat& plane0, cv::Mat& plane1);


int main(void)
{
	Mat test = imread("lena.jpg", 0);
	test.convertTo(test, CV_32FC1);

	//创建通道，存储dft后的实部与虚部（CV_32F，必须为单通道数）
	cv::Mat plane[] = { test.clone(), cv::Mat::zeros(test.size() , CV_32FC1) };
	cv::Mat complexIm;
	cv::merge(plane, 2, complexIm); // 合并通道 （把两个矩阵合并为一个2通道的Mat类容器）
	cv::dft(complexIm, complexIm, 0); // 进行傅立叶变换，结果保存在自身

	// 分离通道（数组分离）
	cv::split(complexIm, plane);
	// 以下的操作是频域迁移
	fftshift(plane[0], plane[1]);
	// 计算幅值
	cv::Mat mag, mag_log, mag_nor;
	cv::magnitude(plane[0], plane[1], mag);
	// 幅值对数化：log（1+m），便于观察频谱信息
	mag += Scalar::all(1);
	cv::log(mag, mag_log);
	cv::normalize(mag_log, mag_nor, 1, 0, NORM_MINMAX);

	cv::Mat BLUR;
	// 再次搬移回来进行逆变换
	fftshift(plane[0], plane[1]);
	cv::merge(plane, 2, BLUR); // 实部与虚部合并
	cv::idft(BLUR, BLUR);       // idft结果也为复数
	BLUR = BLUR / BLUR.rows / BLUR.cols;
	cv::split(BLUR, plane);//分离通道，主要获取通道

	//imshow("original", test / 255);
	//imshow("spectrum", mag_nor);
	//imshow("result", plane[0] / 255);
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