//Incluindo biblioteca MQTT
#include<WiFi.h>
#include <PubSubClient.h>  
#include "Esp32MQTTClient.h" 

//Definindo Pinos
#define GAS   A0
#define BUZZER  6
#define RELE  8

//Definindo configuracoes MQTT
#define BROKER_IP    "localhost"
#define DEV_NAME     "mqttdevice"


//Definições de variaveis
int SensorGas=0;

//Variaveis de acesso a rede local
const char* ssid = "NOME_DA_REDE";
const char* password = "SENHA";


WiFiClient espClient;
PubSubClient client(espClient);



void setup()
{
  Serial.begin(9600);
    
  conectandoWifi();
  
  //Configurando o server do broker Aedes
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  
  SensorGas=analogRead(GAS);//nível de gás.
    
  sinalAlerta(SensorGas);
  
}

//Metodo para efetuar conexão no Wifi
void conectandoWifi() {
  delay(10);
  
  //Utilizando a biblioteca WiFi, conectamos na rede
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Conectado");
}

//Método alerta sonoro de vazamento de gás
void sinalAlerta(int gas)
{
  
  releSeguranca(gas);
  
  if (gas >= 610 ) {//Emergência: Ambiente com alta quantidade de gás!
    
    //Subscrevendo mensagemem formado Json para Topic Apartamento
    client.publish("Apartamento",{"id":225,"ativo":true});
    
    //Loop para definir sonoridade do Buzzer
    noTone(BUZZER);
    delay(200);
    tone(BUZZER, 1500);
    delay(200);
    noTone(BUZZER);
    delay(200);
    tone(BUZZER, 1500);
    delay(200);
    
  }
  else if(gas >= 450 && gas<=610) {//Alerta: Gás se espalhando pelo ambiente!
    
    //Subscrevendo mensagemem formado Json para Topic Apartamento
    client.publish("Apartamento",{"id":225,"ativo":true});
    
    //Loop para definir sonoridade do Buzzer
    noTone(BUZZER);
    delay(1000);
    tone(BUZZER, 1500);
    delay(1000);
    noTone(BUZZER);
    delay(1000);
  tone(BUZZER, 1500);
    delay(1000);
    
  }else if(gas<450){//Detecção abaixo do esperado ou nula!
    
    //Desligamento do Buzzer
    noTone(BUZZER);

    //Subscrevendo mensagemem formado Json para Topic Apartamento
    client.publish("Apartamento",{"id":225,"ativo":false});  }
  
}

//Metodo para desativação do Rele para impedir ligamento de interruptor
void releSeguranca(int gas)
{
  if(gas<450)
  {
  digitalWrite(RELE, LOW);
  }
  else
  {
    digitalWrite(RELE, HIGH);
  }
   
}


  
