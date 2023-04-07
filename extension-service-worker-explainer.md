# Explainer for WebUSB & Extension Service Worker Integration

A proposal to expose WebUSB in service workers created by extensions.

<!-- TOC -->
<!-- /TOC -->

## Introduction
The WebUSB API allows web pages to request permission to access a connected USB
device and, once permission is granted, enables the web page to control the
device. Extensions can currently access the API from the extension [background
page](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/manifest.json/background).
The goal of this proposal is to provide an alternate means for extensions to
access WebUSB API from a similar background context without the use of a
background page.

## Proposed specification changes
The Navigator attribute `navigator.usb` is currently exposed for DedicatedWorker
and SharedWorker. With this proposal it would be exposed for all Workers.

```webidl
[Exposed=(Worker,Window), SecureContext]
interface USB : EventTarget {
  attribute EventHandler onconnect;
  attribute EventHandler ondisconnect;
  Promise<sequence<USBDevice>> getDevices();
  [Exposed=Window] Promise<USBDevice> requestDevice(USBDeviceRequestOptions options);
};

[Exposed=Worker, SecureContext]
partial interface WorkerNavigator {
  [SameObject] readonly attribute USB usb;
};
```

The `requestDevice` method is not exposed in the ServiceWorker. Extensions must
create a Window in order to request device permissions. Once permission is
granted, the device may be accessed from the worker.

An extension service worker may register listeners for the `connect` and
`disconnect` events, but these events may not be dispatched if the service
worker is not active.

## Security considerations
Accessing the WebUSB API from an extension service worker is no less secure than
accessing from an extension background page since the exposed functionality is
the same.

To mitigate against accidentally granting device permissions, requesting new
permissions always requires displaying a permission dialog to the user. Since
`requestDevice` is only exposed in Windows, this means that extensions must also
create a Window before requesting new device permissions.

Once permission is granted, this feature enables an extension to access a device
in the background without creating a tab or window. Implementations should take
care to notify the user when an extension is accessing a device in the
background.

Both WebUSB and Service Worker features block cross-origin access. Both features
are integrated with Feature Policy, enabling developers to control where and how
these features are exposed in their applications.
