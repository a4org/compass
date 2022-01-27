const http = require('http');
const { exec } = require('child_process');

const httpserver = http.createServer();

httpserver.on("request", (req, res) => {
    var compass = exec('./seqbutton.sh',
	    (error, stdout, stderr) => {
		res.write(stdout);
		console.log(stdout);
		console.log(stderr);
		if (error !== null) {
		    console.log(`exec error: ${error}`);
		}
		res.end();
	    }
    );
})

httpserver.on("connection", connection=> {
    console.log("Someone just connected!")
})

httpserver.listen(7125, ()=>console.log("Listenng on port 7125"));
