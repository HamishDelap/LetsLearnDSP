#pragma once
#include <memory>
#include <cstddef>

using namespace std;

template <class T>
class RingBuffer{
public:
	explicit RingBuffer(size_t size) :
		buffer(std::unique_ptr<T[]>(new T[size])),
		max_size(size)
    {

    }

	void put(T item) {

		buffer[head] = item;

		if(full) {
			tail = (tail + 1) % max_size;
		}

		head = (head + 1) % max_size;

		full = head == tail;
	}

    T get() {
        if(isEmpty()) {
            return T();
        }

        auto val = buffer[tail];
        full = false;
        tail = (tail + 1) % max_size;
        
        return val;
    }
    
    void reset(){
        head = tail;
        full = false;
    }
    
    bool isEmpty(){
        return (head == tail && !full);
    }
    
    bool isFull(){
        return (full);
    }
    
    size_t capacity() {
        return max_size;
    }
    
    size_t size(){
        size_t size = max_size;

        if (!full){
            if (head >= tail) {
                size = head - tail;
            } else {
                size = max_size + head - tail;
            }
        }

        return size;
    }

private:
    std::unique_ptr<T[]> buffer;
    size_t head = 0;
    size_t tail = 0;
    const size_t max_size;
    bool full = false;
};
