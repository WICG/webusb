(function() {
  'use strict';

  hterm.defaultStorage = new lib.Storage.Local();

  var port;

  let t = new hterm.Terminal();
  t.onTerminalReady = () => {
  	let io = t.io.push();

  	io.onVTKeystroke = str => {
  		if (port !== undefined) {
        port.send(str2ab(str)).catch(error => {
          t.io.println('Send error: ' + error);
        });
      }
  	};

    io.sendString = str => {
      if (port !== undefined) {
        port.send(str2ab(str)).catch(error => {
          t.io.println('Send error: ' + error);
        });
      }
    };
  };

  function ab2str(buf) {
    return String.fromCharCode.apply(null, new Uint8Array(buf));
  }

  function str2ab(str) {
    let view = new Uint8Array(str.length);
    for (var i = 0; i < str.length; i++) {
      view[i] = str.charCodeAt(i);
    }
    return view.buffer;
  }

  document.addEventListener('DOMContentLoaded', event => {
    t.decorate(document.querySelector('#terminal'));
    t.installKeyboard();

    serial.getPorts().then(ports => {
      if (ports.length == 0) {
        t.io.println('No device found.');
      } else {
        ports[0].connect().then(() => {
          t.io.println('Connected.');
          port = ports[0];
          port.onReceive = data => {
            t.io.print(ab2str(data.buffer));
          }
          port.onReceiveError = error => {
            t.io.println('Receive error: ' + error);
          };
        }, error => {
          t.io.println('Connection error: ' + error);
        });
      }
    });
  });
})();
