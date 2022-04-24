#ifndef LABA5_SIMPLEFIXEDPOOL_H
#define LABA5_SIMPLEFIXEDPOOL_H
#include <iostream>


template<class T, size_t num_chunks = 65536>
class SimpleFixedPool {
private:

    struct Chunk {
        Chunk* next;
    };

    uint8_t* mem_start = nullptr;
    Chunk* next_free = nullptr;
    size_t chunk_size = sizeof(T);

public:
    using value_type = T;
    using pointer = T*;

    template<class U>
    struct rebind {
        using other = SimpleFixedPool<U, num_chunks>;
    };

    SimpleFixedPool(SimpleFixedPool &&other) = delete;
    SimpleFixedPool(const SimpleFixedPool &other) = delete;
    SimpleFixedPool operator=(SimpleFixedPool &&other) = delete;
    SimpleFixedPool operator=(const SimpleFixedPool &other) = delete;

    SimpleFixedPool() {
        uint8_t* block_start = new uint8_t[num_chunks*(chunk_size)];
        mem_start = block_start;

        Chunk* start = reinterpret_cast<Chunk*>(block_start);
        for (int i = 0; i < num_chunks - 1; i++) {
            start->next = reinterpret_cast<Chunk*>(reinterpret_cast<uint8_t *>(start) + std::max(chunk_size, sizeof(Chunk)));
            start = start->next;
        }
        start->next = nullptr;

        next_free = reinterpret_cast<Chunk*>(block_start);
    }

    ~SimpleFixedPool() {
        delete[] mem_start;
        mem_start = nullptr;
    }

    pointer allocate(size_t n) {
        if (n != 1 || next_free == nullptr) {
            throw std::bad_alloc();
        }

        return reinterpret_cast<T*>(pop());
    }

    void deallocate(pointer ptr, size_t n) {
        if (n != 1) {
            throw std::bad_alloc();
        }

        push(reinterpret_cast<Chunk*>(ptr));
    }

    Chunk* pop() {
        Chunk* tmp = next_free;
        next_free = next_free->next;
        return tmp;
    }

    void push(Chunk* ptr) {
        ptr->next = next_free;
        next_free = ptr;
    }

};

#endif
