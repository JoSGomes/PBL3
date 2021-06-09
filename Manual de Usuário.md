# Dispositivo para Motos - Manual de Usuário
Nesse manual é explicado como o usuário pode usar os recursos do ***Dispositivo para motos*** no *Website* e na *Alexa*. Ao fim desse manual o leitor saberá todas as funcionalidades e como o *Website* e a *Alexa* funciona.

> **OBS:** É necessário baixar todos os arquivos presentes no repositório para ser possível fazer as configurações. 

## Primeiros passos

------

Antes de mais nada, é necessário que o Website e a placa NodeMCU estejam configurados com todos os arquivos necessários. Caso ainda não tenha feito as configurações, siga as instruções no <a href="https://github.com/JoSGomes/PBL3/blob/main/Manual%20do%20Sistema.md">Manual de sistema</a>.

Após ter tudo pronto e configurado, conecte a placa a energia e espere que a LED, presente na placa, pisque exatamente 5 vezes, isso significa que ela conseguiu fazer todas as conexões com sucesso. Por conseguinte, abra o site que deve estar hospedado em algum servidor ou que pode estar hospedado localmente. 

> **OBS**: Caso o site esteja hospedado localmente, vá ao terminal, que deve estar aberto na pasta raiz do Website, digite ``php artisan serve`` , vá ao navegado e digite o link ``localhost:8080``. Entretanto, se estiver hospedado no próprio serviço do *AWS* *Elastic Beanstalk*, abra o *Website* utilizando o link disponibilizado pelo serviço.

 Por último, verifique se já tem a *skill* PBLSD da Alexa na sua lista de *skills*.  
Após isso, tudo está pronto para começar a utilizar o sistema!

## Funcionalidades da Alexa

------

A Alexa cumpre duas funcionalidades no sistema como um todo. Ela pode notificar o estado atual de conexão da NodeMCU, caso você deseje consultar, alterar o intervalo de envio da conexão pela placa ao Banco de dados, o intervalo para que o alarme seja ativado, caso a motocicleta esteja parada, e, também, o intervalo chamado de "estou bem", para que o usuário indique que está tudo bem.

Para começar, é necessário abrir a *skill*  na Alexa, para isso, fale ``abrir dispositivo para motos`` , desse modo você estará invocando a *skill* e as suas funcionalidades estarão disponíveis.

### Consultar o estado atual da NodeMCU

Para consultar o estado atual da NodeMCU é necessário falar alguma das 5 frases a seguir:

- Estado da placa
- Qual o estado da placa 
- Qual o estado da conexão 
- Qual a conexão da placa
- A placa está conectada 

Falando qualquer uma dessas frases a Alexa irá ativar a funcionalidade de consultar o estado atual da NodeMCU, podendo falar se a placa está conectada ou desconectada.

### Alterar intervalos

Para alterar os intervalos da NodeMCU é necessário falar alguma das frases a seguir, em que I,J,K são as quantidades de tempo desejadas:

- Altere conexão para **I** minutos alarme para **J** minutos e estou bem para **K** minutos
- Altere o intervalo de conexão para **I** minutos o intervalo de alarme para **J** minutos e o intervalo de estou bem para **K** minutos
- Conexão para **I** minutos alarme para **J** e **K** minutos

O tempo de conexão é o intervalo de tempo que o site testará a conexão com a Placa NodeMCU.

O tempo de alarme é o tempo, em que a placa, ao não receber nenhuma alteração nos sensores, acionará automaticamente o modo alarme.

O tempo de 'Estou Bem' diz respeito ao tempo de tolerância que a NodeMCU terá, do momento que detectar um possível acidente ao momento de ligação dos contatos de emergência, vale lembrar que se o botão da placa for pressionado nesse intervalo de tolerância a placa não irá fazer a ligação para os contatos de emergência. 

Ao falar quaisquer das frases acima, a Alexa irá alterar os intervalos para os tempos que foram ditos.

## Funcionalidades do Website

------

Entrando no *Website* na página inicial é possível ver os contatos de emergência que foram adicionados e a informação se o dispositivo está conectado.

### Botão "Alarme"

Na tela inicial, há o botão "Alarme" Esse botão tem a funcionalidade de que, se for clicado, irá ativar o modo alarme da NodeMCU.

### Cadastro de contatos

No menu do site existe uma opção chamada cadastro de contatos, ao clicar nessa opção é possível adicionar um contato à lista de contatos de emergência que é exibida na tela inicial do site.

### Tabela de Eventos

No menu do site existe uma opção chamada tabela de eventos, ao clicar o usuário será redirecionado a página da tabela de evento, onde existe um filtro de dia e mês, onde o poderá consultar eventos que aconteceram em determinado dia. Estes eventos são os que a placa NodeMCU reporta.

### Configurações

No menu do site existe uma opção chamada Configurações, ao clicar o você será redirecionado a uma página de configurações, onde poderá alterar alguns  intervalos de tempos da placa:

- Tempo de verificação de conexão
- Tempo de acionamento de alarme
- Tempo de 'Estou Bem'

## Funcionalidades da placa NodeMCU

------

A NodeMCU possui algumas funcionalidades com o botão *"FLASH"*, que permite que você tenha acesso manual a ela.

### Botão "FLASH" 

Esse botão é responsável por ativar o modo alarme manualmente, ao pressioná-lo a LED na placa irá acender, o que significa que o alarme está ativado, pressioná-lo novamente desliga o modo alarme e a LED é apagada.

Por ela guardar o histórico de possíveis acidentes, acidentes, de fato, e furtos durante 24 horas, existe uma forma de mostrá-lo a você, para isso é necessário que você configure um monitor serial e conecte a placa ao computador, como foi visto no  <a href="https://github.com/JoSGomes/PBL3/blob/main/Manual%20do%20Sistema.md">Manual de sistema</a> que você pode consultar para realizar essa configuração se ainda não fez.

A forma para o histórico ser mostrado para você é pelo monitor serial, para isso, basta que pressione o botão "FLASH" por 3 segundos, assim todo o histórico das últimas 24 horas que reside na placa será mostrado para você.

### Detecção de acidentes

A placa tem dois sensores: o acelerômetro e o giroscópio. Assim, para que ela detecte que houve um acidente, primeiro ela precisa entrar no estado de "Possível acidente", que acontece quando uma anormalidade é detectada. Nesse sentido, é aí que entra o intervalo de tempo do "estou bem", caso o utilizador do sistema não vá até a NodeMCU e pressione o botão dentro deste intervalo, então é considerado que houve um acidente.

### Detecção de furto

Utilizando os mesmo sensores, caso o modo alarme esteja ativado e os sensores detectem qualquer movimentação na motocicleta a NodeMCU, então, detecta um estado de "Possível furto". Desse modo, o intervalo do "estou bem" é contabilizado, se utilizador não ir até a NodeMCU pressionar o botão, indicando que está tudo bem, dentro deste intervalo, a placa então detecta que um furto aconteceu.
