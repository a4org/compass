#include "currency/currency.hpp"

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
    readcsv("globaldata/currform.csv", codetype);

    // 2.1 Concatenate urls
    // Do not need multithreading (stable)

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

    CurrParser* CParser = new CurrParser();
    VS ret = {};

    for (auto pis : indexurl) {
	// 3.1 Parse this "url", get url and type
	std::string url = pis;
	std::cout << url << std::endl;
	std::string type = "";
	for (auto it = url.begin(); it < url.end(); it++) {
	    if (*it == ',') {
		url.erase(url.begin(), it+1);
		break;
	    }
	    type.push_back(*it);
	}

	if (type == "Stock" || type == "Option") {
	    CurlObj* co = new CurlObj(url);
	    std::string html = co->getData();
	    ret.push_back(CParser->cyahoo(html));
	} else if (type == "Bond") {
	    CurlObj* co = new CurlObj(url);
	    std::string html = co->getData();
	    ret.push_back(CParser->cbond(html));
	} else {
	    ret.push_back("HKD");
	}
    }

    // Write to csv
    std::ofstream testcsv;
    testcsv.open("globaldata/curroutput.csv");

    for (std::string currency : ret) {
	testcsv << currency;
	testcsv << '\n';
    }
    testcsv.close();
}

