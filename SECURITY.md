
## Security 

**Project Name:** LPG-Gas-Leakage-Detection-and-Control-System


## Scope

This project involves critical components for detecting and controlling LPG gas leaks. Security risks can include:

* Unauthorized access via Bluetooth (HC-05)
* Wi-Fi-based attacks (ESP8266)
* Firmware modification or tampering
* Unsecured communication between modules
* Code injection or serial data manipulation

## Supported Versions

Only the latest version of this project is actively maintained with security updates.

| Version | Supported |
| ------- | --------- |
| main    | Yes       |
| others  | No        |

## Reporting a Vulnerability

If you identify a potential security issue in the project:

1. Do not open a public issue on GitHub.
2. Report the issue privately by emailing

We aim to respond within 72 hours and address valid reports within 7 days.

## Recommendations for Users

* Change default Bluetooth pairing passwords (avoid 1234 or 0000)
* Regularly update firmware and code
* Use secure communication methods for the ESP8266 (HTTPS where possible)
* Disable debug messages in production code
* Physically protect your hardware from tampering

## Contributing to Security

Contributions that enhance the security of this project are welcome.
To propose improvements:

* Open an issue labeled `security` or `enhancement`
* Or submit a pull request with clear documentation and reasoning

---

