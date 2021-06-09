/*Bibliotecas para manipulação de I/O e strings*/
#include <stdio.h>
#include <string.h>

/*Biblioteca para captura do 'Timestamp'*/
#include <TimeLib.h>

/*Bibliotecas para fazer comunicação com o servidor MQTT da AWS*/
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>

/*Arquivos contendo senhas e credenciais*/
#include "arduino_secrets.h"
#include "credenciais.h"

/*Bibliotecas para conexão da placa com o banco de dados MySQL*/
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


#include <math.h>
/********************************************************************/
/*****VARIAVEIS E CONSTANTES DEFINIDAS PARA A CAPTURA DO 'TIMESTAMP'***/
/********************************************************************/
//Servidor NTP usado para o 'timestamp':
static const char ntpServerName[] = "us.pool.ntp.org";
unsigned int localPort = 8888;

//Time-zone de São Paulo, Brasil:
const int timeZone = -3;

//Objeto WiFiUDP:
WiFiUDP Udp;

//Buffer para armazenar os pacotes de entrada e saída:
byte packetBuffer[NTP_PACKET_SIZE];

/********************************************************************/
/**********************'DEFINES' NECESSÁRIOS*************************/
/********************************************************************/
//Pino do botão da placa:
#define BUTTON D3

/********************************************************************/
/***************DECLARAÇÃO DE FUNÇÕES/PROCEDIMENTOS******************/
/********************************************************************/
void callback(char* topic, byte *payload, unsigned int length); // Procedimento chamado quando algum tópico inscrito é atualizado.
void setupWifi(); //Procedimento para conectar a placa ao WiFi.
void reconnect(); //Procedimento para conectar a placa ao MQTT.
void carregarArquivos(); // Procedimento que carrega os arquivos de certificados do AWS.
void sendNTPpacket(IPAddress &address);
void enviarEvento(char * hour_, int day_, int month_, char * name_,char * description);
void sendConnection(char * hour_, int interval);
time_t getNtpTime();
void desativarAlarm();
void ativarAlarm();
/********************************************************************/
/***************VARIAVEIS E OBJETOS NECESSÁRIOS**********************/
/********************************************************************/

//Objeto NTPClient e seu cliente UDP (objeto WiFiUDP):
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org");

//Objeto WiFiClientSecure;
WiFiClientSecure espClient;

//Objeto PubSubClient com suas configurações para comunicação com o Broker do AWS:
PubSubClient client(ENDPOINT_AWS, 8883, callback, espClient);

//Endereço IP do banco de dados [3.232.98.125]:
IPAddress server_addr(3,232,98,125);

//Comandos para mandar os dados para o banco de dados:
char INSERT_SQL_EVENTS[] = "INSERT INTO pbl.events (hour, day, month, name, description) VALUES ('%s','%d','%d','%s','%s')";
char UPDATE_SQL_CONNECTION[] = "UPDATE `pbl`.`connections` SET `value` = 'Conectado', `hour` = '%s', `interval` = '%d' WHERE (`id` = '1')";
char query[128];

//Objeto MySQL_Connection com seu Client para conexão com o banco de dados MySQL:
WiFiClient clientSQL;
MySQL_Connection conn((Client*) &clientSQL);

//Matrizes para capturar os dados dos arquivos de dados de rotina:
float accelerometer[3];
float gyroscope[3];

//Variaveis para intervalo de conexão:
int laterMillis; //tempo anterior
int currentMillis; //tempo atual
int intervalConnection = 60000;//intervalo de conexão para envio de status(em milissegundos). Padrão de 1 min.

//Variaveis para intervalo de ativação do alarme:
boolean alarm = false;
int intervalAlarm = 60000; // tempo limite para quando esta parado(em milissegundos), se atingido, então o alarme é ativado.
int laterMillisAlarm;
int currentMillisAlarm = NULL;

//Variaveis para intervalo com tempo limite de falar que "está tudo bem":
boolean imOK = true;
int intervalImOK = 60000;
int laterMillisImOK;
int currentMillisImOK = NULL;

