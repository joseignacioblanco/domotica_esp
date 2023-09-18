#include <UniversalTelegramBot.h> //Este archivo cabecera debe habilitar los metodos de lo que es el BOT de TELEGRAM.
#include <ArduinoJson.h> //Archivo cabecera para interpretar los comandos de telegran





WiFiClientSecure client; //Crea un objeto "client" de la clase WiFiClientSecure
UniversalTelegramBot bot(BOTtoken, client); //Parece que crea un objeto "bot" de la clase 
                                            //UniversalTelegramBot y lo construye con los 
                                            //parametros BOToken de la linea 17 y con el objeto
                                            //client de la linea 25.

//definicion de los pines
const int magnetic_door_ingreso_pin = PUERTA_INGRESO_PIN; //el builtin de la esp32 es gpio 2.
//const int magnetic_door_ingreso_pin = 16; //debe ser para cuando uso la esp32 o la esp8266

//estado inicial del pin
bool magnetic_door_ingreso_state = LOKED; //aqui configura el estado inicial del led en LOW.



//---------FUNCION MANEJADORA DE LA COMUNICACION ENTRE TELEGRAM Y LA ESP---------------

void handleNewMessages(int numNewMessages) //Maneja lo q sucede cada vez q recibe un mensaje (ESTE ES EL PROTOTIPO DE LA FUNCION QUE RECIBE UN ENTERO)
  {

  for (int i = 0 ; i < numNewMessages ; i++)
    {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID)
      {
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue; //Rompe el ciclo acttual y pasa al siguiente ciclo de for.
      }
    
    //Imprime el mensaje recibido 
    String text = bot.messages[i].text;
    Serial.println(text); //para depurar en la terminal serie.

    String from_name = bot.messages[i].from_name; //trae a una variable, el nombre del bot creado. (REVISAR)

    if (text == "/Ayuda")
      {
      String welcome = "Bienvenido al sistema de control EDIFICIO-TUCUMAN, " + from_name + ".\n";
      welcome += "Use estos comandos para control:\n\n";
      welcome += "/bloquear_puerta_ingreso. \n";
      welcome += "/desbloquear_puerta_ingreso. \n";
      welcome += "/Estado muestra el estado general del sistema. \n";
      welcome += "/Ayuda imprime este menú \n";
      bot.sendMessage(chat_id, welcome, "");
      }

    //encender led
    if (text == "/bloquear_puerta_ingreso")
      {
      bot.sendMessage(chat_id, "Cerradura de ingreso BLOQUEADA!", "");
      magnetic_door_ingreso_state = HIGH;
      digitalWrite(magnetic_door_ingreso_pin, !magnetic_door_ingreso_state);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //apagar led
    if (text == "/desbloquear_puerta_ingreso")
      {
      bot.sendMessage(chat_id, "Cerradura de ingreso DESbloqueada", "");
      magnetic_door_ingreso_state = LOW;
      digitalWrite(magnetic_door_ingreso_pin, !magnetic_door_ingreso_state);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //Estado del led
    if (text == "/Estado")
      {
      if (digitalRead(!magnetic_door_ingreso_state)) //Logica invertida para la esp8266 y normal para la esp 32.
        {
        bot.sendMessage(chat_id, "Acceso de ingreso BLOQUEADO", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Acceso de ingreso DES-BLOQUEADO", "");
        }
      }
    }
  } //Aqui cierra la funcion que maneja el mejsaje que llega desde telegram.
