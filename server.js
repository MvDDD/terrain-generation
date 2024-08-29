let http = require("http")
let fs = require("fs")
const { execSync } = require('child_process');

let server = http.createServer((req, res)=>{
	console.log("req")
	if (req.url.split("?=")[0] == "/data.js"){
		try {
			execSync('main.exe ' + req.url.split("?=")[1], { stdio: 'inherit' });
			console.log('Executable completed successfully');
		} catch (error) {
			console.error('Error executing the executable:', error);
		}
		res.writeHead(200, "text/javascript")
		res.end(fs.readFileSync("out.txt", "utf-8"), "utf-8")
	} else if (req.url == "/2d"){
		res.writeHead(200, "text/html")
		res.end(fs.readFileSync("2d.html", "utf-8"), "utf-8")
	} else if (req.url == "/3d"){
		res.writeHead(200, "text/html")
		res.end(fs.readFileSync("3d.html", "utf-8"), "utf-8")
	} else if (req.url == "/three.js"){
		res.writeHead(200, "text/javascript")
		res.end(fs.readFileSync("three.js", "utf-8"), "utf-8")
	}
}).listen(8081)