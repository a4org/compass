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
#define VVS  std::vector<std::vector<std::string> > 
#define VPSS std::vector<std::pair<std::string, std::string> >
#define VPIS std::vector<std::pair<int, std::string> >
#define UMSV std::unordered_map<std::string, std::vector<std::string> >
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

protected:  // can be inherance
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
     * Parser::gparse()
     *
     * Generitic parse function for Yahoo finance
     * return start and end point of the specific key
     */
    std::pair<int, int> gparse(std::string sblk, std::string key);

    /**
     * Parser::bparse()
     *
     * Generitic parse function for bond
     * return start and end point of the specific key
     */
    std::pair<int, int> bparse(std::string html, std::string key);

    /**
     * Parser::getCloseP()
     *
     * Get the Close price which is Fz(36px) as key in yahoo finance
     */
    void getCloseP(std::string html, std::string& Close);

};
