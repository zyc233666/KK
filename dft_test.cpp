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

	//����ͨ�����洢dft���ʵ�����鲿��CV_32F������Ϊ��ͨ������
	cv::Mat plane[] = { test.clone(), cv::Mat::zeros(test.size() , CV_32FC1) };
	cv::Mat complexIm;
	cv::merge(plane, 2, complexIm); // �ϲ�ͨ�� ������������ϲ�Ϊһ��2ͨ����Mat��������
	cv::dft(complexIm, complexIm, 0); // ���и���Ҷ�任���������������

	// ����ͨ����������룩
	cv::split(complexIm, plane);
	// ���µĲ�����Ƶ��Ǩ��
	fftshift(plane[0], plane[1]);
	// �����ֵ
	cv::Mat mag, mag_log, mag_nor;
	cv::magnitude(plane[0], plane[1], mag);
	// ��ֵ��������log��1+m�������ڹ۲�Ƶ����Ϣ
	mag += Scalar::all(1);
	cv::log(mag, mag_log);
	cv::normalize(mag_log, mag_nor, 1, 0, NORM_MINMAX);

	cv::Mat BLUR;
	// �ٴΰ��ƻ���������任
	fftshift(plane[0], plane[1]);
	cv::merge(plane, 2, BLUR); // ʵ�����鲿�ϲ�
	cv::idft(BLUR, BLUR);       // idft���ҲΪ����
	BLUR = BLUR / BLUR.rows / BLUR.cols;
	cv::split(BLUR, plane);//����ͨ������Ҫ��ȡͨ��

	//imshow("original", test / 255);
	//imshow("spectrum", mag_nor);
	//imshow("result", plane[0] / 255);
	waitKey(0);
	system("pause");
	return 0;
}

// fft�任�����Ƶ�װ���
void fftshift(cv::Mat& plane0, cv::Mat& plane1)
{
	// ���µĲ������ƶ�ͼ��  (��Ƶ�Ƶ�����)
	int cx = plane0.cols / 2;
	int cy = plane0.rows / 2;
	cv::Mat part1_r(plane0, cv::Rect(0, 0, cx, cy));  // Ԫ�������ʾΪ(cx, cy)
	cv::Mat part2_r(plane0, cv::Rect(cx, 0, cx, cy));
	cv::Mat part3_r(plane0, cv::Rect(0, cy, cx, cy));
	cv::Mat part4_r(plane0, cv::Rect(cx, cy, cx, cy));

	cv::Mat temp;
	part1_r.copyTo(temp);  //���������½���λ��(ʵ��)
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);

	part2_r.copyTo(temp);  //���������½���λ��(ʵ��)
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	cv::Mat part1_i(plane1, cv::Rect(0, 0, cx, cy));  //Ԫ������(cx,cy)
	cv::Mat part2_i(plane1, cv::Rect(cx, 0, cx, cy));
	cv::Mat part3_i(plane1, cv::Rect(0, cy, cx, cy));
	cv::Mat part4_i(plane1, cv::Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp);  //���������½���λ��(�鲿)
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);

	part2_i.copyTo(temp);  //���������½���λ��(�鲿)
	part3_i.copyTo(part2_i);
	temp.copyTo(part3_i);
}