// ###################################################################################################################################################################################
// In this following code we, the student of BANDIPUR IDEAL ACADEMY FOR BOY, build the code.
// Special THANKS TO TANMOY SAMANTA, class 9th student, for contributer your most valuable time from the past two year.
//And special Thanks all the teachers for the trust on us , specificali on TANMOY SAMANTA.
//CREATED ON: 5th,December,2022. COPYRIGHT : UNDER MIT PUBLIC LICENCE.
//***************************************THIS DEMONSTRATE HOW A WEATHER ACTUALLY WORKS!****************************************************
/* The data from our mini weather station will be uploded on a cloud storage. Bascilly the daata will be uploded to The ThinkSpeak's data senter. And our job is to mode it avalable 
on the Internet so everyone who has Internet connection can access the data. And this is where Twitter comes. The data will be avalable for user. Particularly @lwkfeed on Twitter
 -Local Weather Khardha Feed.

                                                   DONOT FORGET TO FOLLOW US ON TWITTER @lwkfeed    




############################################### GET UPDATE  @lwkfeed -Local Weather Khardha Feed  on Twitter################################################################




 Copyright (C) <2022>  <Tanmoy samanta>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


MAIL ME AT tanmoysamanta99925@gmail.com for further information, if required..




 
*/

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <ESP8266HTTPClient.h>
#define DHTPIN D2                    // Digitl pin 2 is connected to the Nodemcu
#define DHTTYPE DHT11                // Sensor type: DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
HTTPClient http;
// PUTE YOU WIFI INFORMATION HERE
const char WiFiName = 'Jionet';
const char WiFiPasswd = 'youfuck1';



// PUTE YOUR ThingSpeak infornation here
long myChannelNumber = 1128174;
const char myWriteAPIKey[] = "3MW0L5TM6U96NTAA";
String myStatus = "";


// put your ThingTweet information here 
String host = " api.thingspeak.com";
int httpPort = 80;
String url = "/apps/thingtweet/1/statuses/update";
const String ThingTweetApi = "FPBLYWABSMSWEZES";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin("Jionet", "youfuck1");  // wifi name and passdwd here
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.println("Attempthing to connect to the SSID : ");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  // starting the libary 
  dht.begin();
  ThingSpeak.begin(client);
  WiFi.mode(WIFI_STA);
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BIILTIN as output. 
}

void loop() {
  // put your main code here, to run repeatedly:



  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to the  SSID: ");

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin("Jionet", "youfuck1");  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }



  float t = dht.readTemperature();
  delay(300);
  float h = dht.readHumidity();
  delay(300);
  String Temprature = "Temprature:" + String(t) + " oC";
  String Humidity = "Humidity: " + String(h) + " %";

  // ThingSpeak Status for temprature

  if (t<48 & t > 38 ){
    myStatus = String("It is extremely hottest day of the year.Leave a absent from your boss! And don't forget to TURN ON the AC." + String(t) + " oC");
  }
  else if  (t > 30 & t < 38) {

    myStatus = String("That is too hot out side, drink sufficent water! Take care of your health."+ String(t) +" oC");
  } else if (t<30 & t> 25) {
    myStatus = String(" It is the perfect weather for you.Some time you may feel cool! The cold wind may caused of your Fever! "+ String(t) +" oC");

  } else if (t < 25 & t>20 ) {
    myStatus = String("It is cold out side as usual.You should weare Sweater or Jacket, Otherwise you may suffer from Fever! " + String(t) +" oC");
  }
  else if (t<20 & t>10){
    myStatus = String("It is too cold outside, Without Jacket or Sweater, you will be ill."+ String(t) + " oC");
  }
  else if ( t < 10 & t>0 ){
    myStatus = String(" It is extremely coldest day of the year.Leave a absent from your boss! And don't forget to N'Joy!!! "+ String(t) + " oC");
  
  }



  Serial.println(Temprature);
  // it will show tempature in celcious..

  Serial.println(Humidity);
  //it will show Hmidity in persentage..




 int y = ThingSpeak.setStatus(myStatus);
if (y == 200 ){
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW );
  delay(1000);
  Serial.println("[OK] Status update successfully!");

}else{
  Serial.printf("[FAIL] Could not update status!");
}

  //Here we are sending data to ThingSpeak cloud!

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW );
  delay(1000);
  
  Serial.println("Channel update successful.");



  } else  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
 // Data is sucessfully send data to the ThingSpeak cloud!






    http.begin(client, host, httpPort, url);
  
  
  const String RequestBody = "api_key=" + String(ThingTweetApi)+ "&status=" + String(Temprature) + String(Humidity);
  delay(300);

  int StatusCode = http.POST(RequestBody);
  if (StatusCode  == 200 ) {
    Serial.println("[SUCCEED] Data uploded on Twitter!");
  }
  else{
    Serial.println("[FAIL] Couldnot uplode data on Twitter!" );
    Serial.println(StatusCode);
  }
  
  http.end();
  Serial.println("------------------------------------");


  delay(450000);  ///* After posting all of the senced data to ThingSpeak and to the mention Twitter Account it will wait for FORTY-FIVE second(minimuhole system hom). This
                 //is how the whole system will run forever untill we disconnect WiFi connection!*
}




