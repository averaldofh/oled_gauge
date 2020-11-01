#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
#define gas_width 20
#define gas_height 15
static const unsigned char gas_bits[] PROGMEM = { //bitmap da bomba de combustível
   0xc0, 0x7f, 0x00, 0x40, 0xc0, 0x01, 0x40, 0x40, 0x03, 0x40, 0x40, 0x06,
   0x40, 0x40, 0x06, 0x50, 0x40, 0x0c, 0xdc, 0x7f, 0x0c, 0xde, 0x7f, 0x08,
   0xde, 0xff, 0x09, 0xdc, 0x7f, 0x09, 0xd0, 0x7f, 0x09, 0xc0, 0x7f, 0x09,
   0xc0, 0x7f, 0x0f, 0xc0, 0x7f, 0x06, 0xe0, 0xff, 0x00 };
float gs_rad;
float ge_rad; 
int cx=64; //x centro
int cy=62; //y centro
int radius=60; //raio do mostrador
int percent=85; //porcentagem do tamanho da agulha em relação ao tamanho total do mostrador
char buf[64];

int rotX (int cx1, int r, float degree){
  return int(cx1 + (r - 2) * cos(2*3.14 * (degree / 360) - 3.14));
}
int rotY (int cy1, int r, float degree){
  return int(cy1 + (r - 2) * sin(2*3.14 * (degree / 360) - 3.14));
}

void Drawgauge(int x, byte y, byte r, byte p, int v, int minVal, int maxVal) {
 int n=(r/100.00)*p; // calcula tamanho da agulha
 int thick = r/12;
 int a=0;
u8g2.clearBuffer();
      u8g2.drawXBMP(28,45,gas_width,gas_height,gas_bits); //desenha a bomba de combustível
      int per = map(v,minVal,maxVal,0,100);     //mapeia valor de entrada pra 0-100
      sprintf(buf,"%d%%",per);                  //stringa valor com simbolo de %
      if(per<15)u8g2.drawStr(40,39,"FUEL LOW"); //aviso de nivel baixo <15%
      //u8g2.drawStr(53,50,buf);                  //escreve porcentagem no display
      u8g2.drawStr(15,60,"E");                  //escreve letra E no minimo
      u8g2.drawStr(110,60,"F");                 //escreve letra F no maximo
       for (a=0; a<=180 ; a+=15){                   //laço for de 15 em 15 graus para as referencias
        int x1 = rotX(cx,radius,a);                 //calcula pixel inicio da linha em x
        int y1 = rotY(cy,radius,a);                 //calcula pixel inicio da linha em y 
        int x2 = rotX(cx,(radius - radius/16),a);   //calcula pixel fim da linha em x
        int y2 = rotY(cy,(radius - radius/16),a);   //calcula pixel fim da linha em y
        u8g2.drawLine(x1,y1,x2,y2);                 //desenha a linha de referencia
       }
       for (a=0; a<=180 ; a+=45){                   //laço for de 15 em 15 graus para as referencias
        int x1 = rotX(cx,radius,a);                 //calcula pixel inicio da linha em x
        int y1 = rotY(cy,radius,a);                 //calcula pixel inicio da linha em y 
        int x2 = rotX(cx,(radius - radius/5),a);   //calcula pixel fim da linha em x
        int y2 = rotY(cy,(radius - radius/5),a);   //calcula pixel fim da linha em y
        u8g2.drawLine(x1,y1,x2,y2);                 //desenha a linha de referencia
       }
      
       //calculo dos pontos da agulha
       float gs_rad=-1.572;
       float ge_rad=1.572;
       float i=((v-minVal)*(ge_rad-gs_rad)/(maxVal-minVal)+gs_rad);
       int xp = x+(sin(i) * n);
       int yp = y-(cos(i) * n);
       u8g2.drawCircle(x,y,r, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT );       //desenha semicirculo maior
       u8g2.drawCircle(x,y,r-thick, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT ); //desenha semicirculo menor
       u8g2.drawDisc(x,y,r/8, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT);        //desenha disco central
       u8g2.drawLine(x,y,xp,yp);                                                  //desenha agulha
     u8g2.sendBuffer();
}

void setup() {
 u8g2.begin();
 u8g2.setFont(u8g2_font_6x10_tf);
}

void loop() { 
 int c = analogRead(A0);
 Drawgauge(cx,cy,radius,percent,c,0,1024); //Drawgauge(centro x, centro y, raio, porcentagem do tamanho da agulha, valor, minimo, maximo)
}
