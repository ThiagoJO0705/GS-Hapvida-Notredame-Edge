# HealthTrackr
![image](https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge/assets/126472997/999b4c38-5b87-4c7b-ae85-787a0bc129da)
  
Proposta de solução para a Global Solution solicitada pela Hapvida NotreDame Intermédica

## Problema de Saúde Abordado

O contexto atual da saúde enfrenta desafios significativos devido à rápida transformação digital na área médica. A crescente influência da era tecnológica destaca a necessidade urgente de uma abordagem inovadora nos cuidados de saúde. A qualidade de vida e a realização de objetivos humanos estão intrinsecamente ligadas à saúde, tornando essencial uma resposta eficaz às mudanças profundas no setor. A transição para uma saúde digital eficiente é complexa, exigindo foco na prevenção de doenças, automação de procedimentos e aprimoramento da precisão diagnóstica e terapêutica.

## Visão Geral da Solução
O HealthTrackr é uma solução abrangente e tecnologicamente avançada para os desafios contemporâneos na área da saúde. Projetado para capitalizar as inovações tecnológicas, o dispositivo integra um microcontrolador ESP32 como peça central. Este componente gerencia e processa dados provenientes de sensores essenciais, como monitoramento da frequência cardíaca e um MPU6050 para detecção de quedas e medição da temperatura corporal. Esses sensores fornecem informações precisas sobre o estado de saúde do usuário.

Após o processamento, os dados são transmitidos eficientemente por meio do protocolo MQTT, conhecido por sua leveza e eficiência em redes com largura de banda limitada. Essa abordagem assegura a entrega eficaz dos dados coletados e proporciona atualizações rápidas e em tempo real sobre o estado de saúde do usuário.

A integração do Fiware na plataforma do HealthTrackr amplifica suas capacidades, identificando desvios nos padrões de batimentos cardíacos e acionando alertas de emergência quando necessário. Esta funcionalidade é crucial para garantir uma resposta rápida e eficaz em situações críticas, reforçando o compromisso do HealthTrackr com a segurança e o bem-estar do usuário. A abordagem avançada consolida o HealthTrackr como uma ferramenta inovadora e confiável no cenário da saúde digital, visando catalisar mudanças palpáveis e duradouras na saúde global.

# Componentes
Para implementar a solução você precisará dos seguintes componentes:

| Componente                                           | Quantidade |                      
| -------------------                                  | :--------: |
| ESP32 DevKit V1                                      |     1      |
| Sensor de Batimento Cardíaco / Monitor de Pulso      |     1      |
| Acelerômetro e Giroscópio 3 Eixos MPU-6050 GY-521    |     1      |
| Tela OLED Monocromática 128x64 com interface I2C     |     1      |
| Buzzer                                               |     1      |
| Real Time Clocker (O ESP32 já vem com RTC imbutido)  |     1      |
| Protoboard                                           |     1      |
| Cabos Jumpers                                        |     17     |
| Cabo Micro USB                                       |     1      |

Para realizar a montagem dos compenentes, siga o exemplo disponibilizado [aqui](https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge/blob/main/MontagemHealthTrackr.png).

Após a montagem dos componentes é necessário utilizar a IDE Arduino para gravar o [código](https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge/blob/main/codigoHealthTrackr.ino) no microcontrolador ESP32. 

A próxima etapa envolve a transmissão de dados pelo microcontrolador utilizando o protocolo MQTT, integrando-se à plataforma Fiware.

Para essa etapa, alguns pré-requisitos são necessários. Inicialmente, configure uma Máquina Virtual (VM) que simula um ambiente Linux, especificamente o Ubuntu. Dentro dessa máquina virtual, realiza-se a instalação de ferramentas que facilitam a criação, gerenciamento e execução de contêineres de aplicativos essenciais para o projeto. Por fim, realiza-se a instalação do Fiware.

O Fiware é uma plataforma de código aberto destinada a atuar como backend, facilitando o desenvolvimento de aplicações voltadas para smart cities (cidades inteligentes), Internet of Things (IoT ou Internet das Coisas) e sistemas baseados em dados contextuais em tempo real. A plataforma oferece um conjunto de APIs (Interfaces de Programação de Aplicativos) e ferramentas que simplificam a criação de soluções inovadoras, buscando melhorar a qualidade de vida nas cidades e impulsionar a transformação digital em diversos setores.

## Instalação da Máquina VIrtual

Para começar, os requisitos de hardware para esta etapa são os seguintes:
<ul>
  <li>Núcleos de Processamento: 1 vCPU</li>
  <li>Memória RAM: 1 GB</li>
  <li>Armazenamento Secundário Mínimo: 20 GB HD e/ou SSD</li>
</ul>
  
