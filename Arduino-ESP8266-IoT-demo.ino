char serialbuffer[1000];//serial buffer for request url

void setup() {
  Serial.begin(115200);//connection to ESP8266 
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
//  AT+CIPSTART="TCP","data.sparkfun.com",80
//  AT+CIPSEND=108
//  GET https://data.sparkfun.com/input/<public_key>?private_key=<private_key>&temp=<test_data> HTTP/1.0
//  AT+CIPCLOSE

  Serial.println("AT+CIPSTART=\"TCP\",\"data.sparkfun.com\",80");
  WaitForOK(5000);
  Serial.println("AT+CIPSEND=108");
  WaitForOK(5000);
  Serial.println("GET https://data.sparkfun.com/input/<public_key>?private_key=<private_key>&temp=<test_data> HTTP/1.0");
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

boolean WaitForResponse(String response, long timeoutamount) {
  unsigned long timeout = millis() + timeoutamount;
  
  while (millis() <= timeout) {
    while (Serial.available() > 0) {
      int len = Serial.readBytesUntil('\n', serialbuffer, sizeof(serialbuffer));
    
       //trim buffer to length of the actual message
       String message = String(serialbuffer).substring(0,len-1);
       
       if (message == response) {
         return true;
       }
    }
  }
  
  return false;
}
