//===----------------------------------------------------------------------===//
//                         compass
//
// histlines.js
//
// Identification: src/sheet/histlines.js
//
// Usage:
// Get the total columns of today's sheet
// which is the index of tomorrow's start point
//
// Last Modified : 2022.1.26 Jiawei Wang 
//
// Copyright (c) 2022 Angold-4
//
//===----------------------------------------------------------------------===//


// https://stackoverflow.com/questions/21229180/convert-column-index-into-corresponding-column-letter
function columnToLetter(column)
{
  var temp, letter = '';
  while (column > 0)
  {
    temp = (column - 1) % 26;
    letter = String.fromCharCode(temp + 65) + letter;
    column = (column - temp - 1) / 26;
  }
  return letter;
}

function letterToColumn(letter)
{
  var column = 0, length = letter.length;
  for (var i = 0; i < length; i++)
  {
    column += (letter.charCodeAt(i) - 64) * Math.pow(26, length - i - 1);
  }
  return column;
}

// console.log(columnToLetter(40));

const {google} = require('googleapis');
const keys = require('../../../../Keys/compass.json') // safety reason
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
	range: 'HistPrices!D6:ZZ6', 
    };

    let data = await gsapi.spreadsheets.values.get(opt);
    let dataArray = data.data.values;
    // console.log(dataArray);
    // console.log(dataArray[0].length);
    // console.log(columnToLetter(dataArray[0].length + 4));
    let col = columnToLetter(dataArray[0].length + 4);
    console.log(col);
    // let line = dataArray.length + 73; // start index
    // console.log(line);

    fs.writeFile('../globaldata/col.txt', col, (err) => {
	console.log(err || "done");
    });
}
