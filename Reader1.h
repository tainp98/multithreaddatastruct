#ifndef READER1_H
#define READER1_H
#include <opencv2/opencv.hpp>
#include "iostream"
#include "stdio.h"
#include "Matrix.h"
#include "LockBuffer.h"
using namespace std;
using namespace cv;

class Reader1{
  public:
    Reader1(LockBuffer<Matrix<unsigned char>>& _v4l2Buff)
        : v4l2Buff(_v4l2Buff)
    {
        mat.create(480, 640);
    }
    ~Reader1(){
    }

    void run(){
//        std::cout << "[Reader1]:\n";
//        std::cout << "max buffer size = " << v4l2Buff.capacity() << "\n";
//        for(int i = 0; i < v4l2Buff.capacity(); i++){
//            printf("Addr Of buffer[%d] = %p\n", i, v4l2Buff.buffer[i].Ptr);
//            printf("rows = %d, cols = %d, size = %d\n", v4l2Buff.buffer[i].rows, v4l2Buff.buffer[i].cols, v4l2Buff.buffer[i].size_);

//        }
//        std::cout << "n_in = " << v4l2Buff.n_in << " n_out = " << v4l2Buff.n_out << std::endl;
//        std::cout << "readers_active size = " << v4l2Buff.readers_active.size() << std::endl;
        while(true){
            printf("[Reader1]: read_pos = %d\n", read_pos);
            while((read_pos = v4l2Buff.front1(mat)) == -1){
//                printf("[Reader1]: In while\n");
//                this_thread::sleep_for(chrono::milliseconds(1));
            }
            v4l2Buff.pop1();
//            printf("[Reader1]: Addr Of Ptr = %p\n", mat.Ptr);
            cv::Mat img(480, 640, CV_8UC1, mat.Ptr);
//            cv::imshow("Reader1", img);
//            cv::waitKey(7);
            this_thread::sleep_for(chrono::milliseconds(5));
        }

    }

private:
    LockBuffer<Matrix<unsigned char>>& v4l2Buff;
    Matrix<unsigned char> mat;
    int read_pos = -1;
};
#endif // READER1_H