/*************************************************************************/
boolean readTrue = false;
void setup() {
  
  Serial.begin(115200); //inicia o display serial, para depuração.
  pinMode(LED_BUILTIN, OUTPUT); // inicializa o pino do led como saída.
  pinMode(BUTTON, INPUT_PULLUP); //inicializa o pino do botao como entrada.

  //conexão com o WiFi:
  setupWifi();
  delay(1000);

  //Realiza a conexão com o MySQL:
  while (!conn.connect(server_addr, 3306, SECRET_USERDB, SECRET_PASSDB)) {
    Serial.println("Conexão SQL falhou.");
    conn.close();
    Serial.println("passou do conn");
    Serial.println("Conectando SQL novamente.");
  }
  Serial.println("Conectado ao servidor SQL."); 
  
  delay(1000);
  
  //faz o carregamentos dos certificados no espClient:
  carregarArquivos();
  
  //Conecta a placa ao MQTT
  reconnect();
  
   //inicia o objeto Udp na portal local informada (8888):
  Udp.begin(localPort);

  //Faz a sincronização do provedor para capturar o timestemp correto:
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
 
  //Publica para informar que a placa esta conectada:
  char hour_[12];
  sprintf(hour_, "%d:%d:%d", hour(), minute(), second());
  sendConnection(hour_, intervalAlarm/1000);
  
  
  //captura o primeiro milli de inicialização da placa.
  laterMillis = millis();
  File f = SPIFFS.open("/historic.txt", "w");
  f.close();
  
}

void loop() {
  File f = SPIFFS.open("/historic.txt", "r");
  while(f.available()){
    Serial.println(f.readStringUntil('\n'));
  }
  f.close();
  if(!client.connected())
    reconnect();

  //Verifica se os tópicos inscritos foram alterados.
  //se sim, executa callback:
  client.loop();
  
  //Verifica se o button foi pressionado:
  if(!digitalRead(BUTTON)){
    //Verifica o estado da lâmpada:
    if(!digitalRead(LED_BUILTIN) && imOK){
      desativarAlarm();
    }
    else if(imOK){
      ativarAlarm();    
    }    
    if(!imOK){
      imOK = true;
      currentMillisImOK = NULL;
      Serial.println("Usuário clicou no botão antes do tempo limite, está tudo bem.");
    }
    
  }
  
  currentMillis = millis();
  if(currentMillis - laterMillis >= intervalConnection){
    
    //Publica para informar que a placa esta conectada:
      char hour_[12];
      sprintf(hour_, "%d:%d:%d", hour(), minute(), second());
      sendConnection(hour_, intervalAlarm/1000);    
      laterMillis = millis();
  }

  delay(500);
  char hour_[7];
  sprintf(hour_, "%d:%d", hour(), minute());
  
  if(Serial.available()){
    readTrue = true;
    int dado[6];
    int i = 0;
    while(Serial.available()){
      if(i<3){
        accelerometer[i] = ((Serial.read() - 127)*0.015625);
        Serial.println(accelerometer[i]);
      }
      else{
        gyroscope[i-3] = (Serial.read() - 127)*1.953125;
        Serial.println(gyroscope[i-3]);
      }
      i++;
    }
  }
  else{
    readTrue = false;
  }
  
    
  if(readTrue && imOK){
    if(alarm == true){
      if(accelerometer[0] != 0 || gyroscope[0] != 0 || gyroscope[1] != 0 || gyroscope[2] != 0){//verifica o acelerômetro em X,Y,Z
        enviarEvento(hour_, day(), month(), "POSSIVEL FURTO", "A moto pode estar sendo roubada!!");
        imOK = false;
        sendHistoric("POSSIVEL FURTO");
      }
    }
    else{//alarme não ativado, segue rotina normal, em movimento.
      if((gyroscope[0] == 0 && gyroscope[1] <= -60 && gyroscope[2] == 0) && (accelerometer[0] != 0)){
        enviarEvento(hour_, day(), month(), "POSSIVEL ACIDENTE", "TOMBOU PARA DIRETA");     
        imOK = false;
        sendHistoric("POSSIVEL ACIDENTE - TOMBOU PARA A DIREITA");
      }
      
      else if(( gyroscope[0] == 0 && gyroscope[1] >= 60 && gyroscope[2] == 0) && (accelerometer[0] != 0)){
        enviarEvento(hour_, day(), month(),  "POSSIVEL ACIDENTE", "TOMBOU PARA ESQUERDA");
        imOK = false;
        sendHistoric("POSSIVEL ACIDENTE - TOMBOU PARA ESQUERDA");
      }
      else if(( gyroscope[0] >= 50 && gyroscope[1] == 0 && gyroscope[2] == 0) && (accelerometer[0] != 0)){
        enviarEvento(hour_, day(), month(),  "POSSIVEL ACIDENTE", "TOMBOU PARA FRENTE");
        imOK = false;
        sendHistoric("POSSIVEL ACIDENTE - TOMBOU PARA FRENTE");
      }
      else if(( gyroscope[0] <= -70 && gyroscope[1] == 0 && gyroscope[2] == 0) && (accelerometer[0] != 0)){
        enviarEvento(hour_, day(), month(), "POSSIVEL ACIDENTE", "TOMBOU PARA TRÁS");
        imOK = false;
        sendHistoric("POSSIVEL ACIDENTE - TOMBOU PARA TRÁS");
      }
      else if(accelerometer[0] <= -0.786944 || accelerometer[0] >= 0.944182){
        enviarEvento(hour_, day(), month(), "POSSIVEL ACIDENTE", "COLISÃO");
        imOK = false;
        sendHistoric("POSSIVEL ACIDENTE - COLISÃO");
      }
      else if(accelerometer[0] == 0  && gyroscope[0] == 0 && gyroscope[1] == 0 && gyroscope[2] == 0){
        if(currentMillisAlarm == NULL){
            currentMillisAlarm = millis();
        }
        laterMillisAlarm = millis();
        if(laterMillisAlarm - currentMillisAlarm >= intervalAlarm){
          ativarAlarm();
          Serial.println("Moto parada até o tempo limite, alarme ativado!");
        }
      }
      else{
        imOK = true;  
      }
    }
  }

  if(imOK == false){
      if(currentMillisImOK == NULL){
        currentMillisImOK = millis();
      }
      laterMillisImOK = millis();
      if(laterMillisImOK - currentMillisImOK >= intervalImOK){
          Serial.println("Usuário sofreu um acidente ou a moto foi furtada!");
          Serial.println("Notificando contatos...");  
          if(alarm == true){
            enviarEvento(hour_, day(), month(), "FURTO", "MOTO FOI FURTADA");   
          }
          else{
            enviarEvento(hour_, day(), month(), "ACIDENTE", "OCORREU UM ACIDENTE");
          }
          imOK = true;
          currentMillisImOK = NULL;
      }
  }
  
  delay(300);

}

