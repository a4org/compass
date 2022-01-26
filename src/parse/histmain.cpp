#include "histprices/histparse.hpp"


int linecsv(std::string cpath, VPSS& codetype) {
    std::string type; // 1
    std::string code; // 2
    VS typev = {};
    VS codev = {};
    try {
	std::string code, type;
	std::ifstream csvfile(cpath);
	std::getline(csvfile, type);
	std::getline(csvfile, code);

	std::string tempc = "";
	std::string tempt = "";

	for (char c : type) {
	    if (c == ',') {
		type = tempt;
		tempt = "";
		typev.push_back(type);
		continue;
	    }
	    tempt += c;
	}
	typev.push_back(tempt);

	for (char c : code) {
	    if (c == ',') {
		code = tempc;
		tempc = "";
		std::cout << code << std::endl;
		codev.push_back(code);
		continue;
	    }
	    tempc += c;
	}
	codev.push_back(tempc);

	int csize = codev.size();
	int tsize = typev.size();

	if (csize != tsize) std::cout << "invalid input format" << std::endl;

	std::cout << csize << " " << tsize << std::endl;
	int size = std::min(csize, tsize);
	for (int i = 0; i < size; i++) {
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
    // 1. Read csv file from downhist.js (histprices)
    VPSS codetype;
    linecsv("../globaldata/histprices.csv", codetype);

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
    histcsv.open("../globaldata/histoutput.csv");

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
