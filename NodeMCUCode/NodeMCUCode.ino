/* Biblioteca para capturar o timestamp*/
#include <stdio.h>
#include <string.h>

/*Bibliotecas para fazer a comunicação com o MQTT usando o broker do AWS*/
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>

/*Arquivo contendo usuário e senha do MySQL*/
#include "arduino_secrets.h"

/*Arquivo contendo usuário e senha do WiFi e o endPoint da coisa da AWS*/
#include "credenciais.h"

/*Biblioteca para utilizar a memória EEPROM da ESP8266*/
#include <EEPROM.h>

/*Bibliotecas para fazer a comunicação da placa com o banco de dados MySQL*/
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

//Pino do botao da placa
#define BUTTON D3
#define TAMFILES 4//tamanho definido no txt para admitir a rotina
File fA, fG;

unsigned int localPort = 8888;
static const char ntpServerName[] = "us.pool.ntp.org";
//Nome e senha da rede WiFi:
const char * ssid = USER_WIFI;
const char * password = PASSWORD_WIFI;


void callback(char* topic, byte *payload, unsigned int length); // Procedimento chamado quando algum tópico inscrito é atualizado.
void setupWifi(); //Procedimento para conectar a placa ao WiFi.
void reconnect(); //Procedimento para conectar a placa ao MQTT.
void carregarArquivos(); // Procedimento que carrega os arquivos de certificados do AWS.
void sendNTPpacket(IPAddress &address);
void enviarEvento(int day_, int month_, char * description);

time_t getNtpTime();


//O end point da thing criada no AWS:
const char * awsEndPoint = ENDPOINT_AWS;

//instancia um objeto do tipo NTPClient:
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//instancia um objeto do tipo WiFiClientSecure:
WiFiClientSecure espClient;
//Configuração padrão do MQTT:
PubSubClient client(awsEndPoint, 8883  , callback, espClient);

//Endereço IP do Banco de dados [3.232.98.125]:
IPAddress server_addr(3,232,98,125);

//usuário e senha do banco MySQL:
char user[] = SECRET_USERDB;
char pass[] = SECRET_PASSDB;

//Comando para mandar os dados para o banco de dados:
char INSERT_SQL_EVENTS[] = "INSERT INTO pbl.events (day, month, description) VALUES ('%d','%d','%s')";
char query[128];


//Objeto conn, com um clientSQL para a conexão com o MySQL:
WiFiClient clientSQL;
MySQL_Connection conn((Client *)&clientSQL);


int laterMillis;//tempo anterior.
int currentMillis;//tempo atual
int intervalConnection = 10000; //intervalConnectiono para envio de status. default 10s.


int accelerometer[3][TAMFILES];
int gyroscope[3][TAMFILES];

boolean alarm = false;
int intervalAlarm;//tempo limite para quando está parado, se antigido, então alarme é ativado.

int loopSensors = 0;//contador utilizado para variar sequenciamente no Loop o valor dos sensores com base no TXT.

void setup() {
  laterMillis = millis();//captura o primeiro milli de inicialização da placa.
  Serial.begin(115200); //inicia o display serial, para depuração.
  pinMode(LED_BUILTIN, OUTPUT); // inicializa o pino do led como saída.
  pinMode(BUTTON, INPUT_PULLUP); //inicializa o pino do botao como entrada.

  //conexão com o WiFi:
  setupWifi();
  delay(1000);

  //Realiza a conexão com o MySQL:
  while (!conn.connect(server_addr, 3306, user, pass)) {
    Serial.println("Conexão SQL falhou.");
    conn.close();
    Serial.println("passou do conn");
    Serial.println("Conectando SQL novamente.");
  }
  Serial.println("Conectado ao servidor SQL."); 

  delay(1000);
  
  //faz o carregamentos dos certificados no espClient:
  carregarArquivos();

  loadSensors();
  //Conecta a placa ao MQTT
  reconnect();
  
  
}

