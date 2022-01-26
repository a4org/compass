//===----------------------------------------------------------------------===// compass
//
// seqparse.cpp
//
// Identification: src/parse/seqparse.cpp
//
// Last Modified : 2022.1.19 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

#include "./seqparse.hpp"

// stable version of parsing in compass

std::pair<int, int> Parser::bparse(std::string bhtml, std::string key) {
    int start = bhtml.find(key);
    start += key.size();
    int end = bhtml.find("\"", start);

    if ( start == std::string::npos || end == std::string::npos || end < start ) {
	std::cerr << "Searching key error, on bparse bond" << std::endl;
	return std::make_pair(-1, -1);
    }

    return std::make_pair(start, end);
}


// currency, name, Bid, Ask, Coupon, Maturity, Rating, YTM, Seniority, Typ
VS Parser::bond() {
    if (this->phtml.size() < 1000) {
	VS invalidret = {" "};
	return invalidret;
    }
    std::string Currency, IssuerName, Bid, Ask, Coupon, Maturity, Rating, YTM, Seniority, Type;

    // Currency : bondCurrencyCode
    // IssuerName : bondIssuer
    // Bid : endBidPrice
    // Ask : endAskPrice
    // Coupon : bondName 
    // Maturity : bondName
    // Rating : bondCreditRate
    // YTM : yrsToMaturityDisplay
    // Seniority : seniority (2nd)
    // Type : bondType (2nd)
    
    // speed up
    int bblkend = this->phtml.find(":[[");
    std::string bhtml = this->phtml.substr(0, bblkend);
    // Seperately
    // 1. Currency, Coupon, Maturity 
    std::string namekey = "bondName\":\"";
    std::pair<int, int> np = this->bparse(bhtml, namekey);
    std::string Name = bhtml.substr(np.first, np.second - np.first);
    if (np.first == -1) return {};
    // std::cout << "Name: " << Name << std::endl;

    // 1.1 Coupon
    int coupstart = Name.find(' ') + 1;
    int coupend = Name.find('%', coupstart) + 1;
    Coupon = Name.substr(coupstart, coupend - coupstart);
    std::cout << "Coupon: " << Coupon << std::endl;

    // 1.2 Maturity
    int maturstart = Name.find(' ', coupend) + 1;
    int maturend = Name.find(' ', maturstart);
    Maturity = Name.substr(maturstart, maturend - maturstart);
    std::cout << "Maturity: " << Maturity << std::endl;

    // 1.3 Currency
    int currstart = Name.find('(', maturend) + 1;
    int currend = Name.find(')', currstart);
    Currency = Name.substr(currstart, currend - currstart);
    std::cout << "Currency: " << Currency << std::endl;


    // 2. IssuerName
    std::string issuerkey = "bondIssuer\":\"";
    std::pair<int, int> ip = this->bparse(bhtml, issuerkey);
    IssuerName = bhtml.substr(ip.first, ip.second - ip.first);
    std::cout << "IssuerName: " << IssuerName << std::endl;

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

    // 5. Rating
    std::string ratkey = "bondCreditRate\":\"";
    std::pair<int, int> rp = this->bparse(bhtml, ratkey);
    Rating = bhtml.substr(rp.first, rp.second - rp.first);
    std::cout << "Rating: " << Rating << std::endl;

    // 6. YTM
    std::string ytmkey = "yrsToMaturityDisplay\":\"";
    std::pair<int, int> yp = this->bparse(bhtml, ytmkey);
    YTM = bhtml.substr(yp.first, yp.second - yp.first);
    std::cout << "YTM: " << YTM << std::endl;

    // 7. Seniority 2nd
    std::string senkey = "seniority\":\"";
    int sfirst = bhtml.find(senkey) + senkey.size();
    int ssecond = bhtml.find(senkey, sfirst);
    ssecond += senkey.size();
    int send = bhtml.find("\"", ssecond);
    Seniority = bhtml.substr(ssecond, send - ssecond);
    std::cout << "Seniority: " << Seniority << std::endl;

    // 8. Type 2nd
    std::string typekey = "bondType\":\"";
    int tfirst = bhtml.find(typekey) + typekey.size();
    int tsecond = bhtml.find(typekey, tfirst);
    tsecond += typekey.size();
    int tend = bhtml.find("\"", tsecond);
    Type = bhtml.substr(tsecond, tend - tsecond);
    std::cout << "Type: " << Type << std::endl;


    // currency, name, Bid, Ask, Coupon, Maturity, Rating, YTM, Seniority, Type
    VS ret = {Currency, IssuerName, Bid, Ask, Coupon, Maturity, Rating, YTM, Seniority, Type};
    for (std::string& s : ret) {
	for (auto it = s.begin(); it < s.end(); it++) {
	    if (*it == ',') s.erase(it);
	}
    }
    return ret;
}

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
    if (this->phtml.size() < 100) {
	VS invalidret = {" "};
	return invalidret;
    }
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
    // filter ret value avoid xxx,xxx
    for (std::string& s : ret) {
	for (auto it = s.begin(); it < s.end(); it++) {
	    if (*it == ',') s.erase(it);
	}
    }
    return ret;
}

// currency, O, H, L, C, Strike, Ex Date, Put/Call,  Open interest
VS Parser::option() {
    if (this->phtml.size() < 100) {
	VS invalidret = {" "};
	return invalidret;
    }
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

    VS ret = {Currency, O, H, L, C, Strike, ExDate, PutCall, Interest};
    for (std::string& s : ret) {
	for (auto it = s.begin(); it < s.end(); it++) {
	    if (*it == ',') s.erase(it);
	}
    }
    return ret;
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
    /* Debug only
    std::ofstream out("test.html");
    out << blk;
    out.close();
    */
    return shtml.substr(begidx, endidx-begidx); // 5114, 3214 chars
};
