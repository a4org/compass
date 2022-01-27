//===----------------------------------------------------------------------===//
//                         compass
//
// row.js
//
// Identification: src/sheet/histprices/row.js
//
// Usage:
// Get the total row of today's HistPrices sheet
// which is the index of tomorrow's start point
//
// Last Modified : 2022.1.26 Jiawei Wang 
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

    const opt = {
	spreadsheetId: '11nAOR-7XIAjnhYY-1QuAzKk4xhnAPtq4QoohH6ptXNQ', // Test only
	// spreadsheetId: '1mvA960mm3QaFyRdwkfIRxhE1UQJl45QEUTnDVxtxiIE',
	range: 'HistPrices!C13:D2000', 
    };

    let data = await gsapi.spreadsheets.values.get(opt);
    let dataArray = data.data.values;
    // console.log(dataArray);
    let row = dataArray.length + 13; // start index
    console.log(row);

    fs.writeFile('globaldata/row.txt', row, (err) => {
	console.log(err || "done");
    });
}

