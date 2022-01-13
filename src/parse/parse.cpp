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



std::unordered_map<int, VS> final; // multithreading TODO

std::pair<int, int> Parser::gparse(std::string sblk, std::string key) {
    int start = sblk.find(key);
    start = sblk.find('>', start);
    start += 1;
    int end = sblk.find('<', start);
    if ( start == std::string::npos || end == std::string::npos || end < start) {
	std::cerr << "Searching key error, on gparse stock" << std::endl;
	return std::make_pair(-1, -1);
    }
    return std::make_pair(start, end);
}

// currency, O, H, L, C, Volume
VS Parser::stock() {
    std::string Currency, O, H, L, C, Volume;

    // 0. Get Currency
    std::string currencykey = "Currency in ";
    int currencystart = this->phtml.find(currencykey);
    int currencyend   = this->phtml.find("</s", currencystart);
    if (currencystart == std::string::npos || currencyend == std::string::npos || currencyend < currencystart) {
	std::cerr << "Searching key error, on parse stock" << std::endl;
	return {};
    }

    currencystart += 12;
    Currency = this->phtml.substr(currencystart, currencyend-currencystart);
    // std::cout << Currency << std::endl;

    // 1. Get block
    std::string blk = this->getYahooBlk(this->phtml);

    // 2. Get each data

    // 2.1 Open price
    std::string openkey = "OPEN-value";
    std::pair<int, int> op = this->gparse(blk, openkey);
    if (op.first == -1) return {};
    O = blk.substr(op.first, op.second-op.first);

    // Just for debugging
    std::cout << "Open: " << O << std::endl; 


    // 2.2 High and Low price 
    std::string hlkey = "DAYS_RANGE-value";
    std::pair<int, int> hlp = this->gparse(blk, hlkey);
    std::string HL = blk.substr(hlp.first, hlp.second - hlp.first);

    // Just for debugging
    // std::cout << "High and Low block: " << HL << std::endl; 

    int Hend = HL.find('-');
    L = HL.substr(0, Hend-1);
    Hend += 2;
    H = HL.substr(Hend, HL.size()-Hend);

    // Just for debugging
    std::cout << "High: " << H << std::endl;
    std::cout << "Low: " << L << std::endl;


    // 2.3 Close price
    std::string closekey = "PREV_CLOSE-value";
    std::pair<int, int> cp = this->gparse(blk, closekey);

    C = blk.substr(cp.first, cp.second - cp.first);

    // Just for debugging
    std::cout << "Close: " << C << std::endl;

    // 2.4 Volume 
    std::string volumekey = "TD_VOLUME-value";
    // Volume changing everytime, so make sure I can fetch it 
    // More generic way

    int volumestart = blk.find(volumekey);
    volumestart = blk.find("value=", volumestart);
    volumestart += 7;
    int volumeend = blk.find("\"", volumestart);

    Volume = blk.substr(volumestart, volumeend - volumestart);

    for (auto it = Volume.begin(); it < Volume.end(); it++) {
	if (*it == ',') Volume.erase(it);
    }
    std::cout << "Volume: " << Volume << std::endl; 

    VS ret = {Currency, O, H, L, C, Volume};

    return ret;
}

