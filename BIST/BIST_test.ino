#include  <EEPROM.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h> 

#define LED1 7
#define LED2 6
#define LED3 5
#define BUTTON_PIN 3
#define LED4 13
int val;
int count = 0;  
int EEsize = EEPROM.length(); // size in bytes of your board's EEPROM
int buttonState = 0;
int beforeState = 0;   //假設之前的狀態
int presstime = 0;  //假設按下的次數
int count1 = 0;

int bounceDelay = 5; // 設定連續 5 毫秒狀態相同時，判定為穩定



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
int cpu_check(){
  int sum = 0;
  int a = 1 ,b = 1;
  sum=a+b;
  if(sum!=a+b){
    return 0;
  }
  return 1;

}



/////FLASH CHECK
int Flag = 0;
int count2 = 0;
const PROGMEM byte pack[5] = {0x00, 0x10, 0x00, 0x86, 0x6A};
int pack_size = 5;

void printhexbyte(byte x)
{  
   count2+=1;
   Serial.print("0x");
   if (x < 16) {
       Serial.print('0');
   }
   Serial.print(x, HEX);
  
   if(count2 == 4){
     if(x == 0){
       Flag=1;
     }
   }
   Serial.println();
}
int crc_check(){
  byte sum = 0;
   for (int i = 0; i < pack_size-1; i++) {
       sum += pack[i];
   }
   Serial.print("Sum of received data bytes                       = ");
   printhexbyte(sum);

   byte calculated_cksum = -sum;
   Serial.print("Calculated checksum                              = ");
   printhexbyte(calculated_cksum);
   
   byte crcbyte = pack[pack_size-1];
   Serial.print("Received checksum byte                           = ");
   printhexbyte(crcbyte);
   
   byte overall = sum+crcbyte;
   Serial.print("Sum of received data bytes and received checksum = ");
   printhexbyte(overall);
   Serial.println();
   
   delay(1000);
}

void setup()
{
  
  Serial.begin(115200);
  
  Serial.println("Start");
  Serial.println(EEPROM.read(0));
  
  if(EEPROM.read(0)==0){
    Serial.println("TEST");
    crc_check();
    if(Flag==0){
      Serial.println("Flash is broken");
    while(1){}
  }
    if(cpu_check()==0){
      Serial.println("CPU is broken.");
    while(1){}
    }
  }
  
  Serial.println("Finish BIST");
  Serial.println("Start App");
  
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);  
  pinMode(LED3, OUTPUT); 
  pinMode(BUTTON_PIN,INPUT);

  wdt_enable(WDTO_2S);  


 
  
  
  
 
}



void loop()
{     
    
    if(EEPROM.read(0)==0){
        //APP 
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      delay(500);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      delay(500);
      //解決button彈跳問題!
      if(debounce(BUTTON_PIN) ==true){
        
        EEPROM.write(0,1);
        while(1){
        
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
        }
     
      }  
    }  
    else
    {
     digitalWrite(LED3, HIGH);
      delay(500);
      digitalWrite(LED3, LOW);
       delay(500);
    }
    
      wdt_reset();
    
    
}
