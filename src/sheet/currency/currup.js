//===----------------------------------------------------------------------===//
//                         compass
//
// currup.js
//
// Identification: src/sheet/currency/currup.js
//
// Last Modified : 2022.2.13 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

const {google} = require('googleapis');
const keys = require('/home/angold/Desktop/Keys/compass.json') // safety reason
var fs = require('fs')

const client = new google.auth.JWT(
    keys.client_email, 
    null, 
    keys.private_key, 
    ['https://www.googleapis.com/auth/spreadsheets']
); // json web token


fs.readFile('globaldata/currline.txt', "utf8", (error, data) => {
    if (error) {
	throw error;
    }
    line = data.toString(); // the line variable
    // line = line.slice(0, -1);
    client.authorize(function(err, tokens){ // call back function
	if (err) {
	    console.log(err);
	    return;
	}
	console.log('Connected');
	console.log(line);
	gsrun(lient) // download the data
    });
    
    async function gsrun(cl) {
	const gsapi = google.sheets({version:'v4', auth:cl});

	// TODO Define dataArray

	fs.readFile('globaldata/curroutput.csv', "utf8", (error, data) => {
	    if (error) {
		throw error;
	    }
	    console.log(data.toString());

	    var lines = data.split("\n");
	    var dataArray = [];
	    for (var i = 0; i < lines.length; i++) {
		var words = lines[i].split(",");
		dataArray.push(words);
	    }

	    console.log(dataArray);

	    const inputopt = {
		// spreadsheetId: '11nAOR-7XIAjnhYY-1QuAzKk4xhnAPtq4QoohH6ptXNQ', // test only
		spreadsheetId: '1mvA960mm3QaFyRdwkfIRxhE1UQJl45QEUTnDVxtxiIE',
		range: 'Form Responses!T' + line,
		valueInputOption: 'USER_ENTERED',
		resource: {values: dataArray}
	    };

	    gsapi.spreadsheets.values.update(inputopt);
	});
    }
});
