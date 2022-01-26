//===----------------------------------------------------------------------===// compass
//
// histparse.cpp
//
// Identification: src/parse/histparse.cpp
//
// Last Modified : 2022.1.25 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

#include "./parse.hpp"


// O, H, L, C, Volume
int HistParser::hstock(std::string phtml) {
    std::string O, H, L, C, Volume;
    if (phtml.size() < 100) {
	// Invalid
	push_invalid();
	return -1;
    }
    // 1. Get Yahoo Blk
    std::string blk = this->getYahooBlk(phtml);

    // 2. Get each data

    // 2.1 Open price
    std::string openkey = "OPEN-value";
    std::pair<int, int> op = this->gparse(blk, openkey);
    if (op.first == -1) {
	push_invalid();
	return -1;
    }

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

    rcomma(O); rcomma(H); rcomma(L); rcomma(C); rcomma(Volume);

    // 3. Write to vector
    this->Open.push_back(O);
    this->High.push_back(H);
    this->Low.push_back(L);
    this->Close.push_back(C);
    this->Volumn.push_back(Volume);
    this->Bid.push_back("(blank)");
    this->Ask.push_back("(blank)");
    return 0; // normal
};


// Bid, Ask
int HistParser::hbond(std::string phtml) {

    if (phtml.size() < 1000) {
	push_invalid();
	return -1;
    }

    std::string Bid, Ask;

    // Bid : endBidPrice
    // Ask : endAskPrice
    
    // speed up
    int bblkend = phtml.find(":[[");
    std::string bhtml = phtml.substr(0, bblkend);
    // Seperately

    // 3. Bid
    std::string bidkey = "endBidPrice\":";
    std::pair<int, int> bp = this->bparse(bhtml, bidkey);
    Bid = bhtml.substr(bp.first, bp.second - 1 - bp.first);
    std::cout << "Bid: " << Bid << std::endl;

    // 4. Ask 
    std::string askkey = "endAskPrice\":";
    std::pair<int, int> ap = this->bparse(bhtml, askkey);
    Ask = bhtml.substr(ap.first, ap.second - 1 - ap.first);
    std::cout << "Ask: " << Ask << std::endl;

    rcomma(Bid); rcomma(Ask);
    this->Open.push_back("(blank)");
    this->Close.push_back("(blank)");
    this->High.push_back("(blank)");
    this->Low.push_back("(blank)");
    this->Volumn.push_back("(blank)");
    this->Bid.push_back(Bid);
    this->Ask.push_back(Ask);

    return 0;
}


// O, H, L, C
int HistParser::hoption(std::string phtml) {
    if (phtml.size() < 100) {
	push_invalid();
	return -1;
    }
    std::string O, H, L, C;

    // 1. Get block
    std::string blk = this->getYahooBlk(phtml);

    // 2. Get each data

    // 2.1 Open price
    std::string openkey = "OPEN-value";
    std::pair<int, int> op = this->gparse(blk, openkey);
    if (op.first == -1) {
	push_invalid();
	return -1;
    };
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

    rcomma(O); rcomma(H); rcomma(L); rcomma(C);

    // 3. Write to vector
    this->Open.push_back(O);
    this->High.push_back(H);
    this->Low.push_back(L);
    this->Close.push_back(C);
    this->Volumn.push_back("(blank)");
    this->Bid.push_back("(blank)");
    this->Ask.push_back("(blank)");

    return 0; // normal
}


int linecsv(std::string cpath, VPSS& codetype) {
    std::string type; // 1
    std::string code; // 2
    VS typev = {};
    VS codev = {};
    try {
	std::ifstream csvfile(cpath);
	std::getline(csvfile, type);
	std::getline(csvfile, code);

	std::string code, type;
	std::string tempc = "";
	std::string tempt = "";

	for (char c : type) {
	    if (c == ',') {
		type = tempt;
		tempt = "";
		typev.push_back(type);
	    }
	    tempt += c;
	}
	typev.push_back(tempt);

	for (char c : code) {
	    if (c == ',') {
		code = tempc;
		tempc = "";
		codev.push_back(tempc);
	    }
	    tempc += c;
	}
	codev.push_back(tempc);

	int csize = codev.size();
	int tsize = typev.size();

	if (csize != tsize) std::cout << "invalid input format" << std::endl;
	for (int i = 0; i < (csize < tsize) ? csize : tsize; i++) {
	    codetype.push_back(std::make_pair(codev[i], typev[i]));
	}
	csvfile.close();
    } catch (const std::ifstream::failure& e) {
	std::cout << "Exception opening/reading file" << std::endl;
	return 0;
    }
    return 1;
}



// Entry of hist parse
int main() {
    // 1. Read csv file from downhist.js (histprice)
    VPSS codetype;
    linecsv("histprice.csv", codetype);

    for (auto p : codetype) {
	std::cout << p.first << " " << p.second << std::endl;
    }

    // 2. Concatenate urls
    VS indexurl = {};
    for (int i = 0; i < codetype.size(); i++) {
	std::pair<std::string, std::string> ct = codetype[i];
	std::string code = ct.first;
	std::string type = ct.second;


	std::string url = "";

	if (type == "Stock" || type == "Option") url = YAHOO + code;
	else if (type == "Bond") url = BOND + code;
	else {
	    // do not alert
	    url = "";
	}
	indexurl.push_back(type+','+url); 
    }

    // 3. Get time
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::string date = std::to_string((now->tm_year + 1900));
    date += '-';
    date += std::to_string(now->tm_mon + 1);
    date += '-';
    date += std::to_string(now->tm_mday);

    std::cout << date << std::endl; // debug only

    // 4. Create HistParser and parse object
    HistParser* histparser = new HistParser(date);


    // 5. Parse data
    for (int i = 0; i < indexurl.size(); i++) {
	std::string url = indexurl[i]; // index,url

	// 5.1 Parse this "url", get url and type
	std::string type = "";
	for (auto it = url.begin(); it < url.end(); it++) {
	    if (*it == ',') {
		url.erase(url.begin(), it+1);
		break;
	    }
	    type.push_back(*it);
	}


	CurlObj* co = new CurlObj(url);
	std::string html = co->getData();

	// 5.2 Parse (unique)
	if (type == "Stock") {
	    histparser->hstock(html);
	} else if (type == "Option") {
	    histparser->hoption(html);
	} else if (type == "Bond") {
	    histparser->hbond(html);
	} else {
	    histparser->hnull();
	}

    }


    // 6. Write to file
    std::ofstream histcsv;
    histcsv.open("histoutput.csv");

    VVS histfields = {histparser->getOpen(), histparser->getHigh(),
		     histparser->getLow(),  histparser->getClose(),
                     histparser->getVolumn(), histparser->getBid(), histparser->getAsk()}; 


    for (VS histfield : histfields) {
	// each line
	for (std::string s : histfield) {
	    histcsv << s << ',';
	}
	histcsv << '\n';
    }

    histcsv.close();

}



































