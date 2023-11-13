
//Archivos cabecera
#include <UniversalTelegramBot.h> //Este archivo cabecera debe habilitar los metodos de lo que es el BOT de TELEGRAM.
#include <ArduinoJson.h> //Archivo cabecera para interpretar los comandos de telegran
#include <Adafruit_BMP085.h> //este cabecera lo pongo para que ande el sensor bm9180 de presion y temp y altitud. que va por i2c en los pines d1 y d2 de esp8266



WiFiClientSecure client; //Crea un objeto "client" de la clase WiFiClientSecure
UniversalTelegramBot bot(BOTtoken, client); //Parece que crea un objeto "bot" de la clase 
                                            //UniversalTelegramBot y lo construye con los 
                                            //parametros BOToken y con el objeto
                                            //client.

//definicion de los pines (aqui solo los define. no los setea, eso lo hace la app domotica_tucuman).
const int magnetic_door_ingreso_pin = PUERTA_INGRESO_PIN; //GPIO 5 - PIN D1
const int chicharra_pin = CHICHARRA_INGRESO_PIN; //GPIO 16 - PIN D0
const int luz_vereda_pin = LUZ_VEREDA_PIN; //GPIO 4 - PIN D2
const int sirena_antipanico_pin = SIRENA_ANTIPANICO_PIN; //GPIO 14 -PIN D5
const int boton_de_prueba = 0;  //GPIO 0 - PIN D3
const int led_de_prueba = 2; //GPIO 2 - PIN D4
const int luces_terraza_pin = LUCES_TERRAZA_PIN; //GPIO 12 - PIN D6
const int portero_11_pin = PORTERO_11_PIN; //GPIO 13 - PIN D7
const int puerta_terraza_pin = PUERTA_TERRAZA_PIN; //GPIO 15 - PIN D8

const int dht_pin = 3;  //el gpio 3 es el pin RX.  pin para el DHT11. sensor de temp y hum.
DHTesp dht; //Esto crea un objeto de la clase DHTesp.
Adafruit_BMP085 bmp; //Esto crea un objeto para sacar los datos de presion.
//-------1--FUNCION MANEJADORA DE LA COMUNICACION ENTRE TELEGRAM Y LA ESP---------------

