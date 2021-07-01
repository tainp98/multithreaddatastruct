#ifndef LOCKBUFFER_H
#define LOCKBUFFER_H
#include <atomic>
#include <memory>
#include <assert.h>
#include <iostream>
#include <vector>
#include <exception>
#include <array>

template<typename T>
class Mat_ {
  public:
    enum class format{
        GRAY = 0,
        YUV = 1
    };
    Mat_() = default;

    /* Constructor Create Object
       _opt = 0 by default --> do not allocate memory for Ptr
       else --> allocate memory for Ptr */
    Mat_(int _rows, int _cols, int _opt = 0, format _fmt = format::GRAY)
        : rows(_rows), cols(_cols), fmt(_fmt)
    {
        switch (fmt) {
        case format::GRAY :
            size_ = _rows*_cols;
            break;
        case format::YUV :
            size_ = _rows*_cols*1.5;
            break;
        }
        if(_opt != 0)
            Ptr = new T[size_];
    }

    /* Constructor Create Object by pointer
       create ptr
       if _opt = 0 by default --> get ptr data directly
       else --> copy if it can or get ptr data directly */
    Mat_(int _rows, int _cols, void* data, int _opt = 0, format _fmt = format::GRAY)
        : rows(_rows), cols(_cols), fmt(_fmt)
    {
        int data_size;
        // get size
        switch (fmt) {
        case format::GRAY :
            data_size = _rows*_cols;
            break;
        case format::YUV :
            data_size = _rows*_cols*1.5;
            break;
        }

        if(_opt == 0){
            delete [] Ptr;
            size_ = data_size;
            Ptr = data;
        }
        else{
            if(this->size_ == data_size && this->Ptr !=nullptr)
                std::copy(data, data+size_, this->Ptr);
            else{
                if(this->Ptr == nullptr){
                    size_ = data_size;
                    this->Ptr = new T[size_];
                    std::copy(data, data+size_, this->Ptr);
                }
                else{
                    delete [] this->Ptr;
                    size_ = data_size;
                    this->Ptr = new T[size_];
                    std::copy(data, data+size_, this->Ptr);
                }
            }
        }
    }

    // Destructor
    ~Mat_(){
        if(Ptr != nullptr){
            std::cout << "delete [] Ptr\n";
            delete [] Ptr;
            Ptr = nullptr;
        }else{
            std::cout << "null Ptr\n";
        }
    }

    // Copy Constructor
    Mat_(const Mat_& other){
        try{
            std::cout << "====> Copy Constructor\n";
            if(other.Ptr == nullptr)
                throw std::bad_alloc();
            rows = other.rows;
            cols = other.cols;
            fmt = other.fmt;

            if(this->size_ == other.size_ && this->Ptr !=nullptr)
                std::copy(other.Ptr, other.Ptr+size_, this->Ptr);
            else{
                if(this->Ptr == nullptr){
                    std::cout << "==>\n";
                    size_ = other.size_;
                    this->Ptr = new T[size_];
                    std::copy(other.Ptr, other.Ptr+size_, this->Ptr);
                }
                else{
                    delete [] this->Ptr;
                    size_ = other.size_;
                    this->Ptr = new T[size_];
                    std::copy(other.Ptr, other.Ptr+size_, this->Ptr);
                }
            }
        }
        catch(std::exception& e){
            std::cout << e.what() << ": nullptr input" << std::endl;
        }
    }

    // Copy Assignment
    auto operator =(const Mat_& other) -> decltype(*this){
        try{
            std::cout << "====> Copy Assignment\n";
            if(other.Ptr == nullptr){
                throw std::bad_alloc();
            }
            rows = other.rows;
            cols = other.cols;
            fmt = other.fmt;
            if(this->size_ == other.size_ && this->Ptr !=nullptr)
                std::copy(other.Ptr, other.Ptr+size_, this->Ptr);
            else{
                if(this->Ptr == nullptr){
                    std::cout << "==>\n";
                    size_ = other.size_;
                    this->Ptr = new T[size_];
                    std::copy(other.Ptr, other.Ptr+size_, this->Ptr);
                }
                else{
                    delete [] this->Ptr;
                    size_ = other.size_;
                    this->Ptr = new T[size_];
                    std::copy(other.Ptr, other.Ptr+size_, this->Ptr);
                }
            }
        }
        catch(std::exception& e){
            std::cout << e.what() << ": nullptr input\n";
        }
        return *this;
    }

    // Move Constructor
    Mat_(Mat_&& other) noexcept
        : rows(other.rows), cols(other.cols), fmt(other.fmt), size_{other.size_}, Ptr{other.Ptr} {
        std::cout << "====> Move Constructor\n";
        other.Ptr = nullptr;
        other.size_ = 0;
        other.rows = 0;
        other.cols = 0;
        other.fmt = format::GRAY;
    }

