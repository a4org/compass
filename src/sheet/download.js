//===----------------------------------------------------------------------===//
//                         compass
//
// download.js
//
// Identification: src/sheet/download.js
//
// Last Modified : 2022.1.12 Jiawei Wang
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//

const {google} = require('googleapis');
const keys = require('./creds.json')
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
	spreadsheetId: '11nAOR-7XIAjnhYY-1QuAzKk4xhnAPtq4QoohH6ptXNQ',
	// spreadsheetId: '1mvA960mm3QaFyRdwkfIRxhE1UQJl45QEUTnDVxtxiIE',
	range: 'Data!C9:D20',
    };

    let data = await gsapi.spreadsheets.values.get(opt);
    let dataArray = data.data.values;
    console.log(dataArray);


    var csv = dataArray.map(function(d){
	return d.join();
    }).join('\n');

    // console.log(csv);

    fs.writeFile('../sheet.csv', csv, (err) => {
	console.log(err || "done");
    });

}














