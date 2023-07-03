#ifndef LABA6_CONSTEXPR_H
#define LABA6_CONSTEXPR_H

template<int ...Args>
class Polynom {
public:
    constexpr long long Pow(int x, int pwr) const {
        #if GOOGLE_TEST == 0
        if (std::is_constant_evaluated()) {}
        else std::cerr << "Function \"Pow\" evaluated at run-time\n";
        #endif

        return pwr == 0 ? 1 : x * Pow(x, pwr-1);
    }

    constexpr long long FastPow(int x, int pwr) const {
        #if GOOGLE_TEST == 0
        if (std::is_constant_evaluated()) {}
        else std::cerr << "Function \"FastPow\" evaluated at run-time\n";
        #endif

        if (pwr == 0)
            return 1;
        long long tmp = FastPow(x, pwr >> 1);
        long long res = tmp * tmp;
        if (pwr % 2)
            res *= x;
        return res;
    }

    constexpr int GetAt(int x) const {
        #if GOOGLE_TEST == 0
        if (std::is_constant_evaluated()) {}
        else std::cerr << "Function \"GetAt\" evaluated at run-time\n";
        #endif

        long long res = 0;
        for (int i = 0; i < size_; i++) {
            res += data_[i] * FastPow(x, size_ - i - 1);
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Polynom<Args...>& poly) {
        #if GOOGLE_TEST == 0
        if (std::is_constant_evaluated()) {}
        else std::cerr << "Function \"ostream operator<<\" evaluated at run-time\n";
        #endif

        stream << "Polynom of degree " << size_ - 1 << ":\n";

        for (int i = 0; i < size_; i++) {
            if (data_[i] == 0) {
                continue;
            }
            else {
                stream << (data_[i] > 0 ? i ? " + " : "" : i ? " - " : "-");
                if ((size_ - i - 1) == 0) {
                    stream << abs(data_[i]);
                }
                else if ((size_ - i - 1) == 1) {
                    stream << (abs(data_[i]) == 1 ? "x" : std::to_string(abs(data_[i])) + "x");
                }
                else {
                    stream << (abs(data_[i]) == 1 ? "x^" + std::to_string(size_ - i - 1) : std::to_string(abs(data_[i]))
                        + "x^" + std::to_string(size_ - i - 1));
                }
            }
        }
        return stream;
    }
private:
    static constexpr std::array<int, sizeof...(Args)> data_ = { {Args...} };
    static constexpr int size_ = sizeof...(Args);
};

#endif
