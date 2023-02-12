#include "parseman/Parser.hpp"
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

enum Tests {
    BoolVal = 0,
    IntegerVal,
    DoubleVal,
    StringVal
};

int main(void) {
    std::string line = "test --bool-val true -i 90 --double 1.5 s \"Hello, World!\"";
    std::vector<std::string> vec {};
    vec.push_back(line);
    parseman::Parser<Tests> p {vec};

    p.setType(Tests::BoolVal, typeid(bool));
    p.setPattern(Tests::BoolVal, "(--bool-val\\s+)(true|false)");
    assert(p.get<bool>(Tests::BoolVal));

    p.setType(Tests::IntegerVal, typeid(int));
    p.setPattern(Tests::IntegerVal, "(-i\\s*)(\\d+)");
    assert(p.get<int>(Tests::IntegerVal) == 90);

    p.setType(Tests::DoubleVal, typeid(double));
    p.setPattern(Tests::DoubleVal, "(--double\\s*)(\\d+\\.\\d+)");
    assert(p.get<double>(Tests::DoubleVal) == 1.5);

    p.setType(Tests::StringVal, typeid(std::string));
    p.setPattern(Tests::StringVal, "(?<=s\\s+\")(.*?)(?=\")");
    std::cout << p.get<std::string>(Tests::StringVal, 0) << std::endl;
    std::cout << p.get<std::string>(Tests::StringVal, 1) << std::endl;
    std::cout << p.get<std::string>(Tests::StringVal, 2) << std::endl;
    assert(p.get<std::string>(Tests::StringVal) == std::string("Hello, World!"));
}
