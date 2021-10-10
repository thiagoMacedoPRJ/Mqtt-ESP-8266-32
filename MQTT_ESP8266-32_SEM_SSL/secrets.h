                                           /* Configurações HiddenS Básicas!! */

                                                /* CONFIGURAÇOES DE CONEXÕES */


#define AP_TIMEOUT 100
#define SERIAL 115200

//MQTT CONFIGURAÇOES
#define MQTT_AUTH false
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_ID "BIOCYBER-TECH"
#define LED LED_BUILTIN



const char* AP_NAME  = "BIOCYBER-TECH"; // Nome Do Acess Point || Ponto de Acesso da Rede Caso Não Encontre*

const char* TOPICO = "BIOCYBER-TECH/Horta"; // Topico que Se Conecta Dentro Do Broker

const char* BROKER = "broker.mqtt-dashboard.com"; // Nome Do Broker

WiFiClient wclient;
PubSubClient client(BROKER, 1883, wclient);





void checkWifi()
{

  WiFiManager wifiManager;

  wifiManager.setTimeout(AP_TIMEOUT);

  if (!wifiManager.autoConnect(AP_NAME)) //AP com Senha 12345678 -> wifiManager.autoConnect(AP_NAME,"12345678");
  {

    Serial.println("Falha ao tentar conectar tempo limite excedido, reiniciando em 3s!! ");
    delay(3000);

    //reinicia a tenta novamente ligar-se ao nosso wi-fi
    ESP.restart();
    delay(5000);

  }  
}


bool checkMqttConnection()
{

  if (!client.connected())
  {
    
    Serial.print("Conectando ao Broker MQTT: ");
    
    if (MQTT_AUTH ? client.connect(AP_NAME, MQTT_USERNAME, MQTT_PASSWORD) : client.connect(AP_NAME))
    {
      
        //SUBSCRIÇÃO DE TOPICOS
         if (client.connect(MQTT_ID)) {
              Serial.println("Conectado ao Broker com sucesso!");
              client.subscribe(TOPICO);
          } 
          else 
          {
              Serial.println("Noo foi possivel se conectar ao broker.");
              Serial.println("Nova tentatica de conexao em 10s");
              delay(10000);
          }
     }
    
  }
  return client.connected();

}



// RECEBER MENSAGENS!!
void RecebePayload(char* topic, byte* payload, unsigned int length)
{
  String msg;

  // Exibir payload recebido!
  Serial.println("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }


  //converter o byte em string do payload recebido!
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }

  if (msg == "0")
  {
    digitalWrite(LED, HIGH);
  }

  if (msg == "1")
  {
    digitalWrite(LED, LOW);
  }

}
