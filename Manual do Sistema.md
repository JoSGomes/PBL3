# Dispositivo para Motos - Manual do Sistema
Nesse manual será explicado como o usuário deve fazer a integração das ferramentas utilizadas para o desenvolvimento da aplicação ***Dispositivo para Motos***. Ao final do mesmo, o usuário será capaz de utilizar todos os recursos fornecidos pela aplicação.

> **OBS:** É necessário baixar todos os arquivos presentes no repositório para ser possível fazer as configurações. 

## Amazon Web Service - AWS

Uma das ferramentas utilizada para o desenvolvimento do ***Dispositivo Para Motos*** é o ***AWS***, um sistema de serviços web da Amazon, que disponibiliza vários recursos em nuvem para desenvolvimento de aplicações. Dentre os recursos disponibilizados, os utilizados para essa aplicação *- e que você, usuário, deve configurar -* foram o ***IoT Core***, recurso que disponibiliza um servidor para comunicações MQTT, e o ***RDS***, recurso que disponibiliza um sistema de banco de dados relacional.

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

O serviço ***RDS*** foi utilizado para criar uma base de dados para o aplicativo afim de fazer o controles dos dados trocados entre -
<img src = "desktop1.jpg">



