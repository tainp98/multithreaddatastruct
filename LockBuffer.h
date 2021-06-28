#ifndef LOCKBUFFER_H
#define LOCKBUFFER_H
#include <atomic>
#include <memory>
#include <assert.h>
#include <iostream>
#include <vector>

template<typename T, int rows, int cols>
class Mat_{
  public:
    enum class format{
        GRAY = 0,
        YUV = 1
    };
    Mat_(){
        std::cout << "create Mat_ default constructor\n";
        size_ = rows*cols*sizeof(T);
        Ptr = (T*)malloc(size_);
        printf("Ptr add = %p\n", Ptr);
    }
    Mat_(int _rows, int _cols, format _fmt = format::GRAY)
        : fmt(_fmt)
    {
        size_ = _rows*_cols*sizeof(T);

        switch (fmt) {
        case format::GRAY :
            Ptr = (T*)malloc(size_);
            break;
        case format::YUV :
            Ptr = (T*)malloc(size_*1.5);
            break;
        }
    }
    ~Mat_(){
        if(Ptr != nullptr){
            std::cout << "delete Ptr\n";
            delete Ptr;
        }else{
            std::cout << "null Ptr\n";
        }
    }

public:
    T* Ptr = nullptr;
    format fmt;
    int size_;
};

template<typename T, size_t N>
class LockBuffer{
public:
    LockBuffer(int _in, int _out)
        : n_in(_in), n_out(_out), read_pos{0}, write_pos{0}, ref_count{0}, size_{0}
    {
        assert(size_.is_lock_free());
        assert(ref_count.is_lock_free());
        for(int i = 0; i < N; i++){
//            buffer[i] = std::make_shared<Mat_<unsigned char>>(480, 640, Mat_<unsigned char>::format::GRAY);
            buffer[i] = std::make_shared<T>();
            printf("object add = %p\n", buffer[i].get());

        }

    }

    ~LockBuffer(){

    }
    size_t size() const {
        return size_.load();
    }



private:
    size_t read_pos = 0;
    size_t write_pos = 0;
    int n_in, n_out;
    std::atomic<size_t> ref_count{0};
    std::atomic<size_t> size_{0};
//    std::shared_ptr<Mat_<unsigned char>> buffer[N];
    std::shared_ptr<T> buffer[N];
};
#endif // LOCKBUFFER_H
