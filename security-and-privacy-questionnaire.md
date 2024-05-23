# [Self-Review Questionnaire: Security and Privacy](https://w3ctag.github.io/security-questionnaire/)

[Markdown template](https://raw.githubusercontent.com/w3ctag/security-questionnaire/main/questionnaire.markdown)

> 01.  What information does this feature expose,
>      and for what purposes?

(For WebUSB in general:) By default, the API exposes no information about a
user's connected devices. Once the user grants permission for a site to access a
device, the API exposes details about the device including device identifiers
like the serial number string, vendor/product IDs, product name string, device
firmware version, USB device class, supported configurations and interfaces.

Applications use the vendor ID, product ID, and firmware version to identify the
connected device model and determine compatibility.

The serial number string is used to identify a specific device and distinguish
the device from other devices with the same vendor and product IDs.

The product name string is used as a human-facing identifier so that the
application can present the device to the user with the same name that is used
in permissions surfaces.

The device's supported configurations and interfaces describe how to communicate
with the device. Applications use this information to deduce the device's
capabilities, compose transfers to be sent to the device, and interpret
transfers received from the device.

By allowing a site to communicate with a peripheral device it is exposed to any
information that the device makes available through that connection. Information
communicated with the device may include device identifiers that can be used for
fingerprinting, private user data, security credentials, and other sensitive
data. WebUSB [restricts the classes of devices](https://wicg.github.io/webusb/#has-a-protected-interface-class)
that can be accessed through the API to limit the potential for abuse.

(For Unrestricted WebUSB:) The Unrestricted WebUSB feature removes the
restrictions on protected device classes so that trusted applications can access
these devices.

> 02.  Do features in your specification expose the minimum amount of information
>      necessary to implement the intended functionality?

(For WebUSB in general:) Yes, a site only receives information about the device
the user has chosen to share with it.

(For Unrestricted WebUSB:) Applications must opt-in to the ability to access
otherwise protected devices by requesting the `"usb-unrestricted"` permission
which may subject them to additional scrutiny through a process external to this
API.

> 03.  Do the features in your specification expose personal information,
>      personally-identifiable information (PII), or information derived from
>      either?

(For WebUSB in general:) Yes, WebUSB exposes the serial number string descriptor
if the device has one. The serial number is assumed to be unique among devices
with the same vendor and product IDs and is therefore useful for fingerprinting.
Specific devices may expose personal information related to the device
capabilities.

(For Unrestricted WebUSB:) Yes, the protected devices exposed by this feature
are very likely to expose personal information including keystroke data,
security credentials, webcam audio and video, and data stored on portable
storage devices.

> 04.  How do the features in your specification deal with sensitive information?

(For WebUSB:) The API is designed to be safe by default, exposing no information
about connected devices until a user has granted permission. Additionally, the
API blocks access to the most sensitive device classes. Typically these devices
are accessible through another higher-level web platform API.

(For Unrestricted WebUSB:) The feature does not attempt to protect sensitive
information. We expect applications that use this feature will be carefully
reviewed before publishing. If a published application is found to be abusing
the permission then the application will be unpublished through a process
external to this API.

> 05.  Do the features in your specification introduce state
>      that persists across browsing sessions?

Yes, permissions may persist across browsing sessions for devices that can be
reliably re-identified by the implementation. In Chromium, this requires that
the device has a non-empty serial number string descriptor. Permissions for
devices without a serial number are revoked when the device is disconnected or
the browser is closed.

Additionally, a device may have its own state that persists independently of the
browser.

> 06.  Do the features in your specification expose information about the
>      underlying platform to origins?

Yes, the API exposes information about connected devices and enables an origin
to communicate with these devices.

> 07.  Does this specification allow an origin to send data to the underlying
>      platform?

Yes.

> 08.  Do features in this specification enable access to device sensors?

Yes, if they are USB devices. Unrestricted USB additionally exposes low-level
access to audio and video sensors that are normally blocked.

> 09.  Do features in this specification enable new script execution/loading
>      mechanisms?

No.

> 10.  Do features in this specification allow an origin to access other devices?

Yes, the API is intended to provide access to connected USB peripherals. This
feature expands the number of devices that a trusted application can access.

> 11.  Do features in this specification allow an origin some measure of control over
>      a user agent's native UI?

No. The `requestDevice()` method, which can cause the user agent to display
native UI, requires a user gesture in order to mitigate abuse.

> 12.  What temporary identifiers do the features in this specification create or
>      expose to the web?

This specification does not create temporary identifiers.

> 13.  How does this specification distinguish between behavior in first-party and
>      third-party contexts?

The specification leverages [Feature Policy](https://w3c.github.io/webappsec-feature-policy/)
to control access to WebUSB API in third-party contexts. If a third-party
context is trusted by the first-party context it can be explicitly granted
access to the `"usb"` feature.

For Unrestricted USB a new `"usb-unrestricted"` feature is added. The feature is
only available in isolated application contexts which makes all third-party
contexts ineligible to access the feature.

> 14.  How do the features in this specification work in the context of a browser’s
>      Private Browsing or Incognito mode?

Implementations are expected to implement separate
storage for device permissions between the "normal" and "incognito" modes. This
mitigates passive leakage of information between sessions. Permissions granted
during a private browsing session are expected to be cleared at the end of that
session.

As discussed above, exposed device identifiers and communication with a device
grants a site the ability to read potentially identifying information from the
device. Implementations should frame a site's permission request in a way that
brings the user's attention to the powerful nature of this request using words
like "access" or "control". In an incognito context this message may be
strengthened to highlight to potential for this action to "unmask" a user in the
same way that entering personal credentials or uploading a file would.

Since the default state before any permissions are granted is that the site has
no access to devices, it is not possible to detect an incognito session using
this API.

> 15.  Does this specification have both "Security Considerations" and "Privacy
>      Considerations" sections?

(For WebUSB in general:) The specification has a combined [Security and Privacy Considerations](https://wicg.github.io/webusb/#security-and-privacy)
section.

(For Unrestricted WebUSB:) The explainer has sections for [Security considerations](https://github.com/WICG/webusb/blob/main/unrestricted-usb-explainer.md#security-considerations)
and [Privacy considerations](https://github.com/WICG/webusb/blob/main/unrestricted-usb-explainer.md#privacy-considerations).

> 16.  Do features in your specification enable origins to downgrade default
>      security protections?

Yes, Unrestricted WebUSB enables trusted applications to opt-out of default
security protections for protected device classes. This is necessary for the
intended use case where a trusted application forwards low-level device access
to a virtualized desktop operating system.

> 17.  What happens when a document that uses your feature is kept alive in BFCache
>      (instead of getting destroyed) after navigation, and potentially gets reused
>      on future navigations back to the document?

The WebUSB specification does not integrate with BFCache.

Claimed USB interfaces are not shareable resources. Implementations should
release any claimed interfaces and close the device when a document enters
BFCache so that the resources can be accessed by other applications.

> 18.  What happens when a document that uses your feature gets disconnected?

When the document is disconnected, if there are open connections to USB devices
then the connections are closed and claimed interfaces are released.

> 19.  What should this questionnaire have asked?

I have nothing more to add.
