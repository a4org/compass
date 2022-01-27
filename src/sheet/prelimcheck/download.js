//===----------------------------------------------------------------------===//
//                         compass
//
// download.js
//
// Identification: src/sheet/prelimcheck/download.js
//
// Last Modified : 2022.1.25 Jiawei Wang 
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

// Test Logic

const {google} = require('googleapis');
const keys = require('/home/angold/Desktop/Keys/compass.json') // safety reason

var fs = require('fs')

const client = new google.auth.JWT(
    keys.client_email, 
    null, 
    keys.private_key, 
    ['https://www.googleapis.com/auth/spreadsheets']
); // json web token


var line = 0; // line variable (global)

fs.readFile('globaldata/line.txt', "utf8", (error, data) => {
    if (error) {
	throw error;
    }
    line = data.toString();   // the line variable
    // line = line.slice(0, -1); // remove the last character
    client.authorize(function(err, tokens){ // call back function
	if (err) {
	    console.log(err);
	    return;
	}
	console.log('Connected');
	console.log(line);
	gsrun(client) // download the data
    });

    async function gsrun(cl) {
	const gsapi = google.sheets({version:'v4', auth:cl});

	const opt = {
	    // spreadsheetId: '11nAOR-7XIAjnhYY-1QuAzKk4xhnAPtq4QoohH6ptXNQ', // Test only
	    spreadsheetId: '1mvA960mm3QaFyRdwkfIRxhE1UQJl45QEUTnDVxtxiIE',
	    range: 'Prelim Check!H' + line + ':I2000', // I only need that data today
	};

	let data = await gsapi.spreadsheets.values.get(opt);
	let dataArray = data.data.values;

	console.log(dataArray);

	var csv = dataArray.map(function(d){
	    return d.join();
	}).join('\n');

	// console.log(csv);

	fs.writeFile('globaldata/sheet.csv', csv, (err) => {
	    console.log(err || "done");
	});
    }
});

