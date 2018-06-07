
#include <9341-com.h>

#include <SPI.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(ILI9341_RAMRD_C);
  delay(500);
  
  n3n_ILI9341 ntft = n3n_ILI9341(8, 10, 9);
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));

  ntft.TFTInit();

  ntft.writeCommand(ILI9341_RAMWR);

  uint16_t x=0;
  uint16_t y=0;
  for(uint16_t z=0; z<65537; z++){
    x++;
    //y++;
    if(x==240){ x=0; y++;}
    if(y==320){ y=0; x+=50;}
    ntft.spiWrite16((x<<4)+(y>>4) + (z>>8));}

}

void loop() {
  // put your main code here, to run repeatedly:

}
