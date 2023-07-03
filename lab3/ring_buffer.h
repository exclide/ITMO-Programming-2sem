#ifndef LABA3_RING_BUFFER_H
#define LABA3_RING_BUFFER_H


#include <iostream>
#include <memory>
#include <algorithm>

template <class T, class A = std::allocator<T>>
class RingBuffer {
public:
    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;
    template<bool isConst> class RingIterator;

    typedef RingIterator<false> iterator;
    typedef RingIterator<true> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    template<bool isConst = false>
    class RingIterator {
    public:
        typedef RingIterator<isConst> self_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename A::difference_type difference_type;
        typedef typename A::value_type value_type;
        typedef typename std::conditional<isConst, const value_type&, value_type&>::type reference;
        typedef typename std::conditional<isConst, const value_type*, value_type*>::type pointer;
        typedef typename std::conditional<isConst, const RingBuffer<value_type>*,
                RingBuffer<value_type>*>::type buf_ptr;


        RingIterator(buf_ptr ptr, size_type index, size_type size_cmp) :
        ibuff_(ptr), index_(index), size_cmp_(size_cmp) {}

        RingIterator(const iterator& other) :
        ibuff_(other.ibuff_), index_(other.index_), size_cmp_(other.size_cmp_) {}

        self_type& operator=(const iterator& other) {
            ibuff_ = other.ibuff_;
            index_ = other.index_;
            size_cmp_ = other.size_cmp_;
            return *this;
        }

        bool operator==(const iterator& x) const {
            return ibuff_ == x.ibuff_ && index_ == x.index_ && size_cmp_ == x.size_cmp_;
        }
        bool operator!=(const iterator& x) const {
            return !(operator==(x));
        }
        bool operator<(const iterator& x) const {
            return size_cmp_ > x.size_cmp_;
        }
        bool operator>(const iterator& x) const {
            return x.size_cmp_ > size_cmp_;
        }
        bool operator<=(const iterator& x) const {
            return !(x > *this);
        }
        bool operator>=(const iterator& x) const {
            return !(x < *this);
        }

        self_type& operator++() {
            index_ = (index_ + 1) % (*ibuff_).capacity();
            --size_cmp_;
            return *this;
        }

        self_type operator++(int) {
            self_type temp(*this);
            ++(*this);
            return temp;
        }

        self_type& operator--() {
            index_ = (index_ - 1 + (*ibuff_).capacity()) % (*ibuff_).capacity();
            ++size_cmp_;
            return *this;
        }

        self_type operator--(int) {
            self_type temp(*this);
            --(*this);
            return temp;
        }

        iterator& operator+=(size_type n) {
            index_ = (index_ + n) % (*ibuff_).capacity();
            size_cmp_ -= n;
            return *this;
        }
        iterator operator+(size_type n) const {
            self_type temp(*this);
            temp += n;
            return temp;
        }

        iterator& operator-=(size_type n) {
            index_ = (index_ - n + (*ibuff_).capacity()) % (*ibuff_).capacity();
            size_cmp_ += n;
            return *this;
        }

        iterator operator-(size_type n) const {
            self_type temp(*this);
            temp -= n;
            return temp;
        }

        difference_type operator-(self_type other) const {
            return abs(size_cmp_ - other.size_cmp_);
        }


        reference operator*() const { return (*ibuff_)[index_]; }
        pointer operator->() const { return &(operator*()); }


    private:
        buf_ptr ibuff_;
        size_type index_;
        size_type size_cmp_;
    };

    iterator begin() { return iterator(this, front_, size_); }
    const_iterator begin() const { return const_iterator(this, front_, size_); }
    const_iterator cbegin() const { return const_iterator(this, front_, size_); }
    iterator end() { return iterator(this, back_, 0); }
    const_iterator end() const { return const_iterator(this, back_, 0); }
    const_iterator cend() const { return const_iterator(this, back_, 0); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator((end())); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }


    allocator_type get_allocator() const { return alloc_; }

