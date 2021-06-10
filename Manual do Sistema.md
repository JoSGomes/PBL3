# Dispositivo para Motos - Manual do Sistema
Nesse manual será explicado como o usuário deve fazer a integração das ferramentas utilizadas para o desenvolvimento da aplicação ***Dispositivo para Motos***. Ao final do mesmo, o usuário será capaz de utilizar todos os recursos fornecidos pela aplicação.

> **OBS:** É necessário baixar todos os arquivos presentes no repositório para ser possível fazer as configurações. 

## Amazon Web Service - AWS

Uma das ferramentas utilizada para o desenvolvimento do ***Dispositivo Para Motos*** é o ***AWS***, um sistema de serviços web da Amazon, que disponibiliza vários recursos em nuvem para desenvolvimento de aplicações. Dentre os recursos disponibilizados, os utilizados para essa aplicação *- e que você, usuário, deve configurar -* foram o ***IoT Core***, recurso que disponibiliza um servidor para comunicações MQTT, e o ***RDS***, recurso que disponibiliza um sistema de banco de dados relacional.

> Além desses recursos, um outro - Elastic Beanstalk - será utilizado. Na sessão Website esse recurso será abordado.

Sem mais conversas, vamos *"por a mão na massa"*!

### IoT Core

Nesse serviço serão criadas algumas conexões para que seja possível utilizar o servidor MQTT. Essas conexões são a partir de *Things*, *Policies* e *Certificates*.

#### Criação de *Thing* e *Policy*

No ***AWS*** navegue até o serviço **IoT Core** e, no menu lateral, selecione o seguinte caminho: *Manage/Thing*. No menu que irá abrir crie uma *Thing* única, dê um nome de sua preferência  e clique em next. *Thing* criada!

Após isso, novamente no menu lateral, selecione o seguinte caminho: *Secure/Policies*. No menu que irá abrir crie uma *Policy* adicionando um nome de sua preferência, além de alterar os campos subsequentes dessa forma: ao campo ***"action"*** coloque ``iot:*`` e no campo ***"resource ARN"*** altere apenas ``topic/replaceWithATopic`` para ``*``. Após fazer as alterações clique em *"create"*. Pronto, *Policy* criada!

*Não saia agora do menu do serviço **IoT Core**, tem mais alguns passos a seguir.*

#### Criação de *Certificate*

Com a *Thing* e a *Policy* criadas, é necessário fazer uma associação entre elas, e para isso é feito o *Certificate*. Primeiramente o *Certificate* deve ser criado, para isso vá ao menu lateral e selecione o seguinte caminho: *Secure/Certificates*. No menu que irá aparecer crie um certificado clicando no botão *"create"*. Após a criação você será direcionado para uma página onde existem alguns arquivos que serão necessários. Faça o download deles: ``A certificate for this thing``; ``A public key``; e ``A private key``. Após isso, abra o link para download de uma *Root CA*, procure por ``RSA 2048 bit key`` clique no link com o botão direito do mouse e o salve.

Agora vem a associação: navegue no menu lateral até *Secure/Certificates* e procure pelo certificado criado por você. Ao lado direito dele, clique em *"..."* e vincule a *Thing* e a *Policy* criadas anteriormente nas opções *"Attach Thing/Attach Policy"*. Pronto, seu certificado foi criado e está pronto!

*Permaneça com o menu do serviço **IoT Core** aberto, logo mais o usaremos. :)*

#### Conversão dos arquivos de certificado

Dos arquivos que foram baixados, três serão utilizados pelo sistema, são esses: ``A certificate for this thing``; ``Root CA``; e ``A private key``. O formato dos arquivos estão em um formato que a placa ***NodeMCU*** - que será falada mais adiante - não reconhece, assim é necessário convertê-los. A conversão será feita para a extensão ``.der``. A conversão pode ser feita por qualquer método contanto que o o arquivo ``...-certificate.pem.crt`` tenha o nome de ``cert.der``, o arquivo ``...-private.pem.key`` tenha o nome de ``private.der`` e o arquivo ``AmazonRootCA1.pem`` tenha o nome de ``ca.der``. Guarde-os, tanto os convertidos, quanto os que foram baixados primeiro.

