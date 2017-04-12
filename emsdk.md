# WebAssembly

## Установка и настройка emsdk


[Download and install](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html)

```
./emsdk update
./emsdk install latest
./emsdk activate latest
```


##### В случае сброса окружения:

```emsdk_env.bat```

```emsdk_env.sh```

##### Настройка путей

Основные настройки находятся в ```C:\Users\USERNAME\.emscripten```

```import os
LLVM_ROOT='.../emsdk/clang/fastcomp/build_incoming_vs2015_64/Release/bin'
NODE_JS='.../emsdk/node/node.exe'
PYTHON='.../emsdk/python/2.7.5.3_64bit/python.exe'
JAVA='.../emsdk/java/7.45_64bit/bin/java.exe'
EMSCRIPTEN_ROOT='.../emsdk/emscripten/incoming'
EMSCRIPTEN_NATIVE_OPTIMIZER='.../emsdk/emscripten/incoming_vs2015_64bit_optimizer/RelWithDebInfo/optimizer.exe'
BINARYEN_ROOT='.../emsdk/binaryen/master_vs2015_64bit_binaryen'
SPIDERMONKEY_ENGINE = ''
V8_ENGINE = ''
TEMP_DIR = '.../temp'
COMPILER_ENGINE = NODE_JS
JS_ENGINES = [NODE_JS]
```

Для работы требуется python2.7

В случае наличия установленных 2 и 3 версии, emcc может работать некорректно, в этом случае нужно вручную указать путь до python.exe версии 2.7 в файле ```.emscripten```

Аналогично прописать путь до  необходимой nodejs, если emcc перебивает путь на свою node-4.1.1 версию.




## Компиляция WASM


Флаги emcc (не все):
https://kripken.github.io/emscripten-site/docs/tools_reference/emcc.html


Пример компиляции.

```emcc --bind -o post.js -s WASM=1 -O3 -s TOTAL_MEMORY=268435456 Source.cpp -std=c++11```

#### --bind
https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/embind.html

Необходим для поддержки взаимодействия с++ и js через EMSCRIPTEN_BINDINGS.

```
#include <emscripten/bind.h>
.....
wstring myFunction(wstring s) {
	return s;
}
.....
namespace emscripten {
	EMSCRIPTEN_BINDINGS(my_module) {
		function("myFunction", &myFunction);
	}
}
```

Поддерживаемые типы

|C++ type |	JavaScript type|
|:-------:|----------------|
|void |	undefined |
|bool |	true or false |
|char |	Number |
|signed char |	Number |
|unsigned char |	Number |
|short |	Number |
|unsigned short |	Number |
|int |	Number |
|unsigned int |	Number |
|long |	Number |
|unsigned long |	Number |
|float |	Number |
|double |	Number |
|std::string |	ArrayBuffer, Uint8Array, Uint8ClampedArray, Int8Array, or String |
|std::wstring |	String (UTF-16 code units) |
|emscripten::val |	anything |

```emscripten::val``` подключается через ```<emscripten/val.h>```  и требует наличия ключа ```-std=c++11```

#### -o

* ```file.js``` - создаёт на выходе js-лоадер от emscripten и wasm файл
* ```file.html``` - создаёт на выходе js-лоадер от emscripten, wasm файл и html страницу с примером


#### -s WASM=1
Поддержка WebAssembly. Иначе компилируется asmjs файл.

#### -O3

Оптимизация кода (влияет на размер и быстродействие)

O3 - самое оптимальное

Всего выбор от -O0 (без оптимизации) до -O3 (максимальная оптимизация) и -Os\-Oz, которые от O3 особо не отличаются, но иногда ломают импорты функций.


#### -s TOTAL_MEMORY=268435456
Количество выделяемой wasm памяти в ```var TOTAL_MEMORY = Module['TOTAL_MEMORY'] || 268435456;```

Число должно быть кратно 64


#### -std=c++11
Поддержка новой версии стандарта С++



---
После компиляции на выходе получаются два файла: `post.wasm` и `post.js`


## Подключение wasm

### Вариант с подключением лоадера от emscripten
Важна очерёдность: сначала подключается wasm файл, и только потом его js-лоадер.

```
fetch('post.wasm')
  .then(response => response.arrayBuffer())
  .then((buffer) => {
    Module.wasmBinary = buffer;
    const script = document.createElement('script');
    script.src = './post.js'; //лоадер от emscripten
    document.body.appendChild(script);
    })
    .then( ...
````
Функции из Wasm'а вызываются как `Module.myFunction("data")`

____
### Вариант со своим лоадером
cube.c
```
int cube(int a){
	return a*a*a;
}
```
loader.js
```
function loadWebAssembly(filename, imports = {}) {
  return fetch(filename)
    .then(response => response.arrayBuffer())
    .then(buffer => WebAssembly.compile(buffer))
    .then(module => {
      imports.env = imports.env || {}
      Object.assign(imports.env, {
        memoryBase: 0,
        tableBase: 0,
        memory: new WebAssembly.Memory({ initial: 256, maximum: 1024 }),
        table: new WebAssembly.Table({ initial: 0, maximum: 0, element: 'anyfunc' })
      })
      return new WebAssembly.Instance(module, imports)
    })
}

function loadJS (url, imports = {}) {
  return fetch(url)
    .then(response => response.text())
    .then(code => new Function('imports', `return (${code})()`))
    .then(factory => ({ exports: factory(imports) }))
}
```

index.html
```html
<!DOCTYPE html>
<html>
<head>
  <script src="loader.js"></script>
</head>

<body>
  <script>
    loadWebAssembly('./cube.wasm').then(instance => {
      const cube = instance.exports._cube;
	  console.log(cube(3)); //27
    })
  </script>
</body>
</html>
```