void handleNewMessages(int numNewMessages) //Maneja lo q sucede cada vez q recibe un mensaje (ESTE ES EL PROTOTIPO DE LA FUNCION QUE RECIBE UN ENTERO)
  {

  for (int i = 0 ; i < numNewMessages ; i++)
    {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID)
      {
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue; //Rompe el ciclo acttual y pasa al siguiente ciclo de for. lo pone primero para que no siga con el if que viene. (Se evita el else)
      }
    
    //Imprime el mensaje recibido 
    String text = bot.messages[i].text;
    Serial.println(text); //para depurar en la terminal serie. imprime en el monitor serie lo que viene desde telegram.

    String from_name = bot.messages[i].from_name; //trae a una variable, el nombre del bot creado. (REVISAR)


    //----------AQUI EMPIEZA LO EDITABLE PARA AGREGAR FUNCIONES


    if (text == "/Ayuda" || text == "/Ayuda@neder86_bot")//opcion de ayuda--------------------------------------------------------------------
      {
      String welcome = "Control EDIFICIO-TUCUMAN, " + from_name + ".\n";
      welcome += "Comandos para control:\n\n";
      welcome += "/bloquear_puerta_ingreso \n";
      welcome += "/desbloquear_puerta_ingreso \n\n";
      welcome += "/activar_chicharra \n";
      welcome += "/desactivar_chicharra \n\n";
      welcome += "/luz_vereda_ON \n";
      welcome += "/luz_vereda_OFF \n\n";
      welcome += "/sirena_ANTIPANICO \n\n";
      welcome += "/luz_terraza_ON \n";
      welcome += "/luz_terraza_OFF \n\n";
      welcome += "/portero_ENABLED \n";
      welcome += "/portero_DISABLED \n\n";
      welcome += "/bloquear_puerta_terraza \n";
      welcome += "/desbloquear_puerta_terraza \n\n";
      welcome += "/consultar_temperatura \n\n";
      
      //...
      //welcome += "/mondongo \n";
      
      welcome += "/Estado muestra el estado general del sistema. \n";
      welcome += "/Ayuda imprime este Menú \n";
      
      bot.sendMessage(chat_id, welcome, ""); //esta linea ejecuta el envio de el texto welcome al telegram
      }

    //opcion de bloquea puertas de ingreso reja y vidrio.-----------------------------------------------------------------------------------------------
    if (text == "/bloquear_puerta_ingreso" || text == "/bloquear_puerta_ingreso@neder86_bot")
      {
      bot.sendMessage(chat_id, "Cerradura de ingreso BLOQUEADA!", "");
      digitalWrite(magnetic_door_ingreso_pin, LOKED);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //desbloquea puertas de ingreso reja y vidrio. (despues separar cada una cada una)
    if (text == "/desbloquear_puerta_ingreso" || text == "/desbloquear_puerta_ingreso@neder86_bot")
      {
      bot.sendMessage(chat_id, "Cerradura de ingreso DESbloqueada", "");
      digitalWrite(magnetic_door_ingreso_pin, UNLOKED);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }
 //--------------------------------------------------------------------------------------------------------------------------------------------------

    //activa chicharra de puertas abiertas.
    if (text == "/activar_chicharra" || text == "/activar_chicharra@neder86_bot")
      {
      bot.sendMessage(chat_id, "Chicharra ACTIVADA!", "");
      digitalWrite(chicharra_pin, ENABLED);
      }

    //desactiva chicharra de las puertas abiertas)
    if (text == "/desactivar_chicharra" || text == "/desactivar_chicharra@neder86_bot")
      {
      bot.sendMessage(chat_id, "Chicharra DESACTIVADA", "");
      digitalWrite(chicharra_pin, DISABLED);
      }

 //--------------------------------------------------------------------------------------------------------------------------------------------------

    //Enciende la luz del plafon de la vereda.
    if (text == "/luz_vereda_ON" || text == "/luz_vereda_ON@neder86_bot")
      {
      bot.sendMessage(chat_id, "Luz vereda ENCENDIDA!", "");
      digitalWrite(luz_vereda_pin, HIGH);
      }

    //Apaga la luz del plafon de la vereda.
    if (text == "/luz_vereda_OFF" || text == "/luz_vereda_OFF@neder86_bot")
      {
      bot.sendMessage(chat_id, "Luz vereda APAGADA!", "");
      digitalWrite(luz_vereda_pin, LOW);//configurar si es alto o bajo.
      }

 //----------------------------------------------------------------------------------------------------------------------------------------------------

          
     //Activa por 3 segundos la sirena antipanico.
    if (text == "/sirena_ANTIPANICO" || text == "/sirena_ANTIPANICO@neder86_bot")
      {
      bot.sendMessage(chat_id, "Disparo de sirena por 3seg", "");
      digitalWrite(sirena_antipanico_pin, ENABLED);
      delay(3000);
      digitalWrite(sirena_antipanico_pin, DISABLED);
      }


 //--------------------------------------------------------------------------------------------------------------------------------------------------

    //Enciende la luces de la terraza.
    if (text == "/luz_terraza_ON" || text == "/luz_terraza_ON@neder86_bot")
      {
      bot.sendMessage(chat_id, "Luces terraza ENCENDIDAS!", "");
      digitalWrite(luces_terraza_pin, HIGH);
      }

    //Apaga la luz del plafon de la vereda.
    if (text == "/luz_terraza_OFF" || text == "/luz_terraza_OFF@neder86_bot")
      {
      bot.sendMessage(chat_id, "Luces terraza APAGADAS!", "");
      digitalWrite(luces_terraza_pin, LOW);//configurar si es alto o bajo segun la logica del pin o del relé.
      }


//--------------------------------------------------------------------------------------------------------------------------------------------------

    //activa o desactiva el timbre del 11 del portero para que no suene cuando el calo no quiera.
    if (text == "/portero_ENABLED" || text == "/portero_ENABLED@neder86_bot")
      {
      bot.sendMessage(chat_id, "TIMBRE PRTERO SI SUENA!", "");
      digitalWrite(portero_11_pin, ENABLED);
      }

    //desactiva chicharra de las puertas abiertas)
    if (text == "/portero_DISABLED" || text == "/portero_DISABLED@neder86_bot")
      {
      bot.sendMessage(chat_id, "TIMBRE PORTERO NO SUENA", "");
      digitalWrite(portero_11_pin, DISABLED);
      }

 //--------------------------------------------------------------------------------------------------------------------------------------------------


//opcion de bloquea - desbloquea puertas de la terrazza.-----------------------------------------------------------------------------------------------
    if (text == "/bloquear_puerta_terraza" || text == "/bloquear_puerta_terraza@neder86_bot")
      {
      bot.sendMessage(chat_id, "Cerradura de la terraza BLOQUEADA!", "");
      digitalWrite(puerta_terraza_pin, LOKED);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //desbloquea puertas de ingreso reja y vidrio. (despues separar cada una cada una)
    if (text == "/desbloquear_puerta_terraza" || text == "/desbloquear_puerta_terraza@neder86_bot")
      {
      bot.sendMessage(chat_id, "Cerradura de terraza DESbloqueada", "");
      digitalWrite(puerta_terraza_pin, UNLOKED);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32. porque asi esta el led.  no aplica a lo demas pines
      }
 //--------------------------------------------------------------------------------------------------------------------------------------------------


    if(text == "/consultar_temperatura" || text == "/consultar_temperatura@neder86_bot")
    {
      TempAndHumidity data = dht.getTempAndHumidity();
      String temperatura = String(data.temperature,1);
      String humedad = String(data.humidity,1);
      Serial.println("Temperatura: " + temperatura + "ºC - Humedad: " + humedad + "%");

      Serial.print("Temperature = ");
      Serial.print(bmp.readTemperature());
      Serial.println(" *C");
    
      Serial.print("Pressure = ");
      Serial.print(bmp.readPressure());
      Serial.println(" Pa");
    
      // Calculate altitude assuming 'standard' barometric
      // pressure of 1013.25 millibar = 101325 Pascal
      Serial.print("Altitude = ");
      Serial.print(bmp.readAltitude());
      Serial.println(" meters");

      bot.sendMessage(chat_id, "Temp: " + temperatura + "ºC | Hum: " + humedad + "% | Pres: " + bmp.readPressure() + " Pa | Alt: " + bmp.readAltitude() + " m" , "");
      
    }


 //----------------------------------------------------------------------------------------------------------------------------------------------------

 //----------------------------------------------------------------------------------------------------------------------------------------------------
         
          //agregar mas servicios



  





      
  //----------------------------------------------------------------------------------------------------------------------------------------------------

    //Estado general del sistema
    if (text == "/Estado" || text == "/Estado@neder86_bot")   //completar esto para la chicharra.
      {
      if (digitalRead(magnetic_door_ingreso_pin))
        {
        bot.sendMessage(chat_id, "Acceso de ingreso BLOQUEADO", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Acceso de ingreso DES-BLOQUEADO", "");
        }
        //--------------------------------
        
      if (digitalRead(chicharra_pin))
        {
        bot.sendMessage(chat_id, "Chicharra ENCENDIDA", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Chicharra APAGADA", "");
        }
        //--------------------------------

      if (digitalRead(luz_vereda_pin))
        {
        bot.sendMessage(chat_id, "Luz vereda ENCENDIDA", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Luz vereda APAGADA", "");
        }
        //--------------------------------


        //--------------------------------

      if (digitalRead(luces_terraza_pin))
        {
        bot.sendMessage(chat_id, "Luz terraza ENCENDIDA", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Luz terraza APAGADA", "");
        }
        //--------------------------------


        //--------------------------------
        
      if (digitalRead(portero_11_pin))
        {
        bot.sendMessage(chat_id, "portero ACTIVADO", "");
        }
      else
        {
        bot.sendMessage(chat_id, "portero DESACTIVADO", "");
        }
        //--------------------------------


        if (digitalRead(puerta_terraza_pin))
        {
        bot.sendMessage(chat_id, "Acceso terraza BLOQUEADO", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Acceso terraza DES-BLOQUEADO", "");
        }
        //--------------------------------



        //-------------------------------------

        //Aqui va a consultar la temperatuta y humedad nuevamente

        TempAndHumidity data = dht.getTempAndHumidity();
        String temperatura = String(data.temperature,1);
        String humedad = String(data.humidity,1);
        Serial.println("Temperatura: " + temperatura + "ºC - Humedad: " + humedad + "%");

        //bot.sendMessage(chat_id, "Temperatura: " + temperatura + "ºC - Humedad: " + humedad + "%", "");
        bot.sendMessage(chat_id, "Temp: " + temperatura + "ºC | Hum: " + humedad + "% | Pres: " + bmp.readPressure() + " Pa | Alt: " + bmp.readAltitude() + " m" , "");


        //-------------------------------------
        
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
      }//lo unico que hace es levantar la bandera para que entre el mensaje cuando halla disponibilidad.

//----------------------------------------------------------------------------------------------







  
