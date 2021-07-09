#include "Matrix.h"
#include "LockBuffer.h"
#include "Writer.h"
#include "Reader1.h"
#include "Reader2.h"
#include "Reader3.h"
#include <thread>

int main(int argc, char* argv[]){
    LockBuffer<Matrix<unsigned char>> buffer(4, 1, 3);
    buffer.AllocatorBuffer(Matrix<unsigned char>(480, 640));
//    std::cout << "max buffer size = " << buffer.capacity() << "\n";
//    for(int i = 0; i < buffer.capacity(); i++){
//        printf("Addr Of buffer[%d] = %p\n", i, buffer.buffer[i].Ptr);
//        printf("rows = %d, cols = %d, size = %d\n", buffer.buffer[i].rows, buffer.buffer[i].cols, buffer.buffer[i].size_);
//    }
    printf("Addr Of buffer = %p\n", &buffer);
    std::string file_name = "";
    if(argc > 1)
        file_name = std::string(argv[1]);
    Writer writer(buffer, file_name);
    printf("Addr Of v4l2buffer = %p\n", &writer.v4l2Buff);
    std::thread t_writer(&Writer::run, &writer);
    Reader1 reader1(buffer);
    std::thread t_reader1(&Reader1::run, &reader1);
    Reader2 reader2(buffer);
    std::thread t_reader2(&Reader2::run, &reader2);
    Reader3 reader3(buffer);
    std::thread t_reader3(&Reader3::run, &reader3);
    t_writer.join();
    t_reader1.join();
    t_reader2.join();
    t_reader3.join();
}
