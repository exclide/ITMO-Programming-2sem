#include "figures.h"

int main() {
    Point a(0, 0);
    Point b(0, 1);
    Point c(1, 1);
    Point d(1, 0);

    Triangle tri{a,b,c};
    Polyline line{a,b,c};
    Trapezoid trap{a,b,c,d};
    Polygon poly{a, b, c, d, {1, -1}, {0, -1}};
    Polygon poly2{{0,0}, {0,2},{2,3},{4,2},{5,0}};
    Polyline* ptr[5] = {&line, &tri, &trap, &poly,&poly2};

    for (int i = 0; i < 5; i++) {
        cout << "area: " << ptr[i]->get_area() << " perimeter: " << ptr[i]->get_perimeter()
            << " sides: " << ptr[i]->get_sides() << endl;
    }


    try {
        Polygon test{{0,0}, {0,1}, {0,2}, {0,3}};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }

    try {
        Polygon test{{0,0}, {2,2}, {3,0}, {0,2}};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }

    try {
        Polygon test{{0,0}, {2,2}, {3,0}, {0,-2}};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }


    try {
        Triangle gay_triangle{a,b,a};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }

    try {
        Trapezoid gay_trap{a,b,c,{1,-1}};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }

    try {
        Trapezoid gay_trap{a,b,c,{0.5,-1}};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }

    try {
        RegularPolygon regpoly{a,b,c,d};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }

    try {
        RegularPolygon regpoly{a,b,c, {1,-1}};
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << endl;
    }


    return 0;
}