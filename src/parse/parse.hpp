//===----------------------------------------------------------------------===//
//                         compass
//
// parse.hpp
//
// Identification: src/parse/parse.hpp
//
// Last Modified : 2022.1.12 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

#pragma once

#include <curl/curl.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <pthread.h> // multithreading
#include <chrono>
#include <thread>
#include <filesystem>


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

    /**
     * Parser::stock()
     * 
     * Parse all stock-related value from yahoo finance html
     * currency, code, O, H, L, C, Volume and print them with \n
     */
    int stock();

    /**
     * Parser::bond()
     *
     * Parse all bond-related value from bond supermarket html
     *
     * currency, name, Bid, Ask, Coupon, Maturity, Raiting, YTM, Seniority, Type
     * and print them with \n
     */
    int bond();

    /**
     * Parser::option() 
     *
     * Parse all option-related value from yahoo finance html
     *
     * currency, code, O, H, L, C, Strike, Ex Date, Put/Call, Open interest
     * and print them with \n
     */
    int option();

private:
    std::string phtml;

    /**
     * Parser::getStockBlk()
     *
     * get the stock-related data block from stock html 
     * Ease of handling later
     */
    std::string getStockBlk(std::string shtml);

    /**
     * Parser::getBondBlk()
     *
     * get the bond-related data block from stock html 
     * Ease of handling later
     */
    std::string getBondBlk(std::string shtml);

    /**
     * Parser::getOptionBlk()
     *
     * get the option-related data block from stock html 
     * Ease of handling later
     */
    std::string getOptionBlk(std::string shtml);
};

