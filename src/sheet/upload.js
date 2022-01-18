//===----------------------------------------------------------------------===//
//                         compass
//
// upload.js
//
// Identification: src/sheet/upload.js
//
// Last Modified : 2022.1.12 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

const {google} = require('googleapis');
const keys = require('../../../Keys/compass-angold.json') // safety reason
var fs = require('fs')

const client = new google.auth.JWT(
    keys.client_email, 
    null, 
    keys.private_key, 
    ['https://www.googleapis.com/auth/spreadsheets']
); // json web token

client.authorize(function(err, tokens){ // call back function
    if (err) {
	console.log(err);
	return;
    }
    console.log('Connected');
    gsrun(client)
});

async function gsrun(cl) {
    const gsapi = google.sheets({version:'v4', auth:cl});

    // TODO Define dataArray

    fs.readFile('toutput.csv', "utf8", (error, data) => {
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
	    range: '1 Trade Record!V5',
	    valueInputOption: 'USER_ENTERED',
	    resource: {values: dataArray}
	};

	gsapi.spreadsheets.values.update(inputopt);
    });
}



