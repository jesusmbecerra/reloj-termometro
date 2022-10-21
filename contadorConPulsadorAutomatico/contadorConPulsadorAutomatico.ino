//-------------------------------------------------------------------------------------------
//DESCRIPCION DEL PROGRAMA
//-------------------------------------------------------------------------------------------

//Este programa maneja dos display de 7 segmentos por medio del shift register TPIC6B595N
//Tenemos 3 pulsadores para funciones de modo, subir y bajar
//la idea en este es ejecutar la cuenta de forma automatica, tanto ascendente como descendente
//y que la misma se pueda ajustar el inicio o el final con el boton SET


//-------------------------------------------------------------------------------------------
//DECLARACIÓN DE CONSTANTES
//-------------------------------------------------------------------------------------------

//Estas constantes son para la comunicación del primer SR con el arduino
#define CLK   2                                   // Pin CLK / SRCK
#define DATA  3                                   // Pin DATA / SER IN
#define LATCH 4                                   // Pin LATCH / RCK
//Constantes para los pines de entrada que leerán los pulsadores
#define PUSH_SET 5                                // pulsador de SET
#define PUSH_UP 6                                 // pulsador de UP
#define PUSH_DOWN 7                               // pulsador de DOWN

//-------------------------------------------------------------------------------------------
//DECLRACIÓN DE VARIABLES
//-------------------------------------------------------------------------------------------

//Aquí el arreglo binario para la activación de los segmentos segun corresponda por medio de 
//una lista de 10 elementos para mostrar los números de 0 al 9. Faltarían los símbolos
byte disp[12] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x63,0x39};      // 0,1,2,3,4,5,6,7,8,9,°,C
//byte disp[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};      // Display de anodo comun

//declaramos las variables tipo byte del dígito a mostrar en cada display
byte unidadSegundos;
byte decenaSegundos;

//manejo de los pulsadores
int lecturaAnterior = 1;                         // bandera para soltar el pulsador
int temporizadorPulsador = 150;                   //indica la duración luego del flanco de bajada (pulsado)
int contadorTemporizador = 0;                     //indicar las veces que el temporizador ha llegado a cero para acelerar
int delayPulsoAutomatico = 3;                     //modifica la velocidad con la que simula el pulso automatico (pulsado)

//conteo
int contador = 0;                                //variable que llevara el conteo



//-------------------------------------------------------------------------------------------
//RUTINA DE AJUSTE INCIAL
//-------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(57600);
  
  //configuramos las salidas del arduino para el registro de corrimiento
  pinMode(CLK, OUTPUT);                           // Pin CLK como salida
  pinMode(LATCH, OUTPUT);                         // Pin LATCH como salida
  pinMode(DATA, OUTPUT);                          // Pin DATA como salida

  //configuramos los pines de entrada de los pulsadores
  pinMode(PUSH_SET, INPUT);                         // Pin para el pulsador de entrada SET
  pinMode(PUSH_UP, INPUT);                          // Pin para el pulsador de entrada UP
  pinMode(PUSH_DOWN, INPUT);                        // Pin para el pulsador de entrada DOWN  
  
  //reiniciamos las variables del display
  decenaSegundos = 0; 
  unidadSegundos = 0; 
}

//-------------------------------------------------------------------------------------------
//RUTINA PRINCIPAL DEL PROGRAMA
//-------------------------------------------------------------------------------------------

void loop()
{

    //leemos el pulsador de ajuste
    int lectura = digitalRead(PUSH_SET);

    //detecta el flanco de bajada y revisa si queda presionado para hacer el cambio automatico
    
    if (lectura == 0){    //si se ha detectado un flanco de bajada
        temporizadorPulsador--;                   //decrementamos el temporizador
        if (temporizadorPulsador == 0) {           //si este ha llegado a cero
            temporizadorPulsador = 150;            //reestablecemos el temporizador
            contador++;                           //entonces incrementamos nuestra variable de conteo
            contadorTemporizador++;               //incrementamos el ccontador de desborde del temporizador
           // Serial.println(contador);
        }

        if (contadorTemporizador > 7) delayPulsoAutomatico = 1;           //si se han llegado a esta cantidad de desbordes porque sigue presionado
                                                                      //bajamos el delay de cambio o pulso automatico
        
        delay(delayPulsoAutomatico);              //este es el delay para cambiar la cuenta de forma automatica
    } 
    
    //detecta el flanco de bajada pero en esta ocasión es un pulso corto porque no entró en la validacion del temporizador     
    if (lecturaAnterior == 1 && lectura == 0){
      contador++;
      temporizadorPulsador = 150;               //si el temporizador alcanzó a cambiar pero no a desbordar lo resetea
      //Serial.println(contador);
    }

    //detecta el flanco de subida para reestablecer el delay     
    if (lecturaAnterior == 0 && lectura == 1){
      delayPulsoAutomatico = 3;
      contadorTemporizador = 0;
    }      
    
    lecturaAnterior = lectura;
    
    //escribe en los displays
    if (contador == 10) {
          contador = 0;
          decenaSegundos++;
          if (decenaSegundos == 10) {
              decenaSegundos = 0; 
          }
      }
      unidadSegundos = contador; 
      digitalWrite(LATCH, LOW);                     // Pin LATCH en estado bajo
      shiftOut(DATA, CLK, MSBFIRST, disp[unidadSegundos]);        // Envia el contador.
      shiftOut(DATA, CLK, MSBFIRST, disp[decenaSegundos]);
      //unidadSegundos++;
 
      digitalWrite(LATCH, HIGH);                    // Carga el dato
}
      
 
