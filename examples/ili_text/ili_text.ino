//#include <n3n-9341.h>
//#include <TFT_FastPin.h>
//#include <User_Setup.h>

#include <9341-com.h>

//#include "9341-com.h"
#include <SPI.h>

//uint8_t xarr[16]={0x01,0x11, 0x02,0x21, 0x03,0x31, 0x05,0x51, 0x07,0x71, 0x0a,0xa1, 0x0b,0xb1, 0x0c,0xc1};

//uint8_t yarr[16];

char strx[90] = "0123456789abcdefghijklmnopqrstuvwxyz. ?!*+-=0123456789abcdefghijklmnopqrstuvwxyz. ?!*+-=";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.println(ILI9341_RAMRD_C);
  //delay(500);
  
  //n3n_ILI9341 
  
  //ntft = n3n_ILI9341(8, 10, 9);

  n3n_ILI9341 ntft = n3n_ILI9341(8, 10, 9);

  //char strx[90] = "0123456789abcdefghijklmnopqrstuvwxyz. ?!*+-=0123456789abcdefghijklmnopqrstuvwxyz. ?!*+-=";

  /*
  for(int i=0; i<44;i++){
    uint8_t test = ntft.tinyfont_getind(strx[i]);
    int t2 = ntft.tinyfont(test);
    Serial.print(strx[i]);
    Serial.print(" ");
    Serial.print(test);
    Serial.print(" ");
    Serial.println(t2, HEX);

    }

    */

  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  ntft.TFTInit();

  //delay(2500);

  ntft.writeCommand(ILI9341_RAMWR);

  //ntft.writeCommand(ILI9341_RAMRD);

  //ntft.writeCommand(ILI9341_RAMWR_C);

  ntft.fillRect(10,10,220,300, TFT_BLACK);

  uint16_t xwid = 200;

  ntft.setAddrWin(20,20,20+xwid-1,100);

//  ntft.writeCommand(ILI9341_RAMRD_C);


/*
  uint16_t x=0;
  uint16_t y=0;
  uint8_t x8=0;
  boolean x2=false;
  for(uint16_t z=0; z<65537; z++){
    x+=15;
    if(!x2) for(x8=0; x8<16; x8++){ xarr[x8] = ((xarr[x8]*3 + y) % 256) * ((y%1)==0);}
    //x8 += 1;
    //xarr[x8%8] = y%256;
    //y++;
    if(x>=480){ x-=480; y+=1;}
    if(y==320){ y=0; x2=!x2;}//x+=1;}
    //ntft.spiWrite( (uint8_t) (z>>8));
    //ntft.spiWrite16((x<<4)+(y>>4) + (z>>8));
    if(!x2 || !(y<120)) ntft.spiWriteN(xarr, 16);

    if(x2 & (y<120)){
    ntft.writeCommand(ILI9341_RAMRD_C);
    delay(1);
    ntft.spiBufferN2(16);
    
    ntft.writeCommand(ILI9341_RAMWR_C);

    Serial.print(256*xarr[0] + xarr[1]); //, xarr[1]);
    Serial.print(" ");
    Serial.print(ntft._buffN[4]); //, yarr[1]);
    Serial.print(" ");
    Serial.print(ntft._buffN[5]); //, yarr[1]);
    Serial.print(" ");
    Serial.print(ntft._buffN[6]); //, yarr[1]);
    Serial.println();
    }
    delay(1);
    }

*/


  uint8_t fs = 0;
  uint8_t fs2 = 0;
  uint8_t sbtest;

  uint8_t xfin = 0;



  while(1){ delay(100);
  fs=0;  
  fs2 = fs2 % 10 +1;
  for(uint8_t jj=0; jj<10; jj++){
    xfin =0; //(uint8_t) jj;
    fs++;
    //fs2 = fs+3;
  
  while(xfin<80){
  
  sbtest = ntft.tinyfont_Sbuff(strx, xfin, fs, 80, xwid, 0);

  //Serial.print("Size: ");
  //Serial.println(sbtest);
  
//    for(int i=0; i<sbtest/2;i+=1){
//    Serial.print(" ");
//    Serial.print(ntft._buffS[i], HEX);      
//    }

ntft.tinyfont_Sbuff_GFX(sbtest, fs, fs2, 0xF00F, 0x0FF0, xwid);
//Serial.println();

sbtest = ntft.tinyfont_Sbuff(strx, xfin, fs, 80, xwid, 1);
ntft.tinyfont_Sbuff_GFX(sbtest, fs, fs2, 0xF00F, 0x0FF0, xwid);
//Serial.println();

sbtest = ntft.tinyfont_Sbuff(strx, xfin, fs, 80, xwid, 2);
ntft.tinyfont_Sbuff_GFX(sbtest, fs, fs2, 0xF00F, 0x0FF0, xwid);

sbtest = ntft.tinyfont_Sbuff(strx, xfin, fs, 80, xwid, 3);
ntft.tinyfont_Sbuff_GFX(sbtest, fs, fs2, 0xF00F, 0x0FF0, xwid);

xfin += sbtest*2;
  } delay(15); }}

}

void loop() {
  // put your main code here, to run repeatedly:

}