// currency, O, H, L, C, Strike, Ex Date, Put/Call,  Open interest
VS Parser::option() {
    std::string Currency, O, H, L, C, Strike, ExDate, PutCall, Interest;

    // 0.1 Get Currency
    std::string currencykey = "Currency in ";
    int currencystart = this->phtml.find(currencykey);
    int currencyend   = this->phtml.find("</s", currencystart);
    if (currencystart == std::string::npos || currencyend == std::string::npos || currencyend < currencystart) {
	std::cerr << "Searching key error, on parse option" << std::endl;
	return {};
    }

    currencystart += 12;
    Currency = this->phtml.substr(currencystart, currencyend-currencystart);
    std::cout << "Currency: " << Currency << std::endl;

    // 0.2 Get Put / Call
    std::string pckey = "</h1></div>";
    int pcend = this->phtml.find(pckey);
    int pcstart = this->phtml.rfind(' ', pcend)+1; 
    if (pcstart == std::string::npos || pcend == std::string::npos || pcend < pcstart) {
	std::cout << "pcstart: " << pcstart << std::endl;
	std::cout << "pcend: " << pcend << std::endl;
	std::cerr << "Searching key error, on parse option" << std::endl;
	return {};
    }
    PutCall = this->phtml.substr(pcstart, pcend-pcstart);
    if (PutCall != "put" && PutCall != "call") {
	std::cout << PutCall << std::endl;
	std::cerr << "Searching key error, on parse put/call" << std::endl;
	return {};
    }

    std::cout << "Put or Call: " << PutCall << std::endl;

    // 1. Get block
    std::string blk = this->getYahooBlk(this->phtml);

    // 2. Get each data

    // 2.1 Open price
    std::string openkey = "OPEN-value";
    std::pair<int, int> op = this->gparse(blk, openkey);
    if (op.first == -1) return {};
    O = blk.substr(op.first, op.second-op.first);

    // Just for debugging
    std::cout << "Open: " << O << std::endl; 


    // 2.2 High and Low price 
    std::string hlkey = "DAYS_RANGE-value";
    std::pair<int, int> hlp = this->gparse(blk, hlkey);
    std::string HL = blk.substr(hlp.first, hlp.second - hlp.first);

    // Just for debugging
    // std::cout << "High and Low block: " << HL << std::endl; 

    int Hend = HL.find('-');
    L = HL.substr(0, Hend-1);
    Hend += 2;
    H = HL.substr(Hend, HL.size()-Hend);

    // Just for debugging
    std::cout << "High: " << H << std::endl;
    std::cout << "Low: " << L << std::endl;


    // 2.3 Close price
    std::string closekey = "PREV_CLOSE-value";
    std::pair<int, int> cp = this->gparse(blk, closekey);

    C = blk.substr(cp.first, cp.second - cp.first);

    // Just for debugging
    std::cout << "Close: " << C << std::endl;

    // 2.4 Strike
    std::string strikekey = "STRIKE-value";
    std::pair<int, int> strikep = this->gparse(blk, strikekey);
    
    Strike = blk.substr(strikep.first, strikep.second - strikep.first);

    std::cout << "Strike: " << Strike << std::endl;

    // 2.5 Expire date
    std::string exkey = "EXPIRE_DATE-value";
    std::pair<int, int> ExDatep = this->gparse(blk, exkey);
    
    ExDate = blk.substr(ExDatep.first, ExDatep.second-ExDatep.first);

    std::cout << "ExDate: " << ExDate << std::endl;

    // 2.6 Open Interest
    std::string interestkey = "OPEN_INTEREST-value";
    std::pair<int, int> Interestp = this->gparse(blk, interestkey);

    Interest = blk.substr(Interestp.first, Interestp.second-Interestp.first);

    std::cout << "Open Interest: " << Interest << std::endl;

    return {};
}


std::string Parser::getYahooBlk(std::string shtml) {
    std::string begblk = "Previous Close</span>";
    std::string endblk = "</tbody></table></div></div></div></div></div>";
    int begidx = shtml.find(begblk);
    int endidx = shtml.find(endblk);
    if (begidx == std::string::npos || endidx == std::string::npos || endidx < begidx) {
	std::cerr << "Searching key error, on function getStockBlk" << std::endl;
	return "";
    }

    std::string blk = shtml.substr(begidx, endidx-begidx);
    std::ofstream out("test.html");
    out << blk;
    out.close();
    return shtml.substr(begidx, endidx-begidx); // 5114, 3214 chars
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

    std::string testcode = "APA220414P00027500";
    std::string testtype = "Option";
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
    VS datafield = parser->option();
    // std::string subhtml = parser->testwrapper();


    // 5. Write to csv file

    std::ofstream testcsv;
    testcsv.open("../coutput.csv");
    for (std::string s : datafield) {
	testcsv << s << ',';
    }
    testcsv.close();

    /* // for debugging
    std::ofstream out("test.html");
    out << subhtml;
    out.close();
    */

}
