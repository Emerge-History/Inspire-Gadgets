#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include "FS.h"
#define D0 16
extern "C" {
#include "user_interface.h"
}
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
//
//uint8 buf[48 * 512];
uint8 initCmds[8] = {29, 118, 48, 0, 48, 0, 255, 1};
uint8 printDensity[5] = { 27, 55, 2, 200, 30 };
//uint32 curPos = 0;

void initialize() {
  pinMode(D0, OUTPUT);
  delay(500);
  digitalWrite(D0, HIGH);
  delay(500);
  digitalWrite(D0, LOW);
  delay(500);
  digitalWrite(D0, HIGH);
  delay(500);
  digitalWrite(D0, LOW);

  Serial.begin(9600);

  Serial.write(27);
  Serial.write((char)'@');
  delay(10);

}

String getContentType(String filename) {
  if (webServer.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


File saveTo;

void renderFile(String path) {
  File f = SPIFFS.open(path, "r");
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    webServer.streamFile(f, contentType);
    f.close();
  } else {
    webServer.send(404, "text/plain", "File Not Found");
  }
}



void setup() {
  SPIFFS.begin();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("zhaoliaoliao");
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.on("/print", HTTP_POST, []() {
    webServer.send(200, "text/plain", "Printer Operational");
    Serial.write(27);
    Serial.write((char)'@');
    delay(500);
    Serial.write(printDensity, 5);
    delay(500);
    Serial.write(initCmds, 8);
    delay(500);
    if(saveTo) { saveTo.close(); }
    saveTo = SPIFFS.open("/print", "r");
    while(saveTo.available()) {
      Serial.write(saveTo.read());
//      delayMicroseconds(1000);
    }
    saveTo.close();
  }, []() {
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START) {
      if(SPIFFS.exists("/print")) {
        SPIFFS.remove("/print");  
      }
      saveTo = SPIFFS.open("/print", "w");
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      saveTo.write(upload.buf, upload.currentSize);
    } else if (upload.status == UPLOAD_FILE_END) {
      //do nothing
    }
    yield();
  });
  webServer.on("/", HTTP_GET, []() {
    webServer.sendHeader("Connection", "close");
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    renderFile("/index.html");
  });
  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.sendHeader("Connection", "close");
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    renderFile("/index.html");
  });
  webServer.begin();
  initialize();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}


//
//void setup() {
////
////  delay(10);
////   for(int y = 0; y < (256 + 1 * 256); y++) {
////        for(int x = 0; x < 48 * 8; x+=8) {
////            Serial.write(0xA0);
////        }
////   }
//}
//
//void loop() {
//}