Com esses requisitos atendidos, o primeiro passo é acessar o site da [Virtual Machine](https://www.virtualbox.org) e clicar no botão de download para instalar a máquina virtual, que, no nosso caso, será a VirtualBox. Em seguida, instale o programa necessário para essa etapa, o [Ubuntu](https://ubuntu.com/download/server) na versão 22.04 LTS. O terceiro passo envolve a criação e configuração da máquina virtual. Basta clicar em "novo" dentro do VirtualBox, nomear a máquina como "ubuntu", e seguir as instruções até a conclusão. Após a criação, selecione a máquina criada, vá em "configurações", clique em "armazenamento", adicione o arquivo do Ubuntu baixado e, por fim, inicie a máquina para realizar as configurações iniciais de usuário.

## Inicialização

Após abrir o sistema Linux, a primeira etapa é abrir o terminal. Em seguida, é necessário instalar uma ferramenta chamada Docker Compose, que facilita a criação, gerenciamento e execução de contêineres de aplicativos. Para fazer isso, clique aqui e siga as instruções fornecidas.

Com essa etapa concluída, procedemos à instalação e inicialização do Fiware. Utilize os seguintes comandos no terminal:

```bash
git clone https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge.git
cd fiware
docker-compose up -d
```
Neste momento, verificamos se as portas estão abertas corretamente. Acesse o site do [Postman](https://www.postman.com), um suporte de API usado como comunicador e receptor de respostas do ESP32. Crie uma conta, um "my workspace", adicione os arquivos do repositório do GitHub, vá para a pasta 1.1, crie uma variável chamada "url" com seu endereço IP e clique em "send". Se a resposta for OK, as portas estão configuradas corretamente.

Agora, para testar o programa, primeiro crie sua variável na porta 3. Ao concluir, basta ligar o ESP32 para enviar e solicitar dados nas portas 17. Ao finalizar o programa, para fechar as portas, utilize o comando:
```bash
docker-compose down
```

# Componentes Complementares na Plataforma Fiware

A plataforma Fiware apresenta componentes complementares que merecem destaque. São eles:

## Orion Context Broker

O Orion Context Broker é responsável pela gestão de contextos, abrangendo a coleta, armazenamento e disponibilização de informações contextuais relevantes. Ele lida com dados provenientes de sensores, informações de localização e estados de dispositivos, sendo particularmente útil para a gestão das tagid.

## STH Comet

O STH-Comet, parte essencial da plataforma Fiware, desempenha um papel fundamental na preservação em grande escala de informações contextuais passadas. Colaborando com o Orion Context Broker, ele possibilita a captura, armazenamento e consulta eficientes de dados históricos. Com recursos avançados, como o armazenamento otimizado de séries temporais e a realização de consultas agregadas e de séries temporais, o STH-Comet oferece uma interface RESTful para interação, permitindo aos desenvolvedores acessar e analisar dados passados de maneira ágil. Em suma, o STH-Comet simplifica o armazenamento e a recuperação de informações contextuais históricas na estrutura Fiware.

## MongoDB

O MongoDB é um sistema de banco de dados NoSQL utilizado na plataforma Fiware para armazenar dados contextuais provenientes do Orion Context Broker e de outros componentes. Ele suporta documentos JSON flexíveis, possui recursos avançados para consultas e escalabilidade horizontal, tornando-se adequado para lidar com grandes volumes de dados na plataforma Fiware.

## Eclipse Mosquitto

O Eclipse Mosquitto é um broker MQTT utilizado na plataforma Fiware para facilitar a troca de mensagens entre dispositivos IoT e outros componentes. Ele suporta recursos de autenticação, segurança e controle de acesso, proporcionando uma solução escalável e configurável para a comunicação MQTT na plataforma Fiware. Os dados são armazenados no tópico TEF (Telemetria e Monitoramento de Equipamentos).

Para melhor entendimento sobre o Fiware e seus componentes complementares [clique aqui](https://github.com/fabiocabrini/fiware.git).

# Arquitetura do projeto
  
![image](https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge/assets/126472997/91b86c68-bdba-47c1-86f3-66e17c584f9c)

Caso queira acessar o arquivo da arquitetura [clique aqui](https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge/blob/main/HealthTrackr.drawio).

# Wokwi
Foi feito uma simulação no Wokwi, que é uma plataforma online que oferece uma simulação interativa de dispositivos eletrônicos baseados em microcontroladores, como Arduino e ESP32. Essa plataforma permite que os usuários projetem, testem e compartilhem circuitos eletrônicos de forma virtual.

Para acessar esse simulação [clique aqui](https://wokwi.com/projects/382215334300084225).

# Tecnologias Usadas

Para este projeto, a programação do ESP32 é realizada na linguagem C++ utilizando a IDE do Arduino. Inicialmente, é necessário instalar o pacote do ESP32 da Espressif, o que pode ser feito seguindo um tutorial específico. A configuração da tela OLED e do acelerômetro é feita utilizando bibliotecas padrão da IDE do Arduino, tais como a Adafruit GFX Library, Adafruit SSD1306, e Adafruit MPU6050.

As bibliotecas utilizadas para configurar o sistema de internet e o protocolo MQTT são a PubSubClient (desenvolvida por Nick O'Larry). Após essa etapa, o código resultante pode ser gravado no ESP32.

Em seguida, é necessário ligar a máquina virtual e seguir os passos específicos para configurar o Fiware, conforme mencionado anteriormente, e testar o programa conforme descrito.

O [código](https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge/blob/main/codigoHealthTrackr.ino) completo do ESP32 está disponível para visualização, oferecendo uma visão detalhada do projeto. Este método, ao utilizar principalmente a linguagem C++ para a programação do microcontrolador, proporciona uma implementação eficiente e abrangente para o monitoramento de saúde proposto.

# Integrantes
Nenhum participante é de outra turma. Todos são da 1ESPF
- Nikolas Rodrigues Moura dos Santos - RM551566 
- Thiago Jardim de Oliveira - RM551624
