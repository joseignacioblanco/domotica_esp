
//Archivos cabecera
#include <UniversalTelegramBot.h> //Este archivo cabecera debe habilitar los metodos de lo que es el BOT de TELEGRAM.
#include <ArduinoJson.h> //Archivo cabecera para interpretar los comandos de telegran



WiFiClientSecure client; //Crea un objeto "client" de la clase WiFiClientSecure
UniversalTelegramBot bot(BOTtoken, client); //Parece que crea un objeto "bot" de la clase 
                                            //UniversalTelegramBot y lo construye con los 
                                            //parametros BOToken y con el objeto
                                            //client.

//definicion de los pines (aqui solo los define. no los setea, eso lo hace la app domotica_tucuman).

const int boton_de_prueba = 0;  //GPIO 0 - PIN D3
const int led_de_prueba = 2; //GPIO 2 - PIN D4
const int farol_pin = FAROL_PIN; //GPIO 15 - PIN D8
const int luz_vereda_pin = LUZ_VEREDA_PIN; //GPIO 4 - PIN D2
const int portero_pin = PORTERO_PIN; //GPIO 13 - PIN D7
const int sirena_antipanico_pin = SIRENA_ANTIPANICO_PIN; //GPIO 14 -PIN D5

const int dht_pin = DHT_PIN;  //el gpio 15 es el pin D8.  pin para el DHT11. sensor de temp y hum.
DHTesp dht; //Esto crea un objeto de la clase DHTesp.

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


    if (text == "/Ayuda" || text == "/Ayuda@Iyire_bot")//opcion de ayuda- septimo lugar 7.---------------------------------------------------
      {
      String welcome = "Control CASA-ORAN, " + from_name + ".\n";
      welcome += "Comandos para control:\n\n";

      welcome += "🔊 /Portero_Activar. \n";
      welcome += "🔇 /Portero_Desactivar. \n\n";
      welcome += "🅰 /Farol_Modo_Automatico. \n";
      welcome += "Ⓜ /Farol_Modo_Manual: \n";
      welcome += " -->💡 /Luz_vereda_Prender. \n";
      welcome += " -->📴 /Luz_vereda_Apagar. \n\n";
      welcome += "📢 /Alarma: se activa por 3 seg.\n\n";

      welcome += "🌡💧 /Temperatura_Humedad. \n\n";
      
      //...
      //welcome += "/mondongo \n";
      
      welcome += "📊 /Estado: Muestra el estado general del sistema. \n\n";
      welcome += "🆘 /Ayuda: Imprime este Menú \n";
      
      bot.sendMessage(chat_id, welcome, ""); //esta linea ejecuta el envio de el texto welcome al telegram
      }


 //--------------------------------------------------------------------------------------------------------------------------------------------------


    //Enciende la luz del plafon de la vereda. deberia ser la tercera 3.
    
      if (text == "/Luz_vereda_Prender" || text == "/Luz_vereda_Prender@Iyire_bot")
       {
        if(digitalRead(farol_pin) == MANUAL)//si es tru es porque esta en manual 
          {
          bot.sendMessage(chat_id, "Luz Farol ENCENDIDA!", "");
          digitalWrite(luz_vereda_pin, HIGH);
          }
        else
          {
          bot.sendMessage(chat_id, "Para encender la luz del farol cambie a modo Manual", "");
          }
       }

      //Apaga la luz del plafon de la vereda.
      if (text == "/Luz_vereda_Apagar" || text == "/Luz_vereda_Apagar@Iyire_bot")
       {
        if(digitalRead(farol_pin) == MANUAL) //si es tru es porque esta en manual 
          {
          bot.sendMessage(chat_id, "Luz Farol APAGADA!", "");
          digitalWrite(luz_vereda_pin, LOW);//configurar si es alto o bajo.
          }
        else
          {
            bot.sendMessage(chat_id, "Para apagar la luz del farol cambie a modo Manual", "");
          }
       }

    
 //----------------------------------------------------------------------------------------------------------------------------------------------------

          
     //Activa por 3 segundos la sirena antipanico. deberia ser la cuarta 4.
    if (text == "/Alarma" || text == "/Alarma@Iyire_bot")
      {
      bot.sendMessage(chat_id, "Disparo de alarma por 3seg.", "");
      digitalWrite(sirena_antipanico_pin, ENABLED);
      delay(3000);
      digitalWrite(sirena_antipanico_pin, DISABLED);
      }


 //--------------------------------------------------------------------------------------------------------------------------------------------------


    //activa o desactiva el timbre del portero para que no suene cuando no quiera.(deberia ser la primera 1)
    if (text == "/Portero_Activar" || text == "/Portero_Activar@Iyire_bot")
      {
      bot.sendMessage(chat_id, "TIMBRE PRTERO SI SUENA!", "");
      digitalWrite(portero_pin, ENABLED);
      }

    //desactiva chicharra de las puertas abiertas)
    if (text == "/Portero_Desactivar" || text == "/Portero_Desactivar@Iyire_bot")
      {
      bot.sendMessage(chat_id, "TIMBRE PORTERO NO SUENA", "");
      digitalWrite(portero_pin, DISABLED);
      }

 //--------------------------------------------------------------------------------------------------------------------------------------------------