void sendHistoric(char * description){
  File f = SPIFFS.open("/historic.txt", "r"); //day,hour:minute:second\tvalores\tdescrição\n
  if(f == NULL)
    f = SPIFFS.open("/historic.txt", "w");
  else{
    int data[4];
    String firstLine = f.readStringUntil('\n');
    if(firstLine != NULL){
      char firstLineChar[firstLine.length()];
      Serial.println("antes do strcpy");
      strcpy(firstLineChar, firstLine.c_str());
      Serial.println(firstLineChar);
      int dia = atoi(strtok(firstLineChar, ","));
      Serial.println(dia);
      int hora = atoi(strtok(NULL, ":"));
      Serial.println(hora);
      int minuto = atoi(strtok(NULL, ":"));
      Serial.println(minuto);
      
      if(dia != day()){
        File faux = SPIFFS.open("/aux.txt","w");
        
        if(hora < hour()){
          while(f.available()){
            faux.println(f.readStringUntil('\n'));
          }
          f.close();
  
          faux.close();
  
          faux = SPIFFS.open("/aux.txt", "r");
          
          f = SPIFFS.open("/historic.txt", "w");
    
          while(faux.available()){
            f.println(faux.readStringUntil('\n'));
          }
          
          f.close();
          faux.close();
          
        }
        else if(hora == hour()){
          if( minuto <= minute()){
            while(f.available()){
              faux.print(f.readStringUntil('\n'));
            }
            f.close();
    
            faux.close();
    
            faux = SPIFFS.open("/aux.txt", "r");
            
            f = SPIFFS.open("/historic.txt", "w");
      
            while(faux.available()){
              f.print(faux.readStringUntil('\n'));
            }
            
            f.close();
            faux.close();
          }
        }
      }
    }
  }


  f = SPIFFS.open("/historic.txt","a");
  if(!f)
    Serial.println("Erro ao abrir o histórico");
  else{
    f.print(day());
    f.print(",");
    f.print(hour());
    f.print(":");
    f.print(minute());
    f.print("\t");
    f.print(accelerometer[0]);
    f.print(",");
    f.print(accelerometer[1]);
    f.print(",");
    f.print(accelerometer[2]);
    f.print(",");
    f.print(gyroscope[0]);
    f.print(",");
    f.print(gyroscope[1]);
    f.print(",");
    f.print(gyroscope[2]);
    f.print("\t");
    f.println(description);
    f.close();
  }
}
/*
 * O procedimento que é chamada sempre que algum tópico assinado é atualizado
 * 
 * Parâmetros:
 *    char *topic         -> nome do tópico que foi atualizado;
 *    byte * payload      -> conteúdo do tópico que foi enviado;
 *    unsigned int length -> tamanho do conteúdo que foi enviado.
 */
