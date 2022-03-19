#ifndef LABA1_FIGURES_H
#define LABA1_FIGURES_H

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
using namespace std;

struct Point {
    Point() : x(0), y(0) {};
    Point(double x, double y) : x(x), y(y) {};
    //Point(Point const& other) = default;
    //Point& operator=(Point const& other) = default;

    double get_x() const {
        return x;
    }
    double get_y() const {
        return y;
    }

private:
    double x;
    double y;
};

struct Polyline {
    //Polyline() = default;
    //Polyline(Polyline const& other) = default;
    Polyline(initializer_list<Point> p_list) : verticles(p_list) {};
    Polyline(vector<Point> const& verticles) : verticles(verticles) {};

    template<class ...T>
    Polyline(T& ...args) : verticles(vector<Point>{args...}) {};

    Point get_point(size_t i) const {
        return verticles[i];
    }
    virtual size_t get_sides() const {
        return verticles.size()-1;
    }

    pair<double,double> get_line(const Point& x, const Point& y) const {
        return make_pair(y.get_x() - x.get_x(), y.get_y() - x.get_y());
    }

    double determinant(const Point& x, const Point& y, const Point& z, const Point& a) const {
        pair<double,double> line1 = get_line(x, y);
        pair<double,double> line2 = get_line(z, a);
        return line1.first * line2.second - line1.second*line2.first;
    }

    double get_distance(const Point& x, const Point& y) const {
        pair<double,double> line = get_line(x,y);
        return sqrt(pow(line.first, 2) + pow(line.second, 2));
    }

    double ccw(Point& a, Point& b, Point& c) {
        return (c.get_y() - a.get_y()) * (b.get_x() - a.get_x()) > (b.get_y() - a.get_y()) * (c.get_x() - a.get_x());
    }

    bool intersect(Point&& a, Point&& b, Point&& c, Point&& d) {
        return ccw(a, c, d) != ccw(b, c, d)
               && ccw(a, b, c) != ccw(a, b, d);
    }

    virtual double get_perimeter() const {
        double sum = 0;
        for (size_t i = 0; i != verticles.size()-1; i++) {
            sum += get_distance(verticles[i], verticles[i+1]);
        }
        return sum;
    }

    virtual double get_area() const {
        return 0;
    }

private:
    vector<Point> verticles;
};

struct ClosedPolyline : public Polyline {
    using Polyline::Polyline;

    size_t get_sides() const override {
        return Polyline::get_sides()+1;
    }

    double get_perimeter() const override {
        Point first = get_point(0);
        Point second = get_point(get_sides()-1);
        return Polyline::get_perimeter() + get_distance(first, second);
    }

};

struct Polygon : public ClosedPolyline {
    Polygon(initializer_list<Point> p_list) : ClosedPolyline(p_list) {
        check_intersect();
    }
    Polygon(vector<Point> const& verticles) : ClosedPolyline(verticles) {
        check_intersect();
    }

    template<class ...T>
    Polygon(T& ...args) : ClosedPolyline(args...) {
        check_intersect();
    }

    bool polygon_intersect() {
        size_t sz = ClosedPolyline::get_sides();
        for (int i = 0; i < sz - 1; i++) {
            for (int j = i+1; j < sz; j++) {
                if (intersect(get_point(i), get_point(i+1), get_point(j), get_point((j+1)%sz)))
                    return true;
            }
        }
        return false;
    }

    void check_intersect() {
        if (polygon_intersect()) {
            throw std::runtime_error("Intersection detected");
        }
    }

    double get_area() const override {
        size_t size = get_sides();
        double sum = 0;
        for (size_t i = 0; i != size; i++) {
            Point first = get_point(i);
            Point second = get_point((i+1)%size);
            double x1 = first.get_x();
            double x2 = second.get_x();
            double y1 = first.get_y();
            double y2 = second.get_y();
            sum += (x2-x1) * (y1+y2);
        }
        return sum/2;
    }
};

struct Triangle : public Polygon {
    Triangle(const Point& a, const Point& b, const Point& c) : Polygon(vector<Point>{a,b,c}) {
        check_triangle();
    };

    void check_triangle() {
        double det1 = determinant(get_point(0), get_point(1), get_point(1), get_point(2));
        double det2 = determinant(get_point(1), get_point(2), get_point(2), get_point(0));

        if (det1 == 0 && det2 == 0) {
            throw std::runtime_error("Triangle ne Triangle");
        }
    }
};

struct Trapezoid : public Polygon {
    Trapezoid(const Point& a, const Point& b, const Point& c, const Point& d) : Polygon(vector<Point>{a,b,c,d}) {
        check_trapezoid();
    };


    void check_trapezoid() const {
        double det1 = determinant(get_point(0), get_point(1), get_point(2), get_point(3));
        double det2 = determinant(get_point(1), get_point(2), get_point(0), get_point(3));

        if (det1 != 0 && det2 != 0) {
            throw std::runtime_error("Trapezoid ne Trapezoid");
        }
    }
};

struct RegularPolygon : public Polygon {
    RegularPolygon(vector<Point> const & verticles) : Polygon(verticles) {
        check_regular();
    };
    RegularPolygon(initializer_list<Point> p_list) : Polygon(p_list) {
        check_regular();
    };
    template<class ...T>
    RegularPolygon(T& ...args) : Polygon(args...) {
        check_regular();
    };

    void check_regular() const {
        double dist = get_distance(get_point(0), get_point(1));
        for (size_t i = 1; i < get_sides()-1; i++) {
            if (get_distance(get_point(i), get_point(i+1)) != dist)
                throw std::runtime_error("Polygon ne Regular");
        }
    }
};

#endif LABA1_FIGURES_H
