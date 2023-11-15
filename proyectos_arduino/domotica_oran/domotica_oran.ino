//------------------------------------------------------------------------------------------------------------------------------------
//PROYECTO DE DOMOTICA PARA ORAN
//PLACA DE DESARROLLO ESP-8266
//VERSION: 1.0
//FECHA: 14/11/23
//tombonia@gmail.com
//-------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------ARCHIVOS CABECERA--------------------------------------------------------------------------------------

#include <ESP8266WiFi.h> //Este archivo cabeccera lo activo cuando es la placa ESP8266.
#include <DHTesp.h> //Este cabecera tiene las bibliotecas para usar el sensor dht11.
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
  
  pinMode(led_de_prueba, OUTPUT); //configura el pin d4 de la placa (LED) como salida y le mete high para que se apague led incorporado en pa placa gpio2
  digitalWrite(led_de_prueba, HIGH);//tiene logica invertida con low se prende y con high se apaga. va prender cuando toquen el timbre y este apagada la chicharra.
  //podria poner tambien un rele con una luz indicadora para que indique que suena el timbre o un buzzer.

  pinMode(boton_de_prueba, INPUT_PULLUP); //configura el pin d3 de la placa (BOTON FLASH) como entrada con pullup gpio0 (aqui va ir el octocoplador para meter graund y pullup)

  pinMode(farol_pin, OUTPUT); // configura el pin de puerta de la terraza como salida y le mete un LOW para disablearlo o sea desbloqueada.
  digitalWrite(farol_pin, AUTOMATIC);
  
  pinMode(luz_vereda_pin, OUTPUT); // configura el pin D0 de luz vereda como salida y le mete un high luz prendida. gpio16
  digitalWrite(luz_vereda_pin, LOW); //(es la luz del farol)

  pinMode(sirena_antipanico_pin, OUTPUT); // configura el pin de sirena antipanico como salida y le mete un LOW pa que no este sonando. gpio14 pin D5
  digitalWrite(sirena_antipanico_pin, DISABLED);

  pinMode(portero_pin, OUTPUT); // configura el pin de portero de la terraza como salida y le mete un LOW para disablearlo. pin D7 gpio13
  digitalWrite(portero_pin, ENABLED);


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

  //Aqui lo del sensor de temperatuta DHT11

    //DHTesp dht; //Esto crea una variabe dht del tipo DHTesp: (no se si es una variable o un objeto de una clase: buscar bien)la declaro en funciones porque sino esta fuera de alcance.
    dht.setup(dht_pin, DHTesp::DHT11); //esto setea la variable dht a su pin 3 definido en linea 27 de cabecera funciones.h y el modelo 11.  esos :: era espacio de nombre queno?

  
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
        Serial.println("BOTON TIMBRE PRESIONADO");
       bot.sendMessage(CHAT_ID, "TOCARON EL TIMBRE!!!", "");
       Serial.println("Led de Prueba prendido");
       digitalWrite(led_de_prueba, LOW); //es logica invertida.
       delay(2000);
       Serial.println("Led de prueba apagado");
       digitalWrite(led_de_prueba, HIGH);
       bandera = LOW; //reinicia la bandera porque sino seguiria alcaguetiando.
      }
  } //fin del void loop()



  //-------------------------------------FIN-------------------------------------------------
