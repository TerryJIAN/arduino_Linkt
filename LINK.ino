#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

// 定義步進馬達轉一圈所需的步數及輸出的腳位
#define stepPin A3 //CLK+
#define dirPin A2 //CW+
#define enPin A4 //EN+
/******************************************************/
#define        COV_RATIO                       0.2            //ug/mmm / mv
#define        NO_DUST_VOLTAGE                 400            //mv
#define        SYS_VOLTAGE                     5000           

const int iled = 7;      //D7 drive the led of sensor 
const int vout = 0;      //A0 analog input 

float density, voltage;
int   adcvalue;
/******************************************************/


/******************************************************/
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 4  //D4
/*******************************************************/

#define LED 8

#define Fan3 3
#define Fan5 5
#define Fan6 6
#define Fan9 9
#define Fan10 10
#define Fan11 11
#define Fan12 12
#define Fan13 13

int fan_high = 255;
int fan_in = 150;
int fan_low  = 50;
int fan_off  =  0;
int cc = 0;  //10秒 計數器
int ccc = 0; //1秒  計數器

int cccc = 0; //馬達

#define time_out  15 //每15分鐘
int Sec =0;
int Min =0;

float dataA; //PM
float dataB; //溫度
float dataC; //溼度

boolean flagA=false;
boolean flagB=false;
boolean Motor=false;
int Bdata[5];
int Bdatanum= 0;
void pm(){
   digitalWrite(iled, HIGH);
  delayMicroseconds(280);
  adcvalue = analogRead(vout);
  digitalWrite(iled, LOW);
  
  adcvalue = Filter(adcvalue);
  
  /*
  covert voltage (mv)
  */
  voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;
  
  /*
  voltage to density
  */
  if(voltage >= NO_DUST_VOLTAGE)
  {
    voltage -= NO_DUST_VOLTAGE;
    
    density = voltage * COV_RATIO;
  }
  else
    density = 0;
    
  /*
  display the result
  */
//  Serial.print("P: "+ String(density));
//  Serial.print(density);
//  Serial.print(" ug/m3\n");  
  }
void temp(){
  DHT11.read(DHT11PIN);
  
//  Serial.print("T: "+String(((float)DHT11.temperature)-3, 2)+" H: "+ String((float)DHT11.humidity, 2));
//  Serial.print(((float)DHT11.temperature)-3, 2);
//  Serial.print("  ");
//  Serial.print("Humidity (%): ");
//  Serial.println((float)DHT11.humidity, 2);

  }
int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0)
  {
    flag_first = 1;

    for(i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}


void Fan_in()
{
        analogWrite(Fan3,fan_in);
        analogWrite(Fan5,fan_in);
        analogWrite(Fan6,fan_in);
        analogWrite(Fan9,fan_in);
        analogWrite(Fan10,fan_in);
        analogWrite(Fan11,fan_in);
        analogWrite(Fan12,fan_in);
        analogWrite(Fan13,fan_in);
}
void Fan_high()
{
        analogWrite(Fan3,fan_high);
        analogWrite(Fan5,fan_high);
        analogWrite(Fan6,fan_high);
        analogWrite(Fan9,fan_high);
        analogWrite(Fan10,fan_high);
        analogWrite(Fan11,fan_high);
        analogWrite(Fan12,fan_high);
        analogWrite(Fan13,fan_high);
}
void Fan_low()
{
        analogWrite(Fan3,fan_low);
        analogWrite(Fan5,fan_low);
        analogWrite(Fan6,fan_low);
        analogWrite(Fan9,fan_low);
        analogWrite(Fan10,fan_low);
        analogWrite(Fan11,fan_low);
        analogWrite(Fan12,fan_low);
        analogWrite(Fan13,fan_low);
}
void Fan_off()
{
        analogWrite(Fan3,fan_off);
        analogWrite(Fan5,fan_off);
        analogWrite(Fan6,fan_off);
        analogWrite(Fan9,fan_off);
        analogWrite(Fan10,fan_off);
        analogWrite(Fan11,fan_off);
        analogWrite(Fan12,fan_off);
        analogWrite(Fan13,fan_off);
}



void setup()
{
  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW); //iled default closed
  
  pinMode(LED, OUTPUT);
  pinMode(Fan9, OUTPUT);
  
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  
  Serial.begin(9600);
  Serial1.begin(57600);



}