    RingBuffer(size_t capacity = 7, const allocator_type& alloc = allocator_type()) :
        size_(0), capacity_(capacity), front_(0), back_(0),
        alloc_(alloc), buff_(alloc_.allocate(capacity))
    {}

    RingBuffer(const RingBuffer& other) :
        size_(other.size_), capacity_(other.capacity_), front_(other.front_),
        back_(other.back_), alloc_(other.get_allocator()), buff_(alloc_.allocate(capacity_)) {
        for (size_type i = front_; i != back_; i = (i+1) % capacity_) {
            buff_[i] = other.buff_[i];
        }
    }

    RingBuffer& operator=(const RingBuffer& other) {
        if (other != this) {
            size_ = other.size_;
            capacity_ = other.capacity_;
            front_ = other.front_;
            back_ = other.back_;
            alloc_ = other.get_allocator();
            buff_ = alloc_.allocate(capacity_);
            for (size_type i = front_; i != back_; i = (i + 1) % capacity_) {
                buff_[i] = other.buff_[i];
            }
        }
        return *this;
    }


    ~RingBuffer() {
        alloc_.deallocate(buff_, capacity_);
    }

    friend bool operator==(const RingBuffer& a, const RingBuffer& b) {
        if (a.size() != b.size()) {
            return false;
        }
        for (size_type i = a.front_; i != a.back_; i = (i+1) % a.capacity_) {
            if (a.buff_[i] != b.buff_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const RingBuffer& b) const {
        return !(*this == b);
    }


    reference front() {
        return buff_[front_];
    }
    const_reference front() const {
        return buff_[front_];
    }
    reference back() {
        return buff_[(back_ + capacity_ - 1) % capacity_];
    }
    const_reference back() const {
        return buff_[(back_ + capacity_ - 1) % capacity_];
    }

    void push_back(const value_type& a) {
        buff_[back_] = a;
        back_ = (back_ + 1) % capacity_;
        if (size_ == capacity_) {
            --size_;
            front_ = (front_ + 1) % capacity_;
        }
        ++size_;
    }

    void push_front(const value_type& a) {
        front_ = (front_ - 1 + capacity_) % capacity_;
        buff_[front_] = a;
        if (size_ == capacity_) {
            --size_;
            back_ = (back_ - 1 + capacity_) % capacity_;
        }
        ++size_;
    }

    void pop_front() {
        if (empty()) {
            throw std::length_error("Pop called on empty buffer");
        }
        --size_;
        front_ = (front_ + 1) % capacity_;
    }

    void pop_back() {
        if (empty()) {
            throw std::length_error("Pop called on empty buffer");
        }
        --size_;
        back_ = (back_ -1 + capacity_) % capacity_;
    }

    reference real_position(size_type i) const {
        if (i >= capacity() || i < 0) {
            throw std::out_of_range("Out of range access");
        }
        return buff_[i];
    }

    reference operator[](size_type i) {
        return real_position(i);
    }

    const_reference operator[](size_type i) const {
        return real_position(i);
    }
    reference at(size_type i) {
        return real_position(i);
    }
    const_reference at(size_type i) const {
        return real_position(i);
    }

    void swap(RingBuffer& a) {
        std::swap(size_, a.size_);
        std::swap(capacity_, a.capacity_);
        std::swap(front_, a.front_);
        std::swap(back_, a.back_);
        std::swap(buff_, a.buff_);
    }
    size_type size() const {
        return size_;
    }
    bool empty() const {
        return size_ == 0;
    }
    bool full() const {
        return size_ == capacity_;
    }
    size_type capacity() const {
        return capacity_;
    }
    void clear() {
        front_ = 0;
        back_ = 0;
        size_ = 0;
    }

    void change_capacity(size_t new_capacity) {
        RingBuffer<T> temp(new_capacity);
        for (auto& x: *this) {
            temp.push_back(x);
        }
        swap(temp);
    }

private:
    size_type size_;
    size_type capacity_;
    size_type front_;
    size_type back_;
    allocator_type alloc_;
    value_type* buff_;
};

#endif LABA3_RING_BUFFER_H
