#include<iostream>
#include<opencv2/opencv.hpp>
#include<ctime>
using namespace std;
using namespace cv;
int main(void)
{
	// 读取图片
	Mat src = imread("lena.jpg");//imread()函数载入图像
	// 判断是否为空
	if (src.empty())
	{
		cout << "Can not load image\n" << endl;//
		return -1;
	}
	// vector创建三通道Mat
	vector<cv::Mat> channels;
	cv::split(src, channels);
	// OpenCV的BGR色彩空间同传统意义的RGB相反，所以1通道是Blue蓝，2通道是Green绿，3通道是Red红
	// 蓝通道削弱，再合并，看图片效果
	channels[0] = channels[0] * 0;
	channels[1] = channels[1] * 0;
	cv::Mat result;
	cv::merge(channels, result);

	imshow("original", src);
	imshow("result", result);
	waitKey(0);
	system("pause");
	return 0;
}