Module = {};
let jsonData;

fetch('post.wasm')
  .then(response => response.arrayBuffer())
  .then((buffer) => {
    Module.wasmBinary = buffer;
    const script = document.createElement('script');
    script.src = 'post.js';
    document.body.appendChild(script);
    })
    .then(function() {
    fetch("cn.json")
          .then(response => response.json())
          .then((data) => {
            jsonData = JSON.stringify(data);
          });
    });