    // Move Assignment
    Mat_& operator =(Mat_&& other) noexcept{
        std::cout << "====> Move Assignment\n";
        rows = other.rows;
        cols = other.cols;
        fmt = other.fmt;
        Ptr = other.Ptr;
        size_ = other.size_;
        other.Ptr = nullptr;
        other.size_ = 0;
        other.rows = 0;
        other.cols = 0;
        other.fmt = format::GRAY;
        return *this;
    }

    // allocate function
    void allocate(Mat_&& other){
        rows = other.rows;
        cols = other.cols;
        fmt = other.fmt;

        switch (fmt) {
        case format::GRAY :
            size_ = rows*cols;
            break;
        case format::YUV :
            size_ = rows*cols*1.5;
            break;
        }
        Ptr = new T[size_];
    }

    // overlap another allocate function
    void allocate(){
        if(size_ > 0){
            delete [] Ptr;
            Ptr = new T[size_];
        }
    }

public:
    T* Ptr = nullptr;
    format fmt;
    int size_{0};
    int rows{0};
    int cols{0};
};

template<typename T, int max_size>
class LockBuffer{
public:
    LockBuffer() = default;
    LockBuffer(int _max_size, int _in, int _out)
        : n_in(_in), n_out(_out), read_pos{0}, write_pos{0}, ref_count{0}, size_{0}
    {
        assert(size_.is_lock_free());
        assert(ref_count.is_lock_free());
        std::unique_ptr<T[]> arr(new T[max_size]);
        buffer = std::move(arr);
//        for(int i = 0; i < N; i++){
////            buffer[i] = std::make_shared<Mat_<unsigned char>>(480, 640, Mat_<unsigned char>::format::GRAY);
//            buffer[i] = std::make_shared<T>();
//            printf("object add = %p\n", buffer[i].get());

//        }

    }

    ~LockBuffer(){

    }
    size_t size() const {
        return size_.load();
    }

    void Allocator(T&& tmp){
        std::cout << "Allocator\n";
        for(int i = 0; i < max_size; i++){
            std::cout << i << "\n";
            buffer[i].allocate(std::move(tmp));
        }

    }

    void push(T& new_value){
        if(size_.load() >= max_size){
            if(readers_active[previous_write_pos] > 0){
                return;
            }
            else{
                writers_active[previous_write_pos].fetch_add(1);
                buffer[previous_write_pos] = new_value;
                writers_active[previous_write_pos].fetch_sub(1);
            }
            return;
        }
        while(readers_active[write_pos].load() > 0){
            write_pos = (write_pos + 1) % max_size;
            // some threads are still reading this write_pos mem
            // check next postion to push
        }
        // lock while writer push data
        // critical section
        writers_active[write_pos].fetch_add(1);

        buffer[write_pos] = new_value;
        previous_write_pos = write_pos;
        write_pos = (write_pos + 1) % max_size;
        // end of critical section
        // free writers lock
        writers_active[previous_write_pos].fetch_sub(1);
        size_.fetch_add(1, std::memory_order_relaxed);
    }

    void front(T& value, int& read_pos){
        while(size_.load() == 0){

        }
        while(writers_active[read_pos].load() > 0);
        readers_active[read_pos].fetch_add(1);
        value = buffer[read_pos];
    }

    void pop(int& read_pos){
        if(readers_active[read_pos].load() == n_out){
            readers_active[read_pos].store(0);
            size_.fetch_sub(1);
        }
        read_pos = (read_pos + 1) % max_size;
    }

    void createReadPos(int& read_pos){
        read_pos = 0;
    }

    int createReadPos(){
        return 0;
    }

    void getLastestValue(T& value){
        while(size_.load() == 0){
        }
        while(writers_active[previous_write_pos] > 0){
        }
        readers_active[previous_write_pos].fetch_add(1);
        value = buffer[previous_write_pos];
        readers_active[previous_write_pos].fetch_sub(1);
    }
private:
    size_t write_pos = 0;
    size_t read_pos = 0;
    size_t previous_write_pos = 0;
    int n_in, n_out;
//    int max_size;
    std::atomic<size_t> size_{0};
//    std::shared_ptr<Mat_<unsigned char>> buffer[N];
    std::unique_ptr<T[]> buffer;
    std::array<std::atomic<int>, max_size> readers_active;
    std::array<std::atomic<int>, max_size> writers_active;
//    int read_pos[5];
//    int write_pos[5];

//    std::unique_ptr<T> buffer[N];
//    std::unique_ptr<Mat_<unsigned char>[]> arr(new Mat_<unsigned char>[10]);
//    std::shared_ptr<T> buffer[N];
};
#endif // LOCKBUFFER_H
