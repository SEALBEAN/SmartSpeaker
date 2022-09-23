/*
 * H: điện áp out sẽ giư nguyên cho đến khi không còn chuyển động
 * L: Out tự động chuyển về 0 khi hết thời gian Delay dù còn chuyển động 
 * 
 * 
 * VCC - 3.3v
 * GND - GND
 * CSN - 8
 * CE - 7
 * SCK - 13
 * MOSI - 11
 * MISO - 12
 */


 
int initPower = 5; // Nguon cho Sensor
int cambien = 4; //Chân cảm biến nối chân số 4 Arduino
int giatri;
int relay = 9; //Chan relay so 9 

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(2,3); //RX, TX
DFRobotDFPlayerMini myDFPlayer;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
RF24 radio(7, 8); // CE, CSN
const byte diachi[6] = "12345";


void setup() 
{
  Serial.begin(9600);
  pinMode(relay, OUTPUT);

  
  pinMode(initPower, OUTPUT);
  pinMode(cambien, INPUT);

  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  } else {
    Serial.println("Module setup thanh cong!");
  }

  radio.openReadingPipe(1, diachi);
  //Lệnh openReadingPipe có số đường truyền từ 0-5
  //Nhưng đường 0 đã được dùng cho ghi (mặc định)
  //Vì vậy chỉ dùng 1-5, nếu dùng không sẽ bị chồng lấn
  //Mở 1 kênh có địa chỉ 12345 trên đường truyền 1
  //kênh này chỉ đọc data trên địa chỉ 12345
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với TX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
    mySoftwareSerial.begin(9600);
    if(!myDFPlayer.begin(mySoftwareSerial, true, false)){ while(true){delay(0);}}
    myDFPlayer.volume(90);
  }
}

void nhac_mo_dau()
{
  myDFPlayer.play(1);
}

void loop() 
{
  digitalWrite(initPower, HIGH); // cap dien cho sensor
  giatri = digitalRead(cambien); //Đọc giá trị digital từ cảm biến và gán vào biến giatri
  if (giatri == 1){
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }
  if (radio.available()) 
  {
    char nhan[30] = ""; //30 là số kí tự trong mảng
    radio.read(&nhan, sizeof(nhan));
    Serial.println(nhan);
    int num = 0;
    if(nhan[0] == '1'){
    num = 1;
    }
    else if(nhan[0] =='2'){
    num = 2;
    }else{
    num=3;
    }
  switch (num){
        case 1:
        myDFPlayer.play(1);
        break;

        case 2:
        myDFPlayer.play(2);
        break;

        case 3:
        myDFPlayer.play(3);
        break;
    }
  }

}
