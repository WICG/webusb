(function() {
  'use strict';

  function ab2str(buf) {
    return String.fromCharCode.apply(null, new Uint8Array(buf));
  }

  document.addEventListener('DOMContentLoaded', event => {
    let statusDisplay = document.querySelector('#status');
    let redSlider = document.querySelector('#red');
    let greenSlider = document.querySelector('#green');
    let blueSlider = document.querySelector('#blue');
    let port;

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

    serial.getPorts().then(ports => {
      if (ports.length == 0) {
        statusDisplay.textContent = 'No device found.';
      } else {
        port = ports[0];

        port.connect().then(() => {
          statusDisplay.textContent = 'Connected.';

          port.onReceive = data => {
            console.log(ab2str(data));
          }
          port.onReceiveError = error => {
            console.error(error);
          };
        }, error => {
          console.error(error);
        });
      }
    });
  });
})();
