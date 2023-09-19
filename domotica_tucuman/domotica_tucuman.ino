//------------------------------------------------------------------------------------------------------------------------------------
//PROYECTO DE DOMOTICA PARA EL EDIFICIO
//PLACA DE DESARROLLO ESP-8266
//VERSION: 1.0
//FECHA: 18/9/23
//tombonia@gmail.com
//-------------------------------------------------------------------------------------------------------------------------------------



//---------------------------------------ARCHIVOS CABECERA------------------------------------------------------------
//#include <WiFi.h> //Este archivo cabecera lo activo cuando es la placa esp32. Hay que cambiar tambien en "placa" del ide de arbino.(podria hacerlo con hiderwors o algo similar)

#include <ESP8266WiFi.h> //Este archivo cabeccera lo activo cuando es la placa ESP8266.
#include <WiFiClientSecure.h> //Todavia no se para que sirve este archivo cabecera: al parecer hay que crear un objeto cliente de la clase WiFiClientSecure.
#include "credenciales.h" //aqui van a estar las claves de ssid password bottoken y chat_id, para personalizar mejor el codigo segun el usuario.
#include "funciones.h"
//para agregar el archivo cabecera fui al triangulito de arriba a la derecha y puse nueva pestaña y le pongo el nombre.h y adentro los #defines.



//-----------------------------------PROTOTIPADO DE FUNCIONES----------------------------------------------------------
void handleNewMessages(int numNewMessages); //Maneja el menu de opciones, los gpio y los mensajes a la comunicacion serie y a telegram.



//------------------------------ALGUNAS DECLARACIONES-----------------------------------------
const unsigned long tiempo = 1000; //delay entre cada verificacion de mensaje.
unsigned long tiempoAnterior; //última vez que se realizó el análisis de mensajes. Truquito para que funcione con millis()



 //------------------------------VOID SETUP---------------------------------------------------
void setup()
  {
  Serial.begin(115200); //Inicia y configura el puerto serie.
  client.setInsecure(); //No se que hace esto pero setea algo del oblejo client de la clase WiFiClientSecure.
  
  //Configura el pinaut GPIOS y los setea.
  pinMode(magnetic_door_ingreso_pin, OUTPUT); //Configura el pin 2 de la placa esp como salida digital. Linea 33.
  digitalWrite(magnetic_door_ingreso_pin, LOKED); //le pone un HIGH al pin del magnetico de la puerta de ingreso(definido en credenciales.h) y lo bloquea.
  
  pinMode(chicharra_pin, OUTPUT);
  digitalWrite(chicharra_pin, ENABLED);


  
  //Conexion a red de WiFi
  WiFi.mode(WIFI_STA); //configura el WIFI de la esp como cliente o STATION creo que lo hace atravez de comandos AT y lo elige en modo 1. el modo 2 es AP y el 3 es mixto.
  WiFi.begin(SSID, PASSWORD); //aqui toma los parametros de credenciales.h.

  int ciclo = 0; //la primera imprime conectando a wifi y despues puntos suspensivos nomas
  while (WiFi.status() != WL_CONNECTED) //Este while no termina hasta que se conecte a la red.
    {
    delay(300);
    ciclo ? Serial.print(".") : Serial.print("\n --> Conectando a WiFi."); //oerador ternario para que ponga puntos suspensivos hasta que se conecte
    ciclo++;
    }
  Serial.print("Se ha conectado a la red wifi. Dirección IP: "); //Una vez conectado a la red, manda mensaje de conectado y devuelve el IP.
  Serial.println(WiFi.localIP());
  
  }



//----------------------------------VOID LOOP-----------------------------------
void loop()
  {
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
  }



  //-------------------------------------FIN-------------------------------------------------





  
