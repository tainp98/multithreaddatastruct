#include "Matrix.h"
#include "LockBuffer.h"
#include "Writer.h"
#include "Reader1.h"
#include "Reader2.h"

int main(int argc, char* argv[]){
    LockBuffer<Matrix<unsigned char>> buffer(10, 1, 2);
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
    writer.run();
    Reader1 reader1(buffer);
    Reader2 reader2(buffer);
    reader1.run();
    reader2.run();



}