void callback(char * topic, byte * payload, unsigned int length){
  //mostra no serial o que foi recebido:
  Serial.print("Mensagem recebida no tópico ");
  Serial.println(topic);

  //Verifica em qual tópico foi recebida a mensagem:
  int i=0;
  char aux[length];
  if(!strcmp(topic, "INTERVALO_SITE_CONNECTION")){
     while(i < length){
      aux[i] = (char) payload[i];
      i++;
     }
      intervalConnection = atoi(aux);
      Serial.println(intervalConnection);
      intervalConnection = intervalConnection * 1000;//para milissegundos
  }
  else if(!strcmp(topic, "INTERVALO_SITE_ALARM")){
    while(i < length){
      aux[i] = (char) payload[i];
      i++;
    }
    intervalAlarm = atoi(aux);
    Serial.println(intervalAlarm);
    intervalAlarm = intervalAlarm * 1000;//para milissegundos
  }
  else if(!strcmp(topic, "SET_ALARM")){
    while(i < length){
      aux[i] = (char) payload[i];
      i++;
    }
    int set_alarm = atoi(aux);
    if(set_alarm == 1){
      ativarAlarm();  
    }
    else{
      desativarAlarm();  
    }
    
  }
}

/*
 * atualiza a variável de alarme para informar que o alarme foi ativado.
 * 
 */
void ativarAlarm(){
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Alarme ativado!");
  alarm = true;
}

/*
 * Atualiza a variavel de alarme para informar que o alarme foi desativado.
 */
void desativarAlarm(){  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Alarme desativado!");
  currentMillisAlarm = NULL;
  alarm = false;
}

/*
 * Procedimento que realiza a conexão com o WiFi
 * 
 */
void setupWifi(){
  //acende o led para indicar que a placa esta se conectando ao WiFi
  digitalWrite(LED_BUILTIN, LOW);
  delay(10);

  //Setta o buffer
  espClient.setBufferSizes(512, 512);

  //Mostra as informacoes do WiFi
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(USER_WIFI);

  //Inicia conexao com WiFi
  WiFi.begin(USER_WIFI, PASSWORD_WIFI);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(". ");
  }

  // Amostra da rede WiFi que esta conectado
  Serial.println();
  Serial.println("Conectado no WiFi!");
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());
  
  //Inicia a instancia timeClient para se conectar ao servido ntp
  timeClient.begin();
  while (!timeClient.update())
    timeClient.forceUpdate();

  espClient.setX509Time(timeClient.getEpochTime());
  
  //apaga o led quando a conexao for bem sucedida:
  digitalWrite(LED_BUILTIN, HIGH);
}

/*
 * Procedimento que faz a leitura dos certificados do AWS armazenados na memória da placa,
 * e os carrega para o objeto espClient, que faz o tratamento de segurança do AWS.
 * 
 */
