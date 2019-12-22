#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define STASSID "your-ssid"
#define STAPSK  "your-password"

// Put your public key here
#define OTA_PUBKEY "-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtBnvzfNmn0zw93VbyPQU\no3Ow6RkTrZapFsN/PQF5cpubugj95IGEBqX7nN4BhkvsYrQX43djP92YQpDdA+TC\nMNtCq53YHMXNfUcTlN1sg3spqVnaTaml/7ZUA5txGV94sr9eV91jc5iQyeMgzK48\nLLupgV2KWkiD4Z6r0V1wLhonqK5xJm5JQ/umZM32yCaKYZM2u6mjFQTa0MoIaCzu\nq2xfHMV8QA53vIqesZhbjAvkiFhQkO0rfknqiHBzUwk+pVraGIv41bvJYYRoTJNE\na1Ay1EAY9XavjmfhkiHqg8K5Wh0L2bkaRPOD7TRkjdO/suM9+CpS6cDa3oJWCPR4\nZQIDAQAB\n-----END PUBLIC KEY-----"
BearSSL::PublicKey signPubKey(OTA_PUBKEY);
BearSSL::HashSHA256 hash;
BearSSL::SigningVerifier sign(&signPubKey);

const char* ssid = STASSID;
const char* password = STAPSK;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  Update.installSignature( &hash, &sign );
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
}
