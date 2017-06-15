// загружаем установленные библиотеки
var express = require('express')

// пакет say позволяет произносить текст
// подробнее: https://github.com/marak/say.js
var say = require('say')

// cоздаем express-приложение согласно документации express
// https://expressjs.com/en/starter/hello-world.html
var app = express()

// запускаем его на 8000 порту
// это значит, что теперь оно слушает все запросы,
// приходящие по адресу http://localhost:8000
app.listen(8000)

// на пустой запрос сервер отвечает html-файлом
app.get('/', function (req, res) {
  res.sendfile('index.html')
})

// на запрос по адресу /say (то есть http://localhost:8000/say)
// сервер отвечает текстом ok,
// помещает в консоль все данные, которые отправляются на этот адрес
// а в случае, если есть данные под именем data, то он их произносит
app.get('/say', function (req, res) {
  res.send('ok')
  console.log('text', req.query)
  if (req.query.data) {
    say.speak(req.query.data)
  }
})
