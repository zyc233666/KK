#include<iostream>
#include<opencv2/opencv.hpp>
#include<ctime>
using namespace std;
using namespace cv;
int main(void)
{
	// ��ȡͼƬ
	Mat src = imread("lena.jpg");//imread()��������ͼ��
	// �ж��Ƿ�Ϊ��
	if (src.empty())
	{
		cout << "Can not load image\n" << endl;//
		return -1;
	}
	// vector������ͨ��Mat
	vector<cv::Mat> channels;
	cv::split(src, channels);
	// OpenCV��BGRɫ�ʿռ�ͬ��ͳ�����RGB�෴������1ͨ����Blue����2ͨ����Green�̣�3ͨ����Red��
	// ��ͨ���������ٺϲ�����ͼƬЧ��
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