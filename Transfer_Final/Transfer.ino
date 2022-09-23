
/*
 * https://maniacbug.github.io/RF24/classRF24.html
 * 
 * VCC - 3.3v
 * GND - GND
 * CSN - 8
 * CE - 7
 * SCK - 13 mặc định SPI
 * MOSI - 11 SPI
 * MISO - 12 SPI
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <IRremote.h> // thư viện hỗ trợ IR remote

RF24 radio(7, 8); // CE, CSN
const byte diachi[6] = "12345"; //Mảng kí tự dạng chuỗi có 6 kí tự

const int receiverPin = 6; // chân digital 6 dùng để đọc tín hiệu
IRrecv irrecv(receiverPin); // tạo đối tượng IRrecv mới
decode_results results;// lưu giữ kết quả giải mã tín hiệu

void setup() 
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // start the IR receiver
  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  } else {
    Serial.println("Module setup thành công!");
  }
  
  radio.openWritingPipe(diachi);
  //Lệnh openWritingPipe mặc định là đường truyền 0
  //mở 1 kênh có địa chỉ 12345 trên đường truyền 0
  // kênh này ghi data lên địa chỉ 12345  
  radio.setPALevel(RF24_PA_MIN); 
  //Cài bộ khuyết địa công suất ở mức MIN, MAX, HIGH, LOW
  radio.setChannel(80); // 125 kênh từ 0-124; TX và RX phải cùng kênh
                        // 2.4GHz ~ 2400Mhz, bước kênh là 1MHz
                        // setchannel(1) => 2401Mhz
                        // Cao nhất là 2525MHz, Tức là 2.525GHz
                        
  radio.setDataRate(RF24_250KBPS); //Tốc độ truyền dữ liệu trong không khí
                                   //250kbps, 1Mbps hoặc 2Mbps
                                   //250 thấp nhất nhưng truyền xa, 1Mb và 2Mb mạnh nhưng truyền không xa                                   
  /*                                   
   * Tốc độ truyền dữ liệu không khí 2Mbps, băng thông 2MHz bị chiếm dụng nhiều tần số kênh 
   * rộng hơn độ phân giải của cài đặt tần số kênh RF
   * Vì vậy, để đảm bảo các kênh không chồng chéo và giảm kết nối chéo ở chế độ 2Mbps
   * bạn cần giữ khoảng cách 2MHz giữa hai kênh.                                    
   * 
   * A: Xe TX-RX kênh 80, tốc độ truyền là 2Mb 80, 81, 82
   * B: Máy bay TX-RX kênh 83, tốc độ truyền là 250Kb                                    
   */
  radio.stopListening(); //Cài đặt module là TX
  
  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }  
}

void loop() 
{
    if (irrecv.decode(&results)){
        switch(results.value){
          case 2534850111: //Keypad button "1"
          {
            char text = '1';
            radio.write(&text, sizeof(text));
            Serial.print("đã gửi: ");
            Serial.println(text);
          }
          break;
          case 1033561079: //Keypad button "2"
          {
            char text = '2';
            radio.write(&text, sizeof(text));
            Serial.print("đã gửi: ");
            Serial.println(text);
          }
          break;

          case 1635910171: //Keypad button "3"
          {
            char text = '3';
            radio.write(&text, sizeof(text));
            Serial.print("đã gửi: ");
            Serial.println(text);
          }
          break;
          }
        irrecv.resume(); 
    }
//  if (Serial.available())
//    {
//    String cmd = Serial.readStringUntil("\n");
//    int n = cmd.length();
// 
//    // declaring character array
//    char text[n + 1];
// 
//    // copying the contents of the
//    // string to char array
//    strcpy(text, cmd.c_str());
    
//    radio.write(&text, sizeof(text));
//    // &: Trả lại địa chỉ của một biến.
//    // sizeof: trả về số byte bộ nhớ của một biến 
//    //hoặc là trả về tổng số byte bộ nhớ của một mảng
//    Serial.print("đã gửi: ");
//    Serial.println(cmd);
//    Serial.println(cmd.length());
//    delay(1000);
//    }
}
