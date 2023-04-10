#include <SDL.h>
#include <cmath>
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const int WIDTH = 640;
const int HEIGHT = 480;

void hilbert(Mat& input, Mat& output);

int main(int argc, char* argv[]) {
    SDL_Window* window = SDL_CreateWindow("sin(x) graph",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // set background color to black
    SDL_RenderClear(renderer);                      // clear the screen

    // Draw the graph of sin(x)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // set the color to white
    SDL_RenderDrawLine(renderer, 0, HEIGHT / 2, WIDTH, HEIGHT / 2); // draw the x-axis
    Mat input_siginal = Mat::zeros(WIDTH, 1, CV_32F);
    Mat output_siginal;
    for (int i = 0; i < WIDTH; i++) {
        double x = 2 * M_PI * i / WIDTH;  // convert pixel position to x value
        double y = sin(x);               // calculate sin(x)
        input_siginal.at<float>(i, 0) = y;
        //double yy = cos(x);
        //int j = (int)(y * HEIGHT / 2 + HEIGHT / 2); // convert y value to pixel position
        //int jj = (int)(yy * HEIGHT / 2 + HEIGHT / 2);
        //SDL_RenderDrawPoint(renderer, i, j);   // draw the pixel
        //SDL_RenderDrawPoint(renderer, i, jj);

    }
    hilbert(input_siginal, output_siginal);
    for (int i = 0; i < WIDTH; i++) {
        double x = 2 * M_PI * i / WIDTH;  // convert pixel position to x value
        double y = sin(x);
        double yy = output_siginal.at<float>(i, 0);
        cout << y << endl;
        int j = (int)(y * HEIGHT / 2 + HEIGHT / 2); // convert y value to pixel position
        int jj = (int)(yy * HEIGHT / 2 + HEIGHT / 2);
        SDL_RenderDrawPoint(renderer, i, j);   // draw the pixel 
        SDL_RenderDrawPoint(renderer, i, jj);
    }

    SDL_RenderPresent(renderer);                   // show the rendered image
    //SDL_Delay(5000);                               // wait for 5 seconds
    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window);
    //SDL_Quit();
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    return 0;
}


void hilbert(Mat& input, Mat& output) {
    cv::Mat hilbert_kernel_r = Mat::zeros(input.rows, 1, CV_32F);
    dft(input, input, DFT_COMPLEX_OUTPUT);
    Mat in_re, in_im;
    Mat input_line[] = { in_re,in_im };
    split(input, input_line);
    Mat out_re, out_im;
    Mat output_line[] = { out_re,out_im };
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
    multiply(input_line[0], hilbert_kernel_r, output_line[0]);
    multiply(input_line[1], hilbert_kernel_r, output_line[1]);
    merge(output_line,2, output);
    idft(output, output, DFT_COMPLEX_OUTPUT);
    output = output / output.rows;
    split(output, output_line);
    output = output_line[1];
}
