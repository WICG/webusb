(function() {
  'use strict';

  function ab2str(buf) {
    return String.fromCharCode.apply(null, new Uint8Array(buf));
  }

  document.addEventListener('DOMContentLoaded', event => {
    let connectButton = document.querySelector("#connect");
    let statusDisplay = document.querySelector('#status');
    let redSlider = document.querySelector('#red');
    let greenSlider = document.querySelector('#green');
    let blueSlider = document.querySelector('#blue');
    let port;

    function connect() {
      port.connect().then(() => {
        statusDisplay.textContent = '';
        connectButton.textContent = 'Disconnect';

        port.onReceive = data => {
          console.log(ab2str(data));
        }
        port.onReceiveError = error => {
          console.error(error);
        };
      }, error => {
        statusDisplay.textContent = error;
      });
    }

    function onUpdate() {
      if (!port) {
        return;
      }

      let view = new Uint8Array(3);
      view[0] = parseInt(redSlider.value);
      view[1] = parseInt(greenSlider.value);
      view[2] = parseInt(blueSlider.value);
      port.send(view);
    };

    redSlider.addEventListener('change', onUpdate);
    greenSlider.addEventListener('change', onUpdate);
    blueSlider.addEventListener('change', onUpdate);

    connectButton.addEventListener('click', function() {
      if (port) {
        port.disconnect();
        connectButton.textContent = 'Connect';
        statusDisplay.textContent = '';
      } else {
        serial.requestPort().then(selectedPort => {
          port = selectedPort;
          connect();
        });
      }
    });

    serial.getPorts().then(ports => {
      if (ports.length == 0) {
        statusDisplay.textContent = 'No device found.';
      } else {
        statusDisplay.textContent = 'Connecting...';
        port = ports[0];
        connect();
      }
    });
  });
})();
