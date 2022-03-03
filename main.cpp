#include <iostream>
#include <unordered_map>
#include <set>
#include <unistd.h>
#include <boost/spirit/home/qi.hpp>

#ifdef DEBUG
#include <ctime>
#endif

#define MAX_MARKET 15000
#define NUM_OF_PARAM 5

std::unordered_map<std::string, uint8_t> keyHash = {
    {"market", 1},
    {"price", 2},
    {"volume", 3},
    {"is_buy", 4},
};

std::unordered_map<std::string, uint8_t> boolHash = {
    {"true", 1},
    {"false", 0},
};

enum MarketInfo {
    TotalVol = 0,
    TotalPrice,
    TotalPriceMultiplyVol,
    TotalBuy,
    TotalCount
};

void extractInfoFromJson(const std::string &json, double data[MAX_MARKET][NUM_OF_PARAM], std::set<uint16_t> &activeMarketSet) {
    uint16_t market;
    double vol, price;
    bool isBuy;
    uint jsonStrLen = json.length();
    std::string key, value;
    for (uint i = 0, j = 0; i < jsonStrLen; ++i) {
        if (json[i] == '\"') {
            // get value after the first quote
            // start checking the pos of the end quote
            j = i + 1;
            while (json[j] != '\"') ++j;
            key = json.substr(i+1, j - i - 1);
            // update the ptr to the last scanned location
            i = j;
        }
        else if (json[i] == ':') {
            // get value after the colon
            // start getting the value of the key
            j = i + 1;
            while (!(json[j] == ',' || json[j] == '}')) ++j;
            value = json.substr(i+1, j - i - 1);

            // after getting the value in string value, convert the respective value in string format to their numeric value
            try {
                switch (keyHash[key]) {
                    case 1:
                        // market
                        market = std::stoi(value);
                        break;
                    case 2:
                        // price
                        boost::spirit::qi::parse(&json[i+1], &json[j-1], boost::spirit::qi::double_, price);
                        break;
                    case 3:
                        // vol
                        boost::spirit::qi::parse(&json[i+1], &json[j-1], boost::spirit::qi::double_, vol);
                        break;
                    case 4:
                        // isBuy
                        isBuy = boolHash[value];
                    default:
                        break;
                }
            } catch (std::exception &error) {
                std::cerr << error.what() << std::endl;
            }
            // update the ptr to the last scanned location
            i = j;
        }
    }

    // record which market is active so we don't print all unused market id
    activeMarketSet.insert(market);
    data[market][MarketInfo::TotalVol] += vol;
    data[market][MarketInfo::TotalPrice] += price;
    data[market][MarketInfo::TotalPriceMultiplyVol] += vol * price;
    data[market][MarketInfo::TotalBuy] += (isBuy ? 1 : 0);
    data[market][MarketInfo::TotalCount] += 1;
}

int main() {
    std::string buf;
    double data[MAX_MARKET][NUM_OF_PARAM];
    std::set<uint16_t> activeMarketSet;

#ifdef DEBUG
    std::clock_t start_t = std::clock();
    int runCount = 0;
    double duration;
#endif

    char *cbuf = nullptr;
    size_t len;

    while (getline(&cbuf, &len, stdin) != -1) {
        cbuf[strcspn(cbuf, "\n")] = 0;
        buf = cbuf;
        if (buf == "BEGIN") {
            while (getline(&cbuf, &len, stdin) != -1) {
                cbuf[strcspn(cbuf, "\n")] = 0;
                buf = cbuf;
                if (buf == "END") break;

#ifdef DEBUG
                runCount += 1;
#endif

                // read json string line into numeric value and parse
                extractInfoFromJson(buf, data, activeMarketSet);

#ifdef DEBUG
                if (runCount % 100000 == 0) {
                    std::cout << "runCount: " << runCount << std::endl;
                    duration = (std::clock() - start_t) / (double) CLOCKS_PER_SEC;
                    std::cout << "Operation took " << duration << " seconds" << std::endl;
                }
#endif
            }
            break;
        }
    }

    // output result in json format
    double totalVol, meanPrice, meanVol, VWAP, percentageBuy, totalCount;
    for (std::set<uint16_t>::const_iterator it = activeMarketSet.begin(); it != activeMarketSet.end(); ++it) {
        totalCount = data[*it][MarketInfo::TotalCount];
        totalVol = data[*it][MarketInfo::TotalVol];
        meanPrice = data[*it][MarketInfo::TotalPrice] / totalCount;
        meanVol = totalVol / totalCount;
        VWAP = data[*it][MarketInfo::TotalPriceMultiplyVol] / totalVol;
        percentageBuy = data[*it][MarketInfo::TotalBuy] / totalCount;
        std::cout << "{\"market\":" << *it
            << ",\"total_volume\":" << totalVol
            << ",\"mean_price\":" << meanPrice
            << ",\"mean_volume\":" << meanVol
            << ",\"volume_weighted_average_price\":" << VWAP
            << ",\"percentage_buy\":" << percentageBuy << "}" << std::endl;
    }
};