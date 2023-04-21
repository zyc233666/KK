#include <iostream>
#include <cmath>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void kramers_kronig(Mat& input_img, Mat& output_img);

int main() {
    ifstream inputFile("I5.txt"); // Replace example.txt with the name of your file
    ofstream outputFile("example.txt");
    if (inputFile) { // Check if the file was successfully opened
        std::vector<std::vector<double>> data;
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::vector<double> row;
            double val;
            while (iss >> val) {
                row.push_back(val);
            }
            data.push_back(row);
        }
        cv::Mat matrix(data.size(), data[0].size(), CV_64FC1);
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data[i].size(); j++) {
                matrix.at<double>(i, j) = data[i][j];
            }
        }
        //std::cout << matrix << std::endl; // Do something with the matrix
        Mat output_img;
        kramers_kronig(matrix, output_img);
        int nx = output_img.rows;
        int ny = output_img.cols;
        for (int i = 0; i < nx; i++) {
            for (int j = 0; j < ny; j++) {
                outputFile << std::setw(20) << std::left<<output_img.at<double>(i, j);
            }
            outputFile<<"\n";
        }
        //cout << output_img << endl;
    }
    else {
        std::cout << "Error opening file" << std::endl;
    }

    inputFile.close(); // Don't forget to close the file

    return 0;
}


void kramers_kronig(Mat& input_img, Mat& output_img)
{
    cv::Mat hilbert_kernel_c = Mat::zeros(input_img.rows, input_img.cols, CV_64F);
    int ncx = input_img.rows;
    int ncy = input_img.cols;
    for (int i = 0; i < ncx; i++) {
        for (int j = 0; j < ncy; j++) {
            input_img.at<double>(i, j) = log(input_img.at<double>(i, j))/2;
        }
    }

    // 将输入图像转换为浮点型复数矩阵
    Mat complex_img;
    input_img.convertTo(complex_img, CV_64FC2);

	// 进行傅里叶变换，获取复数矩阵
	dft(complex_img, complex_img, DFT_COMPLEX_OUTPUT);

	// 分离实部和虚部
	Mat re, im;
	Mat plane[] = { re, im };
	cv::split(complex_img, plane);

    // 创建Hilbert核
    
    int midle;
    if (ncx > 0 && ncx % 2 == 0) {
        midle = ncx / 2;
    }
    else {
        midle = 0;
    }
    for (int i = 0; i < ncx; i++) {
        for (int j = 0; j < ncy; j++) {
            if (i == 0 || i == midle) {
                hilbert_kernel_c.at<double>(j, i) = 1;
            }
            else if (i <ncx / 2) {
                hilbert_kernel_c.at<double>(j, i) = 2;
            }
            else {
                hilbert_kernel_c.at<double>(j, i) = 0;
            }
        }
    }

    // 对实部和虚部分别应用Hilbert核
    Mat hilbert_re = Mat::zeros(input_img.size(), CV_64F);
    Mat hilbert_im = Mat::zeros(input_img.size(), CV_64F);
    multiply(plane[0], hilbert_kernel_c, hilbert_re);
    multiply(plane[1], hilbert_kernel_c, hilbert_im);

    Mat hilbert_v[] = { hilbert_re, hilbert_im };
    cv::Mat BLUR;
    cv::merge(hilbert_v, 2, BLUR);
    cv::idft(BLUR, BLUR, DFT_COMPLEX_OUTPUT);
    BLUR = BLUR / BLUR.rows / BLUR.cols;
    cv::split(BLUR, plane);
    output_img = plane[1];
}