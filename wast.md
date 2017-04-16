## WAST


* https://github.com/WebAssembly/binaryen

* Вместе с пакетом emsdk (..\emsdk\binaryen\master_vs2015_64bit_binaryen\bin)

### wasm-as
WAST to WASM
___



sum.wast
```
(module
  (export "sum" (func $s))
  (func $s (param $x i32) (param $y i32) (result i32)
    (return
      (i32.add
        (get_local $x)
        (get_local $y)
      )
    )
  )
)
```

`wasm-as.exe sum.wast -o sum.wasm`

sum.wasm (в hex редакторе)
```
00 61 73 6D 01 00 00 00 01 87 80 80 80 00 01 60 02 7F 7F 01 7F 03 82 80 80 80 00 01 00 07 87 80 80 80 00 01 03 73 75 6D 00 00 0A 8E 80 80 80 00 01 88 80 80 80 00 00 20 00 20 01 6A 0F 0B
```

Запуск в браузере

```
WebAssembly.compile(new Uint8Array(`
00 61 73 6D 01 00 00 00 01 87 80 80 80 00 01 60 02 7F 7F 01 7F 03 82 80 80 80 00 01 00 07 87 80 80 80 00 01 03 73 75 6D 00 00 0A 8E 80 80 80 00 01 88 80 80 80 00 00 20 00 20 01 6A 0F 0B
`.trim().split(/[\s\r\n]+/g).map(str => parseInt(str, 16))
)).then(module => {
  const instance = new WebAssembly.Instance(module);
  const sum = instance.exports.sum;
  console.log(sum(42, 37)); // 79
});
```

### wasm-dis
WASM to WAST

`wasm-dis.exe sum.wasm`

```
(module
 (type $0 (func (param i32 i32) (result i32)))
 (memory $0 0)
 (export "sum" (func $0))
 (func $0 (type $0) (param $var$0 i32) (param $var$1 i32) (result i32)
  (return
   (i32.add
    (get_local $var$0)
    (get_local $var$1)
   )
  )
 )
)
```
