#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdio.h>
using namespace std;

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue(){

    }

    void push(T new_value){
        std:;shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value){
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]{return !data_queue.empty();});
//        printf("address of myqueue.front = %p\n", data_queue.front());
        value = std::move(*data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]{return !data_queue.empty();});
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value){
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop(){
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
//        printf("address of myqueue.front = %p\n", &data_queue.front());
//        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    bool empty() const{
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

};

class Class1{
public:
    Class1(){

    }
    void display(){
        std::cout << "Hello Class1\n";
    }
    std::shared_ptr<int> p = std::make_shared<int>(43);
};

int main()
{
    Class1 c1;
    threadsafe_queue<Class1> myqueue;
    printf("address of object c1 = %p\n", &c1);

    myqueue.push(c1);
    Class1 c2;
    printf("address of object c2 = %p\n", &c2);
    myqueue.wait_and_pop(c2);
    std::shared_ptr<Class1> c3 = myqueue.wait_and_pop();

    printf("address of object c3 = %p\n", c3.get());
    std::cout << c3.use_count() << "\n";
    printf("address of shared_ptr c3 = %p\n", &c3);
    printf("address of object c1 = %p\n", &c1);

//    c2 = std::move(c1);
    std::cout << *c1.p << "\n";
    return 0;
}
