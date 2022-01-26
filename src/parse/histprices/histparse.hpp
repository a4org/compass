#pragma once

#include "../sequencial/seqparse.hpp"

// HistParser is the class to deal with the history data html file
// Which is inherance from Parser class
class HistParser : private Parser{
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
	return 0;
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
     * HistParser::push_invalid
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
