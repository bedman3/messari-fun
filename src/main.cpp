#include <iostream>
#include <nlohmann/json.hpp>
#include <unistd.h>

#define MSGSIZE 200

// for convenience
using json = nlohmann::json;

int main() {
    std::cout << "Hello world!" << std::endl;

    std::string buf;
    std::cin >> buf;
    if (buf == "BEGIN") {
        while (std::cin >> buf) {
//            json j2 = buf;
//            std::cout << j2[]


            if (buf == "END") break;
        }
    }



    json j;


    std::string str;
//    std::cin >> buf;


    std::cout << "input: " << buf << std::endl;


    std::string test = j2.dump();

//    std::cout << buf << std::endl;
//    std::cout << test << std::endl;
};