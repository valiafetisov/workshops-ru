var express = require('express')

var counter = 0

var app = express()
app.listen(8000)

app.get('/', function (req, res) {
  res.sendfile('index.html')
})

app.get('/counter/increment', (req, res) => {
  console.log(req.headers)
  counter++
  res.send({counter: counter})
})

app.get('/counter/get', (req, res) => {
  res.send({counter: counter})
})
