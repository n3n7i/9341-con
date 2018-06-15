


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

void n3n_ILI9341::spiWriteN(uint8_t b[], uint8_t n) {
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);

  for(uint8_t x=0; x<n; x++)  
    SPI.transfer(b[x]);
  //delay(2);
  digitalWrite(_pin_cs, HIGH);
  //Serial.print(b);
  //Serial.println(" :Writedata");
}

void n3n_ILI9341::spiBufferN(uint8_t b[], uint8_t n) {
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);

  //for(uint8_t x=0; x<n; x++)  
    SPI.transfer(b, n);
  //delay(2);
  digitalWrite(_pin_cs, HIGH);
  //Serial.print(b);
  //Serial.println(" :Writedata");
  //return b;
}

void n3n_ILI9341::spiBufferN2(int n) {
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);

  for(int x=0; x<n; x++){
    //while(!(SPIF) || (WCOL));	  
	while(!(SPSR & _BV(SPIF)));
    _buffN[x] = SPDR; //SPI.transfer(0x00);
	digitalWrite(_pin_cs, HIGH);
	digitalWrite(_pin_cs, LOW);
    }
  //delay(2);
  digitalWrite(_pin_cs, HIGH);
  //Serial.print(b);
  //Serial.println(" :Writedata");
  //return b;
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



// 16-bit character lib
// 3x3 active pixels in 4x4 pixel space
// one int per char

//0x757 = char 0
//0000 
//0XXX -7
//0X0X -5
//0XXX -7

int n3n_ILI9341::tinyfont(uint8_t ind){
	
  //int tinyfont_numerals[38] = {0x757, 0x222, 0x326, 0x343, 0x574, 0x623, 0x133, 0x742, 0x673, 0x664, 0x002, 0x255, 0x133, 0x717, 0x353, 0x616, 0x731, 0x357, 0x575, 0x727, 0x723, 0x535, 0x117,
   //                       0x631, 0x471, 0x757, 0x331, 0x332, 0x335, 0x623, 0x722, 0x557, 0x552, 0x463, 0x525, 0x522, 0x326 };
						  
  int tinyfont_numerals[38] = {0x757, 0x222, 0x623, 0x616, 0x571, 0x326, 0x466, 0x712, 0x376, 0x331, 0x002, 0x255, 0x466, 0x747, 0x656, 0x343, 0x764, 0x657, 0x575, 0x727, 0x726, 0x565, 0x447,
                          0x364, 0x174, 0x757, 0x664, 0x662, 0x665, 0x326, 0x722, 0x557, 0x552, 0x136, 0x525, 0x522, 0x623 };						  
  int k = 0;
  if(ind<38) k=tinyfont_numerals[ind]; 
  //) || tinyfont_numerals[0];
  return k;
  }

uint8_t n3n_ILI9341::tinyfont_getind(char c){
  uint8_t r1 = c - '0'; //((int) c) - ((int) '0'); 
  if(r1>9){ // || (r1>9)){
    r1 = c - 'A'+12; //10 + ((int) c) - ((int) 'A'); 
    if((r1<12) || (r1>37))
      r1 = c-'a'+12; 
	if(r1<10) r1+37;
  }
  return r1;
}


uint8_t n3n_ILI9341::tinyfont_Scalc(uint8_t fsize, uint16_t wid){
  return wid % (fsize*4);
  }
  
  //int zlen = floor((float) wid / (float) xchar);
  

uint8_t n3n_ILI9341::tinyfont_Sbuff(char buff[], uint8_t start, uint8_t fsize, uint8_t len, uint16_t wid, uint8_t row){

  int xmod = tinyfont_Scalc(fsize, wid);
  int xchar = ((int) fsize*4);
  int zlen = (wid-xmod) / xchar;
  //Serial.println(zlen);
  uint8_t clen = zlen;
  uint8_t xrow = (3-row) *4;
  if((len-start)<zlen) clen = (len-start); 
  if(clen > 64) clen = 64;
  clen -= clen%2;
  //Serial.println(clen);
  int j=0;
  for(int i=0; i<clen; i++){
	if((i%2)==0) _buffS[j] = 0;
//	uint16_t ttemp = tinyfont(tinyfont_getind(buff[start + i]));
	uint16_t temp = ((tinyfont(tinyfont_getind(buff[start + i]))>>xrow) & 0x0F);
	//Serial.println(ttemp, HEX);
	//Serial.print(temp, HEX);
    _buffS[j] = (_buffS[j]<<4) | temp; //((uint8_t)(tinyfont(tinyfont_getind(buff[start + i]))>>xrow) & 0x0F);
	//Serial.print(" : ");
	//Serial.println(_buffS[j], HEX);
	j += i%2;
    }
  if(clen%2) _buffS[j]<<4;
  return clen/2;
  }

  
