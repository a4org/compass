//===----------------------------------------------------------------------===//
//                         compass
//
// parse.hpp
//
// Identification: src/parse/parse.hpp
//
// Last Modified : 2022.1.25 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

#pragma once

#include <curl/curl.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <pthread.h> // multithreading
#include <chrono>
#include <thread>
#include <filesystem>
#include <ctime>

#define BOND  "https://www.bondsupermart.com/main/ws/bond-info/bond-factsheet/"
#define YAHOO "https://finance.yahoo.com/quote/"
#define THRLIMIT 16
#define VVS  std::vector<std::vector<std::string>> 
#define VPSS std::vector<std::pair<std::string, std::string>>
#define VPIS std::vector<std::pair<int, std::string>>
#define UMSV std::unordered_map<std::string, std::vector<std::string>>
#define VS std::vector<std::string>

// CurlObj is the class to handle url
class CurlObj {
public:
    CurlObj (std::string url) {
        curl = curl_easy_init();
	if (!curl) throw std::string("Curl did not initialize.");
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
	curl_easy_perform(curl);
    }

    static int curlWriter(char* data, int size, int nmemb, std::string* buffer) {
	// write to the buffer
	int result = 0;
	if (buffer != NULL) {
	    buffer->append(data, size * nmemb);
	    result = size * nmemb;
	}
	return result;
    }

    std::string getData() {
	return curlBuffer;
    }

private:
    CURL* curl;
    std::string curlBuffer; // write to curlBuffer
};


// Parser is the class to deal with the html file
class Parser {
public:
    Parser (std::string html) : phtml(html){};
    Parser() = default;

    /**
     * Parser::stock()
     * 
     * Parse all stock-related value from yahoo finance html
     * currency, code, O, H, L, C, Volume
     */
    VS stock();

    /**
     * Parser::bond()
     *
     * Parse all bond-related value from bond supermarket html
     *
     * currency, name, Bid, Ask, Coupon, Maturity, Raiting, YTM, Seniority, Type
     */
    VS bond();

    /**
     * Parser::option() 
     *
     * Parse all option-related value from yahoo finance html
     *
     * currency, code, O, H, L, C, Strike, Ex Date, Put/Call, Open interest
     */
    VS option();


    // Testing function, whatever wange needed
    /*
    std::string testwrapper() {
	return this->getStockBlk(this->phtml);
    }
    */
protected: // can access by subclass
    std::string phtml;

    /**
     * Parser::getYahooBlk()
     *
     * get the data block from stock html (yahoo)
     * Ease of handling later
     */
    std::string getYahooBlk(std::string shtml);


    /**
     * Parser::getOptionBlk()
     *
     * get the option-related data block from stock html 
     * Ease of handling later
     */
    std::string getOptionBlk(std::string shtml);

    /**
     * Parser::gparse
     *
     * Generitic parse function for Yahoo finance
     * return start and end point of the specific key
     */
    std::pair<int, int> gparse(std::string sblk, std::string key);

    /**
     * Parser::bparse
     *
     * Generitic parse function for bond
     * return start and end point of the specific key
     */
    std::pair<int, int> bparse(std::string html, std::string key);
};



// HistParser is the class to deal with the history data html file
// Which is inherance from Parser class
class HistParser : private Parser {
public:
    HistParser(std::string date) {  // init with date
	this->Open = {date, "Open"};
	this->High = {date, "High"};
	this->Low = {date, "Low"};
	this->Close = {date, "Close"};
	this->Volumn = {date, "Volumn"};
	this->Bid = {date, "Bid"};
	this->Ask = {date, "Ask"};
    };

    /**
     * HistParser::hstock()
     *
     * Parse all fixed prices from Yahoo finance (O H L C Volume)
     */
    int hstock(std::string phtml);


    /**
     * HistParser::hbond()
     *
     * Parse all fixed prices from Yahoo finance (Bid Ask)
     */
    int hbond(std::string phtml);


    /**
     * HistParser::hoption()
     *
     * Parse all fixed prices from Yahoo finance (O H L C)
     */
    int hoption(std::string phtml);



    /**
     * HistParser::hnull()
     *
     * all empty (make space)
     */
    int hnull() {
	this->Open.push_back(" ");
	this->High.push_back(" ");
	this->Low.push_back(" ");
	this->Close.push_back(" ");
	this->Volumn.push_back(" ");
	this->Bid.push_back(" ");
	this->Ask.push_back(" ");
    }


    // Getter methods
    VS getOpen() { return this->Open; }
    VS getHigh() { return this->High; }
    VS getLow() { return this->Low; }
    VS getClose() { return this->Close; }
    VS getVolumn() { return this->Volumn; }
    VS getBid() { return this->Bid; }
    VS getAsk() { return this->Ask; }



private:
    VS Open;
    VS High;
    VS Low;
    VS Close;
    VS Volumn;
    VS Bid;
    VS Ask;

    /**
     * HistParser::push_invalid()
     *
     * Detect a invalid code, or error
     * just push invalid
     */
    void push_invalid() {
	this->Open.push_back("(invalid)");
	this->High.push_back("(invalid)");
	this->Low.push_back("(invalid)");
	this->Close.push_back("(invalid)");
	this->Volumn.push_back("(invalid)");
	this->Bid.push_back("(invalid)");
	this->Ask.push_back("(invalid)");
    }

    inline void rcomma(std::string &value) {
	for (auto it = value.begin(); it < value.end(); it++) if (*it == ',') value.erase(it);
    }
};
