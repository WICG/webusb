# Explainer for Unrestricted WebUSB

A proposal to enable trusted web applications to bypass security restrictions
in the WebUSB API.

<!-- TOC -->
<!-- /TOC -->

## Introduction
The original intent of the WebUSB API was to support writing companion
applications for devices not otherwise supported by the browser or operating
system. Many USB devices use standard drivers included with the operating system
and are accessed by applications and websites through higher-level interfaces.
The interface classes for which sites should be using a higher-level interface
are placed on the [protected interface class list](https://wicg.github.io/webusb/#has-a-protected-interface-class)
and cannot be claimed by a site using WebUSB. Similarly, the [device blocklist](https://github.com/WICG/webusb/blob/main/blocklist.txt)
lists a number of security devices which are not accessible to sites using
WebUSB.

These security restrictions make WebUSB unsuitable for the USB redirection
features common in Virtual Desktop Infrastructure (VDI) and remote desktop
applications, where local peripherals need to be controlled by drivers and
applications running on a remote system. While high-level redirection is
sometimes possible these systems typically require low-level USB access in
order to provide the best compatibility.

## Proposed specification changes
The WebUSB API already requires an application to request permission from the
user for each device it wants to access. Access to the WebUSB feature is also
controlled by Permissions Policy using the feature `"usb"`.

This proposal introduces a new Permissions Policy feature `"usb-unrestricted"`.
An application with the `"usb-unrestricted"` feature is allowed to access
protected USB interfaces and blocklisted USB devices.

The `"usb-unrestricted"` feature exposes device capabilities deemed too
dangerous to be accessed by an untrusted application. Untrusted applications
will not be able to request `"usb-unrestricted"`. Under this proposal,
`"usb-unrestricted"` can only be used with [Isolated Web Apps](https://github.com/WICG/isolated-web-apps/blob/main/README.md)
by including the feature in the `"permissions_policy"` manifest field.

```
"permissions_policy": {
   "usb": [ "self" ],
   "usb-unrestricted": [ "self" ]
}
```

## Security considerations
USB device access can be abused to attack the device or the host, as described
in the [Security and Privacy Considerations](https://wicg.github.io/webusb/#security-and-privacy)
section of the WebUSB spec. The threat model is unchanged, except that a
malicious application with `"usb-unrestricted"` can access more devices than
would normally be accessible through WebUSB. This could include input devices
like keyboards and second factor authentication keys, recording devices like
cameras and microphones, mass storage devices containing the user's private
data, and other vulnerable devices.

The authors of the WebUSB specification believe it is not possible to expose
unrestricted USB access to arbitrary web pages in a way that protects the user
and also enables USB redirection. The Isolated Web Apps proposal provides an
alternate path for applications that require powerful capabilities considered
too dangerous for the web.

Isolated Web Apps include the [maximal set of permissions they could request](https://github.com/WICG/isolated-web-apps/blob/main/Permissions.md)
as a field in the application manifest. This enables administrators to audit
an application's permission usage and verify that it is not abusing powerful
capabilities.

## Privacy considerations
Unrestricted USB access can be used to access private information that is
normally managed by the operating system or browser. For example, the operating
system uses file permissions to ensure that sensitive data cannot be accessed by
unprivileged applications. Unrestricted low-level access to USB mass storage
devices would enable script to bypass any permissions model and access private
data.

Device identifiers like product names and serial numbers are permanent
identifiers that can be used to fingerprint the user. To mitigate fingerprinting
risk, WebUSB by default exposes no information about devices. A site can read
the identifiers for a device only after the user has granted permission for the
site to access the device.

Exposing additional interface classes is unlikely to significantly increase the
fingerprinting surface since the identifiers exposed by these classes are not
any more useful for fingerprinting than identifiers from devices with
unprotected USB interface classes. Additionally, these identifiers will only be
accessible from inside trusted applications and cannot be used to track the user
on untrusted sites.
