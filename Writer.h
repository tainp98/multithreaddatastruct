#ifndef WRITER_H
#define WRITER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "Matrix.h"
#include "LockBuffer.h"
#include <thread>
#include <chrono>

using namespace std;
//template<typename T, int max_size>
class Writer{
  public:
    Writer(LockBuffer<Matrix<unsigned char>>& _v4l2Buff, string _fileName)
        : v4l2Buff(_v4l2Buff), fileName(_fileName)
    {
        mat.create(480, 640);
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
        if(fileName != ""){
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
                cv::resize(frame, frame, cv::Size(640, 480), 0, 0, cv::INTER_LINEAR);
                mat.copyFrom(frame.data);
//                printf("[Writer]: Addr Of Ptr = %p\n", mat.Ptr);
                v4l2Buff.push1(mat);
//                printf("[Writer]: Addr Of Ptr = %p\n", mat.Ptr);
//                std::cout << "[Writer]: v4l2Buff Size = " << v4l2Buff.size_.load() << "\n";
                std::this_thread::sleep_for(chrono::milliseconds(10));
                stop = std::chrono::high_resolution_clock::now();
//                std::cout << "Time read = " << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count()/1000.0 << std::endl;

            }
        }

    }
public:
    cv::VideoCapture cap;
    std::string fileName;
    Matrix<unsigned char> mat;
    LockBuffer<Matrix<unsigned char>>& v4l2Buff;
};
#endif // WRITER_H
