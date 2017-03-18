Module = {};
fetch('JsonLib.wasm')
  .then(response => response.arrayBuffer())
  .then((buffer) => {
    Module.wasmBinary = buffer;
    const script = document.createElement('script');
    script.src = 'JsonLib.js';
    document.body.appendChild(script);

    document.getElementById('myButton').addEventListener('click', () => {

      var j = new Module.JsonLib();
      var s = '{"n": 42, "abc": "qwe"}';

      j.setJson(s);
      console.log(j.getJson());
      j.del("abc");
      console.log(j.getJson());
      j.delete();

    });
  });
