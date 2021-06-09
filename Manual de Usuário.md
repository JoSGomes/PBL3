# Dispositivo para Motos - Manual de Usuário
Nesse manual é explicado como o usuário pode usar os recursos do ***Dispositivo para motos*** no *Website* e na *Alexa*. Ao fim desse manual o leitor saberá todas as funcionalidades e como o *Website* e a *Alexa* funciona.

> **OBS:** É necessário baixar todos os arquivos presentes no repositório para ser possível fazer as configurações. 

# Primeiros passos

Antes de mais nada, é necessário que o Website e a placa NodeMCU estejam configurados com todos os arquivos necessários. Caso ainda não tenha feito as configurações, siga as instruções no **Manual de Sistema**.

Após ter tudo pronto e configurado, conecte a placa a energia e espere que a LED, presente na placa, pisque exatamente 5 vezes, isso significa que ela conseguiu fazer todas as conexões com sucesso. Por conseguinte, abra o site que deve estar hospedado em algum servidor ou que pode estar hospedado localmente. 

> **OBS**: Caso o site esteja hospedado localmente, vá ao terminal, que deve estar aberto na pasta raiz do Website, digite ``php artisan serve`` , vá ao navegado e digite o link ``localhost:8080``. Por outro lado, se estiver hospedado no próprio serviço do *AWS* *Elastic Beanstalk*, abra o *Website* utilizando o link disponibilizado pelo mesmo serviço.

 Por último, verifique se já tem a *skill* PBLSD na sua lista de *skills*. 