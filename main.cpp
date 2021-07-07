//#include <iostream>
//#include <queue>
//#include <thread>

//#include <mutex>
//#include <condition_variable>
//#include <stdio.h>
//#include <atomic>
//#include <assert.h>
//#include "LockBuffer.h"
//#include <memory>
//#include <chrono>
//#include <cstring>
//#include <array>
//#include <vector>


//using namespace std;

//template<typename T>
//class threadsafe_queue
//{
//private:
//    mutable std::mutex mut;
//    std::queue<std::shared_ptr<T>> data_queue;
//    std::condition_variable data_cond;
//public:
//    threadsafe_queue(){

//    }

//    void push(T new_value){
//        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
//        std::lock_guard<std::mutex> lk(mut);
//        data_queue.push(data);
//        data_cond.notify_one();
//    }

//    void wait_and_pop(T& value){
//        std::unique_lock<std::mutex> lk(mut);
//        data_cond.wait(lk, [this]{return !data_queue.empty();});
////        printf("address of myqueue.front = %p\n", data_queue.front());
//        value = std::move(*data_queue.front());
//        data_queue.pop();
//    }

//    std::shared_ptr<T> wait_and_pop(){
//        std::unique_lock<std::mutex> lk(mut);
//        data_cond.wait(lk, [this]{return !data_queue.empty();});
//        std::shared_ptr<T> res = data_queue.front();
//        data_queue.pop();
//        return res;
//    }

//    bool try_pop(T& value){
//        std::lock_guard<std::mutex> lk(mut);
//        if(data_queue.empty())
//            return false;
//        value = std::move(*data_queue.front());
//        data_queue.pop();
//        return true;
//    }

//    std::shared_ptr<T> try_pop(){
//        std::lock_guard<std::mutex> lk(mut);
//        if(data_queue.empty())
//            return std::shared_ptr<T>();
////        printf("address of myqueue.front = %p\n", &data_queue.front());
////        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
//        std::shared_ptr<T> res = data_queue.front();
//        data_queue.pop();
//        return res;
//    }

//    bool empty() const{
//        std::lock_guard<std::mutex> lk(mut);
//        return data_queue.empty();
//    }

//};

//class Class1{
//public:
//    Class1(){

//    }
//    void display(){
//        std::cout << "Hello Class1\n";
//    }
//    std::shared_ptr<int> p = std::make_shared<int>(43);
//};

////std::atomic<long long> data{10};
//std::array<long long, 5> return_values{};
//std::atomic_int32_t data{10};



//void do_work(int thread_num){
//    long long value = data.fetch_add(1, std::memory_order_relaxed);
//    return_values[thread_num] = value;
//}

//int create_read_pos(){
//    return 0;
//}

//void push(int& value){
//    value = (value+1) %10;
//}
//int main()
//{
////    Matrix<int> m1(480, 640, Matrix<int>::format::GRAY);
////    int *data, *data1;
////    data = (int*)malloc(640*480*sizeof(int));
////    data1 = (int*)malloc(640*480*sizeof(int));
////    for(int i = 0; i < 480; i++){
////        for(int j = 0; j < 640; j++){
////            data[i*640 + j] = i+j;
////        }
////    }
////    chrono::high_resolution_clock::time_point start, stop;
////    start = chrono::high_resolution_clock::now();
//////    memcpy(data, img.data, img.rows*img.cols*sizeof(unsigned char));

////    std::memcpy(m1.Ptr, data, 640*480*sizeof(int));
////    stop = chrono::high_resolution_clock::now();
////    cout << "memcpy time = " << chrono::duration_cast<chrono::microseconds>(stop - start).count() << endl;
////    cout << "size_ Matrix = " << m1.size_ << endl;

////    Matrix<unsigned char>* m2;

//    LockBuffer<Matrix<unsigned char>> buf(10, 1, 2);
////
//    buf.Allocator(Matrix<unsigned char>(100,100));

////    cout << "size_ t = " << t.size_ << "\n";
//    std::cout << "=======\n";
//    std::array<std::atomic<int>, 10> arr={};
//    for(int i = 0; i < arr.size(); i++){
//        arr[i].store(0);
//    }
//    for(int i = 0; i < arr.size(); i++){
//        arr[i].fetch_add(1);
//    }
//    arr[1].fetch_add(1);
//    for(int i = 0; i < arr.size(); i++){
//        std::cout << "arr[" << i << "] = " << arr[i] << endl;
//    }

//    int i = 0;
//    cout << "arr[i] = " << arr[++i] << endl;
//    cout << "i = " << i << endl;
//    vector<int> a;

//    int read_pos = create_read_pos();
//    push(read_pos);
//    cout << "read_pos = " << read_pos << endl;
//    push(read_pos);
//    cout << "read_pos = " << read_pos << endl;
//    push(read_pos);
//    cout << "read_pos = " << read_pos << endl;
//    push(read_pos);
//    cout << "read_pos = " << read_pos << endl;



////    std::unique_ptr<Matrix<unsigned char>[]> arr(new Matrix<unsigned char>[10]);
////    std::unique_ptr<Matrix<unsigned char>[]> arr1, arr2;
////    Matrix<unsigned char> m1;
////    m1 = Matrix<unsigned char>(100, 100);
////    for(int i = 0; i < 10; i++){

////        arr[i] = m1;
////    }
//////    arr1 = std::make_unique<Matrix<unsigned char>[10]>();
////    arr2 = std::move(arr);
////    if(arr == nullptr)
////        cout << "null\n";
////    for(int i = 0; i < 10; i++){
////        cout << "size_ = " << arr2[i].size_ << endl;
////    }

////    m2 = new Matrix<unsigned char>[10];
////    arr(new Matrix<unsigned char>[10]);



////    delete [] m2;

////    std::cout << data.is_lock_free() << endl;
////    assert(data.is_lock_free());
////    std::thread th0{do_work, 0};
////    std::thread th1{do_work, 1};
////    std::thread th2{do_work, 2};
////    std::thread th3{do_work, 3};
////    std::thread th4{do_work, 4};
////    th0.join();
////    th1.join();
////    th2.join();
////    th3.join();
////    th4.join();

////    std::cout << "Result : " << data << '\n';

////      for (long long val : return_values) {
////          std::cout << "Seen return value : " << val << std::endl;
////      }



//    return 0;
//}