Para a conversão é recomendado fazer a utilização do software *OpenSSL* fazendo uso das seguintes linhas de comando:

-  ``openssl x509 -in "código do seu arquivo"-certificate.pem.crt -out cert.der -outform DER``

-  ``openssl rsa -in "código do seu arquivo"-private.pem.key -out private.der -outform DER``

-  ``openssl x509 -in AmazonRootCA1.pem -out ca.der -outform DER``

Execute-os no terminal do SO.

### RDS - Relational Database Services

O serviço ***RDS*** foi utilizado para criar uma base de dados para o aplicativo afim de fazer o controles dos dados trocados entre os dispositivos (*website*, *placa* e *Alexa*). Para a presente aplicação o banco de dados já se encontra configurado e pronto para uso sem necessidade do usuário fazer configurações adicionais. O banco de dados escolhido foi o *MySQL*.

Para acessar esse recurso e visualizar os dados, além também de poder alterar algo de acordo a sua preferência é possível usar o software *MySQL Workbench* com as seguintes credenciais: 

> **Hostname:** ``pbl.cxqwhnwtx9nu.us-east-1.rds.amazonaws.com``
> **Port:** ``3306``
> **User:** ``admin``
> **Password:** ``password``

*Essas credenciais servirão também para conectar a placa e o site!*

## Alexa

O aplicativo tem a possibilidade de conexão com a ***Alexa***, a assistente virtual da Amazon. Através da ***Alexa*** você pode fazer algumas funcionalidades do aplicativo - funcionalidades essas que serão descritas no <a href = "https://github.com/JoSGomes/PBL3/blob/main/Manual%20de%20Usu%C3%A1rio.md">Manual do Usuário</a>.

As funcionalidades são feitas a partir de *"Skills"* que podem ser feitas por qualquer um a partir do site https://developer.amazon.com/alexa/console/ask. É por meio desse site que também será feita a configuração da *skill* da ***Alexa*** para a aplicação ***Dispositivo para Motos***.

> **OBS:** Para configurar a *Skill* para a sua conta é necessário que você esteja "logado" em sua conta da ***Alexa*** ao entra nesse site.

### Configurando a *Skill*

Após entrar nesse site você visualiza facilmente o botão *"create skill"*. Clique nele para ser direcionado ao menu de criação. Quando entrar no menu de criação escolha:

- Um nome para sua *Skill* *(de sua preferência)*;
- Português como linguagem;
- O modelo personalizado;
- E a linguagem de programação *"Python"*. 

Após escolher esses itens clique em *"create skill"* - ou *"criar habilidade"*, em português.

Após isso, você verá duas opções de modelo para começar a criar a sua *skill* - ignore-as. Essa escolha só é importante caso você queira criar uma skill do zero. Para apenas configurar escolha a opção *"import skill"* no canto superior direito e informe o seguinte *link:*

​								``https://github.com/JoSGomes/Skill-PBL3-SD.git``

> Esse link é de um repositório contendo a skill configurada.

Após o *build* da *skill* finalizar você será direcionado para o *console* da ***Alexa***.

### Definindo *Invocation*

No *console*, no menu lateral, vá até *Invocation*. No campo *"Skill Invocation Name"* escolha como você deseja chamar a skill - É uma escolha arbitrária, sugerimos *"dispositivo para motos"*. No menu superior, após escolher a invocação, clique em *"Save Model"* e *"Build Model"*, respectivamente, e espere o processo concluir.

> O nome dado a invocação só deve conter letras minúsculas.

### Importando bibliotecas 

