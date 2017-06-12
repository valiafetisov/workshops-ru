var Telegram = require('node-telegram-bot-api')

var telegramToken = 'your_telegram_token_here'
var answers = [
  'ответ 1',
  'ответ 2',
  'ответ 3',
  'ответ 4'
]

// с этого момента программа подключается к серверу telegram
// и начнет принимать данные и ждать от нас команд
var bot = new Telegram(telegramToken, {polling: true})

bot.on('message', function (msg) {
  console.log(msg)

  const reply = answers[parseInt(Math.random() * answers.length)]
  bot.sendMessage(msg.chat.id, reply)

  // if (msg.text.toLowerCase().indexOf('найди картинку') >= 0) {
  //   // bot.sendMessage(msg.chat.id, 'привет')
  // }
})
