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

O serviço ***RDS*** foi utilizado para criar uma base de dados para o aplicativo afim de fazer o controles dos dados trocados entre os dispositivos (*website*, *placa* e *Alexa*). Para a presente aplicação o banco de dados já se encontra configurado e pronto para uso sem necessidade do usuário fazer configurações adicionais. O banco de dados escolhido foi o *MySQL*.

Para acessar esse recurso e visualizar os dados, além também de poder alterar algo de acordo a sua preferência é possível usar o software *MySQL Workbench* com as seguintes credenciais: 

> **Hostname:** ``pbl.cxqwhnwtx9nu.us-east-1.rds.amazonaws.com``
> **Port:** ``3306``
> **User:** ``admin``
> **Password:** ``password``

*Essas credenciais servirão também para conectar a placa e o site!*

## Alexa

O aplicativo tem a possibilidade de conexão com a ***Alexa***, a assistente virtual da Amazon. Através da ***Alexa*** você pode fazer algumas funcionalidades do aplicativo - funcionalidades essas que serão descritas no <a href = "https://github.com/JoSGomes/PBL3/blob/main/Manual%20de%20Usu%C3%A1rio.md">Manual do Usuário</a>.

As funcionalidades são feitas a partir de *"Skills"* que podem ser feitas por qualquer um a partir do site https://developer.amazon.com/alexa/console/ask.  Após entrar nesse site você visualiza facilmente o botão *"create skill"*. Clique nele para ser direcionado ao menu de criação.

### Criando uma *Skill*

Quando entrar no menu de criação escolha um nome para sua *Skill*, a linguagem, o modelo e também a linguagem de programação que deseja usar. É recomendado utilizar o idioma português e o modelo personalizado para que você faça do seu jeito! Após escolher clique em *"create skill"* - ou criar habilidade, em português.

Após isso, você verá duas opções de modelo para começar a criar a sua *skill* - você pode escolher. Depois de escolher selecione o botão *"create with template"* e você será direcionado ao painel de criação da *skill*.

### Definindo *Invocation* e uma *Intent*

No painel de criação, no menu lateral esquerdo navegue para *Invocation* e defina como deseja invocar a sua *skill* com a ***Alexa***. Após escolher a sua *"skill invocation name"*, clique em *"save model"* e em seguida em *"build model"*.

Novamente no menu lateral esquerdo navegue para *Interaction Model/Intents* e crie uma nova *"intent"* clicando no botão *"Add Intent"*. Escolha um nome e clique em *"create custom intent"*. Pronto, sua *"intent"* foi criada. Ela será uma interação que você terá com a ***Alexa***. Após criá-la, você será direcionado ao painel de edição da *"intent"* onde você poderá escolher formas de invocá-la. Você também pode adicionar variáveis na invocação para capturar dados utilizando esse formato: ``{variavel}``. Após criar a *"intent"* e as suas invocações, clique em *"save model"* e em seguida em *"build model"*.

*Sua invocation e intent foram criadas, agora vamos para o próximo passo!*

### Codificando

Na aba *code* você encontrará o código referente a sua *skill*. É aí que você faz as suas preferências. Quando você utiliza a invocação da *skill* criada a ***Alexa*** executa função ``handle`` da *class* ``LaunchRequestHandler(...)``. Se você quiser alterar o texto de inicio, basta alterar o valor da variável ``speak_output``.

Agora a parte mais importante. Para que seja possível utilizar a *"intent"* criada, você deve fazer uma *class* com ``"nome da sua intent"``+``Handle``, no mesmo padrão das outras *class* presentes no código - pode copiar e colar, o que vai alterar é que na função ``can_handle`` é necessário mudar o parâmetro da função que é retornada para o nome da sua *"intent"*. Dentro da função ``handle``, enfim, faça o que deseja fazer com a sua *skill*.

Após fazer todas essas configurações, no fim do código coloque ``sb.add_request_handler("nome da sua class")`` logo abaixo de ``sb = SkillBuilder()``.

Com isso, dê um *"save"* e *"deploy"*. Pronto, *skill* criada com sucesso! Você pode testar pela aba *"Test"*.

