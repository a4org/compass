//===----------------------------------------------------------------------===//
//                                  compass
//
// currency.hpp
//
// Identification: src/parse/currency/currency.hpp
//
// Last Modified : 2022.2.13 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

#include "../sequencial/seqparse.hpp"


// CurrParser is the parser which only need to get the currency
// of given stock price from Form Responses page
class CurrParser : private Parser { 
public:
    /**
     * CurrParser::cstock()
     * Only need to get currency of specific stock or option
     */
    std::string cyahoo(std::string html);

    /**
     * CurrParser::cbond()
     * Only need to get currency of specific bond
     */
    std::string cbond(std::string html);

};

