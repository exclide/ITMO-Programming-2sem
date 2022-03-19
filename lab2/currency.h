#ifndef LAB2_1_CURRENCY_H
#define LAB2_1_CURRENCY_H

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

class Currency {
public:
    Currency() = default;
    Currency(std::string char_code, std::string name, int nominal, double value) :
        char_code(char_code), name(name), nominal(nominal) {
        values.push_back(value);
    }

    void add_value(double value) {
        values.push_back(value);
    }

    double get_average() const {
        return std::accumulate(values.begin(), values.end(), 0.0) / double(values.size());
    }

    double get_median() {
        std::sort(values.begin(), values.end());
        size_t sz = values.size();
        return sz % 2 ? values[sz/2] : (values[(sz/2)-1] + values[sz/2])/2;
    }

    std::string get_char_code() { return char_code; }
    std::string get_name() { return name; }
    int get_nominal() { return nominal; }
    double get_latest_value() { return values.back(); }

private:
    std::string char_code;
    std::string name;
    int nominal;
    std::vector<double> values;
};

#endif