Navegando no *console* da ***Alexa***, vá até a aba *code*. Nela serão feitas algumas importações de bibliotecas necessárias para que a *skill* funcione como deve. Juntamente com os arquivos baixados do repositório se encontra uma pasta *zipada* com nome de ``Bibliotecas necessárias.zip``, e é nela que estão as bibliotecas que iremos importar para o *console* da ***Alexa***.

Na aba *code*, selecione no menu superior a opção *"Import Code"*, escolha a pasta *zipada* ``Bibliotecas necessárias.zip`` e importe as pastas presentes nela. Após importar todas as pastas, clique em *Deploy* no canto superior direito e espere o processo terminar.

> **OBS:** A opção *"Import Code"* só suporta 100 arquivos por vez, então será necessário importar uma pasta por vez.

Após terminar o processo a sua skill está pronta para uso na sua conta! Você poderá fazer o teste dela na Aba *Test*, digitando "abrir " seguido da invocação que você escolheu para a *skill*.

## A placa *NodeMCU*

A placa ***NodeMCU*** é a responsável por verificar se aconteceu um acidente ou não na moto. Ela deve ficar na moto afim de capturar as informações de movimentação e verificar se está tudo bem.

Para que ela consiga fazer essas verificações, algumas configurações básicas são necessárias. A seguir será explicado o passo a passo para poder configurar com sucesso a sua placa ***NodeMCU***

### Conectar *NodeMCU* ao computador

Para poder configurar a placa com as instruções de execução, é necessário conectá-la ao computador por meio de um cabo *microUSB*. A seguir está o passo a passo de como deve ser feita essa conexão:

-  Instale a IDE do arduino: *Arduino IDE*;

> Você pode encontrá-la no site oficial da *Arduino*: https://www.arduino.cc/en/software ;

-  Conecte sua placa ao computador com um cabo USB e verifique se o seu drive de *USB-Serial CH340* está instalado. Se não, instale-o, será necessário para reconhecer a placa;

-  Abra sua *Arduino IDE* e navegue na barra de menu até *"Ferramentas/Placa/Gerenciar Placas"* e procure por *esp8266* e instale a versão mais recente;

-  Agora navegue no menu novamente até *"Ferramentas/Placa/ESP8266 boards*" e escolha a placa *NodeMCU 1.0(ESP-12E module)*;

-  Selecione a porta COM a qual sua placa está conectada em *"Ferramentas/Porta"*. Feche a *Arduino IDE*. 

Pronto, sua placa está conectada!

### Integração dos arquivos de certificado

A *NodeMCU* fará o uso dos arquivos de certificado baixados e convertidos anteriormente, que, como já foi dito serão três. Para integrar esses arquivos com a ***NodeMCU***, é preciso colocá-los na memória interna da placa. Para tal, siga o passo a passo a seguir:

-  Transfira os arquivos convertidos, ``private.der``, ``cert.der`` e ``ca.der``, para a pasta *data* que se encontra no diretório onde está o arquivo ``NodeMCUCode.ino`` ( presente na pasta de mesmo nome). Deixe-os lá;

>**OBS:** Caso não tenha pasta *data* nesse diretório, crie-a.

-  Vá até a pasta onde se encontram todos os arquivos (a pasta raiz do repositório) e procure por uma pasta *zipada* com nome de ``ESP8266FS-0.5.0.zip`` e extraia o conteúdo. Deixe-o lá;

-  Abra o *Arduino IDE* e navegue na barra de menu até *Arquivo/Preferências* e veja qual o diretório da sua pasta de *SketchBook*. Navegue até essa pasta (normalmente fica em documentos);

-  Chegando na pasta de *SketchBook* crie uma pasta com nome de ``tools`` e coloque a pasta que foi extraída no segundo passo;

-  Feche e abra sua *Arduino IDE*;

-  Navegue na barra de menu até *Ferramentas* e selecione ``ESP8266 Sketch data upload``;

-  Espere o Upload. Pronto, os arquivos já foram carregados na placa.

