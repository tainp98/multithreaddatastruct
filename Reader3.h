#ifndef READER3_H
#define READER3_H
#include <opencv2/opencv.hpp>
#include "iostream"
#include "stdio.h"
#include "Matrix.h"
#include "LockBuffer.h"
using namespace std;
using namespace cv;

class Reader3{
  public:
    Reader3(LockBuffer<Matrix<unsigned char>>& _v4l2Buff)
        : v4l2Buff(_v4l2Buff)
    {
        mat.create(480, 640);
    }
    ~Reader3(){
    }

    void run(){
        chrono::high_resolution_clock::time_point start, stop;
        while(true){
//            printf("[Reader1]: read_pos = %d\n", latest_read_pos);
            start = chrono::high_resolution_clock::now();
            while(v4l2Buff.front1(mat, latest_read_pos) == false){
//                printf("[Reader1]: In while\n");
//                this_thread::sleep_for(chrono::milliseconds(1));
            }
            v4l2Buff.pop1();
//            printf("[Reader1]: Addr Of Ptr = %p\n", mat.Ptr);
            cv::Mat img(480, 640, CV_8UC1, mat.Ptr);
//            cv::imshow("Reader1", img);
//            cv::waitKey(7);
            this_thread::sleep_for(chrono::milliseconds(10));
            stop = chrono::high_resolution_clock::now();
            std::cout << "==========[Reader3]: Time = " << chrono::duration_cast<chrono::microseconds>(stop-start).count()/1000.0 << "\n";
        }

    }

private:
    LockBuffer<Matrix<unsigned char>>& v4l2Buff;
    Matrix<unsigned char> mat;
    int latest_read_pos = -1;
};
#endif // READER3_H
