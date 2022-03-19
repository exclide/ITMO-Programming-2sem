#include "polynom.h"
#include <cassert>

int main() {
    Polynom<int> p1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    Polynom<int> p2{9, 8, 7, 6, 5, 4, 3, 2, 1};
    Polynom<int> prod_norm = p1*p2;
    Polynom<int> prod_karatsuba = karatsuba(p1, p2);
    cout << prod_norm << endl;
    cout << prod_karatsuba << endl;

    assert(prod_norm == prod_karatsuba);

    p1 = {4, 5, 2, 3, 5, 1, 1, 1, 16, 17, 0, 61, 0, 0, 13, 156, 1, 0, 166};
    p2 = {6, 12, 51, 62, 15, 25, 26, 26, 15, 11, 14, 15, 16, 29, 95, 15};
    prod_norm = p1*p2;
    prod_karatsuba = karatsuba(p1,p2);

    cout << prod_norm << endl;
    cout << prod_karatsuba << endl;

    assert(prod_norm == prod_karatsuba);


    Polynom<double> poly1(5);
    Polynom<double> poly2(4);
    cin >> poly1;
    cin >> poly2;
    Polynom<double> product = poly1 * poly2;
    Polynom<double> sum = poly1 + poly2;
    cout << "product: " << product << endl;
    cout << "sum: " << sum << endl;
    return 0;
}
