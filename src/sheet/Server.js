const http = require('http');
const { exec } = require('child_process');

const httpserver = http.createServer();


httpserver.on("request", (req, res) => {
    console.log(req);
    var compass = exec('./compass.sh',
	    (error, stdout, stderr) => {
		console.log(stdout);
		console.log(stderr);
		if (error !== null) {
		    console.log(`exec error: ${error}`);
		}
	    }
    );
    res.write("got request!");
    res.write(compass);
    // res.end();
})


httpserver.on("connection", connection=> {
    console.log("Someone just connected!")
})

httpserver.listen(8080, ()=>console.log("Listenng on port 8080"));
