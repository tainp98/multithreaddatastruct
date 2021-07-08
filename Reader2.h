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
        while(true){
            printf("[Reader2]: read_pos = %d\n", read_pos);
//            printf("[Reader1]: Addr Of Ptr = %p\n", mat.Ptr);
            while((read_pos = v4l2Buff.front1(mat)) == -1){
//                printf("[Reader2]: In while\n");
//                this_thread::sleep_for(chrono::milliseconds(1));
            }
            v4l2Buff.pop1();
//            printf("[Reader1]: Addr Of Ptr = %p\n", mat.Ptr);
            cv::Mat img(480, 640, CV_8UC1, mat.Ptr);

//            cv::imshow("Reader2", img);
            //cv::waitKey(10);
            this_thread::sleep_for(chrono::milliseconds(7));
        }

    }

private:
    LockBuffer<Matrix<unsigned char>>& v4l2Buff;
    Matrix<unsigned char> mat;
    int read_pos = -1;
};

#endif // READER2_H
