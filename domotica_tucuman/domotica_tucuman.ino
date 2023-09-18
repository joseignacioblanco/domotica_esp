//------------------------------------------------------------------------------------------------------------------------------------
//PROYECTO DE DOMOTICA PARA EL EDIFICIO
//PLACA DE DESARROLLO ESP-8266
//VERSION: 1.0
//FECHA: 18/9/23
//tombonia@gmail.com
//-------------------------------------------------------------------------------------------------------------------------------------


 //---------------------------------------ARCHIVOS CABECERA------------------------------------------------------------
//#include <WiFi.h> //Este archivo cabecera lo activo cuando es la placa esp32 y hay que cambiar tambien en "placa" del ide de arbino.
#include <ESP8266WiFi.h> //Este archivo cabeccera lo activo cuando es la placa ESP8266 y hay qe cambiar tambien en "placa" del ide de arbino.
#include <WiFiClientSecure.h> //Todavia no se para que sirve este archivo cabecera: al parecer hay que crear un objeto cliente de la clase WiFiClientSecure.
//#include <UniversalTelegramBot.h> //Este archivo cabecera debe habilitar los metodos de lo que es el BOT de TELEGRAM.
//#include <ArduinoJson.h> //Archivo cabecera para interpretar los comandos de telegran.
#include "credenciales.h" //aqui van a estar las claves de ssid password bottoken y chat_id.
#include "funciones.h"
//para agregar el archivo cabecera fui al triangulito de arriba a la derecha y puse nueva pestaña y le pongo el nombre.h y adentro los #defines.




//--------------------------------------------------------------------------------------------
                               //COMPLETAR CON DATOS DEL USUARIO (ya lo pase a credenciales.h)
                               // Lo pase todo al credenciales.h
//Reemplzar por datos de la red a utilizar donde va a estar la placa.(hacer el truquito de configurar mediante archivo y .h)
//const char* ssid = "la_belen_lava_las_tazas"; //Nombre de la red de WiFi donde va estar la placa esp.
//const char* password = "misumobelescubio"; //Contraseña de la red de WiFi de la linea anterior.

//Inicializacion del Bot de Telegram
//#define BOTtoken "5568512239:AAGlvS7ObKzH0jHHaamfuVPbU5K2Tixbny8"  //Escribir el token que me devuelve el bot father: Es una #define(Completar)
//tambien esto del BOToken y del ID lo puedo configurar a traves del archivito .h

//ID de chat: este ID lo saco de el bot: IDBot.
//#define CHAT_ID "-1001611135015" debe ser que hay un ID para cada grupo de telegram (REVISAR).
//#define CHAT_ID "900499386" //esta es la que anda con prueba.

//---------------------------------------------------------------------------------------------


//prototipado de funciones
void handleNewMessages(int numNewMessages); //prototipo de funcion



//------------------------------ALGUNAS DECLARACIONES-----------------------------------------

//WiFiClientSecure client; //Crea un objeto "client" de la clase WiFiClientSecure
//UniversalTelegramBot bot(BOTtoken, client); //Parece que crea un objeto "bot" de la clase 
                                            //UniversalTelegramBot y lo construye con los 
                                            //parametros BOToken de la linea 17 y con el objeto
                                            //client de la linea 25.




//Configura para que cheguee los mensajes cada 1 segundo.
const unsigned long tiempo = 1000; //tiempo medio entre mensajes de escaneo mil milisegundos es un segundo.
unsigned long tiempoAnterior; //última vez que se realizó el análisis de mensajes

//definicion de los pines
//const int magnetic_door_ingreso_pin = PUERTA_INGRESO_PIN; //el builtin de la esp32 es gpio 2.
//const int magnetic_door_ingreso_pin = 16; //debe ser para cuando uso la esp32 o la esp8266

//estado inicial del pin
//bool magnetic_door_ingreso_state = LOKED; //aqui configura el estado inicial del led en LOW.


//-----------------------------------------------------------------------------------------
 

 
 
 //------------------------------VOID SETUP---------------------------------------------------

void setup()
  {
  Serial.begin(115200); //Inicia y configura el puerto serie.
  client.setInsecure(); //No se que hace esto pero setea algo del oblejo client de la clase WiFiClientSecure.

  pinMode(magnetic_door_ingreso_pin, OUTPUT); //Configura el pin 2 de la placa esp como salida digital. Linea 33.
  digitalWrite(magnetic_door_ingreso_pin, magnetic_door_ingreso_state); //Le pone un LOW al pin 2. (deberia manejarlo con magnetic_door_ingreso_state de la linea 44.) porque es el estado predeterminado.
  
  //Conexion a red de WiFi
  WiFi.mode(WIFI_STA); //configura el WIFI de la esp como cliente o STATION creo que lo hace atravez de comandos AT y lo elige en modo 1. el modo 2 es AP y el 3 es mixto.
  WiFi.begin(SSID, PASSWORD); //aqui toma los parametros definidos en las lineas 13 y 14. (ya no ahora de credenciales.h)
  
  while (WiFi.status() != WL_CONNECTED) //Este while no termina hasta que se conecte a la red.
    {
    delay(1000);
    Serial.println("Conectando a WiFi.. \n");
    }
  Serial.print("Conectado a la red wifi. Dirección IP: "); //Una vez conectado a la red, manda mensaje de conectado y devuelve el IP.
  Serial.println(WiFi.localIP());
  }

//------------------------------------------------------------------------------------------------



                               
                               
   //----------------------------------VOID LOOP-----------------------------------

void loop()
  {
  if (millis() - tiempoAnterior > tiempo)  //Verifica si hay datos nuevos en telegram cada 1 segundo. (o sea, si: pasa un segundo)
    {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1); //declara la variable entrera numerosMensajes y le asigna un entero que viene de un metodo geter del objeto bot.
    while (numerosMensajes)
      {
      Serial.println("Comando recibido");
      handleNewMessages(numerosMensajes); //ESTA ES LA FUNCION MANEJADORA.
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
      }
    tiempoAnterior = millis(); //corre el cronometro para resetear el proximo segundo.
    }
  }

  //-------------------------------------FIN-------------------------------------------------





  
