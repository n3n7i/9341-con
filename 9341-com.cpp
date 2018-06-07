


#include <9341-com.h>

#include "pins_arduino.h"

#include "Arduino.h"
//#include "9341-com.h"
#include <SPI.h>


/**************************************************************************/
/*!
    @brief   Initialize ILI9341 chip
    Connects to the ILI9341 over SPI and sends initialization procedure commands
    @param    freq  Desired SPI clock frequency
*/
/**************************************************************************/


n3n_ILI9341::n3n_ILI9341(int rst, int cs, int cmd){
  pinMode(rst, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(cmd, OUTPUT);
  _pin_rst = rst;
  _pin_cs = cs;  
  _pin_cmd = cmd;  
}

void n3n_ILI9341::writeCommand(uint8_t com){
	digitalWrite(_pin_cmd, LOW);
	digitalWrite(_pin_cs, LOW);
	SPI.transfer(com);
//	Serial.println(SPDR);
//	while (!(SPSR));
	digitalWrite(_pin_cs, HIGH);
//	Serial.print(SPSR);
//	Serial.println(" :Writecom");
}

void n3n_ILI9341::spiWrite(uint8_t b) {
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);
	
  SPI.transfer(b);
    digitalWrite(_pin_cs, HIGH);
  //Serial.print(b);
  //Serial.println(" :Writedata");

}

void n3n_ILI9341::spiWrite16(uint16_t b) {
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);
	
  SPI.transfer16(b);
  digitalWrite(_pin_cs, HIGH);
  //Serial.print(b);
  //Serial.println(" :Writedata");

}


uint8_t n3n_ILI9341::spiRead(uint8_t b) {
  uint8_t k = 0;
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);

  k = SPI.transfer(b);
    digitalWrite(_pin_cs, HIGH);
  //Serial.print(b);
  //Serial.println(" :Sendread");
  
  return k;
}



void n3n_ILI9341::TFTInit(void){

    // toggle RST low to reset
    if (_pin_rst >= 0) {
//        pinMode(_rst, OUTPUT);
        digitalWrite(_pin_rst, HIGH);
        delay(100);
        digitalWrite(_pin_rst, LOW);
        delay(100);
        digitalWrite(_pin_rst, HIGH);
        delay(200);
    }

    //startWrite();

    writeCommand(0xEF);
    spiWrite(0x03);
    spiWrite(0x80);
    spiWrite(0x02);

    writeCommand(0xCF);
    spiWrite(0x00);
    spiWrite(0XC1);
    spiWrite(0X30);

    writeCommand(0xED);
    spiWrite(0x64);
    spiWrite(0x03);
    spiWrite(0X12);
    spiWrite(0X81);

    writeCommand(0xE8);
    spiWrite(0x85);
    spiWrite(0x00);
    spiWrite(0x78);

    writeCommand(0xCB);
    spiWrite(0x39);
    spiWrite(0x2C);
    spiWrite(0x00);
    spiWrite(0x34);
    spiWrite(0x02);

    writeCommand(0xF7);
    spiWrite(0x20);

    writeCommand(0xEA);
    spiWrite(0x00);
    spiWrite(0x00);

    writeCommand(ILI9341_PWCTR1);    //Power control
    spiWrite(0x23);   //VRH[5:0]

    writeCommand(ILI9341_PWCTR2);    //Power control
    spiWrite(0x10);   //SAP[2:0];BT[3:0]

    writeCommand(ILI9341_VMCTR1);    //VCM control
    spiWrite(0x3e);
    spiWrite(0x28);

    writeCommand(ILI9341_VMCTR2);    //VCM control2
    spiWrite(0x86);  //--

    writeCommand(ILI9341_MADCTL);    // Memory Access Control
    spiWrite(0x48);

    /*writeCommand(ILI9341_VSCRSADD); // Vertical scroll
    //SPI_WRITE16(0);                 // Zero
	spiWrite16(0x0000);
	//spiWrite(0x00);
*/
    writeCommand(ILI9341_PIXFMT);
    spiWrite(0x55);

    writeCommand(ILI9341_FRMCTR1);
    spiWrite(0x00);
    spiWrite(0x18);

    writeCommand(ILI9341_DFUNCTR);    // Display Function Control
    spiWrite(0x08);
    spiWrite(0x82);
    spiWrite(0x27);

    writeCommand(0xF2);    // 3Gamma Function Disable
    spiWrite(0x00);

    writeCommand(ILI9341_GAMMASET);    //Gamma curve selected
    spiWrite(0x01);

    writeCommand(ILI9341_GMCTRP1);    //Set Gamma
    spiWrite(0x0F);
    spiWrite(0x31);
    spiWrite(0x2B);
    spiWrite(0x0C);
    spiWrite(0x0E);
    spiWrite(0x08);
    spiWrite(0x4E);
    spiWrite(0xF1);
    spiWrite(0x37);
    spiWrite(0x07);
    spiWrite(0x10);
    spiWrite(0x03);
    spiWrite(0x0E);
    spiWrite(0x09);
    spiWrite(0x00);

    writeCommand(ILI9341_GMCTRN1);    //Set Gamma
    spiWrite(0x00);
    spiWrite(0x0E);
    spiWrite(0x14);
    spiWrite(0x03);
    spiWrite(0x11);
    spiWrite(0x07);
    spiWrite(0x31);
    spiWrite(0xC1);
    spiWrite(0x48);
    spiWrite(0x08);
    spiWrite(0x0F);
    spiWrite(0x0C);
    spiWrite(0x31);
    spiWrite(0x36);
    spiWrite(0x0F);

    writeCommand(ILI9341_SLPOUT);    //Exit Sleep
    delay(120);
    writeCommand(ILI9341_DISPON);    //Display on
    delay(120);
    //endWrite();

    //_width  = ILI9341_TFTWIDTH;
    //_height = ILI9341_TFTHEIGHT;
}
