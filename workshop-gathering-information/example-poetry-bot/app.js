var lowdb = require('lowdb')
var db = lowdb('./chatbot.json')
db.defaults({messages: [], users: []}).write()

var fs = require('fs')
var poetry = fs.readFileSync('./poetry.txt', 'utf-8').split('\n')
poetry = poetry.slice(0, poetry.length - 1)
console.log(poetry)

var Telegram = require('node-telegram-bot-api')
var token = 'HERE_IS_YOUR_TOKEN'
var bot = new Telegram(token, {polling: true})
bot.on('message', function (message) {
  console.log(message)
  var messagesNumber = db.get('messages').find({id: message.id}).value().length
  bot.sendMessage(message.chat.id, poetry[messagesNumber % poetry.length])
  // записываем в бд все сообщения без исключения
  db.get('messages').push(message).write()
  // проверяем, если в базе уже есть пользователи с message.from.id
  if (!db.get('users').find({id: message.from.id}).value()) {
    // если такого нет, записываем
    db.get('users').push(message.from).write()
  }
})
