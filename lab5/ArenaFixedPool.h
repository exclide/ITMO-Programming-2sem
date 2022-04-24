#ifndef LABA5_ARENAFIXEDPOOL_H
#define LABA5_ARENAFIXEDPOOL_H
#include <vector>
#include <cstdlib>
#include <memory>

template<class T, size_t max_alloc_bytes = 1 << 27>
class ArenaFixedPool {

    class Arena {
    private:

        struct Chunk {
            Chunk* next;
        };

        uint8_t* mem_start = nullptr;
        Chunk* next_free = nullptr;
        size_t chunk_size = 0;
        size_t num_chunks = 0;

    public:
        Arena(size_t chunk_size, size_t block_size) : chunk_size(chunk_size), num_chunks(block_size / chunk_size) {
            uint8_t* block_start = new uint8_t[block_size];
            //printf("block sz: %d chunk sz: %d num chunks: %d\n", block_size, chunk_size, num_chunks);
            mem_start = block_start;

            Chunk* start = reinterpret_cast<Chunk*>(block_start);
            for (int i = 0; i < num_chunks - 1; i++) {
                start->next = reinterpret_cast<Chunk*>(reinterpret_cast<uint8_t*>(start) + chunk_size);
                start = start->next;
            }

            start->next = nullptr;

            next_free = reinterpret_cast<Chunk*>(block_start);

        }

        ~Arena() {
            delete[] mem_start;
            mem_start = nullptr;
        }

        void* allocate() {
            if (next_free == nullptr) {
                std::cerr << "next_free == nullptr";
                throw std::bad_alloc();
            }

            return reinterpret_cast<void*>(pop());
        }

        void deallocate(void* ptr, size_t n = -1) {
            push(reinterpret_cast<Chunk*>(ptr));
        }

        bool IsAdrFromArena(void* ptr) {
            return (uint8_t*)ptr >= mem_start && (uint8_t*)ptr < mem_start + chunk_size * num_chunks;
        }

        bool IsFull() {
            return next_free == nullptr;
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


    std::vector<Arena*> arenas;
    size_t max_arenas;

    uint32_t NextPowerOfTwo(size_t bytes) {
        int i = 3;
        while (bytes > (1ll << i))
            i++;

        return i;
    }

public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;

    template<class U>
    struct rebind {
        using other = ArenaFixedPool<U, max_alloc_bytes>;
    };

    template <class U>
    constexpr ArenaFixedPool (const ArenaFixedPool <U, max_alloc_bytes>& other) = delete;


    ArenaFixedPool(ArenaFixedPool<T, max_alloc_bytes> &&other) = delete;
    ArenaFixedPool(const ArenaFixedPool<T, max_alloc_bytes> &other) = delete;
    ArenaFixedPool operator=(ArenaFixedPool<T, max_alloc_bytes> &&other) = delete;
    ArenaFixedPool operator=(const ArenaFixedPool<T, max_alloc_bytes> &other) = delete;

    size_t max_size() {
        return 1 << max_arenas;
    }

    ArenaFixedPool() {
        max_arenas = NextPowerOfTwo(max_alloc_bytes);

        for (int i = 3; i <= max_arenas; i++) {
            Arena* tmp = new Arena(1ll << i, 1 << max_arenas);
            arenas.push_back(tmp);
        }
    }

    ~ArenaFixedPool() {
        for (auto arena : arenas) {
            arena->~Arena();
            delete arena;
        }
    }

    void* Allocate(size_t bytes) {
        int i = NextPowerOfTwo(bytes);
        i -= 3;

        if (i > max_arenas) {
            std::cerr << "i > max_arenas\n";
            throw std::bad_alloc();
        }

        while (i < max_arenas && arenas[i]->IsFull()) {
            i++;
        }

        if (i == max_arenas) {
            std::cerr << "i == max_arenas\n";
            throw std::bad_alloc();
        }

        void* tmp = arenas[i]->allocate();

        if (tmp == nullptr) {
            std::cerr << "tmp is nullptr\n";
            throw std::bad_alloc();
        }

        return tmp;
    }

    void Deallocate(void* ptr, size_t bytes = -1) {
        for (int i = 0; i < max_arenas; i++) {
            if (arenas[i]->IsAdrFromArena(ptr)) {
                arenas[i]->deallocate(ptr);
                return;
            }
        }

        std::cerr << "Dealloc throw\n";
        throw std::bad_alloc();
    }

    pointer allocate(size_t n) {
        //printf("size t: %d n: %d\n", sizeof(T), n);
        return reinterpret_cast<pointer>(Allocate(n*sizeof(T)));
    }

    void deallocate(pointer ptr, size_t bytes) {
        Deallocate(ptr, bytes);
    }

};

template <class T, class U>
bool operator==(const ArenaFixedPool <T>&, const ArenaFixedPool <U>&) { return true; }
template <class T, class U>
bool operator!=(const ArenaFixedPool <T>&, const ArenaFixedPool <U>&) { return false; }

#endif
