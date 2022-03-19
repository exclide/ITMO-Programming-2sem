#include <iostream>
#include <curl.h>
#include <json.hpp>
#include "VariadicTable.h"
#include <conio.h>
#include "currency.h"
#include "parse.h"
#include "pretty_table.h"


int main(int argc, char** argv) {
    system("chcp 65001 > nul");
    const uint8_t interval_seconds = 2;

    std::string read_buffer;
    CURL* handle = handle_init(read_buffer);

    std::vector<Currency> currencies;

    json data = get_json(handle, read_buffer);


    parse_json(data, currencies, true);
    print_table(currencies);

    std::clock_t prev = std::clock();
    while (!kbhit()) {
        if ((std::clock() / CLOCKS_PER_SEC)  >= (prev / CLOCKS_PER_SEC) + interval_seconds) {
            data = get_json(handle, read_buffer);
            parse_json(data, currencies, false);

            print_table(currencies);

            prev = std::clock();
        }
        //Sleep(interval_seconds*1000);
    }

    curl_easy_cleanup(handle);

    std::cout << "\n\n\n\n";
    print_final_table(currencies);

    return 0;
}
