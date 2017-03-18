Module = {};
fetch('jsonTest.wasm')
  .then(response => response.arrayBuffer())
  .then((buffer) => {
    Module.wasmBinary = buffer;
    const script = document.createElement('script');
    script.src = 'jsonTest.js';
    document.body.appendChild(script);

    document.getElementById('myButton').addEventListener('click', () => {
      Module._sendJson();
      Module._printTitle();
    });
  });
