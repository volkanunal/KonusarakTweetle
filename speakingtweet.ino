//VOLKAN UNAL
//https://engineeringvolkan.wordpress.com/
//https://www.youtube.com/watch?v=R6CS2p5V_H4 uygulama videosu
#include <SoftwareSerial.h>
SoftwareSerial dbg(7,8);  
SoftwareSerial BT(11, 10);
String voice;
#define esp Serial
#define butonPin 12
#define SSID "agadiniz"
#define PASS "agşifreniz"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /apps/thingtweet/1/statuses/update?key=XFJ2NHRC0E6ATD33&status=";
String Sent = "";
int butondurum=0;
int sondurum=0;
long debounce=1000;
int time=0;
int tweetdurum=0;
void setup(){
  pinMode(butonPin, INPUT);
 
 BT.begin(9600);
  esp.begin(9600);
  dbg.begin(9600);
  dbg.println("begin.");
  esp.println("AT+RST");
  delay(5000);
  esp.println("AT");
  delay(2000);
  if(esp.find("OK")){
    connectWiFi();}}
void loop(){
  butondurum=digitalRead(butonPin);
   // Serial.println(butondurum);
 if(butondurum==1 && sondurum==0 && millis()-time>debounce)
 {
   if(tweetdurum==1)
   { butondurum=0;}
   else
   {tweetdurum=1; }
   time=millis();}
 sondurum=butondurum;
  if(tweetdurum==0 ){ 
  BT.listen();
  while(BT.available()){ 
  delay(10);  
  char c = BT.read(); 
  if(c=='#'){break;}
   voice += c; }
if(voice.length()>0){
    Serial.println(voice.length());
    delay(100);}}
 else if (tweetdurum==1)
{
  sendtweet(Sent);
  esp.println(Sent);
  delay(30000);
  tweetdurum=0;}}
void sendtweet(String Sent){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  esp.println(cmd); 
  delay(2000);
  if(esp.find("Error")){dbg.println("error");
   return;}
  cmd = GET;
  cmd += voice;
  cmd += "\r\n\r\n";
  esp.print("AT+CIPSEND=");
  esp.println(cmd.length());
  dbg.print("AT+CIPSEND=");
  dbg.println(cmd.length());
  delay(2000);
  if(esp.find("> ")){
    esp.print(cmd);
    dbg.println(cmd);
  }else{
    esp.println("AT+CIPCLOSE");
    dbg.println("AT+CIPCLOSE");
  }
    voice="";
}
boolean connectWiFi(){
  esp.println("AT+CWMODE=3");
  delay(2000);
    String cmd =String("AT+CWJAP=\"")+SSID+"\",\""+PASS+"\"";
  esp.println(cmd);
  delay(5000);
  if(esp.find("OK")){
    return true;
  }else{
    return false; }}
