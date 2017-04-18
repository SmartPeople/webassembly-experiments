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

___

### .WAST, генерируемый emcc

sum.c
```
int sum(x, y){
	return x+y;
}
```

`emcc -s SIDE_MODULE=1 -s WASM=1 -O3 sum.c -o sum_emcc.wasm`


`wasm-dis.exe sum_emcc.wasm`

```
(module
 (type $0 (func (param i32 i32) (result i32)))
 (type $1 (func))
 (import "env" "memoryBase" (global $import$0 i32))
 (import "env" "memory" (memory $0 256))
 (import "env" "table" (table 0 anyfunc))
 (import "env" "tableBase" (global $import$3 i32))
 (global $global$0 (mut i32) (i32.const 0))
 (global $global$1 (mut i32) (i32.const 0))
 (export "__post_instantiate" (func $2))
 (export "_sum" (func $0))
 (export "runPostSets" (func $1))
 (func $0 (type $0) (param $var$0 i32) (param $var$1 i32) (result i32)
  (i32.add
   (get_local $var$1)
   (get_local $var$0)
  )
 )
 (func $1 (type $1)
  (nop)
 )
 (func $2 (type $1)
  (block $label$0
   (set_global $global$0
    (get_global $import$0)
   )
   (set_global $global$1
    (i32.add
     (get_global $global$0)
     (i32.const 5242880)
    )
   )
   (call $1)
  )
 )
 ;; custom section "dylink", size 5
)
```

___

### .WAST, генерируемый wabt

https://github.com/WebAssembly/wabt

`wast2wasm sum.wast -o sum.wasm`


```00 61 73 6D 01 00 00 00 01 07 01 60 02 7F 7F 01 7F 03 02 01 00 07 07 01 03 73 75 6D 00 00 0A 0A 01 08 00 20 00 20 01 6A 0F 0B```

wabt выдаёт более компактный код(!)