void carregarArquivos(){
    
  //inicia o SPIFFS, ferramenta para acessar a memoria flash da placa para manipulacao de arquivos:
  if(!SPIFFS.begin()){
    //Se nao conseguir, avisa e da um return para o restante do código nao executar:
    Serial.println("FALHA NO SETUP");
    return;
  }
  
  //Mostra o endereço do ponteiro HEAP:
  Serial.print("Heap: ");
  Serial.println(ESP.getFreeHeap());
  
  //Carrega o arquivo de certificado:
  File cert = SPIFFS.open("/cert.der", "r");
  if(!cert)
    Serial.println("Arquivo de certificado de conexão não teve êxito na abertura.");
  else{
    Serial.println("Arquivo de certificado de conexão teve êxito na abertura.");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
  
  if(espClient.loadCertificate(cert)){
    Serial.println("Arquivo de certificado carregado.");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
  else
    Serial.println("Falha ao carregar o arquivo de certificado.");
  
  //Carrega o arquivo de chave privada:
  File private_key = SPIFFS.open("/private.der", "r");
  if(!private_key)
    Serial.println("Arquivo de chave privada não teve êxito na abertura.");
  else{
    Serial.println("Arquivo de chave privada teve êxito na abertura.");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
  
  if(espClient.loadPrivateKey(private_key)){
    Serial.println("Arquivo de chave privada carregado.");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
  else
    Serial.println("Falha ao carregar o arquivo de chave privada.");
  
  //Carrega o arquivo do AWS Root:
  File ca = SPIFFS.open("/ca.der", "r");
  if(!ca)
    Serial.println("Arquivo do AWS Root não teve êxito na abertura.");
  else{
    Serial.println("Arquivo do AWS Root teve êxito na abertura.");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
  
  if(espClient.loadCACert(ca)){
    Serial.println("Arquivo do AWS Root carregado.");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
  else
    Serial.println("Falha ao carregar o arquivo do AWS Root.");
}
/* 
 *  Procedimento que realiza a conexão da placa com o MQTT ( broker)
 *  
 */
void reconnect(){
  
  //variavel para contagem de erros (se houver):
  int contagem_erro = 0;
  
  //Fica em loop enquanto o cliente nao estiver conectado
  while(!client.connected()){

    //Informa:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("TENTATIVA DE CONEXAO COM MQTT #");
    Serial.print(contagem_erro);
    Serial.println("...");

    // se o cliente se conectar ao "ESPthing", a conexao eh bem sucedida 
    if(client.connect("ESPthing")){
      delay(1500);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("CONECTADO");

      //Faz a inscrição em tópicos:  
      client.subscribe("INTERVALO_SITE_CONNECTION");
      client.subscribe("INTERVALO_SITE_ALARM");
      client.subscribe("SET_ALARM");
    }
    else{
      
      contagem_erro++;
      char buf[256];
      
      //Captura o erro decorrente ta tentativa de conexao:
      espClient.getLastSSLError(buf, 256);

      //informa o erro:
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      Serial.println("Aguarde 5 segundos para uma nova tentativa.");
      delay(5000);
    }
  }
}


/*
 * Procedimento que faz a sincronização com o provedor.
 * 
 * return:
 *    Retorna os segundos, desde 1970 até o instante foi chamada.
 *    Retorna 0 se não conseguir sincronizar.
 */
time_t getNtpTime(){
  IPAddress ntpServerIP; //enderço IP do servidor NTP

  //descartar todos os pacotes recebidos anteriormente
  while (Udp.parsePacket() > 0);
  // obtém um servidor aleatório do pool
  WiFi.hostByName(ntpServerName, ntpServerIP);

  //Envia uma solicitação NTP para o servidor obtido aleatóriamente:
  sendNTPpacket(ntpServerIP);

  //Permanece no loop por 1500 ms:
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      //ler o pacote no buffer Udp:
      Udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long secsSince1900;
      // Converte quatro bytes começando na localização 40 em um long int:
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  
  return 0; // Retorna 0 se não for possível obter a hora
}

/*
 * Envia uma solicitação NTP para o servidor de horário no endereço fornecido
 * 
 * Parâmetros:
 *    endereço IP do servidor.
 */
void sendNTPpacket(IPAddress &address){
  // Setta todos os bytes no buffer para 0:
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  
  // Inicializa os valores necessários para formar a solicitação NTP:
  packetBuffer[0] = 0b11100011;   // LI, versão, modo
  packetBuffer[1] = 0;     // estrado, ou tipo de clock
  packetBuffer[2] = 6;     // Intervalo de votação
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes de zero para Root Delay e Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  //Agora, envia um pacote solicitando um carimbo de data/hora:
  Udp.beginPacket(address, 123); // Os pedidos NTP são para a porta 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

/*
 * Envia o Evento que ocorreu nos sensores para o banco de dados.
 * 
 * Parâmetros:
 *    hour_ -> hora do evento
 *    day_ -> dia do evento
 *    month_ -> mês do evento
 *    name_ -> nome do evento
 *    description -> descrição do evento
 *    
 */
void enviarEvento(char * hour_ ,int day_, int month_, char * name_, char * description){
  //transforma os dados em um comando do MySQL:
  sprintf(query, INSERT_SQL_EVENTS, hour_, day_, month_, name_,description);
  // Inicia a instancia da classe de consulta:
  MySQL_Cursor * cur_mem = new MySQL_Cursor(&conn);
  //Executa a consulta (query):
  cur_mem->execute(query);
  Serial.println("Evento enviado!");
  // deleta o cursor para liberar memória:
  delete cur_mem;
}

/*
 * Envia informações de conexão da placa para o banco de dados
 * 
 * Parâmetros:
 *    hour_ -> hora da conexão
 *    interval -> intervalo de verificação em segundos
 * 
 */
void sendConnection(char * hour_, int interval){
  //transforma os dados em um comando do MySQL:
  sprintf(query, UPDATE_SQL_CONNECTION, hour_, interval);
  // Inicia a instancia da classe de consulta:
  MySQL_Cursor * cur_mem = new MySQL_Cursor(&conn);
  //Executa a consulta (query):
  cur_mem->execute(query);
  Serial.println("Estado da conexão enviado!");
  // deleta o cursor para liberar memória:
  delete cur_mem;
}
