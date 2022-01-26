#include "sequencial/seqparse.hpp"

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


    VVS datafields;

    // 1. Read csv file from download.js (sheet)
    VPSS codetype; // code with its type
    readcsv("../globaldata/sheet.csv", codetype);

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

    // 2.2 Mapping them into a url map
    UMSV stockurlmap = {};  // stock url map, map url with its data
    UMSV optionurlmap = {}; // option url map, map url with its data
    UMSV bondurlmap = {};   // bond url map, map url with its data

    // 3. For each url

    for (auto pis : indexurl) {
    // for each url, in this step, we do not care whether it is valid or not
    // (actually, we cannot do that now) just check when push back to datafields

	VS datafield = {}; // store this data (S/O/B) ans

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

	// 3.2 Check whether map contains this url
	if (type == "Stock") {
	    if (stockurlmap.find(url) != stockurlmap.end()) {
		datafield = stockurlmap[url];
	    } else {
		CurlObj* co = new CurlObj(url);
		std::string html = co->getData();

		Parser* parser = new Parser(html);
		VS stockdata = parser->stock();

		std::string currency = stockdata[0];
		if (currency == " ") {
		    // invalid code, build invalid datafield check-ticker code set to 1
		    for (int i = 0; i < 31; i++) {
			datafield.push_back(" ");
		    }
		    datafield.push_back("1"); datafield.push_back(" ");
		    datafields.push_back(datafield);
		    stockurlmap[url] = datafield;
		    continue; // next url
		}
		// parse code 
		std::string code = "";
		for (auto rit = url.rbegin(); rit < url.rend(); rit++)  {
		    if (*rit == '/') {
			break;
		    }
		    code += *rit;
		}
		std::reverse(code.begin(), code.end());
		stockdata.erase(stockdata.begin());
		datafield = {currency};
		if (currency != "Invalid Code") datafield.push_back(code);
		for (std::string data : stockdata) datafield.push_back(data);
		stockurlmap[url] = datafield;
	    }
	} else if (type == "Option") {
	    if (optionurlmap.find(url) != optionurlmap.end()) {
		datafield = optionurlmap[url];
	    } else {
		CurlObj* co = new CurlObj(url);
		std::string code = "";
		for (auto rit = url.rbegin(); rit < url.rend(); rit++)  {
		    if (*rit == '/') {
			break;
		    }
		    code += *rit;
		}
		std::reverse(code.begin(), code.end());
		std::string html = co->getData();
		Parser* parser = new Parser(html);
		datafield = parser->option();
		std::string currency = datafield[0];
		if (currency == " ") {
		    // invalid code, build invalid datafield check-ticker code set to 1
		    for (int i = 0; i < 31; i++) {
			datafield.push_back(" ");
		    }
		    datafield.push_back("1"); datafield.push_back(" ");
		    datafields.push_back(datafield);
		    stockurlmap[url] = datafield;
		    continue; // next url
		}
		datafield.erase(datafield.begin());  // move currency to begin
		VS optionfield = {currency, " ", " ", " ", " ", " ", " ", " ", " ", " ", 
		    " ", " ", " ", " ", " ", " ", code};
		for (std::string data : datafield) {
		    optionfield.push_back(data);
		}
		datafield = optionfield;
		optionurlmap[url] = datafield;
	    }
	} else if (type == "Bond") {
	    if (bondurlmap.find(url) != bondurlmap.end()) {
		datafield = bondurlmap[url];
	    } else {
		CurlObj* co = new CurlObj(url);
		std::string html = co->getData();
		Parser* parser = new Parser(html);
		datafield = parser->bond();
		std::string currency = datafield[0];
		if (currency == " ") {
		    // invalid code, build invalid datafield check-ticker code set to 1
		    for (int i = 0; i < 31; i++) {
			datafield.push_back(" ");
		    }
		    datafield.push_back("1"); datafield.push_back(" ");
		    datafields.push_back(datafield);
		    stockurlmap[url] = datafield;
		    continue; // next url
		}
		datafield.erase(datafield.begin());
		VS bondfield = {currency, " ", " ", " ", " ", " ", " "};
		for (std::string data : datafield) {
		    bondfield.push_back(data);
		}
		datafield = bondfield;
		bondurlmap[url] = datafield;
	    }
	} else if (type == "Cash" || type == "TD") {
	    std::string currency = "HKD"; // default currency
	    datafield = {currency};
	} else {
	    // invalid code, build invalid datafield check-ticker code set to 1
	    for (int i = 0; i < 31; i++) {
		datafield.push_back(" ");
	    }
	    datafield.push_back("1"); datafield.push_back(" ");
	    stockurlmap[url] = datafield;
	}

	// 4. Push valid data into datafields
	datafields.push_back(datafield);
    }
    // 5. Write to file
    std::ofstream testcsv;
    testcsv.open("../globaldata/toutput.csv");

    for (VS datafield : datafields) {
	for (std::string s : datafield) {
	    testcsv << s << ',';
	}
	testcsv << '\n';
    }
    testcsv.close();
}
