#include <opencv2/core.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

Mat createHilbertKernel(int size) {
    Mat kernel(size, size, CV_32FC1);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == 0 && j == 0) {
                kernel.at<float>(i, j) = 0.0;
            }
            else if (i == size / 2 || j == size / 2) {
                kernel.at<float>(i, j) = 0.0;
            }
            else {
                kernel.at<float>(i, j) = 1.0 / (M_PI * (i + j) * (size - i - size / 2 + j));
            }
        }
    }
    return kernel;
}

int main() {
    Mat kernel = createHilbertKernel(5);
    cout << kernel << endl;
    return 0;
}