void n3n_ILI9341::tinyfont_Sbuff_GFX(uint8_t clen, uint8_t fsize, uint8_t fsize_h, uint16_t fcol, uint16_t bcol, uint16_t wid){

  uint8_t h = 0;
  uint16_t i = 0;
  uint8_t j = 0;
  uint8_t k = 0;
  

  uint16_t col = 0;
  
  uint8_t rem = wid - clen*(fsize*8);
  
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);

  for(uint8_t n = 0; n<fsize_h; n++){ 
  h=0;
  i=0;
  while(i<clen){
	j=0;
	while(j<8){
	  col = bcol;
	  if(_buffS[i] & _BV(7-j)) col = fcol;
	  k=0;
	  while(k<fsize){
		//Serial.print(col);  
		SPI.transfer16(col);
		//delay(2);
		//spiWrite16(col);
		k++;
		//h++;
	    }
	  j++;
	  }
    i++;
  }
  //i = i*8*fsize;
  while(h<rem){
	//spiWrite16(0xFFFF);
    SPI.transfer16(0xFFFF);
	//Serial.print('X');
	h++;
  }}
  digitalWrite(_pin_cs, HIGH);	
  }

  
  
void n3n_ILI9341::setAddrWin(int16_t x0, int16_t y0, int16_t x1, int16_t y1){

  writeCommand(ILI9341_CASET);
  spiWrite16(x0);
  spiWrite16(x1);
  
  writeCommand(ILI9341_PASET);
  spiWrite16(y0);
  spiWrite16(y1);
  
  writeCommand(ILI9341_RAMWR);
  }

void n3n_ILI9341::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){

  setAddrWin(x, y, x + w - 1, y + h - 1);
  uint16_t wb = w;
  uint16_t hb = h;
  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);	
  while (hb>0){ hb--; wb = w;
    while(wb>0){ wb--; //spiWrite16(color); }}
      SPI.transfer16(color);
      }}
  //TFT_CS_H;
  digitalWrite(_pin_cs, HIGH);	
  //spi_end();
  }

 
void n3n_ILI9341::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color){
  bool xrev = x1<x0;
//  bool yrev; //= y1<y0;
  bool xdir = true;
  uint16_t ix0 = x0;
  uint16_t iy0 = y0;
  uint16_t ix1 = x1;
  uint16_t iy1 = y1;
  uint16_t ix2;
  int16_t iy2;

  if(xrev){ 
    ix0 = x1;
	ix1 = x0;
    iy0 = y1;
	iy1 = y0;	
    }

  bool yrev = iy1<iy0;	
  ix2 = ix1 - ix0;
  iy2 = iy1 - iy0;
  ix2 += ix2==0;
  iy2 += iy2==0;
  /*if(yrev){ 
    iy0 = y1;
	iy1 = y0;
    }
  //ix2 = ix1 - ix0;
  iy2 = iy1 - iy0;
  */
 
  float xdy = (float) (ix2) / ((float) abs(iy2));
  float ydx =  ((float) iy2 / ((float) ix2));
  //if(xdy < 1.0) { xdir = false; xdy = (float) iy1 / (ix1+1);}
  if(abs(ydx)>xdy){ xdy *= abs(ydx);} else { ydx *= xdy;}
  
  iy2 = abs(iy2);
  
  //if(abs(ydx)>xdy) xdy*=ydx;
  
  float xd = 0;
  float yd = 0;
  float yz = 2;
  //if((xrev & !yrev) || (!xrev & yrev)) yz = -1;
  uint16_t i=1;
  while(i>0){
  //  if(xdy>ix2) xdy = ix2;
//	if(abs(ydx)>iy2) ydx = iy2;
//	ix2 -= xdy;
//	iy2 -= abs(ydx);
	
	xd = xd + xdy;
	yd = yd + ydx; //(1/xdy)*yz;
	//if(xd > ix2) xd -= ceil(xd)
	if(!yrev)
	  fillRect(ix0, iy0, (uint16_t) ceil(xd), (uint16_t) ceil(yd), color);
    if(yrev)
	  fillRect(ix0, iy0 + yd, (uint16_t) ceil(xd), ceil(abs(yd)), color);
    ix0 = ix0 + floor(xd);
	xd -= floor(xd);
	if(!yrev){
      iy0 = iy0 + floor(yd);
	  yd -= floor(yd);
	  }
	if(yrev){
	  iy0 = iy0 - floor(abs(yd));
	  yd += floor(abs(yd));
	  }
	i++;
	if(i>320) i=0;
	if((ix0 > ix1) || (iy0>iy1 && !yrev) || (iy0<iy1 && yrev) ) i = 0;
	//delay(20);
  }
}

