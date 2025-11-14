#include <SoftwareSerial.h>   //INCLUCION DE LIBRERIA


// CONEXIONES PARA EL BLUETOOTH.

int bluetoothTx = 2;       // DEFINE LOS PINES 2(T9 Y 3(RX) DEL ARDUINO PARA COMUNICARSE CON EL BLUETOOTH
int bluetoothRx = 3;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);    //SE CREA EL PARAMETRO BLUETOOTH QUE SIRVE PARA ENVIAR Y RECIBIR DATOS POR ESOS PINES

// MOTOR 1. SE USAN DOS PINES PARA PODER CONTROLAR EL SENTIDO DE GIRO

const int Motor1A = 5;
const int Motor1B = 6;

// MOTOR 2.

const int Motor2A = 9;
const int Motor2B = 10;

void setup ()
{
  bluetooth.begin(115200);  // SE ABRE LA COMUNICACION A 115200 BAUDIOS
  bluetooth.print("$$$");   //ENVIA EL COMANDO $$$ PARA QUE EL HC05 ENTRE EN MODO DE CONFIGURACION  
  delay(100);    //TIEMPO DE ESPERA
  bluetooth.println("U,9600,N");  //SE ENVIA U,9600,N PARA CAMBIAR A LA VELOCIDAD ESTANDAR
  bluetooth.begin(9600);   // SE VUELVE A INICIAR PARA COMUNICARSE NORMALMENTE

  pinMode( Motor1A, OUTPUT );   //SE CONFIGURA LOS PINES COMO SALIDA 
  pinMode( Motor1B, OUTPUT ); //Motor 1

  pinMode( Motor2A, OUTPUT ); //Motor 2
  pinMode( Motor2B, OUTPUT );

  digitalWrite( Motor1A, LOW );   //SE INICIA LOW PARA QUE EMPIECE DETENIDO
  digitalWrite( Motor2A, LOW );
  digitalWrite( Motor1B, LOW );
  digitalWrite( Motor2B, LOW );
}

//ESTAS VARIABLES GUARDAN EL ULTIMO ESTADO. SIRVEN PARA EVITAR REPETIR COMANDOS CUANDO EL QUE QUEREMOS FUE EL ULTIMO MOVIMIENTO

int estadoMovimiento = 0;   // CONTROLA EL MOVIMIENTO ADELANTE, ATRAS, STOP 
int estadoGiro = 0;   //CONTROLA EL GIRO IZQUIERDA, DERECHA, NEUTRO 



void loop()
{
  if(bluetooth.available())   // VERIFICA SI EL BLUETOOTH ENVIO ALGO 
  {
    char comandoBT = (char)bluetooth.read();  // SI HAY DATOS, SE LEEN Y SE GUARDAN EN comandoBT
    if(comandoBT == 'S')   // CADA LETRA QUE LLEGA DESDE EL CELULAR INDICA UNA ACCION
    {
      estadoMovimiento = 0;
      estadoGiro = 0;

      digitalWrite( Motor1A, LOW);  // "S" SIGNIFICA STOP, PONE TODOS LOS MOTORES EN LOW
      digitalWrite( Motor1B, LOW);

      digitalWrite( Motor2A, LOW),
      digitalWrite( Motor2B, LOW);
    }
    else if( comandoBT == 'F' )  //"F" ES FORWARD QUE ES AVANZAR MIENTRAS QUE "G" E "I" SON COMANDOS DE GIRO
    {
      if (estadoMovimiento != 1)
      {
        // ESTOS COMANDOS HARAN QUE VAYA PARA ADELANTE.
        estadoMovimiento = 1;
        digitalWrite( Motor1A, HIGH); //Motor 1
        digitalWrite( Motor1B, LOW);

        digitalWrite( Motor2A, HIGH); //Motor 2
        digitalWrite( Motor2B, LOW);
      }
    }
    else if(comandoBT == 'B' )  // "B" ES DE BACKWARD QUE ES REVERSA MIENTRAS QUE "H" Y "J" SON COMANDOS DE GIRO PERO CONTRARIOS QUE EL ANTERIOR
    {
      if(estadoMovimiento != -1)
      {
        // ESTOS HARAN LA REVERSA.
        estadoMovimiento = -1;
        digitalWrite( Motor1A, LOW); //Motor 1
        digitalWrite( Motor1B, HIGH);

        digitalWrite( Motor2A, LOW ); //Motor 2
        digitalWrite( Motor2B, HIGH);
      }
    }
    else if(comandoBT == 'L' )  // "L" DE LEFT QUE ES IZQUIERDA "G" Y "H" SON DE AVANZAR O RETROCEDAR HACIA LA IZQUIERDA
    {
      if(estadoGiro != -1)
      {
        // ESTOS HARAN QUE GIRE HACIA LA IZQUIERDA.
        estadoGiro = -1;
        digitalWrite( Motor1A, LOW); //Motor 1
        digitalWrite( Motor1B, HIGH );

        digitalWrite( Motor2A, HIGH); //Motor 2
        digitalWrite( Motor2B, LOW); 

      }
    }
    else if(comandoBT == 'R' )  //"R" DE RIGHT QUE ES DERECHA "I" "J" SON COMANDOS PARA RETROCEDER O AVANZAR HACIA LA DERECHA
    {
      if(estadoGiro != 1)
      {
        // ESTOS HARAN QUE GIRE HACIA LA DERECHA.
        estadoGiro = 1;
        digitalWrite( Motor1A, HIGH );
        digitalWrite( Motor1B, LOW); //Motor 1

        digitalWrite( Motor2A, LOW); //Motor 2
        digitalWrite( Motor2B, HIGH );
      }
    }
    else
    {
      if(estadoMovimiento != 0 && estadoGiro != 0)    // CUANDO NO LEA NINGUN GIRO, ESTE SERVIRA PARA QUE SOLO VAYA RECTO
      {
        estadoMovimiento = 0;
        estadoGiro = 0;
        digitalWrite ( Motor1A, LOW);
        digitalWrite ( Motor1B, LOW);  
        digitalWrite ( Motor2A, LOW);
        digitalWrite ( Motor2B, LOW);  
      }
    }
  }
}