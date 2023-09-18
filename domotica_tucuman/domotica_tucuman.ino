//------------------------------------------------------------------------------------------------------------------------------------
//PROYECTO DE DOMOTICA PARA EL EDIFICIO
//PLACA DE DESARROLLO ESP-8266
//VERSION: 1.0
//FECHA: 18/9/23
//tombonia@gmail.com
//-------------------------------------------------------------------------------------------------------------------------------------

//#include <WiFi.h> //Este archivo cabecera lo activo cuando es la placa esp32 y hay que cambiar tambien en "placa" del ide de arbino.
#include <ESP8266WiFi.h> //Este archivo cabeccera lo activo cuando es la placa ESP8266 y hay qe cambiar tambien en "placa" del ide de arbino.
#include <WiFiClientSecure.h> //Todavia no se para que sirve este archivo cabecera: al parecer hay que crear un objeto cliente de la clase WiFiClientSecure.
#include <UniversalTelegramBot.h> //Este archivo cabecera debe habilitar los metodos de lo que es el BOT de TELEGRAM.
#include <ArduinoJson.h> //Archivo cabecera para interpretar los comandos de telegran.
#include "credenciales.h" //aqui van a estar las claves de ssid password bottoken y chat_id.

//--------------------------------------------------------------------------------------------
                               //COMPLETAR CON DATOS DEL USUARIO
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

WiFiClientSecure client; //Crea un objeto "client" de la clase WiFiClientSecure
UniversalTelegramBot bot(BOTtoken, client); //Parece que crea un objeto "bot" de la clase UniversalTelegramBot y lo construye con los parametros BOToken de la linea 17 y con el objeto client de la linea 25.

//Configura para que cheguee los mensajes cada 1 segundo.
const unsigned long tiempo = 1000; //tiempo medio entre mensajes de escaneo
unsigned long tiempoAnterior; //última vez que se realizó el análisis de mensajes

//definicion de los pines
const int ledPin = 2; //el builtin de la esp32 es gpio 2.
//const int ledPin = 16; //debe ser para cuando uso la esp32 o la esp8266

//estado inicial del pin
bool ledState = LOW; //aqui configura el estado inicial del led en LOW.



             //---------FUNCION MANEJADORA DE LA COMUNICACION ENTRE TELEGRAM Y LA ESP---------------

void handleNewMessages(int numNewMessages) //Maneja lo q sucede cada vez q recibe un mensaje (ESTE ES EL PROTOTIPO DE LA FUNCION QUE RECIBE UN ENTERO)
  {

  for (int i=0; i<numNewMessages; i++)
    {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID)
      {
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue; //Rompe el ciclo acttual y pasa al siguiente ciclo de for.
      }
    
    //Imprime el mensaje recibido
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/Ayuda")
      {
      String welcome = "Bienvenido al sistema de control luces de la DNT iot, " + from_name + ".\n";
      welcome += "Use estos comandos para controlar los leds.\n\n";
      welcome += "/led_on para encender GPIO \n";
      welcome += "/led_off para apagar GPIO \n";
      welcome += "/Estado muestra el estado de los GPIO \n";
      welcome += "/Ayuda imprime este menú \n";
      bot.sendMessage(chat_id, welcome, "");
      }

    //encender led
    if (text == "/led_on")
      {
      bot.sendMessage(chat_id, "El LED esta encendido", "");
      ledState = HIGH;
      digitalWrite(ledPin, !ledState);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //apagar led
    if (text == "/led_off")
      {
      bot.sendMessage(chat_id, "El LED esta apagado", "");
      ledState = LOW;
      digitalWrite(ledPin, !ledState);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //Estado del led
    if (text == "/Estado")
      {
      if (digitalRead(!ledState)) //Logica invertida para la esp8266 y normal para la esp 32.
        {
        bot.sendMessage(chat_id, "El LED esta encendido", "");
        }
      else
        {
        bot.sendMessage(chat_id, "El LED esta apagado", "");
        }
      }
    }
  } //Aqui cierra la funcion que maneja el mejsaje que llega desde telegram.

//-----------------------------------------------------------------------------------------
                               //SETUP

void setup()
  {
  Serial.begin(115200); //Inicia y configura el puerto serie.
  client.setInsecure(); //No se que hace esto pero setea algo del oblejo client de la clase WiFiClientSecure.

  pinMode(ledPin, OUTPUT); //Configura el pin 2 de la placa esp como salida digital. Linea 33.
  digitalWrite(ledPin, ledState); //Le pone un LOW al pin 2. (deberia manejarlo con ledState de la linea 44.) porque es el estado predeterminado.
  
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
                               //LOOP

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
