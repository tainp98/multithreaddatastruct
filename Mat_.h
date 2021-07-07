#ifndef MAT__H
#define MAT__H

#include <memory>
#include <assert.h>
#include <iostream>
#include <vector>
#include <exception>


template<typename T>
class Matrix {
  public:
    enum class format{
        GRAY = 0,
        YUV = 1
    };
    Matrix() = default;

    /* Constructor Create Object
       _opt = 0 by default --> do not allocate memory for Ptr
       else --> allocate memory for Ptr */
    Matrix(int _rows, int _cols, int _opt = 0, format _fmt = format::GRAY)
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
    Matrix(int _rows, int _cols, void* data, int _opt = 0, format _fmt = format::GRAY)
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
    ~Matrix(){
        if(Ptr != nullptr){
            delete [] Ptr;
            Ptr = nullptr;
        }
    }

    // Copy Constructor
    Matrix(const Matrix& other){
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
    auto operator =(const Matrix& other) -> decltype(*this){
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
    Matrix(Matrix&& other) noexcept
        : rows(other.rows), cols(other.cols), fmt(other.fmt), size_{other.size_}, Ptr{other.Ptr} {
        std::cout << "====> Move Constructor\n";
        other.Ptr = nullptr;
        other.size_ = 0;
        other.rows = 0;
        other.cols = 0;
        other.fmt = format::GRAY;
    }

    // Move Assignment
    Matrix& operator =(Matrix&& other) noexcept{
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
    void allocate(Matrix&& other){
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
#endif // MAT__H
