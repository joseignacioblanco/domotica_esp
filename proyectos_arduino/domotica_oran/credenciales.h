//Archivo cabecera credenciales.h para configurar los parametros de funcionamiento y adaptar cada placa en particular.

//----------------Credenciales--------------------------------------------

//#define SSID "Jonas"
#define SSID "la_belen_lava_las_tazas" //entre comillas poner el nombre de red wifi de donde va funcionar la placa.
                                       //antes estaba funcionando como const char* ssid = "la_belen_lava_las_tazas";.

//#define PASSWORD "t0r1b10t1buda"                                       
#define PASSWORD "misumobelescubio" //entre comillas poner la contraseña de la red wifi.
                                    //antes estaba cuncionando como const char* password = "misumobelescubio"; como constante simbolica tambien funciona.
                                    
#define BOTtoken "6934161704:AAEG8TayLzH0VRerDJCOyf5V95Q4-USxcvg" //este token lo deuelve el BotFather de telegram.(Iyre_bot)

#define CHAT_ID "-4090388151" //este id lo devuelve el bot IDBot de telegram. para el grupo de control remoto CASA - ORAN.


//-----------------Otras Configuraciones-----------------------------------

#define LOKED HIGH //estados para la cerradura de las puertas
#define UNLOKED LOW
#define AUTOMATIC LOW //para el farol en modo automatico con el temporizador o manual con el bot de telegrant
#define MANUAL HIGH
#define ENABLED HIGH //esta la uso para ???
#define DISABLED LOW

//---------------------Configuracion de pines GPIO------------------------
//el pin D4 que pertenece al GPIO 2 es el del LED de la placa builtin.(para prender el led hay que meter un LOW porque es logica invertida)
//el pin D3 que pertenece al GPIO 0 es el del boton FLASH de la placa.

//En el programa hay que poner el numero de GPIO para que lo reconozca el ide de arduino.  NO SE SI LO RECONOCE CON EL PIN   DI, D2, D3, etc.

#define FAROL_PIN 15 //gpio 15 es el pin D8.
#define LUZ_VEREDA_PIN 16  //gpio16 pin D0.
#define PORTERO_PIN 13 //gpio 13  es el pin D7
#define SIRENA_ANTIPANICO_PIN 14 //gpio 14 es el pin D5
