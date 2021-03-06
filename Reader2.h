#ifndef READER2_H
#define READER2_H

#include <opencv2/opencv.hpp>
#include "iostream"
#include "stdio.h"
#include "Matrix.h"
#include "LockBuffer.h"
#include <thread>
#include <chrono>
using namespace std;
using namespace cv;

class Reader2{
  public:
    Reader2(LockBuffer<Matrix<unsigned char>>& _v4l2Buff)
        : v4l2Buff(_v4l2Buff)
    {
        mat.create(480, 640);
    }
    ~Reader2(){
    }

    void run(){
        chrono::high_resolution_clock::time_point start, stop;
        while(true){
//            printf("[Reader2]: read_pos = %d\n", latest_read_pos);
//            printf("[Reader1]: Addr Of Ptr = %p\n", mat.Ptr);
            start = chrono::high_resolution_clock::now();
            while(v4l2Buff.front1(mat, latest_read_pos) == false){
//                printf("[Reader2]: In while\n");
//                this_thread::sleep_for(chrono::milliseconds(1));
            }
            v4l2Buff.pop1();
//            printf("[Reader1]: Addr Of Ptr = %p\n", mat.Ptr);
            cv::Mat img(480, 640, CV_8UC1, mat.Ptr);

            cv::imshow("Reader2", img);
            cv::waitKey(30);
            stop = chrono::high_resolution_clock::now();
            std::cout << "==========[Reader2]: Time =   " << chrono::duration_cast<chrono::microseconds>(stop-start).count()/1000.0 << "\n";
//            this_thread::sleep_for(chrono::milliseconds(7));
        }

    }

private:
    LockBuffer<Matrix<unsigned char>>& v4l2Buff;
    Matrix<unsigned char> mat;
    int latest_read_pos = -1;
};

#endif // READER2_H
