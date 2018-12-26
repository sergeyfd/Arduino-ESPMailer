# ESPMailer
A very simple SMTP Client with SSL support for ESP8266 Platform.


##Usage

`ESPMailer mail`: Create a new mail client.

`ESPMailerSecure mail`: Create a new secure mail client.

`mail.Host`: Address of your server.

`mail.Port`: Defaults to 25 for standard maile and to 465 for a secure mailer, change if you need.

`mail.setFingerprint(char *)`: Set SSL Fingerprint for a Secure client. You can use provided GMAIL_FINGERPRINT, YAHOO_FINGERPRINT or your own.

`mail.setFrom(address, [name])`: Set sender, optional with name.

`mail.addToAddress(address, [name])`: Add a new recipient. 

`mail.addCCAddress(address, [name])`: Add a new CC address.

`mail.addBCCAddress(address, [name])` Add a new BCC address.

`mail.Subject`: Subject of the message.

`mail.Body`: The main message.

`mail.setDebugLevel(level)`: MUTED, ERRORS, CLIENT, SERVER, CLIENT_AND_SERVER

`mail.send()` Returns true if sent successfully.
