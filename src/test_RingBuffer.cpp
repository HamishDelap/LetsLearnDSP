#include "RingBuffer.h"


int main() {

    size_t three = 3;
    RingBuffer<int> buffer(three);
    for (int i = 0; i < 1; i++) {
        buffer.put(i);
    }
    printf(""+buffer.get());
}
