# Графический пользовательский интерфейс

__Примеры__:
 - net.art
    - [jodi.org](http://jodi.org)
    - [Алексей Шульгин](http://easylife.org), в частности [form art](http://www.c3.hu/collection/form/)
    - [Оля Лялина](http://art.teleportacia.org/#CenterOfTheUniverse)
 - [Rafaël Rozendaal](http://www.newrafael.com/):
    - [fallingfalling](http://www.fallingfalling.com/)
    - [pleaselike](http://www.pleaselike.com/)
 - Экспериментальные дизайн-студии. например [studiomoniker](https://studiomoniker.com)
    - [pointerpointer.com](http://pointerpointer.com/), [описание](https://studiomoniker.com/projects/pointer-pointer)
    - [donottouch.org](http://donottouch.org/)
 - Бруталистское движение в веб-дизайне [brutalistwebsites.com](http://brutalistwebsites.com/)
 - Другие интересные сайты [hoverstates](http://hoverstat.es)


### Первый статичный файл

HTML (расшифровывается как HyperText Markup Language — «язык гипертекстовой разметки») — стандартизированный язык разметки документов. Большинство веб-страниц в интернете описаны на языке HTML (или XHTML). Состоит из вложенных в друг друга элементов (тегов) выделяемых символами `<` и `>`. Теги могут содержать атрибуты, например `<meta charset="utf-8">`.

Структура. HTML начитается с объявления спецификацию языка, затем следует тег `<html></html>` который включает в себя весь код страницы. Внутри тега `<html>` возможны только два тега: `<head>` и `<body>`. Внутри тега `<head>` указывается служебная информация, необходимая браузеру до начала рендера страницы. Например кодировка, заголовок страницы и тд.

Пример простейшего файла:

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>empty example</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style type="text/css">
      html, body {
        background-color: black;
      }
    </style>
    <script type="text/javascript">
      console.log('test')
    </script>
  </head>
  <body>
    <div>text</div>
  </body>
</html>
```

Каждый тег имеет определенное значение, заданное браузером. Например:

- `<!doctype html>` – сообщает браузеру, какую спецификацию языка html использовать, в данном случае это html5
- `<meta charset="utf-8">` – сообщает браузеру в какой кодировке стоит интерпретировать текст на странице.
- `<title>empty example</title>` – задает название страницы, указанное на вкладке
- `<meta name="viewport" content="width=device-width, initial-scale=1">` – задает параметры того, как страница должна отображаться на разного экранах с разным разрешением (например телефонах, часах и телевизорах)
- `<style type="text/css"></style>` внутри этого тега можно поместить CSS код, о котором чуть позже
- `<script type="text/javascript"></script>` внутри этого тега можно поместить JS код, который выполнится в браузере у пользователя

Весь этот текст можно сохранить в обыкновенный текстовый файл, с расширением `.html` и затем открыть с помощью браузера. Пример готового файла: [example-static-page](example-static-page/index.html)


### Страница посылающая запросы

Для любого элемента страницы, внутри `<body>` можно задать атрибут `onclick="functionName()"`. Тогда, при клике на этот элемент, вызовется соответствующая функция, заданная выше. Таким образом можно привязать выполнение кода к элементам страницы.

JavaScript, выполняемый на странице, также получает доступ к переменной `document` в которой находятся все элементы страницы. Чтобы например, получить тект написанный пользователем в тектовое поле, нужно сначала найти на странице нужный элемент, например используя встроенный в `document` метод `document.querySelector('.myClass input')` (найдет на странице элемент с классом `myClass` и `input` внутри него). А затем прочитать параметр `value` у этого текстового элемента.

Отправить запрос на сервер можно с помощью `xhttp`, например можно создать функцию, которая при будет отправлять текст из `input` на локальный адрес `/say`:

```javascript
var send = function () {
  // находим элемент на странице
  var input = document.querySelector('input')
  // создаем запрос к серверу
  var xhttp = new XMLHttpRequest()
  // создаем функцию, которая выполнится после запроса
  xhttp.onreadystatechange = function () {
    // если запрос произошел с ошибкой, прекращаем выполнение
    if (this.readyState !== 4 || this.status !== 200) return
    // логируем ответ сервера
    console.log('response', this.responseText)
    // очищаем текстовое поле
    input.value = ''
  }
  // задаем тип запроса (GET) и адрес (/say)
  xhttp.open('GET', '/say', true)
  // задаем заголовки запроса
  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded')
  // отправляем текст введенный в текстовое поле
  xhttp.send('data=' + input.value)
}
```


### Сервер

Для того, чтобы принять запрос отправленный страницей нам понадобится сервер: программа, которая будет _отдавать_ файл index.html _клиенту_ и принимать запросы.

Самый простой и популярный способ создания сервера в node.js – пакет `express`, который мы можем использовать. Для этого его нужно установить в папку проекта (`npm install express`). И добавить файл с кодом сервера:

```javascript
// записываем установленный пакет в перменную
var express = require('express')

// cоздаем express-приложение согласно документации express
// https://expressjs.com/en/starter/hello-world.html
var app = express()

// слушаем локальный порт 8000 на нашем компьютере
app.listen(8000)

// при запросе без параметров
// (то есть http://localhost:8000)
// отдаем выше созданную страницу
app.get('/', function (request, response) {
  response.sendfile('index.html')
})

app.get('/say', function (request, response) {
  response.send('OK')
  console.log(request.query)
})
```

Теперь, если запустить приложение `node app.js` и открыть браузер по адресу [http://localhost:8000](http://localhost:8000) можно увидеть страницу. При нажатии на кнопку `send`, в консоль должна попасть фраза, введенная в текстовом поле. Полный пример приложения, которое произносит (на сервере) введенный пользователем текст на странице: [example-say-input](example-say-input).

Такое приложение можно поместить в интернет с помощью несложных действий, а именно [_обратного туннеля_](../workshop-deploy#3-Обратный-туннель).
