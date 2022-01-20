WebUSB
======

Connecting hardware to the web.

![Build status](https://github.com/WICG/webusb/actions/workflows/pr-push.yml/badge.svg)

Documents
---------

* [Specification](https://wicg.github.io/webusb/) (Editor's Draft)
* [Test API Specification](https://wicg.github.io/webusb/test/) (Editor's Draft)

Communication
-------------

* [Stack Overflow](https://stackoverflow.com/questions/tagged/webusb) for questions & answers about **using** WebUSB.
* [Issue Tracker](https://github.com/wicg/webusb/issues) for **specification** issues.
* [Chromium Issue Tracker](https://bugs.chromium.org/p/chromium/issues/list?q=component%3ABlink>USB) for Chrome **implementation** issues.

Implementation Status
---------------------

* WebUSB is available in Chrome 61 for Android, Chrome OS, Linux, macOS and Windows.
* A port for Node.js also exists - https://github.com/thegecko/webusb

**Note:** The WebUSB landing page notification is currently disabled in Chrome on Windows due to [Chromium issue 656702](https://crbug.com/656702). Implementation of new, more stable USB support for Windows is tracked by Chromium issues [422562](https://crbug.com/422562) and [637404](https://crbug.com/637404).

Demos
-----

* [Arduino Integration](https://github.com/webusb/arduino/)
* [WebLight](https://github.com/sowbug/weblight)
* [Yubikey HOTP](https://github.com/santiagotorres/yubikey-webusb)
* [USB Device Firmware Upgrade (DFU)](https://github.com/devanlai/webdfu)
* [WebADB](https://github.com/webadb/webadb.js)
* [Zephyr JS IDE](https://github.com/01org/zephyrjs-ide), [hosted IDE](https://01org.github.io/zephyrjs-ide/)
* [DSLR camera control](https://web-gphoto2.rreverser.com/)

Articles
--------

* [Access USB Devices on the Web](https://developers.google.com/web/updates/2016/03/access-usb-devices-on-the-web)
* [The WebUSB Security Model](https://medium.com/dev-channel/the-webusb-security-model-f48ee04de0ab)
* [WebUSB and JavaScript Robotics](http://iceddev.com/blog/webusb-and-javascript-robotics/) (PageNodes)
* [Program a smart device directly, no install needed](https://medium.com/@kennethrohde/program-a-smart-device-directly-no-install-needed-cd8b29320d76) (Arduino running Zephyr)
* [Web enabling legacy devices](https://medium.com/@larsgk/web-enabling-legacy-devices-dc3ecb9400ed) Discusses USB CDC (USB Serial devices)
