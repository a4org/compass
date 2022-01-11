//===----------------------------------------------------------------------===//
//                         compass
//
// parse.cpp
//
// Identification: src/parse/parse.cpp
//
// Last Modified : 2022.1.12 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

#include "./parse.hpp"

#define VPSS std::vector<std::pair<std::string, std::string>>
#define VPIS std::vector<std::pair<int, std::string>>




std::unordered_map<int, std::vector<int>> final; // multithreading TODO


// currency, code, O, H, L, C, Volume and print them with \n
int Parser::stock() {
    // 0. Get Currency
    std::string currencykey = "Currency in ";
    int currencystart = this->phtml.find(currencykey);
    int currencyend   = this->phtml.find("</s", currencystart);
    if (currencystart == std::string::npos || currencyend == std::string::npos || currencyend < currencystart) {
	std::cerr << "Searching key error, on function getStockBlk" << std::endl;
	return 1;
    }

    currencystart += 12;
    std::string currency = this->phtml.substr(currencystart, currencyend-currencystart);
    // std::cout << currency << std::endl;

    // 1. Get block
    std::string blk = this->getStockBlk(this->phtml);

    // 2. Get each statement

}


std::string Parser::getStockBlk(std::string shtml) {
    std::string begblk = "Previous Close</span>";
    std::string endblk = "</tbody></table></div></div></div></div></div>";
    int begidx = shtml.find(begblk);
    int endidx = shtml.find(endblk);
    if (begidx == std::string::npos || endidx == std::string::npos || endidx < begidx) {
	std::cerr << "Searching key error, on function getStockBlk" << std::endl;
	return "";
    }
    return shtml.substr(begidx, endidx-begidx); // 5114 chars
};


int readcsv(std::string cpath, VPSS& codetype) {
    std::string line;
    try {
	std::ifstream csvfile(cpath);
	while (std::getline(csvfile, line)) {
	    std::string code, type;
	    std::string temp = "";
	    for (char c : line) {
		if (c == ',') {
		    code = temp;
		    temp = "";
		    continue;
		}
		temp += c;
	    }
	    type = temp;
	    codetype.push_back(std::make_pair(code, type));
	}
	csvfile.close();
    } catch (const std::ifstream::failure& e) {
	std::cout << "Exception opening/reading file" << std::endl;
	return 0;
    }
    return 1;
}

/**
 * main()
 * Entry of the entire parse program
 */
int main() {
    // 1. Read csv file from download.js (sheet)
    VPSS codetype; // code with its type
    readcsv("../sheet.csv", codetype);
    /*// for debugging
    for (auto p : codetype) {
	std::cout << p.first << " " << p.second << std::endl;
    }
    */


    // 2. Concatenate urls
    // {{1, url}, {2, url}} for multithreading TODO 

    VPIS indexurl = {};
    for (int i = 0; i < codetype.size(); i++) {
	std::pair<std::string, std::string> ct = codetype[i];
	std::string code = ct.first;
	std::string type = ct.second;

	std::string url = "";

	if (type == "Stock" || "Option") url = YAHOO + code;
	else if (type == "Bond") url = BOND + code;
	else {
	    std::cerr << "invalid type!" << std::endl;
	    return 1;
	}
	
	indexurl.push_back(std::make_pair(i, url));
    }

    /*// for debugging
    for (auto p : indexurl) {
	std::cout << p.first << " " << p.second << std::endl;
    }
    */

    std::string testcode = "IBM";
    std::string testtype = "Stock";
    std::string url = YAHOO + testcode;

    // 3. Fetch html

    CurlObj* co = new CurlObj(url);
    std::string html = co->getData();

    // for debugging
    std::ofstream out("../test.html");
    out << html;
    out.close();

    // 4. Parse html
    
    Parser* parser = new Parser(html);
    parser->stock();
    // std::string subhtml = parser->testwrapper();

    /* // for debugging
    std::ofstream out("test.html");
    out << subhtml;
    out.close();
    */

}
