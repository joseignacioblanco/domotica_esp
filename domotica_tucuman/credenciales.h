

//----------------Credenciales--------------------------------------------

#define SSID "la_belen_lava_las_tazas" //entre comillas poner el nombre de red wifi de donde va funcionar la placa.
                                       //antes estaba funcionando como const char* ssid = "la_belen_lava_las_tazas";.
#define PASSWORD "misumobelescubio" //entre comillas poner la contraseña de la red wifi
                                    //antes estaba cuncionando como const char* password = "misumobelescubio";.
#define BOTtoken "5568512239:AAGlvS7ObKzH0jHHaamfuVPbU5K2Tixbny8" //este token lo deuelve el BotFather de telegram.

//#define CHAT_ID "-1001922861125" //este id lo devuelve el bot IDBot de telegram. para el grupo de control remoto
#define CHAT_ID "-1001611135015" //este id lo devuelve el bot IDBot de telegram. para el grupo de control remoto
//#define CHAT_ID "900499386" //este id lo devuelve el bot IDBot de telegram. para solo prueba 1

//-----------------Otras Configuraciones-----------------------------------

#define LOKED HIGH //estados para la cerradura de las puertas
#define UNLOKED LOW
#define ENABLED HIGH
#define DISABLED LOW
//---------------------Configuracion de pines GPIO------------------------
//el pin D4 que pertenece al GPIO 2 es el del LED de la placa builtin.(para prender el led hay que meter un LOW porque es logica invertida)
//el pin D3 que pertenece al GPIO 0 es el del boton FLASH de la placa.

#define PUERTA_INGRESO_PIN 5
#define CHICHARRA_INGRESO_PIN 16
#define LUZ_VEREDA_PIN 4
