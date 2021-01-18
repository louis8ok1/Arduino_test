#include <avr/wdt.h>
#include  <EEPROM.h>


#define LED1 7
#define LED2 6
#define LED3 5
#define BUTTON_PIN 3

int bounceDelay = 5; // 設定連續 5 毫秒狀態相同時，判定為穩定

int flag = 0;

int ending(){
    Serial.println("Ending work Start");
    flag = 0;
    EEPROM.write(1,flag);
}

boolean debounce(int PIN)
{
  boolean state;
  boolean previousState;
  previousState = digitalRead(PIN);  // 儲存按鈕狀態

  for(int counter=0;counter<bounceDelay;counter++)
  {
    //delay(1);
    state = digitalRead(PIN);// 再讀取一次按鈕狀態      // 如果兩次狀態不同，把 counter 設為零，重新確認狀態
    if(state != previousState) 
    {
      counter=0;
      previousState = state;
    }
  }
  return state;
}
void setup(){
    Serial.begin(115200);
    if(EEPROM.read(1)==1){
        ending();
    }
    Serial.println("Start");
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);  
    pinMode(LED3, OUTPUT); 
    pinMode(BUTTON_PIN,INPUT);
    wdt_enable(WDTO_2S); 
}

void loop(){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(500);    
    if(debounce(BUTTON_PIN) ==true){
        while(1){
            flag = 1;
            EEPROM.write(1,flag);
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
        }
        
    }
wdt_reset();
    
}