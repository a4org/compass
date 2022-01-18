const http = require('http');

const httpserver = http.createServer();


httpserver.on("request", (req, res) => {
    console.log(req);
    res.write("got request!");
    res.end();
})


httpserver.on("connection", connection=> {
    console.log("Someone just connected!")
})

httpserver.listen(8080, ()=>console.log("Listenng on port 8080"));
