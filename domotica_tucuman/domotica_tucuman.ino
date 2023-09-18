
#include <WiFi.h> //esta activo cuando es la  esp32 y hay qe cambiar tambien en placa


//#include <ESP8266WiFi.h> //esta activo cuando es la  8266 y hay qe cambiar tambien en placa
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>

//Reemplzar por datos de la red
const char* ssid = "la_belen_lava_las_tazas"; //Nombre de la red de WiFi
const char* password = "misumobelescubio"; //Contraseña de la red de WiFi

//Inicializacion del Bot de Telegram
#define BOTtoken "5568512239:AAGlvS7ObKzH0jHHaamfuVPbU5K2Tixbny8"  //Escribir token

//ID de chat
//#define CHAT_ID "-1001611135015"
#define CHAT_ID "900499386"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Chequea los mensajes cada 1 segundo
const unsigned long tiempo = 1000; //tiempo medio entre mensajes de escaneo
unsigned long tiempoAnterior; //última vez que se realizó el análisis de mensajes

//definicion de los pines
const int ledPin = 2;
//const int ledPin = 16;

//estado inicial del pin
bool ledState = LOW;

//Maneja lo q sucede cada vez q recibe un mensaje
void handleNewMessages(int numNewMessages) {

  for (int i=0; i<numNewMessages; i++) {
 
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue;
    }
    
    //Imprime el mensaje recibido
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/Ayuda") {
      String welcome = "Bienvenido al sistema de control luces de la DNT iot, " + from_name + ".\n";
      welcome += "Use estos comandos para controlar los leds.\n\n";
      welcome += "/led_on para encender GPIO \n";
      welcome += "/led_off para apagar GPIO \n";
      welcome += "/Estado muestra el estado de los GPIO \n";
      welcome += "/Ayuda imprime este menú \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    //encender led
    if (text == "/led_on") {
      bot.sendMessage(chat_id, "El LED esta encendido", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }

    //apagar led
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "El LED esta apagado", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }

    //Estado del led
    if (text == "/Estado") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "El LED esta encendido", "");
      }
      else{
        bot.sendMessage(chat_id, "El LED esta apagado", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  client.setInsecure();

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  //Conexion a red de WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi.. \n");
  }
  Serial.print("Conectado a la red wifi. Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Verifica si hay datos nuevos en telegram cada 1 segundo
  if (millis() - tiempoAnterior > tiempo)
  {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);

    while (numerosMensajes)
    {
      Serial.println("Comando recibido");
      handleNewMessages(numerosMensajes);
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    }

    tiempoAnterior = millis();
  }
}