### Credenciais de acesso na *NodeMCU*

Algumas credencias devem ser adicionadas para que a NodeMCU faça a conexão correta com o *WiFi* local e com o banco de dados também. Essas credencias são adicionadas à arquivos ``.h`` que estão presentes na pasta ``NodeMCUCode``.

Para adicionar as credenciais a este arquivo basta que o código ``NodeMCUCode.ino`` seja aberto na *IDE* do *Arduino* que as abas com os arquivos ``.h`` já estarão presentes lá.

Em ``arduino_secrets.h`` adicione as seguintes credenciais do banco de dados:

- **SECRET_USERDB**: *"admin"*
- **SECRET_PASSDB**:  *"password"*

Em ``credenciais.h`` adicione as seguintes credenciais do *WiFi* local e também da sua *Thing* do ***AWS***.

- **USER_WIFI**: *"nome do seu WiFi"*
- **PASSWORD_WIFI**: *"senha do seu WiFi"*
- **ENDPOINT_AWS**: *"endpoint da sua Thing"*

> **OBS:** o endpoint pode ser encontrado indo ao painel do ***IoT Core*** no ***AWS***, no final do menu lateral esquerdo em *settings*. Assim que abrir o menu *Settings* você verá o *endpoint*.

### Bibliotecas necessárias para *NodeMCU*

Para que seja possível carregar as instruções de execução na placa, primeiramente é necessário integrar algumas bibliotecas ao seu arduino. As bibliotecas necessárias são as  pastas presentes na pasta ``lib_nodeMCU.zip``. Após a localização dessa pasta, extraia seu conteúdo e copie as pastas dentro e cole no diretório do *Sketchbook* do *Arduino IDE* na pasta com nome de ``libaries`` (geralmente em *Documentos\Arduino\libraries*).

###  Carregar código à placa NodeMCU

Por fim, após realizar todos esses passos vocês está pronto para carregar o código em sua placa *NodeMCU*. Com sua placa conectada ao computador, abra na *Arduino IDE* o arquivo ``NodeMCUCode.ino`` (presente na pasta de mesmo nome) e clique no ícone de seta no canto superior esquerdo para carregar o código em sua placa. Espere todo o processo, e pronto, ***sua placa está configurada para uso!***

## Website

O site presente no sistema foi criado a partir do *framework Laravel* e estará presente no diretório do sistema na pasta com nome de ``PBL2-website`` e para fazer uso dessa ferramenta é necessário configurá-la. Para a configuração será necessário a presença do Xampp, do composer e do Laravel em seu computador (Vide como fazer instalação desses recursos).

>Para auxiliar esse processo o software ***Visual Studio Code*** da *Microsoft* é recomendado, porém não é essencial.

###  Configurar Website

Abra o terminal do seu SO e navegue à pasta ``IoTwebSite`` (se tiver utilizando o VS Code, basta abrir a pasta com o software e usar o terminal do mesmo). Execute os comandos a seguir nessa mesma ordem: ``composer install``; ``composer update``. Espere os processos entre esses comandos.

Após executar esses comandos, vá ao diretório do website, ``IoTwebSite``, crie um arquivo com nome vazio e extensão ``.env``, abra o arquivo ``.env.example`` que está presente na pasta e copie todo o seu conteúdo para o arquivo ``.env`` que você acabou de criar. Salve. Após isso, execute o comando ``php artisan key:generate`` no terminal. Espere o processo.

### Credenciais do Banco de dados no Website

Abra o arquivo ``.env`` e vá até a linha 10 do mesmo, onde você verá várias variáveis com o prefixo ``DB_``. São elas que você vai alterar. Nas seguintes variáveis, atribua os valores correspondentes:

- **DB_HOST:**  *pbl.cxqwhnwtx9nu.us*-east-1.rds.amazonaws.com
- **DB_USERNAME:** *admin*
- **DB_PASSWORD:** *password*

