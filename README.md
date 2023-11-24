# HealthTrackr
Proposta de solução para a Global Solution solicitada pela Hapvida NotreDame Intermédica

## Problema de Saúde Abordado

O contexto atual da saúde enfrenta desafios significativos devido à rápida transformação digital na área médica. A crescente influência da era tecnológica destaca a necessidade urgente de uma abordagem inovadora nos cuidados de saúde. A qualidade de vida e a realização de objetivos humanos estão intrinsecamente ligadas à saúde, tornando essencial uma resposta eficaz às mudanças profundas no setor. A transição para uma saúde digital eficiente é complexa, exigindo foco na prevenção de doenças, automação de procedimentos e aprimoramento da precisão diagnóstica e terapêutica.

## Visão Geral da Solução
O HealthTrackr é uma solução abrangente e tecnologicamente avançada para os desafios contemporâneos na área da saúde. Projetado para capitalizar as inovações tecnológicas, o dispositivo integra um microcontrolador ESP32 como peça central. Este componente gerencia e processa dados provenientes de sensores essenciais, como monitoramento da frequência cardíaca e um MPU6050 para detecção de quedas e medição da temperatura corporal. Esses sensores fornecem informações precisas sobre o estado de saúde do usuário.

Após o processamento, os dados são transmitidos eficientemente por meio do protocolo MQTT, conhecido por sua leveza e eficiência em redes com largura de banda limitada. Essa abordagem assegura a entrega eficaz dos dados coletados e proporciona atualizações rápidas e em tempo real sobre o estado de saúde do usuário.

A integração do Fiware na plataforma do HealthTrackr amplifica suas capacidades, identificando desvios nos padrões de batimentos cardíacos e acionando alertas de emergência quando necessário. Esta funcionalidade é crucial para garantir uma resposta rápida e eficaz em situações críticas, reforçando o compromisso do HealthTrackr com a segurança e o bem-estar do usuário. A abordagem avançada consolida o HealthTrackr como uma ferramenta inovadora e confiável no cenário da saúde digital, visando catalisar mudanças palpáveis e duradouras na saúde global.

## Componentes
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

Para realizar a montagem dos compenentes, siga o exemplo:
  
![image](https://github.com/ThiagoJO0705/GS-Hapvida-Notredame-Edge/assets/126472997/1e231993-1651-483c-aa85-3e7c27036588)

Após a montagem dos componentes é necessário utilizar a IDE Arduino para gravar o código, disponibilizado neste mesmo repositório, no microcontrolador ESP32. 

Caso não tenha a IDE instalada na sua máquina [Clique Aqui](https://www.arduino.cc/en/software).

A próxima etapa envolve a transmissão de dados pelo microcontrolador utilizando o protocolo MQTT, integrando-se à plataforma Fiware.

Para essa etapa, alguns pré-requisitos são necessários. Inicialmente, configure uma Máquina Virtual (VM) que simula um ambiente Linux, especificamente o Ubuntu. Dentro dessa máquina virtual, realiza-se a instalação de ferramentas que facilitam a criação, gerenciamento e execução de contêineres de aplicativos essenciais para o projeto. Por fim, realiza-se a instalação do Fiware.

O Fiware é uma plataforma de código aberto destinada a atuar como backend, facilitando o desenvolvimento de aplicações voltadas para smart cities (cidades inteligentes), Internet of Things (IoT ou Internet das Coisas) e sistemas baseados em dados contextuais em tempo real. A plataforma oferece um conjunto de APIs (Interfaces de Programação de Aplicativos) e ferramentas que simplificam a criação de soluções inovadoras, buscando melhorar a qualidade de vida nas cidades e impulsionar a transformação digital em diversos setores.