//opcion de farol modo manual o auto (deberia ser la segunda 2).-----------------------------------------------------------------------------------
    if (text == "/Farol_Modo_Manual" || text == "/Farol_Modo_Manual@Iyire_bot")
      {
      bot.sendMessage(chat_id, "Farol en modo MANUAL", "");
      digitalWrite(farol_pin, MANUAL);//el gpio2 tiene logica ivertida en la esp8266 pero normal en la esp32.
      }

    //desbloquea puertas de ingreso reja y vidrio. (despues separar cada una cada una)
    if (text == "/Farol_Modo_Automatico" || text == "/Farol_Modo_Automatico@Iyire_bot")
      {
      bot.sendMessage(chat_id, "Farol en modo AUTOMATICO", "");
      digitalWrite(farol_pin, AUTOMATIC);
      }
 //--------------------------------------------------------------------------------------------------------------------------------------------------

    // opcion de ver temperatura en quinto lugar 5
    if(text == "/Temperatura_Humedad" || text == "/Temperatura_Humedad@Iyire_bot")
    {
      TempAndHumidity data = dht.getTempAndHumidity();
      String temperatura = String(data.temperature,1);
      String humedad = String(data.humidity,1);
      Serial.println("Temperatura: " + temperatura + "ºC - Humedad: " + humedad + "%");

      bot.sendMessage(chat_id, "Temp🌡: " + temperatura + "ºC | Hum💧: " + humedad + "%", "");
      
    }


 //----------------------------------------------------------------------------------------------------------------------------------------------------

 //----------------------------------------------------------------------------------------------------------------------------------------------------
         
          //agregar mas servicios



  





      
  //----------------------------------------------------------------------------------------------------------------------------------------------------

    //Estado general del sistema  el sexto lugar 6.
    if (text == "/Estado" || text == "/Estado@Iyire_bot")   //completar esto para la chicharra.
      {


        //--------------------------------

        
      if (digitalRead(portero_pin))
        {
        bot.sendMessage(chat_id, "Timbre del portero ACTIVADO", "");
        }
      else
        {
        bot.sendMessage(chat_id, "Timbre del portero DESACTIVADO", "");
        }

        
        //--------------------------------


        if (digitalRead(farol_pin))  //pregunta primero si esta en manual y si es tru entonces pregunta si esta prendido o apagado
        {
        bot.sendMessage(chat_id, "Farol modo Manual", "");
        if (digitalRead(luz_vereda_pin))
          {
           bot.sendMessage(chat_id, "Luz del farol ENCENDIDA", "");
          }
        else
          {
          bot.sendMessage(chat_id, "Luz del farol APAGADA", "");
          }
        }
      else
        {
        bot.sendMessage(chat_id, "Farol modo Automatico", "");
        }


        //-------------------------------------

        //Aqui va a consultar la temperatuta y humedad nuevamente

        TempAndHumidity data = dht.getTempAndHumidity();
        String temperatura = String(data.temperature,1);
        String humedad = String(data.humidity,1);
        Serial.println("Temperatura: " + temperatura + "ºC - Humedad: " + humedad + "%");

        bot.sendMessage(chat_id, "Temp🌡: " + temperatura + "ºC - Hum💧: " + humedad + "%", "");


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







  
