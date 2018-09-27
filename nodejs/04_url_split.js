// Use with http://localhost:3030/?year=2017&month=July
const http = require('http');
var url = require('url');

const hostname = '127.0.0.1';
const port = 8080;

const server = http.createServer(function(req, res) {
  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/plain');
  res.write("URL " + req.url + "\n");
  // Dump object to console
  //console.log("my object: %o", req)
  var q = url.parse(req.url, true).query;
  var txt = q.year + " " + q.month;
  res.write(txt);
  res.end();
});

server.listen(port, hostname, function() {
  console.log('Server running at http://'+ hostname + ':' + port + '/');
});

