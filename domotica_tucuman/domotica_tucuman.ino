//------------------------------------------------------------------------------------------------------------------------------------
//PROYECTO DE DOMOTICA PARA EL EDIFICIO
//PLACA DE DESARROLLO ESP-8266
//VERSION: 1.0
//FECHA: 18/9/23
//tombonia@gmail.com
//-------------------------------------------------------------------------------------------------------------------------------------







//------------------------------------------------------------ARCHIVOS CABECERA--------------------------------------------------------------------------------------

//#include <WiFi.h> //Este archivo cabecera lo activo cuando es la placa esp32. Hay que cambiar tambien en "placa" del ide de arbino.(podria hacerlo con hiderwors o algo similar).TODO
#include <ESP8266WiFi.h> //Este archivo cabeccera lo activo cuando es la placa ESP8266.
#include <WiFiClientSecure.h> //Todavia no se para que sirve este archivo cabecera: al parecer hay que crear un objeto cliente de la clase WiFiClientSecure.
#include "credenciales.h" //aqui van a estar las claves de ssid password bottoken y chat_id, para personalizar mejor el codigo segun el usuario.
#include "funciones.h"
//para agregar el archivo cabecera fui al triangulito de arriba a la derecha y puse nueva pestaña y le pongo el nombre.h y adentro los #defines.


//-------------------------------------------------------PROTOTIPADO DE FUNCIONES------------------------------------------------------------------------------------
void handleNewMessages(int numNewMessages); //Maneja el menu de opciones, los gpio y los mensajes a la comunicacion serie y a telegram.
void IRAM_ATTR buttonInput(); //funcion interrupcion para cuando se detecta boton o sensor.


//------------------------------------------------------ALGUNAS DECLARACIONES----------------------------------------------------------------------------------------
const unsigned long tiempo = 1000; //delay entre cada verificacion de mensaje.
unsigned long tiempoAnterior; //última vez que se realizó el análisis de mensajes. Truquito para que funcione con millis()


 //----------------------------------------------------------------VOID SETUP------------------------------------------------------------------------------------------
void setup()
  {
  Serial.begin(115200); //Inicia y configura el puerto serie.
  client.setInsecure(); //No se que hace esto pero setea algo del oblejo client de la clase WiFiClientSecure. dice que lo setea como inseguro

  //---------------------------------------
  //Configura el pinaut GPIOS y los setea.
  pinMode(magnetic_door_ingreso_pin, OUTPUT); //Configura el pin 2 de la placa esp como salida digital. y le mete un high(bloqueada)
  digitalWrite(magnetic_door_ingreso_pin, LOKED); 
  
  pinMode(led_de_prueba, OUTPUT); //configura el pin d4 de la placa (LED) como salida y le mete high para que se apague
  digitalWrite(led_de_prueba, HIGH);//tiene logica invertida con low se prende y con high se apaga. 

  pinMode(boton_de_prueba, INPUT_PULLUP); //configura el pin d3 de la placa (BOTON FLASH) como entrada con pullup
  
  pinMode(chicharra_pin, OUTPUT);  //configura el pin de chicharra como salida y le mete un high habilitada
  digitalWrite(chicharra_pin, ENABLED);

  pinMode(luz_vereda_pin, OUTPUT); // configura el pin de luz vereda como salida y le mete un high luz prendida.
  digitalWrite(luz_vereda_pin, HIGH);

  pinMode(sirena_antipanico_pin, OUTPUT); // configura el pin de sirena antipanico como salida y le mete un LOW pa que no este sonando.
  digitalWrite(sirena_antipanico_pin, LOW);

  pinMode(luces_terraza_pin, OUTPUT); // configura el pin de luz terraza como salida y le mete un LOW apagada.
  digitalWrite(luces_terraza_pin, LOW);

  pinMode(portero_11_pin, OUTPUT); // configura el pin de portero de la terraza como salida y le mete un LOW para disablearlo.
  digitalWrite(portero_11_pin, DISABLED);

  //TODO sensor de puerta abierta de la terraza y liman de la puerta de la terraza para que cierre. dos pines mas.

   // aqui agregar mas pines para configurar mas funciones.



  
  //Conexion a red de WiFi
  WiFi.mode(WIFI_STA); //configura el WIFI de la esp como cliente o STATION creo que lo hace atravez de comandos AT y lo elige en modo 1. el modo 2 es AP y el 3 es mixto.
  WiFi.begin(SSID, PASSWORD); //aqui toma los parametros de credenciales.h.

  int ciclo = 0; //la primera imprime conectando a wifi y despues puntos suspensivos nomas.
  while (WiFi.status() != WL_CONNECTED) //Este while no termina hasta que se conecte a la red.
    {
    delay(300);
    ciclo ? Serial.print(".") : Serial.print("\n --> Conectando a WiFi."); //operador ternario para que ponga puntos suspensivos hasta que se conecte
    ciclo++;
    }//fin del while
  Serial.print("Se ha conectado a la red wifi. Dirección IP: "); //Una vez conectado a la red, manda mensaje de conectado y devuelve el IP.
  Serial.println(WiFi.localIP());
  for(int k = 0 ; k <= 4 ; k++) //parapadea 5 veces para indicar que esta en regimen.
    {
    digitalWrite(led_de_prueba, LOW);
    delay(200);
    digitalWrite(led_de_prueba, HIGH);
    delay(200);
    }

  //interrupcion 

  attachInterrupt(digitalPinToInterrupt(boton_de_prueba), buttonInput, RISING); //aqui instala la señal!!! dice que la interrupcion se va activar con un high de boton_de_prueba
                                                                              //que esta en el gpio 0 pin d3 y que eso va a activar la funcion buttonInput. por flanco de subida o sea cuando pase de low a high.
   }//fin del void setup()


//---------------------------------------------------------------------VOID LOOP----------------------------------------------------------------------------------------------------
void loop()
  {                         //--1--AQUI LA FUNCION QUE ATIVA LAS COSAAS DESDE TELEGRAAM HACIA ESP
  if (millis() - tiempoAnterior > tiempo)  //Verifica si hay datos nuevos en telegram cada 1 segundo. (o sea, si pasa un segundo)
    {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1); //declara la variable entrera numerosMensajes y le asigna un entero que viene de un metodo geter del objeto bot.
    while (numerosMensajes)
      {
      Serial.println("Comando recibido");
      handleNewMessages(numerosMensajes); //ESTA ES LA FUNCION MANEJADORA.
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
      }
    tiempoAnterior = millis(); //corre el cronometro para resetear el proximo segundo. usa millis para no usar delay que es mas bloqueante.
    }

      //---------------------------------------------------------------------------
                            //--2--AQUI SERVICIOS DE SENSOREO DESDE ESP HACIA TELEGRAM
          
  if(bandera) //si es 1 HIGH o 5v entra al if
      {
        Serial.println("BOTON PRESIONADO");
       bot.sendMessage(CHAT_ID, "BOTON DE PRUEBA PRESIONADO!", "");
       Serial.println("Led de Prueba prendido");
       digitalWrite(led_de_prueba, LOW); //es logica invertida.
       delay(2000);
       Serial.println("Led de prueba apagado");
       digitalWrite(led_de_prueba, HIGH);
       bandera = LOW; //reinicia la bandera porque sino seguiria alcaguetiando.
      }
  } //fin del void loop()



  //-------------------------------------FIN-------------------------------------------------


  