void loop()
{
   
   if (Serial1.available()) {
        char str[32]=""; //字串上限 
        int strnum=0;
        String data[8];  // 幾組資料
        int datanum = 0;
        
        memset( str, 0, strlen(str) );
        
        while (Serial1.available() > 0){
            str[strnum++] = Serial1.read();
            delay(3);
        }
        
        char* test1 = str;
        char* test2;
        
         test2 = strtok(test1,",");
         while(test2 != NULL){
            data[datanum++] = (String)test2;   
            test2 = strtok(NULL,",");
          }
          Serial.println(data[0]);
          Serial.println(data[1]);
          Serial.println(data[2]);
          Serial.println(data[3]);
          Serial.println(data[4]);
          Serial.println(data[5]);
      
//        Serial.print(str);
//        s1 = Serial1.readString();       
//        Serial.print(s1);
        
        if (data[0]=="on"){
          digitalWrite(LED, HIGH);}
          
        else if (data[0]=="off"){
          digitalWrite(LED, LOW);}

        else if (data[0] == "A model")
        {
          flagA=true;flagB=false;
          Serial.println ("set model A");
        }
        else if (data[0] == "B model")
        {
          Serial.println ("set model B");
          flagA=false;flagB=true;
          Bdata[0] = data[1].toInt();
          Bdata[1] = data[2].toInt();
          Bdata[2] = data[3].toInt();
          Bdata[3] = data[4].toInt();
          Bdata[4] = data[5].toInt();
          
        }
        else if (data[0] == "Fan off")
        {
           Serial.println ("set model off");
          flagA=false;flagB=false;
        }
    }

   if (ccc == 100){     //每1秒
      pm();
      temp();
      
      if (density-20 >= 151 and Motor == false) {Motor = true; cccc = 1;}
      
      dataA =  dataA + density-20;
      dataB =  dataB + ((float)DHT11.temperature)-3;
      dataC =  dataC + (float)DHT11.humidity;
      ccc=0;


        if (flagB)
        {          
            Sec ++;
            if(Sec == 60){
              Min ++;
              Sec =0;     
            }
       
            if (Min == time_out ){ //15分
              Min =0;
                if (Bdatanum < 4)
                {Bdatanum++;} 
                else
                {Bdatanum = 0;}
            }
          ///////////////////////////////////////////////

          if (Bdata[Bdatanum]>70 and Bdata[Bdatanum]<90){
                Fan_in();
                Serial.println("Fan_in");
            }
           else if (Bdata[Bdatanum] >= 90 ){    
                Fan_high();
                Serial.println("Fan_high");
              }
           else if (Bdata[Bdatanum] <= 70 ) {      
                Fan_low();
                Serial.println("Fan_low");
              }
          
        }



        if (flagA==false and flagB==false )
        {
           Fan_off();
           Serial.println("Fan_off");
          }
       
    }
     
  if (cc == 1000){    //每10秒
//      pm();
//      temp();
//      Serial1.print(String(density-20)+","+String(((float)DHT11.temperature)-3, 2)+","+ String((float)DHT11.humidity, 2));
    
      Serial1.print(String(dataA/10)+","+String(dataB/10, 2)+","+ String(dataC/10, 2));
      
      if (dataC/10>70 and dataC/10<90 and flagA){     
          Fan_in();
          Serial.println("Fan_in");
        }
      else if (dataC/10 >= 90 and flagA){    
          Fan_high();
          Serial.println("Fan_high");
        }
      else if (dataC/10 <= 70 and flagA) {      
          Fan_low();
          Serial.println("Fan_low");
        }

      
      dataA=0;
      dataB=0;
      dataC=0;
      cc = 0;
    }
  if (Motor and cccc <= 5){
    digitalWrite(dirPin,HIGH); 
        Serial.print("+++:");
        Serial.println(cccc);
           
        for(int x = 0; x < 10; x++) {
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500); 
        } 
        cccc++;
      }
   else if (Motor and cccc > 5 and cccc <= 15){       
        cccc++;
      }   
  else if (Motor and cccc > 15 and cccc <= 20){
    digitalWrite(dirPin,LOW);
        Serial.print("---:");
        Serial.println(cccc);
        
        for(int x = 0; x < 10; x++) {
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500); 
        } 
        cccc++;

      }

  delay(10);
  ccc++;    
  cc++;
  if (cccc>20) {Motor = false;}
}