void loop() {
  
  if(!client.connected())
    reconnect();

  //Verifica se os tópicos inscritos foram alterados.
  //se sim, executa callback:
  client.loop();
  
  //Verifica se o button foi pressionado:
  if(!digitalRead(BUTTON)){
    //Verifica o estado da lâmpada:
    if(!digitalRead(LED_BUILTIN)){
      digitalWrite(LED_BUILTIN, HIGH);
      
      alarm = false;
    }
    else{
      digitalWrite(LED_BUILTIN, LOW);
      enviarEvento(5, 15, "Apenas um teste.");//está aqui somente para teste
      alarm = true;
      
    }
  }
  
  currentMillis = millis();
  if(currentMillis - laterMillis >= intervalConnection){
    client.publish("CONEXAO","{\"status\": \"CONNECTED\"}");
    laterMillis = millis();
  }

  delay(500);
  if(alarm == true){
    if(accelerometer[0][loopSensors] != 0 || accelerometer[1][loopSensors] != 0 || accelerometer[2][loopSensors] != 0)//verifica o acelerômetro em X,Y,Z   
      enviarEvento(16,5, "A moto está sendo roubada!!");//colocando as datas manualmente... usar a bib dps
  }
  else{//alarme não ativado, segue rotina normal, em movimento.
    
    
  }
  
  loopSensors++;
  if(loopSensors == (TAMFILES - 1))//se já percorreu todos os valores dos sensores, então volta desde o primeiro valor. -1 pois loopSensors acessa os vetores de tamanho -> TAMFILES.
    loopSensors = 0;
  delay(300);
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
      intervalConnection = intervalConnection * 1000; //para millisegundos
  }
  
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
  Serial.println(ssid);

  //Inicia conexao com WiFi
  WiFi.begin(ssid, password);
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
      
      //Publica para informar que a placa esta conectada:
      client.publish("CONEXAO","{\"status\": \"PLACA CONECTADA\"}");

      //Faz a inscrição em tópicos:  
      client.subscribe("INTERVALO_SITE_CONNECTION");
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

void loadSensors(){
  //inicia o SPIFFS, ferramenta para acessar a memoria flash da placa para manipulacao de arquivos:
  if(!SPIFFS.begin()){
    //Se nao conseguir, avisa e da um return para o restante do código nao executar:
    Serial.println("FALHA NO SETUP");
    return;
  }
  
  fA = SPIFFS.open("/accelerometer.txt", "r");
  fG = SPIFFS.open("/gyroscope.txt", "r");
  if(fA == NULL){
    Serial.println("***ARQUIVOS NAO ENCONTRADOS!***");    
  }
  else{   
    int j = 0;
    int i = 0;
    char* divLineA;
    char* divLineG;
    while(fA.available()){//fA e fG mesmo tamanho     
      String lineFile = fA.readStringUntil('\n');
      char lineA[lineFile.length()];    
      strcpy(lineA, lineFile.c_str());

      divLineA = strtok(lineA, ",");
      accelerometer[i][j] = atoi(divLineA);
      i++;
      while(divLineA != NULL){
        divLineA = strtok(NULL, ",");            
        if(divLineA != NULL){
          accelerometer[i][j] = atoi(divLineA); 
          i++;
        }
      }
      i = 0;
      j++;      
    }
    i = 0;
    j = 0;
    
    while(fG.available()){
      
      String lineFile = fG.readStringUntil('\n');
      char lineG[lineFile.length()];
      strcpy(lineG, lineFile.c_str());
      
      divLineG = strtok(lineG, ",");
      gyroscope[i][j] = atoi(divLineG);
      i++;
      while(divLineG != NULL){
        divLineG = strtok(NULL, ",");         
        if(divLineG!= NULL){
          gyroscope[i][j] = atoi(divLineG);
          i++;         
        }
      }
      i = 0;
      j++;
    }

  }
  fA.close();
  fG.close();

  Serial.println("Sensores funcionando.");  
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
 * Envia o Estado da lampada para a tabela do banco de dados MySQL estado
 * 
 * Parâmetros:
 *    char * estado -> Estado atual da lampada.
 *    
 */
void enviarEvento(int day_, int month_, char * description){
  //transforma os dados em um comando do MySQL:
  sprintf(query, INSERT_SQL_EVENTS, day_, month_, description);
  // Inicia a instancia da classe de consulta:
  MySQL_Cursor * cur_mem = new MySQL_Cursor(&conn);
  //Executa a consulta (query):
  cur_mem->execute(query);
  
  Serial.println("Evento enviado!");
  // deleta o cursor para liberar memória:
  delete cur_mem;
}
