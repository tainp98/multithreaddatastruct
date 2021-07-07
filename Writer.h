#ifndef WRITER_H
#define WRITER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "Matrix.h"
#include "LockBuffer.h"
using namespace std;
//template<typename T, int max_size>
class Writer{
  public:
    Writer(LockBuffer<Matrix<unsigned char>>& _v4l2Buff, string _fileName)
        : v4l2Buff(_v4l2Buff), fileName(_fileName)
    {

    }
    ~Writer(){

    }

    void runVideo(){
        cv::Mat frame;
        std::chrono::high_resolution_clock::time_point start, stop;


        if( !cap.isOpened() )
            throw "Error when reading steam_avi";
        for( ; ; )
        {
            start = std::chrono::high_resolution_clock::now();
            cap >> frame;
            if(frame.empty()){
                "=========finish video\n";
                break;
            }
    //        frame_YUV = cv::Mat(480, 640, CV_8UC1, data);
    //        cv::imshow("img", frame);
    //        cv::waitKey(1);
            cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
            cv::Mat frame1 = frame.clone();
            stop = std::chrono::high_resolution_clock::now();
            std::cout << "Time read = " << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count()/1000.0 << std::endl;
        }
        cv::waitKey(0);
    }

    void run(){
        if(videoFile != ""){
            cap.open(fileName);
            cv::Mat frame;
            std::chrono::high_resolution_clock::time_point start, stop;


            if( !cap.isOpened() )
                throw "Error when reading steam_avi";
            for( ; ; )
            {
                start = std::chrono::high_resolution_clock::now();
                cap >> frame;
                if(frame.empty()){
                    "=========finish video\n";
                    break;
                }
        //        frame_YUV = cv::Mat(480, 640, CV_8UC1, data);
        //        cv::imshow("img", frame);
        //        cv::waitKey(1);
                cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
                cv::Mat frame1 = frame.clone();
                stop = std::chrono::high_resolution_clock::now();
                std::cout << "Time read = " << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count()/1000.0 << std::endl;
            }
        }
        cout << "[Writer]:\n";
        std::cout << "max buffer size = " << v4l2Buff.capacity() << "\n";
        for(int i = 0; i < v4l2Buff.capacity(); i++){
            printf("Addr Of buffer[%d] = %p\n", i, v4l2Buff.buffer[i].Ptr);
            printf("rows = %d, cols = %d, size = %d\n", v4l2Buff.buffer[i].rows, v4l2Buff.buffer[i].cols, v4l2Buff.buffer[i].size_);

        }
        std::cout << "n_in = " << v4l2Buff.n_in << " n_out = " << v4l2Buff.n_out << std::endl;
        std::cout << "readers_active size = " << v4l2Buff.readers_active.size() << std::endl;

    }
public:
    cv::VideoCapture cap;
    std::string fileName;

    LockBuffer<Matrix<unsigned char>>& v4l2Buff;
};
#endif // WRITER_H
