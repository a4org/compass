//===----------------------------------------------------------------------===//
//                         compass
//
// currline.js
//
// Identification: src/sheet/currency/currline.js
//
// Usage:
// Get the total line of today's sheet
// which is the index of tomorrow's start point
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

client.authorize(function(err, tokens){ // call back function
    if (err) {
	console.log(err);
	return;
    }
    console.log('Connected');
    gsrun(client)
});

async function gsrun(cl) {
    const gsapi = google.sheets({version:'v4', auth:l});

    const opt = {
	// spreadsheetId: '11nAOR-7XIAjnhYY-1QuAzKk4xhnAPtq4QoohH6ptXNQ', // Test only
	spreadsheetId: '1mvA960mm3QaFyRdwkfIRxhE1UQJl45QEUTnDVxtxiIE',
	range: 'Form Responses!T2:T2000', 
    };

    let data = await gsapi.spreadsheets.values.get(opt);
    let dataArray = data.data.values;
    // console.log(dataArray);
    let line = dataArray.length + 1; // start index
    console.log(line);

    fs.writeFile('globaldata/currline.txt', line, (err) => {
	console.log(err || "done");
    });
}
c
