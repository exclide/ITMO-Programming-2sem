#ifndef LAB2_1_PARSE_H
#define LAB2_1_PARSE_H
using json = nlohmann::json;


size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void parse_json(json& data, std::vector<Currency>& currencies, bool init = false) {
    if (init) {
        for (auto& row : data["Valute"]) {
            Currency temp(row["CharCode"].get<std::string>(), row["Name"].get<std::string>(),
                    row["Nominal"].get<int>(), row["Value"].get<double>());
            currencies.push_back(temp);
        }
        return;
    }

    int i = 0;

    for (auto& row : data["Valute"]) {
        currencies[i].add_value(row["Value"].get<double>());
        i++;
    }
}

json get_json(CURL* handle, std::string& read_buffer) {
    curl_easy_perform(handle);
    json data = json::parse(read_buffer);
    read_buffer = "";
    return data;
}

CURL* handle_init(std::string& read_buffer) {
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* easy_handle = curl_easy_init();

    curl_easy_setopt(easy_handle, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily_json.js");
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &read_buffer);

    return easy_handle;
}

#endif