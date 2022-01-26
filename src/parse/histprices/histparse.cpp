//===----------------------------------------------------------------------===//
//                                  compass
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

#include "./histparse.hpp"


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
