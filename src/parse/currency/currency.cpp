//===----------------------------------------------------------------------===//
//                                  compass
//
// currency.cpp
//
// Identification: src/parse/currency/currency.cpp
//
// Last Modified : 2022.2.13 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

#include "./currency.hpp"


std::string CurrParser::cbond(std::string html) {
    if (html.size() < 1000) {
	return "error";
    }

    std::string Currency;

    // speed up
    int bblkend = html.find(":[[");
    std::string bhtml = html.substr(0, bblkend);
    // Seperately
    // 1. Currency, Coupon, Maturity 
    std::string namekey = "bondName\":\"";
    std::pair<int, int> np = this->bparse(bhtml, namekey);
    std::string Name = bhtml.substr(np.first, np.second - np.first);
    if (np.first == -1) return "error";

    // 1.1 Coupon
    int coupstart = Name.find(' ') + 1;
    int coupend = Name.find('%', coupstart) + 1;

    // 1.2 Maturity
    int maturstart = Name.find(' ', coupend) + 1;
    int maturend = Name.find(' ', maturstart);

    // 1.3 Currency
    int currstart = Name.find('(', maturend) + 1;
    int currend = Name.find(')', currstart);
    Currency = Name.substr(currstart, currend - currstart);
    std::cout << "Currency: " << Currency << std::endl;

    return Currency;
}

std::string CurrParser::cyahoo(std::string html) {
    if (html.size() < 100) {
	return "error";
    }
    std::string Currency;

    // 0. Get Currency
    std::string currencykey = "Currency in ";
    int currencystart = html.find(currencykey);
    int currencyend   = html.find("</s", currencystart);
    if (currencystart == std::string::npos || currencyend == std::string::npos || currencyend < currencystart) {
	std::cerr << "Searching key error, on parse stock" << std::endl;
	return "error";
    }

    currencystart += 12;

    Currency = html.substr(currencystart, currencyend-currencystart);

    return Currency;
}

