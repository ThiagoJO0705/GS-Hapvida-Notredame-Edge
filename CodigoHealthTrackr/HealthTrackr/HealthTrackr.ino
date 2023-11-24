#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <time.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h> 
 
//defines:
//defines de id mqtt e tópicos para publicação e subscribe denominado TEF(Telemetria e Monitoramento de Equipamentos)
#define TOPICO_SUBSCRIBE    "/TEF/healthmonitor011/cmd"        //tópico MQTT de escuta
#define TOPICO_PUBLISH      "/TEF/healthmonitor011/attrs"      //tópico MQTT de envio de informações para Broker
#define TOPICO_PUBLISH_2    "/TEF/healthmonitor011/attrs/i"    //tópico MQTT de envio de informações para Broker
                                                      //IMPORTANTE: recomendamos fortemente alterar os nomes
                                                      //            desses tópicos. Caso contrário, há grandes
                                                      //            chances de você controlar e monitorar o ESP32
                                                      //            de outra pessoa.
#define ID_MQTT  "fiware_healthmonitor_011"      //id mqtt (para identificação de sessão)
                                 //IMPORTANTE: este deve ser único no broker (ou seja, 
                                 //            se um client MQTT tentar entrar com o mesmo 
                                 //            id de outro já conectado ao broker, o broker 
                                 //            irá fechar a conexão de um deles).
                                 // o valor "n" precisa ser único!
// Definições de pinos e constantes
#define LARGURA_TELA 128
#define ALTURA_TELA 64
#define RESET_OLED    -1         

// Inicialização do display OLED e sensor MPU6050
Adafruit_SSD1306 display(LARGURA_TELA, ALTURA_TELA, &Wire, RESET_OLED);
Adafruit_MPU6050 mpu;

// Configurações do hardware
#define PINO_INTERRUPCAO     5
#define PINO_BUZZER          15  

// Configurações de rede e tempo
#define NTP_SERVIDOR         "a.ntp.br"
#define FUSO_HORARIO_UTC     0
#define FUSO_HORARIO_DST     0




#define PULSOS_POR_BATIMENTO 1
#define INTERVALO_AMOSTRAGEM    1000

volatile uint16_t pulso;  
uint16_t contador;  
float frequenciaCardiaca;  
float temperatura = 0;            

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;  // Mutex para garantir acesso seguro a pulse

void IRAM_ATTR InterrupcaoFrequenciaCardiaca() {
  portENTER_CRITICAL_ISR(&mux);
  pulso++;
  portEXIT_CRITICAL_ISR(&mux);
}

// WIFI
const char* SSID = "FIAP-IBM"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "Challenge@23!"; // Senha da rede WI-FI que deseja se conectar
  
// MQTT
const char* BROKER_MQTT = "46.17.108.113"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
 
int D4 = 2;

//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída
  
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);



/* 
 *  Implementações das funções
 */
void setup() 
{
    Serial.begin(115200);

  pinMode(PINO_INTERRUPCAO, INPUT);
  attachInterrupt(digitalPinToInterrupt(PINO_INTERRUPCAO), InterrupcaoFrequenciaCardiaca, RISING);
  Wire.setClock(400000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print("BPM: ");
  display.display();

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(FUSO_HORARIO_UTC, FUSO_HORARIO_DST, NTP_SERVIDOR);

  
  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

 
  pinMode(PINO_BUZZER, OUTPUT);   

 

    while (!Serial);
    
    //inicializações:
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH, "s|on");
}
  
//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(115200);
}
 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
  
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
  
//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
     
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
    
    //toma ação dependendo da string recebida:
    //verifica se deve colocar nivel alto de tensão na saída D0:
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    if (msg.equals("lamp001@on|"))
    {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }
 
    //verifica se deve colocar nivel alto de tensão na saída D0:
    if (msg.equals("lamp001@off|"))
    {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
     
}
  
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
//Função: envia ao Broker o estado atual do output 
//Parâmetros: nenhum
//Retorno: nenhum
void EnviaEstadoOutputMQTT(void)
{
    if (EstadoSaida == '1')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|on");
    }
    if (EstadoSaida == '0')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|off");
    }
    delay(1000);
}
 
//Função: inicializa o output em nível lógico baixo
//Parâmetros: nenhum
//Retorno: nenhum
void InitOutput(void)
{
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    
    boolean toggle = false;

    for(int i = 0; i <= 10; i++)
    {
        toggle = !toggle;
        digitalWrite(D4,toggle);
        delay(200);           
    }
}
 
char msgBuffer[100]; // Declare a variável msgBuffer com tamanho suficiente para a conversão

// Função para calcular a frequência cardíaca
void FrequenciaCardiaca() {
  static unsigned long startTime;
  static bool emergenciaAcionada = false;

  // Verifica o intervalo de amostragem
  if (millis() - startTime < INTERVALO_AMOSTRAGEM) return;
  startTime = millis();

  // Leitura e reset do contador de pulsos
  portENTER_CRITICAL(&mux);
  contador = pulso;
  pulso = 0;
  portEXIT_CRITICAL(&mux);

  // Cálculo da frequência cardíaca
  frequenciaCardiaca = map(contador, 0, 220, 0, 220);

  // Exibição da frequência cardíaca no console serial
  Serial.println("Frequência Cardíaca: " + String(frequenciaCardiaca, 2) + " BPM");
  
  // Atualização do display OLED e tratamento de emergências
  if (frequenciaCardiaca > 50 && frequenciaCardiaca < 100) {
    AtualizarDisplay();
    imprimirHorarioLocal();
    medirTemperatura();
  } else if (frequenciaCardiaca > 100 && frequenciaCardiaca < 200 && !emergenciaAcionada) {
    taquicardia();
    emergenciaAcionada = true; // Define a variável de estado
  } else if (frequenciaCardiaca < 50 && !emergenciaAcionada) {
    bradicardia();
    emergenciaAcionada = true;
  } else {
    emergenciaAcionada = false; // Reset da variável de estado
  }
} 

