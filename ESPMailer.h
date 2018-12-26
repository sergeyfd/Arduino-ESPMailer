/*
 * ESPMailer.h - a very simple SMTP Client with SSL support for ESP8266 Platform.
 * Created by Serge Dubrouski <sertgeyfd@gmail.com>. December, 2018.
 */
#ifndef ESPMAILER_H_
#define ESPMAILER_H_

#include <ESP8266WiFi.h>
#include <base64.h>

#define MAX_ADDRESS_COUNT 5
#define X_MAILER "ESPMailer v1.0 (http://github.com/sergeyfd/Arduino-ESPMailer)"

const char GMAIL_FINGERPRINT[] PROGMEM = "C2:28:8E:B2:BB:42:4B:27:59:A5:18:E4:ED:A5:D6:60:52:35:FD:0C";
const char YAHOO_FINGERPRINT[] PROGMEM = "31:6A:F6:32:74:B1:94:73:8D:68:16:03:27:83:CD:20:37:4C:1D:13";

enum DEBUG_LEVEL {
	MUTED = -1,
	ERRORS,
	CLIENT,
	SERVER,
	CLIENT_AND_SERVER
};

class MailAddress {
public:
	String Address;
	String Name;

	MailAddress(String);
	MailAddress(String, String);
	String str();
};

class AddressBook {
private:
	MailAddress* _book[MAX_ADDRESS_COUNT];
	int _size = 0;
public:
	AddressBook();
	int size() {return _size;}
	void add(MailAddress*);
	MailAddress* get(int);
};


class ESPMailer {
private:
	WiFiClient _client;
	MailAddress* _from;

	AddressBook* _to;
	AddressBook* _cc;
	AddressBook* _bcc;
	void addAddress(AddressBook*, MailAddress*);
	int   _debug = 0;

	bool sendCmd(String, int);
	bool authenticate();
	bool sendTo(AddressBook*);
	void sendToHeaders(char*, AddressBook*);
	void sendHeaders();
	// These methods depend on WiFiClient type
	virtual WiFiClient* getClient() {return &_client;}
	virtual int connect() {return _client.connect(Host.c_str(), Port);}
	virtual uint8_t connected() {return _client.connected();}
	virtual int available() {return _client.available();}
public:
	ESPMailer();

	String Host;
	int Port = 25;
	String Username;
	String Password;
	String Subject;
	String Body;
	int Timeout = 300;

	void setFrom(String address) {return setFrom(address, "");};
	void setFrom(String, String);
	void addToAddress(String address) {return addToAddress(address, "");}
	void addToAddress(String, String);
	void addCCAddress(String address) {return addCCAddress(address, "");}
	void addCCAddress(String, String);
	void addBCCAddress(String address) {return addBCCAddress(address, "");}
	void addBCCAddress(String, String);
	void setDebugLevel(DEBUG_LEVEL debug) {_debug = debug;}
	bool send();
};


class ESPMailerSecure : public ESPMailer {
private:
	WiFiClientSecure _clientSecure;

	virtual WiFiClient* getClient() {return &_clientSecure;}
	virtual int connect() {return _clientSecure.connect(Host.c_str(), Port);}
	virtual uint8_t connected() {return _clientSecure.connected();}
	virtual int available() {return _clientSecure.available();}
public:
	ESPMailerSecure();
	bool setFingerprint(const char* fingerprint) {return _clientSecure.setFingerprint(fingerprint);}
};

#endif /* ESPMAILER_H_ */