uint16_t n3n_ILI9341::s1(uint16_t xA, int16_t xB, uint8_t xN){
  return xA + floor((xB - (int16_t) xA) * ((float)xN / 255.0));
  }
  
uint16_t n3n_ILI9341::s2(uint16_t xA, uint16_t xB, uint16_t xC, uint8_t xN){
  return s1( s1(xA, xB, xN), s1(xB, xC, xN), xN);
  }

void n3n_ILI9341::drawCurve(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
  uint8_t xN = 0;
  uint16_t xCA = x0, yCA = y0;
  uint16_t xCB, yCB;
  	//Serial.print(x0);
	//Serial.print(" ");
	//Serial.println(y0);
	//Serial.print(x1);
	//Serial.print(" ");
	//Serial.println(y1);
	//Serial.print(x2);
	//Serial.print(" ");
	//Serial.println(y2);

  while(xN < 245){
    xN+=10;
	xCB = s2(x0, x1, x2, xN);
	yCB = s2(y0, y1, y2, xN);
	drawLine(xCA, yCA, xCB, yCB, color);
	xCA = xCB;
	yCA = yCB;
	//Serial.print(xCB);
	//Serial.print(" ");
	//Serial.println(yCB);
	//delay(25);
    }
  }
    
  



  
  
//void n3n_ILI9341::drawFastHVLine(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
	
	//fillRect(x,y, w, h, color);
//#ifdef CLIP_CHECK
  // Rudimentary clipping
  //if ((x >= _width) || (y >= _height)) return;
  //if ((x + w - 1) >= _width)  w = _width - x;
//#endif

  //spi_begin();
  //setAddrWindow(x, y, x + w - 1, y);

  //spiWrite16(color, w);
  //TFT_CS_H;

  //spi_end();
//}
  


/*

void TFT_ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
#ifdef CLIP_CHECK
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((x + w - 1) >= _width)  w = _width - x;
#endif

  spi_begin();
  setAddrWindow(x, y, x + w - 1, y);

  spiWrite16(color, w);
  TFT_CS_H;

  spi_end();
}
  
  
  
  
  //TFT_DC_D;
  //SPDR = x0 >> 8;; spiWait12();
  //addr_col = 0xFFFF;
  //SPDR = x0; spiWait12();
  //if(x1!=win_xe) {
    //SPDR = x1 >> 8; spiWait12();
    //asm volatile( "nop\n\t" ::);
    //win_xe=x1;
    //SPDR = x1; spiWait14();
  //}

//}  

/*

void TFT_ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
#ifdef CLIP_CHECK
  // rudimentary clipping (drawChar w/big text requires this)
  if ((x > _width) || (y > _height) || (w==0) || (h==0)) return;
  if ((x + w - 1) > _width)  w = _width  - x;
  if ((y + h - 1) > _height) h = _height - y;
#endif

  spi_begin();
  setAddrWindow(x, y, x + w - 1, y + h - 1);

  while (h--) spiWrite16(color, w);
  TFT_CS_H;

  spi_end();
}


void TFT_ILI9341::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{

  // Column addr set
  TFT_DC_C;
  TFT_CS_L;
  SPDR = ILI9341_CASET;
  spiWait15();

  TFT_DC_D;
  SPDR = x0 >> 8;; spiWait12();
  addr_col = 0xFFFF;
  SPDR = x0; spiWait12();
  if(x1!=win_xe) {
    SPDR = x1 >> 8; spiWait12();
    asm volatile( "nop\n\t" ::);
    win_xe=x1;
    SPDR = x1; spiWait14();
  }

  // Row addr set
  TFT_DC_C;
  SPDR = ILI9341_PASET; spiWait15();

  TFT_DC_D;
  SPDR = y0 >> 8; spiWait12();
  addr_row = 0xFFFF;
  SPDR = y0; spiWait12();
  if(y1!=win_ye) {
    SPDR = y1 >> 8; spiWait12();
    asm volatile( "nop\n\t" ::);
    win_ye=y1;
    SPDR = y1; spiWait14();
  }

  // write to RAM
  TFT_DC_C;
  SPDR = ILI9341_RAMWR; spiWait14();

  //CS, HIGH;
  //TFT_CS_H;
  TFT_DC_D;

}


*/
	
	

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
