#ifndef LABA1_22_POLYNOM_H
#define LABA1_22_POLYNOM_H

#include <iostream>
#include <vector>
using namespace std;


template<class T>
struct Polynom {
    Polynom() {
        data.resize(1);
    }

    Polynom(size_t size) {
        data.resize(size);
    }

    Polynom(vector<T> const & v) : data(v) {};

    Polynom(initializer_list<T> coeff) : data(coeff) {};


    Polynom(T const* const ptr, size_t size) {
        data.resize(size);
        for (size_t i = 0; i != size; i++)
            data[i] = ptr[i];
    }

    Polynom(Polynom<T> const& other) : data(other.data) {};

    Polynom<T>& operator=(Polynom<T> const& other) {
        data = other.data;
        return *this;
    }

    bool operator==(Polynom<T>& other) {
        remove_zeros();
        other.remove_zeros();
        if (data.size() != other.data.size())
            return false;
        for (size_t i = 0; i != data.size(); i++)
            if (data[i] != other.data[i])
                return false;
        return true;
    }

    bool operator!=(Polynom<T> const& other) {
        return !(this == other);
    }

    Polynom<T> operator-() const {
        Polynom<T> temp(this);
        for (size_t i = 0; i != data.size(); i++)
            temp.data[i] = -data[i];
        return temp;
    }

    Polynom<T>& operator+=(Polynom<T> const& other) {
        size_t max_size = max(data.size(), other.data.size());
        data.resize(max_size);
        for (size_t i = 0; i != max_size; i++)
            data[i] += i < other.data.size() ? other.data[i] : 0;
        return *this;
    }

    Polynom<T>& operator-=(Polynom<T> const& other) {
        size_t max_size = max(data.size(), other.data.size());
        data.resize(max_size);
        for (size_t i = 0; i != max_size; i++)
            data[i] -= i < other.data.size() ? other.data[i] : 0;
        return *this;
    }

    T operator[](size_t i) const {
        return data[i];
    }

    T& operator[](size_t i) {
        return data[i];
    }

    Polynom<T>& operator*=(Polynom<T> const& other) {
        vector<T> temp(data.size() + other.data.size());
        for (size_t i = 0; i != data.size(); i++)
            for (size_t j = 0; j != other.data.size(); j++)
                temp[i+j] += data[i] * other.data[j];
        data = temp;
        return *this;
    }

    Polynom<T>& operator/=(T val) {
        for (size_t i = 0; i != data.size(); i++)
            data[i] /= val;
        return *this;
    }

    friend istream& operator>>(istream& in, Polynom<T>& poly) {
        for (size_t i = 0; i != poly.data.size(); i++)
            in >> poly.data[i];
        return in;
    }

    friend ostream& operator<<(ostream& out, Polynom<T>& poly) {
        out << poly.data[0];
        for (size_t i = 1; i != poly.data.size(); i++) {
            if (poly.data[i]) {
                out << ((poly.data[i] < 0) ? " - " : " + ") << abs(poly.data[i]) << "x^" << i;
            }
        }
        return out;
    }


    ~Polynom() = default;

    size_t size() const {
        return data.size();
    }

    long long get_degree() const {
        return data.size() - 1;
    }

    void resize(size_t size) {
        data.resize(size);
    }

    void remove_zeros() {
        while (data.size() > 1 && data[data.size()-1] == 0) {
            data.pop_back();
        }
    }

//private:
    vector<T> data;
};

template<class T>
Polynom<T> operator*(Polynom<T> first, Polynom<T> const& second) {
    return first *= second;
}

template<class T>
Polynom<T> operator+(Polynom<T> first, Polynom<T> const& second) {
    return first += second;
}

template<class T>
Polynom<T> operator-(Polynom<T> first, Polynom<T> const& second) {
    return first -= second;
}

template<class T>
Polynom<T> operator/(Polynom<T> first, T val) {
    return first /= val;
}

template<class T>
void add_zeros(Polynom<T>& x, Polynom<T>& y)
{
    size_t mx = max(x.size(), y.size());
    size_t next_power_of_two = mx == 1 ? 1 : 1 << (32-__builtin_clz(mx-1)); //количество нулей до первой 1
    x.resize(next_power_of_two);
    y.resize(next_power_of_two);
}

template<class T>
Polynom<T> karatsuba(Polynom<T>& x, Polynom<T>& y)
{
    if (__builtin_popcount(x.size()) > 1 || __builtin_popcount(y.size()) > 1)
        add_zeros(x, y);
    size_t sz = x.size();
    Polynom<T> res(2*sz);

    if (x.size() <= 2) {
        return x * y;
    }
    size_t k = sz / 2;

    vector<T> vxr {x.data.begin(), x.data.begin() + k};
    vector<T> vxl {x.data.begin() + k, x.data.end()};
    vector<T> vyr {y.data.begin(), y.data.begin() + k};
    vector<T> vyl {y.data.begin() + k, y.data.end()};

    Polynom<T> xl(vxl), xr(vxr), yl(vyl), yr(vyr);

    Polynom<T> prod_left = karatsuba(xl, yl);
    Polynom<T> prod_right = karatsuba(xr, yr);

    xl += xr;
    yl += yr;

    Polynom<T> prod_sum = karatsuba(xl, yl);

    prod_sum -= prod_right;
    prod_sum -= prod_left;

    for (size_t i = 0; i < sz; i++)
        res.data[i] = prod_right.data[i];

    for (size_t i = sz; i < 2 * sz; i++)
        res.data[i] = prod_left.data[i-sz];

    for (size_t i = k; i < sz + k; i++)
        res.data[i] += prod_sum[i-k];

    return res;
}

#endif LABA1_22_POLYNOM_H
