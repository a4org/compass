#include <curl/curl.h>
#include <vector>
#include <fstream>
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

#define BOND  "https://www.bondsupermart.com/bsm/bond-factsheet/"
#define YAHOO "https://finance.yahoo.com/quote/"

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

int main() {
    // Stock
    std::string testIBM = "https://finance.yahoo.com/quote/IBM";
    std::string testAMZ = "https://finance.yahoo.com/quote/AMZN";

    // Option
    std::string testUGI = "https://finance.yahoo.com/quote/UGI220414P00040000";
    std::string testNDX = "https://finance.yahoo.com/quote/NDX220414P15500000";

    // Bond
    std::string testAGB = "https://www.bondsupermart.com/bsm/bond-factsheet/AU3TB0000143";
    std::string testCGB = "https://www.bondsupermart.com/bsm/bond-factsheet/HK0000317724";


    std::vector<std::string> testall = {testIBM, testAMZ, testUGI, testNDX, testAGB, testCGB};
    std::vector<std::string> name = {"StockIBM.html", "StockAMZ.html", "OptionUGI.html", 
	"OptionNDX.html", "BondAGB.html", "BondCGB.html"};

    for (int i = 0; i < testall.size(); i++) {
	CurlObj* testco = new CurlObj(testall[i]);
	std::ofstream out("html/" + name[i]);
	std::string testhtml = testco->getData();
	out << testhtml;
	out.close();
    }

}

 
