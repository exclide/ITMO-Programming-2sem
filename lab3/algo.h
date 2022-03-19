#ifndef LABA3_ALGO_H
#define LABA3_ALGO_H

namespace lab {

    template<class It, class Fn>
    bool all_of(It begin, It end, Fn f) {
        for ( ; begin != end; ++begin) {
            if (!f(*begin))
                return false;
        }
        return true;
    }

    template<class It, class Fn>
    bool any_of(It begin, It end, Fn f) {
        for ( ; begin != end; ++begin) {
            if (f(*begin))
                return true;
        }
        return false;
    }

    template<class It, class Fn>
    bool none_of(It begin, It end, Fn f) {
        for ( ; begin != end; ++begin) {
            if (f(*begin))
                return false;
        }
        return true;
    }

    template<class It, class Fn>
    bool one_of(It begin, It end, Fn f) {
        bool found = false;
        for ( ; begin != end; ++begin) {
            if (f(*begin)) {
                if (!found) {
                    found = true;
                }
                else {
                    return false;
                }
            }
        }
        return found;
    }

    template<class It, class Fn>
    bool is_sorted(It begin, It end, Fn f) {
        if (begin == end)
            return true;
        for ( ; begin != --end; ++begin) {
            if (!f(*begin, *next(begin)))
                return false;
        }
        return true;
    }

    template<class It, class Fn>
    bool is_partitioned(It begin, It end, Fn f) {
        for ( ; begin != end; ++begin) {
            if (!f(*begin))
                break;
        }
        for ( ; begin != end; ++begin) {
            if (f(*begin))
                return false;
        }
        return true;
    }

    template<class It, class T>
    It find_not(It begin, It end, T& val) {
        for ( ; begin != end; ++begin) {
            if (*begin != val)
                return begin;
        }
        return end;
    }

    template<class It, class T>
    It find_backward(It begin, It end, T& val) {
        auto it_end = end;
        if (begin == end)
            return it_end;
        for ( ; --end != begin; --end) {
            if (*end == val)
                return end;
        }
        return it_end;
    }

    template<class It, class Fn>
    bool is_palindrome(It begin, It end, Fn f) {
        if (begin == end)
            return true;
        for ( ; begin != --end; ++begin, --end) {
            if (!f(*begin, *end))
                return false;
        }
        return true;
    }


}


#endif //LABA3_ALGO_H
