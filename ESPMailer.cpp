/*
 * ESPMailer.cpp - a very simple SMTP Client with SSL support for ESP8266 Platform.
 * Created by Serge Dubrouski <sertgeyfd@gmail.com>. December, 2018.
 */

#include "ESPMailer.h"

MailAddress::MailAddress(String address) {
	Address = address;
	Name = "";
}

MailAddress::MailAddress(String address, String name) {
	Address = address;
	Name = name;
}

String MailAddress::str() {
	if (Name == "")
		return String(Address);

	return String("\"" + Name + "\" <" + Address +">");
}

AddressBook::AddressBook() {
}

void AddressBook::add(MailAddress* addr){
	if (_size == MAX_ADDRESS_COUNT)	return;
	_book[_size++] = addr;
}

MailAddress* AddressBook::get(int index) {
	if (_size == 0 || index < 0 || index > _size) return NULL;
	return _book[index];
}

ESPMailer::ESPMailer() {
	_from = new MailAddress("");
	_to = new AddressBook();
	_cc = new AddressBook();
	_bcc = new AddressBook();
}

ESPMailerSecure::ESPMailerSecure() {
	Port = 465;
}

void ESPMailer::addAddress(AddressBook* book, MailAddress* addr){
	if ( book->size() == MAX_ADDRESS_COUNT) return;
	book->add(addr);
}

void ESPMailer::setFrom(String address, String name) {
	_from->Address = address;
	_from->Name = name;
}


void ESPMailer::addToAddress(String address, String name) {
	addAddress(_to, new MailAddress(address, name));
}

void ESPMailer::addCCAddress(String address, String name) {
	addAddress(_cc, new MailAddress(address, name));
}

void ESPMailer::addBCCAddress(String address, String name) {
	addAddress(_bcc, new MailAddress(address, name));
}

bool ESPMailer::sendCmd(String cmd, int code) {
	String reply;
	if (!connected()) return false;

	if (cmd != "") {
		getClient()->println(cmd);
		if (_debug == CLIENT || _debug == CLIENT_AND_SERVER) {
			Serial.write('>');
			Serial.println(cmd);
		}
	}

	int timeout = millis() + Timeout;
	while (!available() && millis() < timeout)
		delay(1);

	reply = getClient()->readStringUntil('\n');
	if (_debug == SERVER || _debug == CLIENT_AND_SERVER) {
		Serial.write('<');
		Serial.println(reply);
	}

	if (reply.indexOf(String(code)) == 0) return true;

	if (_debug != MUTED) return false;

	Serial.println("Error while processing following command:");
	Serial.write('>');
	Serial.println(cmd);
	Serial.println(reply);
	return false;
}

bool ESPMailer::authenticate() {
	// Only LOGIN Authentication supported
	if (!sendCmd("AUTH LOGIN", 334)) return false;
	if (!sendCmd(base64::encode(Username), 334)) return false;
	if (!sendCmd(base64::encode(Password), 235)) return false;
	return true;
}

bool ESPMailer::sendTo(AddressBook* book) {
	for (int i=0; i < book->size(); i++) {
		if (!sendCmd(String("RCPT TO: <" + book->get(i)->Address + ">"), 250)) return false;
	}
	return true;
}

void ESPMailer::sendToHeaders(char* type, AddressBook* book){
	for (int i=0; i < book->size(); i++) {
		getClient()->printf("%s: %s\r\n", type, book->get(i)->str().c_str());
	}
}

void ESPMailer::sendHeaders() {
	getClient()->printf("Message-Id: <%ld%ld@%s>\r\n", ESP.getChipId(),ESP.getCycleCount(), Host.c_str());
	getClient()->printf("X-Mailer: %s\r\n", X_MAILER);
	getClient()->printf("Subject: %s\r\n", Subject.c_str());

	getClient()->printf("Content-Type: %s\r\n", "text/plain");
	getClient()->printf("Content-Transfer-Encoding: 8bit\r\n");
	getClient()->printf("From: %s\r\n", _from->str().c_str());
	sendToHeaders("To", _to); sendToHeaders("CC", _cc); sendToHeaders("BCC", _bcc);
	getClient()->println();
}

bool ESPMailer::send() {
	if (!connect()) {
		if (_debug > MUTED) {
			Serial.println("No connection to server!");
			Serial.println(Host);
			Serial.println(Port);
		}
		return false;
	}
	Serial.println("Connected!");

	if (!sendCmd("", 220)) return false;
	if (!sendCmd(String("HELO " + Host), 250)) return false;
	if (Username != "" && !authenticate()) return false;

	if (!sendCmd(String("MAIL FROM: <" + _from->Address + ">"), 250)) return false;
	sendTo(_to); sendTo(_cc); sendTo(_bcc);

	if (!sendCmd("DATA", 354)) return false;
	sendHeaders();

	getClient()->println(Body);
	getClient()->println();
	if (!sendCmd(".", 250)) return false;
	if (!sendCmd("QUIT", 221)) return false;

	return true;
}

