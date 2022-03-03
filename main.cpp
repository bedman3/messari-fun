#include <iostream>
#include <unordered_map>
#include <ctime>
#include <set>
#include "ctpl.h"

#define MAX_MARKET 15000
#define NUM_WORKER_THREAD 2
#define NUM_OF_PARAM 5
#define MULTITHREAD true

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

void extractInfoFromJson(int id, const std::string &json, double data[MAX_MARKET][NUM_OF_PARAM], std::set<uint16_t> &active_market_set) {
    uint16_t market;
    double vol, price;
    bool isBuy;
    int jsonStrLen = json.length();
    std::string key, value;
    for (uint i = 0, j = 0; i < jsonStrLen; ++i) {
        if (json[i] == '\"') {
            // start checking the pos of the end quote
            j = i + 1;
            while (json[j] != '\"') ++j;
            key = json.substr(i+1, j - i - 1);
            i = j;
        }
        else if (json[i] == ':') {
            // start getting the value of the key
            j = i + 1;
            while (!(json[j] == ',' || json[j] == '}')) ++j;
            value = json.substr(i+1, j - i - 1);
            i = j;
            try {
                switch (keyHash[key]) {
                    case 1:
                        // market
                        market = std::stoi(value);
                        break;
                    case 2:
                        // price
                        price = std::stod(value);
                        break;
                    case 3:
                        // vol
                        vol = std::stod(value);
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
        }
    }

    active_market_set.insert(market);
    data[market][MarketInfo::TotalVol] += vol;
    data[market][MarketInfo::TotalPrice] += price;
    data[market][MarketInfo::TotalPriceMultiplyVol] += vol * price;
    data[market][MarketInfo::TotalBuy] += (isBuy ? 1 : 0);
    data[market][MarketInfo::TotalCount] += 1;
}

int main() {
    std::string buf;
    std::clock_t start_t = std::clock();
    double duration, data[NUM_WORKER_THREAD + 1][MAX_MARKET][NUM_OF_PARAM];
    memset(data, 0, sizeof(data));
    std::set<uint16_t> active_market_set[NUM_WORKER_THREAD + 1];
    ctpl::thread_pool tp(NUM_WORKER_THREAD);

    int runCount = 0;
    std::cin >> buf;
    if (buf == "BEGIN") {
        uint8_t instance;
        while (std::cin >> buf) {
            if (buf == "END") break;
            runCount += 1;

            if (MULTITHREAD) {
                instance = (runCount % NUM_WORKER_THREAD) + 1;
                tp.push(extractInfoFromJson, buf, data[instance], active_market_set[instance]);
            } else {
                extractInfoFromJson(0, buf, data[0], active_market_set[0]);
            }

            if (runCount % 100000 == 0) {
                std::cout << "runCount: " << runCount << std::endl;
                duration = ( std::clock() - start_t ) / (double) CLOCKS_PER_SEC;
                std::cout << "Operation took " << duration << " seconds" << std::endl;
            }
        }
    }

    for (int i = 1; i < NUM_WORKER_THREAD + 1; ++i) {
        active_market_set[0].insert(active_market_set[i].begin(), active_market_set[i].end());
    }

    double totalVol, meanPrice, meanVol, VWAP, percentageBuy, totalCount;
    for (std::set<uint16_t>::const_iterator it = active_market_set[0].begin(); it != active_market_set[0].end(); ++it) {
        for (int i = 1; i < NUM_WORKER_THREAD; ++i) {
            for (int j = 0; j < NUM_OF_PARAM; ++j) {
                data[0][*it][j] += data[i][*it][j];
            }
        }

        totalCount = data[0][*it][MarketInfo::TotalCount];
        totalVol = data[0][*it][MarketInfo::TotalVol];
        meanPrice = data[0][*it][MarketInfo::TotalPrice] / totalCount;
        meanVol = totalVol / totalCount;
        VWAP = data[0][*it][MarketInfo::TotalPriceMultiplyVol] / totalVol;
        percentageBuy = data[0][*it][MarketInfo::TotalBuy] / totalCount;
        std::cout << "{\"market\":" << *it
            << ",\"total_volume\":" << totalVol
            << ",\"mean_price\":" << meanPrice
            << ",\"mean_volume\":" << meanVol
            << ",\"volume_weighted_average_price\":" << VWAP
            << ",\"percentage_buy\":" << percentageBuy << "}" << std::endl;
    }
};