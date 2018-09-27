// Use with http://localhost:3030/summer.html or winter.html
var http = require('http');
var url = require('url');
var fs = require('fs');

const hostname = '127.0.0.1';
const port = 8080;

const server = http.createServer(function(req, res) {
  var q = url.parse(req.url, true);
  var filename = "." + q.pathname;
  //console.log("q %o", q);
  console.log("filename " + filename);
  fs.readFile(filename, function(err, data) {
    if (err) {
      res.writeHead(404, {'Content-Type': 'text/html'});
      return res.end("404 Not Found");
    }
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.write(data);
    return res.end();
  });
});

server.listen(port, hostname, function() {
  console.log('Server running at http://'+ hostname + ':' + port + '/');
});

