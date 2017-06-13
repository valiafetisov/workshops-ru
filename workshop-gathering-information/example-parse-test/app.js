var request = require('request')

// создаем функцию которая принимает:
// первым параметром: id фотографии (число)
// вторым параметром: функцию (так называемый callback)
// и ничего не возвращает, только логирует полученные данные в консоль
function getPhotoMarks (photoId, cb) {
  var baseUrl = 'http://club.foto.ru/gallery/photos/photo.php?photo_id='
  request.get(baseUrl + photoId, function (error, response) {
    if (error || response.statusCode !== 200) return console.error('ошибка запроса, фотография:', photoId)

    // находим в исходном тексте любой страницы с фотографией строку
    // <a title='Статистика оценок' id='marks_public' href='/gallery/photo_statistic.php?photo_id=2510325'>[0|0|0|34]</a>
    // составляем по ней регулярное выражение
    var matches = /id='marks_public'.+?>\[(.+?)\]</gm.exec(response.body)

    // проверяем наличие результата
    // если ничего не найдено, выводим ошибку
    if (matches == null || matches[1] == null) return console.error('ничего не найдено, фотография:', photoId)

    // выводим matches в консоль
    var marks = matches[1].split('|')

    // формируем объект, который будет выведен в консоль
    var result = {
      photoId: photoId,
      marks2: marks[0],
      marks3: marks[1],
      marks4: marks[2],
      marks5: marks[3]
    }

    // выводим результат в консоль
    console.log(result)
  })
}

// пример тестового выполнение функции
// getPhotoMarks(2510325)

// цикл на первые 1000 фотографий
// функция getPhotoMarks 'обернута' в функцию setTimeout, которая
// добавляет перерывы между запросами (1000 мс = 1 секунда),
// чтобы бота не заблокировали
for (var i = 1; i <= 1000; i++) {
  setTimeout(getPhotoMarks, 1000 * i, i)
}
