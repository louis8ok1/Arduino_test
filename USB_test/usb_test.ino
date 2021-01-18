#define LED 7 
void setup(){
    Serial.begin(115200);
    pinMode(LED,OUTPUT);
}
int i;
void loop(){
  Serial.flush();
  while(Serial.available()==0){}
  while(Serial.available()>0)
  {
    int data=Serial.read()-'0';//將ASCII轉換成整數
    Serial.print("input number is :");

    Serial.println(data);

    

  }
    
}
