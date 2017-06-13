var moment = require('moment')
var Telegram = require('node-telegram-bot-api')

// загружаем точки в формате json в переменную locations
var locations = require('./history.json').locations
// подставляем токен бота в телеграме
var telegramToken = 'your_telegram_token_here'

// запускаем телеграм-бота
var bot = new Telegram(telegramToken, {polling: true})

// выполняем функцию при получении сообщения
bot.on('message', function (msg) {
  // создаем шаблон даты
  var template = 'HH:mm DD.MM.YYYY'

  // парсим присланный текст согласно нашему шаблону
  var date = moment(msg.text, template)
  // var date = moment().subtract(1, 'year')

  // проверяем если присланный текст это дата
  if (date.isValid() === false) {
    // если нет, отправляем сообщение
    bot.sendMessage(msg.chat.id, 'неправильная дата, используйте шаблон: ' + template)
    // и заканчиваем выполнение функции
    return
  }

  // выполняем фунцию getLocation (описанную ниже)
  // которая ищет локацию по дате
  var location = getLocation(date.valueOf())
  // проверяем, получилось ли найти данные
  if (location == null) {
    // если нет, отправляем сообщение
    bot.sendMessage(msg.chat.id, 'нет данных за этот период')
    // и заканчиваем выполнение функции
    return
  }

  // если все в порядке, отправляем дату в виде сообщения
  bot.sendMessage(msg.chat.id, moment(parseInt(location.timestampMs)).format(template))
  // и положение найденной точки в специального сообщения с картой
  bot.sendLocation(msg.chat.id, convertLocation(location.latitudeE7), convertLocation(location.longitudeE7))
})

// функция поиска положению по времени
// принимает parsedDate – дату в миллисекундах
// возвращает объект с latitudeE7 и longitudeE7
function getLocation (parsedDate) {
  // создаем пустую переменную index
  var index
  // проходим по всем точкам
  for (var i = 0; i < locations.length; i++) {
    // если время текущей точки меньше времени parsedDate
    if (locations[i].timestampMs < String(parsedDate)) {
      // запоминаем текущий индекс
      index = i
      // прекращаем выполнение цикла
      break
    }
  }
  // если переменная index по-прежнему пустая
  // или равна 0 или величине массива с точками, возвращаем null
  if (index == null || index === 0 || index === locations.length - 1) return null
  // если все в порядке, возвращаяем точку
  return locations[index]
}

// функция, которая конвертирует число или строку
// вида 557821583 в строку вида '55.7821583' –
// в стандарт записи геолокации, который принимает телеграм
function convertLocation (loc) {
  return String(loc).substr(0, 2) + '.' + String(loc).substr(2, loc.length)
}
