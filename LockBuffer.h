#ifndef LOCKBUFFER_H
#define LOCKBUFFER_H
#include "Matrix.h"
#include <atomic>
#include <array>

#define MAX_BUFFER_SIZE 20
template<typename T>
class LockBuffer{
public:
//    LockBuffer() = default;
    LockBuffer(const size_t& _maxsize, int _in, int _out)
        : max_size(_maxsize), n_in(_in), n_out(_out), read_pos{0}, write_pos{0}, size_{0}
    {
        assert(size_.is_lock_free());
        if(max_size > MAX_BUFFER_SIZE) max_size = MAX_BUFFER_SIZE;
        std::unique_ptr<T[]> arr(new T[max_size]);
        buffer = std::move(arr);

    }

    ~LockBuffer(){

    }
    size_t size() const {
        return size_.load();
    }

    void AllocatorBuffer(T&& tmp){
        for(int i = 0; i < max_size; i++){
            buffer[i].allocate(std::move(tmp));
        }

    }

    // check readers_active and push data
    // continue push to next place if buffer is full
    // this case, multi readers must wait and until they completely read a same place
    bool push1(T& new_value){
        if(size_.load() >= max_size){
            if(readers_active[latest_write_pos] > 0){
                return false;
            }
            else{
                writers_active[latest_write_pos].fetch_add(1);
                buffer[latest_write_pos] = new_value;
                writers_active[latest_write_pos].fetch_sub(1);
                return true;
            }
        }
//        while(readers_active[write_pos].load() > 0){
//            write_pos = (write_pos + 1) % max_size;
//            // some threads are still reading this write_pos mem
//            // check next postion to push
//        }
        // lock while writer push data
        // critical section
        writers_active[write_pos].fetch_add(1);

        buffer[write_pos] = new_value;
        size_.fetch_add(1);
        writers_active[write_pos].fetch_sub(1);
        latest_write_pos = write_pos;
        write_pos = (write_pos + 1) % max_size;
        return true;
        // end of critical section
        // free writers lock


    }

    // multi readers must read same data at the same time
    // they share the same read_pos
    bool front1(T& value, int& r_pos){
        while(size_.load() == 0){

        }
        while(writers_active[read_pos].load() > 0);
        if(r_pos == read_pos)
            return false;
        readers_active[read_pos].fetch_add(1);
        value = buffer[read_pos];
        r_pos = read_pos;
        return true;
        //read_flag.store(false);
    }
    // pop1 correspond to front1
    bool pop1(){
        if(readers_active[read_pos].load() == n_out){
            readers_active[read_pos].store(0);
            size_.fetch_sub(1);
            read_pos = (read_pos + 1) % max_size;
            return true;
           // read_flag.store(true);
        }
        return false;

    }

    void createReadPos(int& read_pos){
        read_pos = 0;
    }

    int createReadPos(){
        return 0;
    }

    // multi readers can read difference data at the same time
    void getLastestValue(T& value, int& read_pos){
        while(read_pos == latest_write_pos);
        read_pos = latest_write_pos;
        while(writers_active[read_pos] > 0){
        }
        readers_active[read_pos].fetch_add(1);
        value = buffer[read_pos];
        readers_active[read_pos].fetch_sub(1);
    }
    int capacity(){
        return max_size;
    }
public:
    int write_pos = 0;
    int read_pos = 0;
    int latest_write_pos = -1;
    int n_in, n_out;
    size_t max_size;
//    int max_size;
    std::atomic<size_t> size_{0};
//    std::shared_ptr<Matrix<unsigned char>> buffer[N];
    std::unique_ptr<T[]> buffer;
    std::array<std::atomic<int>, MAX_BUFFER_SIZE> readers_active;
    std::array<std::atomic<int>, MAX_BUFFER_SIZE> writers_active;
//    int read_pos[5];
//    int write_pos[5];

//    std::unique_ptr<T> buffer[N];
//    std::unique_ptr<Matrix<unsigned char>[]> arr(new Matrix<unsigned char>[10]);
//    std::shared_ptr<T> buffer[N];
};
#endif // LOCKBUFFER_H
