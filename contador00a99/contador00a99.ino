//Este programa maneja un display de 7 segmentos por medio del shift register TPIC6B595N
//para este programa incorporamos un displays más en cascada con otro SR
//entonces contaremos de 00 a 99


#define CLK   2                                   // Pin CLK / SRCK
#define DATA  3                                   // Pin DATA / SER IN
#define LATCH 4                                   // Pin LATCH / RCK

//Aquí el arreglo binario para la activación de los segmentos segun corresponda por medio de 
//una lista de 10 elementos para mostrar los números de 0 al 9. Faltarían los símbolos

byte disp[12] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x63,0x39};      // 0,1,2,3,4,5,6,7,8,9,°,C
//byte disp[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};      // Display de anodo comun

//declaramos las variables tipo byte

byte unidadSegundos;
byte decenaSegundos;


void setup()
{
  pinMode(CLK, OUTPUT);                           // Pin CLK como salida
  pinMode(LATCH, OUTPUT);                         // Pin LATCH como salida
  pinMode(DATA, OUTPUT);                          // Pin DATA como salida

  decenaSegundos = 10; //enviamos °
  unidadSegundos = 11; //enviamos C
  
  
    digitalWrite(LATCH, LOW);                     // Pin LATCH en estado bajo
    shiftOut(DATA, CLK, MSBFIRST, disp[unidadSegundos]);       // Envia el dato
    //digitalWrite(LATCH, HIGH);                    // Carga el dato
    //delay(500);

    //digitalWrite(LATCH, LOW);                     // Pin LATCH en estado bajo
    shiftOut(DATA, CLK, MSBFIRST, disp[decenaSegundos]);       // Envia el dato
    digitalWrite(LATCH, HIGH);                    // Carga el dato
    delay(500);
}

void loop()
{

/*  for(byte i=0; i<10; i++)                        // Ciclo para recorrer los numeros del 0 a 9
  {
    digitalWrite(LATCH, LOW);                     // Pin LATCH en estado bajo
    shiftOut(DATA, CLK, MSBFIRST, disp[i]);       // Envia el dato
    digitalWrite(LATCH, HIGH);                    // Carga el dato
    delay(500);
  }*/
}
