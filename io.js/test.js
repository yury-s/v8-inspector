var http = require('http');
var fs = require('fs');
var index = fs.readFileSync('/tmp/fib.js');

http.createServer(function (req, res) {
  console.log("Accepted connection (" + req.url + "): " + new Error().stack);
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end(index);
}).listen(2015);