Salve o arquivo.

### Credenciais do MQTT no Website

Abra o arquivo ``.env`` e vá ao final do mesmo e você verá várias variáveis com o prefixo``MQTT``. Serão necessários alterar alguns desses atribuindo os valores correspondentes:

-  **MQTT_HOST:** coloque o *EndPoint* da Thing do AWS;
-  **MQTT_TLS_CA_FILE:** coloque o diretório do arquivo ``Root CA`` baixado;
-  **MQTT_TLS_CLIENT_CERT_FILE:** coloque o diretório do arquivo ``A certificate for this thing`` baixado;
-  **MQTT_TLS_CLIENT_CERT_KEY_FILE:** coloque o diretório do arquivo ``A private key`` baixado;

> **OBS:** Os arquivos devem ser colocados em seus formatos originais, sem conversão;

Salve o arquivo.

# Pronto!

Após todo esse processo seu site está pronto para uso! Basta apenas você executar o comando ``php artisan serve`` no terminal na pasta ``IoTwebSite`` e abrir no navegador com o endereço *IP* passado pelo *Laravel* ou utilizar *localhost:8000*.
Existe também a possibilidade da utilização do serviço *Elastic Beanstalk* do *AWS* para hospedar o site na nuvem. 

##### Caso ache desnecessário, pule esse passo e aproveite o seu ***WebSite :)***.

## Elastic Beanstalk

Para utilizar esse serviço, primeiramente é necessário que você tenha instalado o *Git* no seu computador e ele esteja na *Path* das variáveis de ambiente.
Após ter o *Git* em seu computador vá a pasta do *Website* ``IoTwebSite`` e execute o comando ``git archive -v -o myapp.zip --format=zip HEAD`` trocando o nome ``myapp`` para o nome que desejar nomear o arquivo. Esse comando fará um arquivo *zipado* para você utilizá-lo no serviço do ***AWS***.

### Configurando e *zipando* o arquivo do Website

Antes de mais nada é necessário que você adicione dentro do arquivo *zipado* os arquivos de certificado, chave e *Root* que você guardou anteriormente (copie apenas os arquivos no formato original, ignorando os arquivos que foram alterados os formatos). Após isso, copie o arquivo ``.env`` que está presente na pasta do *Website* já configurado e altere apenas os diretórios dos arquivos de  certificado, chave e *Root*  anteriormente informados, porém, como o local foi mudado é necessário alterar. 

> **OBS:** O arquivo ``.env`` precisa estar na pasta principal e os arquivos de certificado devem estar na pasta public ( pois o laravel reconhece a pasta public como pasta raiz).

Após isso você tem seu arquivo do site pronto para ser postado.

### *Upando* site no Elastic Beanstalk

Navegue no site do ***AWS*** até o menu do ***Elastic Beanstalk*** e selecione, na página inicial, *"create application"*. Escolha um nome de sua preferência, altere a plataforma para *PHP* e em *"Plataform branch"* coloque *PHP 7.4 ...* . Em *"Application code"*  marque *Sample application* e por fim clique no botão laranja *create application*. Espere o processo de criação.

Após o processo você será direcionado para a página de edição da aplicação. Selecione "*Upload and deploy* " e selecione o arquivo *zipado* contendo o site que foi editado anteriormente.

Após fazer o *upload* do arquivo do site, navegue no menu lateral para *"configuration"*. Em *"configuration"* vá na categoria *"software"* e selecione editar. Altere o *"proxy server"* para *Apache* e em *"Document root"* coloque */public*.

Pronto, tudo já foi configurado. Volte agora para a página de edição da aplicação e abra o link fornecido pelo ***Elastic Beanstalk***. Por esse link qualquer pessoa pode acessar seu site.

##### Desfrute da aplicação ***Dispositivos para Motos:two_hearts::two_hearts:***

##### A Equipe de Desenvolvimento agradece :clap::clap::clap::clap:

