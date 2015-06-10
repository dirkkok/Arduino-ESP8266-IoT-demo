char serialbuffer[1000];//serial buffer for request url

void setup() {
  Serial.begin(9600);//connection to ESP8266 
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.println("AT+RST");
  WaitForReady(2000);
  Serial.println("AT+CWMODE=1");
  WaitForOK(2000);
  Serial.println("AT+RST");
  WaitForReady(2000);
  
  Serial.println("AT+CWJAP=\"<ssid>\",\"<password>\"");
  if (WaitForOK(5000)) {
    digitalWrite(13, HIGH); // Connection succesful
  }
}

void loop() {
//  Data url: https://data.sparkfun.com/isen_arduino_esp8266_demo

  Serial.println("AT+CIPSTART=\"TCP\",\"data.sparkfun.com\",80");
  WaitForOK(5000);
  Serial.println("AT+CIPSEND=123");
  WaitForOK(5000);
  Serial.print("GET /input/<public_key>?private_key=<private_key>&temp=22.6 HTTP/1.0\r\n");
  Serial.print("Hostname: data.sparkfun.com\r\n\r\n");
  WaitForOK(5000);
  Serial.println("AT+CIPCLOSE");
  WaitForOK(5000);
  
  delay(10000);
}

boolean WaitForOK(long timeoutamount) {
  return WaitForResponse("OK", timeoutamount);
}

boolean WaitForReady(long timeoutamount) {
  return WaitForResponse("ready", timeoutamount);
}

// Parts used from https://github.com/contractorwolf/ESP8266
boolean WaitForResponse(String response, long timeoutamount) {
  unsigned long timeout = millis() + timeoutamount;
  
  while (millis() <= timeout) {
    while (Serial.available() > 0) {
      int len = Serial.readBytesUntil('\n', serialbuffer, sizeof(serialbuffer));
    
      String message = String(serialbuffer).substring(0,len-1);
       
      if (message == response) {
        return true;
      }
    }
  }
  
  return false;
}