// Função para medir a temperatura usando o sensor do MPU6050
void medirTemperatura() {
  // Leitura do registrador de temperatura do MPU6050
  Wire.beginTransmission(0x68); // Endereço I2C do MPU6050
  Wire.write(0x41); // Endereço do registrador de temperatura
  Wire.endTransmission(false);

  Wire.requestFrom(0x68, 2); // Solicitar 2 bytes de dados
  if (Wire.available() >= 2) {
    int16_t tempRaw = Wire.read() << 8 | Wire.read();
    temperatura = (float)tempRaw / 340.0 + 36.53; // Conversão para graus Celsius
  }

  // Exibição da temperatura no console serial
  Serial.println("Temperatura: " + String(temperatura, 2) + " °C");

  // Lógica para tratamento da temperatura
  if (temperatura > 35.5 && temperatura < 37.2) {
    tempOk();
  } else if (temperatura < 35.4) {
    hipotermia();
  } else if (temperatura > 37.5 && temperatura <= 38) {
    febril();
  } else if (temperatura > 38 && temperatura < 39) {
    febre();
  } else if (temperatura >= 39) {
    febreAlta();
  }
}

// Função para atualizar o display OLED
void AtualizarDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print("BPM: ");
  display.print(frequenciaCardiaca, 1);
  display.display();
}

void imprimirHorarioLocal() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    display.setCursor(0, 40);
    display.println("Erro de Conexão");
    return;
  }

  // Ajuste do fuso horário
  timeinfo.tm_hour -= 3;
  if (timeinfo.tm_hour < 0) {
    timeinfo.tm_hour += 24;
  }

  // Exibição do horário no display OLED
  display.setCursor(0, 0);
  display.println(&timeinfo, "%H:%M");
  display.display();
}

bool verificarAcelerometro() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Exibição da aceleração no console serial
  Serial.println(a.acceleration.x);
  Serial.println(a.acceleration.y);
  Serial.println(a.acceleration.z);

  // Limite para detecção de queda
  float limite = 0.2;

  // Verifica se a aceleração está abaixo do limite
  if (abs(a.acceleration.x) < limite &&
      abs(a.acceleration.y) < limite &&
      abs(a.acceleration.z) < limite) {
    queda();
    return true;
  }

  return false;
}

void ativarBuzzer() {
  tone(PINO_BUZZER, 2200, 1000);
  digitalWrite(PINO_BUZZER, HIGH);
  delay(500);
  digitalWrite(PINO_BUZZER, LOW);
}

void taquicardia() {
  ativarBuzzer();
  display.clearDisplay(); 
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Emergencia");
  display.setCursor(0, 20);
  display.print("Acionada");
  display.display();
  delay(1300);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Risco de:");
  display.setCursor(0, 20);
  display.print("Taquicardia");
  display.display();
}

// Função para lidar com emergências de bradicardia
void bradicardia() {
  ativarBuzzer();
  display.clearDisplay(); 
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Emergencia");
  display.setCursor(0, 20);
  display.print("Acionada");
  display.display();
  delay(1300);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Risco de:");
  display.setCursor(0, 20);
  display.print("Bradicardia");
  display.display();
}

// Função para lidar com emergências de queda
void queda() {
  ativarBuzzer();
  display.clearDisplay(); 
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Emergencia");
  display.setCursor(0, 20);
  display.print("Acionada");
  display.display();
  delay(1300);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Queda");
  display.setCursor(0, 20);
  display.print("Detectada");
  display.display();

  String textoQueda = "Acionando Emergência, queda detectada.";

  char msgBuffer[100]; // Ajuste o tamanho conforme necessário

  // Supondo que uid seja um array de bytes, ajuste conforme necessário
  byte uid[4] = {0x01, 0x02, 0x03, 0x04};

  sprintf(msgBuffer, "%s %02X%02X%02X%02X", textoQueda.c_str(), uid[0], uid[1], uid[2], uid[3]);  
  MQTT.publish(TOPICO_PUBLISH_2, msgBuffer);
  MQTT.loop();

}

// Funções adicionais para tratar diferentes estados de temperatura
void tempOk() {
  display.setCursor(0, 20);
  display.print("Temp. OK");
  display.display();
}

void febril() {
  display.setCursor(0, 20);
  display.print("Febril");
  display.display();
}

void febre() {
  display.setCursor(0, 20);
  display.print("Febre");
  display.display();
}

void febreAlta() {
  display.setCursor(0, 20);
  display.print("Febre Alta");
  display.display();
}

void hipotermia() {
  display.setCursor(0, 20);
  display.print("Hipotermia");
  display.display();
}

//programa principal
void loop() 
{   

  //garante funcionamento das conexões WiFi e ao broker MQTT
  VerificaConexoesWiFIEMQTT();
 
  //envia o status de todos os outputs para o Broker no protocolo esperado
  EnviaEstadoOutputMQTT();

  FrequenciaCardiaca();
  verificarAcelerometro();
  delay(250);
    

  
}