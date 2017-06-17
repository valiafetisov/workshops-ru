var VLC = require('vlc-simple-player')
var mqtt = require('mqtt')

// подключаемся к mqtt-серверу
var client = mqtt.connect('mqtt://students.conformity.io', {
  clientId: 'your_unique_client_id',
  username: 'your_username_for_mqtt_server',
  password: 'your_password_for_mqtt_server'
})

// запускаем видео на компьютере
var player = new VLC('video/TheRingingScene.mp4')

// каждую секунду, получаем статус видео
player.on('statuschange', (error, status) => {
  if (error) return console.log(error)
  console.log('current time', status.time)

  if (status.time > 9 && status.time < 20) {
    // если время видео между 9 и 20 секундой, отправляем сообщение '1'
    client.publish('/rodchenko/movie/bzz', '1')
  } else {
    // если время видео между 9 и 20 секундой, отправляем сообщение '0'
    client.publish('/rodchenko/movie/bzz', '0')
  }
})
