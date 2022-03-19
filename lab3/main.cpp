#include <iostream>
#include "ring_buffer.h"
#include <algorithm>
#include "algo.h"
#include <vector>
#include <string>
#include "rational.h"

bool less_than(double x) {
    return x < 10;
}

template<class T>
bool less(T x, T y) {
    return x < y;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<Rational> vrational = {
            {5,3}, {1,2}, {8,1},
            {9, 2}, {6, 2}, {7, 5},
            {9, 8}, {1, 10}, {1, 100}
    };

    std::cout << lab::all_of(v.begin(), v.end(), less_than) << std::endl;
    std::cout << lab::is_sorted(v.begin(), v.end(), less<double>) << std::endl;

    std::cout << lab::is_sorted(vrational.begin(), vrational.end(), less<Rational>) << std::endl;
    std::sort(vrational.begin(), vrational.end());
    std::cout << lab::is_sorted(vrational.begin(), vrational.end(), less<Rational>) << std::endl;

    RingBuffer<int> buff(7);
    buff.push_back(10);
    buff.push_back(9);
    buff.push_back(8);
    buff.push_back(7);
    buff.push_back(6);
    buff.push_back(5);
    buff.push_back(4);

    buff.push_back(3);
    buff.push_back(2);
    buff.push_back(1);

    std::cout << buff.size() << std::endl;
    std::cout << buff.capacity() << std::endl;
    std::cout << buff.front() << " " << buff.back() << std::endl;
    buff.pop_front();
    for (auto it = buff.begin(); it != buff.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::sort(buff.begin(),buff.end());

    for (auto it = buff.begin(); it != buff.end(); it++) {
        std::cout << *it << " ";
    }


    RingBuffer<int> bf(7);
    std::string str;
    int x;
    std::cout << "push_front x\npush_back x\npop_back\npop_front\nclear\nresize x\n\n";
    while (std::cin >> str) {
        if (str == "push_front") {
            std::cin >> x;
            bf.push_front(x);
        }
        else if (str == "push_back") {
            std::cin >> x;
            bf.push_back(x);
        }
        else if (str == "pop_back") {
            bf.pop_back();
        }
        else if (str == "pop_front") {
            bf.pop_front();
        }
        else if (str == "clear") {
            bf.clear();
        }
        else if (str == "resize") {
            std::cin >> x;
            bf.change_capacity(x);
        }

        std::cout << "size: " << bf.size() << " capacity: " << bf.capacity()
                  << " front: " << bf.front() << " back: " << bf.back() << std::endl;

        for (auto& val : bf) {
            std::cout << val << " ";
        }

        std::cout << "\n\n";
    }
    return 0;
}
