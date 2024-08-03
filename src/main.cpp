#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128                //解像度 128 x 64 で使用します。
#define SCREEN_HEIGHT 64                //SCREEN_HEIGHTは 32 に設定することができます。

#define OLED_RESET     -1               //使用しないので　-1を設定する。
#define SCREEN_ADDRESS 0x3C             //I2Cアドレスは 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




void setup()
{
  

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;);
  }

  display.clearDisplay();               //何か表示されている場合に備えて表示クリア

  display.setTextSize(2);               //フォントサイズは2(番目に小さい)
  display.setTextColor(SSD1306_WHITE);  //色指定はできないが必要
  display.setCursor(20, 5);            //テキストの表示開始位置
  display.print(F("TAMANEGI"));         //表示文字列
  display.setCursor(45, 25);
  display.print(F("XIAO"));
  display.setCursor(20, 45);
  display.print(F("nRF25840"));

  display.display();                    //バッファ転送（表示)
}


void loop()
{
}