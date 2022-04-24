#ifndef LABA5_SPEEDTEST_H
#define LABA5_SPEEDTEST_H
#include <chrono>
#include <list>
#include <forward_list>
#include <set>

using namespace std;
using tclock = std::chrono::high_resolution_clock;
auto start = tclock::now();
auto end = tclock::now();
const size_t iters = 2000000;

using type = int;
using FastAlloc = ArenaFixedPool<type>;
using FixedAlloc = SimpleFixedPool<type, iters>;

#define FIXED_ALLOC 1

#if FIXED_ALLOC == 1
using CurAlloc = FixedAlloc;
#else
using CurAlloc = FastAlloc;
#endif

#define BEGIN(str) {cout << str; start = tclock::now();}
#define END cout << std::chrono::duration_cast<std::chrono::microseconds>(tclock::now() - start).count() << endl;
#define LOOP for (int i = 0; i < iters; i++)

struct CustomType {
    long long t;
};

void SpeedTest() {
#if FIXED_ALLOC == 0
    vector<type> vec1; vector<type, CurAlloc> vec2;
#endif
    list<type> list1; list<type, CurAlloc> list2;
    forward_list<type> flist1; forward_list<type, CurAlloc> flist2;
    set<type> set1; set<type, less<>, CurAlloc> set2;


#if FIXED_ALLOC == 0
    BEGIN("Vec Std: ") LOOP { vec1.push_back(i); } vec1.clear(); LOOP { vec1.push_back(i); } END
    BEGIN("Vec Pool: ") LOOP { vec2.push_back(i); } vec2.clear(); LOOP { vec2.push_back(i); } END
#endif

    BEGIN("List Std: ") LOOP { list1.push_back(i); } LOOP { list1.pop_back(); } LOOP { list1.push_back(i); } END
    BEGIN("List Pool: ") LOOP { list2.push_back(i); } LOOP { list2.pop_back(); } LOOP { list2.push_back(i); } END

    BEGIN("Forward List Std: ") LOOP { flist1.push_front(i); } LOOP { flist1.pop_front(); } LOOP { flist1.push_front(i); } END
    BEGIN("Forward List Pool: ") LOOP { flist2.push_front(i); } LOOP { flist2.pop_front(); } LOOP { flist2.push_front(i); } END

    BEGIN("Set Std: ") LOOP { set1.insert(i); }; set1.clear(); LOOP { set1.insert(i); } END
    BEGIN("Set Pool: ") LOOP { set2.insert(i); }; set2.clear(); LOOP { set2.insert(i); } END

    std::allocator<CustomType> alloc;
    SimpleFixedPool<CustomType, iters> fast_alloc;
    vector<CustomType*> vec_al;
    BEGIN("Custom Std: ") LOOP { CustomType* ptr = alloc.allocate(1); vec_al.push_back(ptr); } LOOP { alloc.deallocate(vec_al[i], 1); } END;
    vec_al.clear();
    BEGIN("Custom Pool: ") LOOP { CustomType* ptr = fast_alloc.allocate(1); vec_al.push_back(ptr); } LOOP { fast_alloc.deallocate(vec_al[i], 1); } END;



}

#endif
