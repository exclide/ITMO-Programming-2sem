#ifndef LAB2_1_PRETTY_TABLE_H
#define LAB2_1_PRETTY_TABLE_H


void print_table(std::vector<Currency>& currencies) {
    VariadicTable<std::string, int, double> vt({"Code", "Nominal", "Value"});

    for (int i = 0; i < currencies.size(); i++) {
        vt.addRow(currencies[i].get_char_code(),
                  currencies[i].get_nominal(), currencies[i].get_latest_value());
    }

    vt.print(std::cout);
}

void print_final_table(std::vector<Currency>& currencies) {
    VariadicTable<std::string, int, double, double> vt({"Code", "Nominal", "Median", "Average"});

    for (int i = 0; i < currencies.size(); i++) {
        vt.addRow(currencies[i].get_char_code(), currencies[i].get_nominal(),
                  currencies[i].get_median(), currencies[i].get_average());
    }

    vt.print(std::cout);
}

#endif