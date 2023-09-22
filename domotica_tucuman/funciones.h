
//Archivos cabecera
#include <UniversalTelegramBot.h> //Este archivo cabecera debe habilitar los metodos de lo que es el BOT de TELEGRAM.
#include <ArduinoJson.h> //Archivo cabecera para interpretar los comandos de telegran





WiFiClientSecure client; //Crea un objeto "client" de la clase WiFiClientSecure
UniversalTelegramBot bot(BOTtoken, client); //Parece que crea un objeto "bot" de la clase 
                                            //UniversalTelegramBot y lo construye con los 
                                            //parametros BOToken de la linea 17 y con el objeto
                                            //client de la linea 25.

//definicion de los pines (aqui solo los define. no los setea, eso lo hace la app domotica_tucuman)
const int magnetic_door_ingreso_pin = PUERTA_INGRESO_PIN; //el builtin de la esp32 es gpio 2.
//const int magnetic_door_ingreso_pin = 16; //debe ser para cuando uso la esp32 o la esp8266
const int chicharra_pin = CHICHARRA_INGRESO_PIN;
const int luz_vereda_pin = LUZ_VEREDA_PIN;
const int boton_de_prueba = 0;  //GPIO 0 - PIN D3
const int led_de_prueba = 2; //GPIO 2 - PIN D4


//estado inicial del pin
bool magnetic_door_ingreso_state = LOKED; //aqui configura el estado inicial del led en LOW.
bool chicharra_state = ENABLED;
bool luz_vereda_state = ENABLED;





//-------1--FUNCION MANEJADORA DE LA COMUNICACION ENTRE TELEGRAM Y LA ESP---------------

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

    if (text == "/Ayuda" || text == "/Ayuda@neder86_bot")
      {
      String welcome = "Control EDIFICIO-TUCUMAN, " + from_name + ".\n";
      welcome += "Comandos para control:\n\n";
      welcome += "/bloquear_puerta_ingreso \n";
      welcome += "/desbloquear_puerta_ingreso \n";
      welcome += "/activar_chicharra \n";
      welcome += "/desactivar_chicharra \n";
      welcome += "/luz_vereda_ON \n";
      welcome += "/luz_vereda_OFF \n";
      welcome += "/Estado muestra el estado general del sistema. \n";
      welcome += "/Ayuda imprime este Menú \n";
      bot.sendMessage(chat_id, welcome, "");
      }

    //bloquea puertas de ingreso reja y vidrio.
    if (text == "/bloquear_puerta_ingreso" || text == "/bloquear_puerta_ingreso@neder86_bot")
      {
      bot.sendMessage(chat_id, "Cerradura de ingreso BLOQUEADA!", "");
      magnetic_door_ingreso_state = HIGH;
      digitalWrite(magnetic_door_ingreso_pin, magnetic_door_ingreso_state);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //desbloquea puertas de ingreso reja y vidrio. (despues separar cada una cada una)
    if (text == "/desbloquear_puerta_ingreso" || text == "/desbloquear_puerta_ingreso@neder86_bot")
      {
      bot.sendMessage(chat_id, "Cerradura de ingreso DESbloqueada", "");
      magnetic_door_ingreso_state = LOW;
      digitalWrite(magnetic_door_ingreso_pin, magnetic_door_ingreso_state);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }
 //--------------------------------------------------------------------------------------------------------------------------------------------------

    //activa chicharra de puertas abiertas.
    if (text == "/activar_chicharra" || text == "/activar_chicharra@neder86_bot")
      {
      bot.sendMessage(chat_id, "Chicharra ACTIVADA!", "");
      chicharra_state = HIGH;
      digitalWrite(chicharra_pin, chicharra_state);// configurar si es en alto o en bajo.
      }

    //desactiva chicharra de las puertas abiertas)
    if (text == "/desactivar_chicharra" || text == "/desactivar_chicharra@neder86_bot")
      {
      bot.sendMessage(chat_id, "Chicharra DESACTIVADA", "");
      chicharra_state = LOW;
      digitalWrite(chicharra_pin, chicharra_state);//configurar si es alto o bajo.
      }

 //--------------------------------------------------------------------------------------------------------------------------------------------------

    //Enciende la luz del plafon de la vereda.
    if (text == "/luz_vereda_ON" || text == "/luz_vereda_ON@neder86_bot")
      {
      bot.sendMessage(chat_id, "Luz vereda ENCENDIDA!", "");
      luz_vereda_state = HIGH;
      digitalWrite(luz_vereda_pin, luz_vereda_state);// configurar si es en alto o en bajo.
      }

    //Apaga la luz del plafon de la vereda.
    if (text == "/luz_vereda_OFF" || text == "/luz_vereda_OFF@neder86_bot")
      {
      bot.sendMessage(chat_id, "Luz vereda APAGADA!", "");
      luz_vereda_state = LOW;
      digitalWrite(luz_vereda_pin, luz_vereda_state);//configurar si es alto o bajo.
      }

 //----------------------------------------------------------------------------------------------------------------------------------------------------

          
          
          //agregar mas servicios



  
      
  //----------------------------------------------------------------------------------------------------------------------------------------------------

    //Estado general del sistema
    if (text == "/Estado" || text == "/Estado@neder86_bot")   //completar esto para la chicharra.
      {
      if (digitalRead(!magnetic_door_ingreso_state)) //Logica invertida para la esp8266 y normal para la esp 32.
        {
        bot.sendMessage(chat_id, "Acceso de ingreso BLOQUEADO", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Acceso de ingreso DES-BLOQUEADO", "");
        }
        //--------------------------------
        
      if (digitalRead(!chicharra_state)) //Logica invertida para la esp8266 y normal para la esp 32.
        {
        bot.sendMessage(chat_id, "Chicharra ENCENDIDA", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Chicharra APAGADA", "");
        }
        //--------------------------------

      if (digitalRead(!luz_vereda_state)) //Logica invertida para la esp8266 y normal para la esp 32.
        {
        bot.sendMessage(chat_id, "Luz vereda ENCENDIDA", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Luz vereda APAGADA", "");
        }
        //--------------------------------

        
         //agregar mas servicios

        
        
      }
    }
  } //Aqui cierra la funcion que maneja el mejsaje que llega desde telegram.



  //-----------------------------------------------------------------------------------------





//-------------------------------------2--FUNCION INTERRUPCION-------------------------------
bool bandera = LOW; //este flag es para que quede registro que se presiono el boton o salto el sensor de la interrupcion y le da tiempo al coigo a actuar.

void IRAM_ATTR buttonInput()
      { //esta es la funcion que se ejecuta cuando entra la interrupcion del boton
        bandera = HIGH;
      }

//----------------------------------------------------------------------------------------------







  
