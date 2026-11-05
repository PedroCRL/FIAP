
// CardioIA - Monitoramento de sinais vitais com ESP32
// Sensores:
//   DHT22  (GPIO15) -> temperatura e umidade
//   Botão  (GPIO4)  -> cada toque é um batimento cardíaco
//   Chave  (GPIO18) -> simula queda do Wi-Fi
//   LED verde (GPIO26) -> conexão | LED vermelho (GPIO27) -> alerta

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <DHTesp.h>
#include <time.h>
#include "config.h"

#define PINO_DHT 15
#define PINO_BOTAO 4
#define PINO_CHAVE 18
#define PINO_LED_VERDE 26
#define PINO_LED_VERMELHO 27

#define TAMANHO_FILA 1000      // quantas leituras cabem na fila offline
#define INTERVALO_LEITURA 2000 // lê os sensores a cada 2 segundos
#define JANELA_BPM 15000       // janela de 15 s para calcular o BPM

// Certificado raiz usado pelo HiveMQ Cloud (Let's Encrypt - ISRG Root X1)
// Fonte: https://letsencrypt.org/certs/isrgrootx1.pem
const char *certificadoHiveMQ = R"(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)";

DHTesp dht;
WiFiClientSecure clienteSeguro;
MQTTClient mqtt(1024); // buffer de 1024 bytes para caber o JSON

// ---------------- Fila circular para guardar as leituras ----------------
struct Leitura
{
  unsigned long sequencia; // número da leitura
  unsigned long horario;   // horário em segundos (0 = sem NTP ainda)
  float temperatura;
  float umidade;
  int bpm;
  bool coletadaOffline; // true se foi coletada sem conexão
};

Leitura fila[TAMANHO_FILA];
int inicioFila = 0;     // posição da leitura mais antiga
int quantidadeFila = 0; // quantas leituras estão esperando envio
unsigned long descartadas = 0;

// ---------------- Variáveis gerais ----------------
unsigned long sequencia = 0;
long idBoot = 0; // número aleatório que muda a cada vez que o ESP32 liga
unsigned long ultimaLeitura = 0;
bool sincronizando = false;
bool comandoOffline = false; // comando "offline" digitado no Serial
bool wifiEstavaConectado = false;
bool jaConectouAntes = false;
bool ntpIniciado = false;

// ---------------- Batimentos cardíacos ----------------
volatile unsigned long batimentos[60]; // horário dos últimos 60 toques
volatile int posicaoBatimento = 0;
volatile unsigned long ultimoToque = 0;
bool modoManual = false;
int bpmSimulado = 75;
int bpmForcado = 0; // usado para testar alertas (comando "bpm 130")

// Interrupção do botão: guarda o horário de cada toque
void IRAM_ATTR botaoPressionado()
{
  unsigned long agora = millis();
  if (agora - ultimoToque > 150)
  { // ignora repique do botão (debounce)
    batimentos[posicaoBatimento] = agora;
    posicaoBatimento = (posicaoBatimento + 1) % 60;
    ultimoToque = agora;
  }
}

// Calcula o BPM: batimentos nos últimos 15 segundos x 4
// Se ninguém apertou o botão nos últimos 15 s, usa a simulação automática
int calcularBpm()
{
  unsigned long agora = millis();

  if (ultimoToque > 0 && agora - ultimoToque < JANELA_BPM)
  {
    modoManual = true;
    int contagem = 0;
    for (int i = 0; i < 60; i++)
    {
      if (batimentos[i] > 0 && agora - batimentos[i] < JANELA_BPM)
        contagem++;
    }
    return contagem * 4;
  }

  modoManual = false;
  if (bpmForcado > 0)
    return bpmForcado + random(-2, 3);
  bpmSimulado = constrain(bpmSimulado + random(-3, 4), 62, 90); // variação natural
  return bpmSimulado;
}

// ---------------- Fila ----------------
void adicionarNaFila(Leitura l)
{
  if (quantidadeFila == TAMANHO_FILA)
  {
    // fila cheia: descarta a leitura mais antiga para guardar a nova
    inicioFila = (inicioFila + 1) % TAMANHO_FILA;
    quantidadeFila--;
    descartadas++;
    Serial.println("[EDGE] Fila cheia! A leitura mais antiga foi descartada.");
  }
  int fim = (inicioFila + quantidadeFila) % TAMANHO_FILA;
  fila[fim] = l;
  quantidadeFila++;
}

void removerDaFila()
{
  inicioFila = (inicioFila + 1) % TAMANHO_FILA;
  quantidadeFila--;
}

// ---------------- Alertas ----------------
String verificarAlerta(float temperatura, int bpm)
{
  String alerta = "";
  if (bpm > LIMITE_BPM_ALTO)
    alerta += "FC_ALTA|";
  if (bpm < LIMITE_BPM_BAIXO)
    alerta += "FC_BAIXA|";
  if (temperatura > LIMITE_TEMP_ALTA)
    alerta += "TEMP_ALTA|";
  if (temperatura < LIMITE_TEMP_BAIXA)
    alerta += "TEMP_BAIXA|";

  if (alerta == "")
    return "NORMAL";
  return alerta.substring(0, alerta.length() - 1); // tira o último "|"
}

// ---------------- Conexões ----------------
bool modoSimulacao()
{
  return String(MQTT_SERVIDOR).indexOf("SEU-CLUSTER") >= 0;
}

bool redeDesligadaParaTeste()
{
  return comandoOffline || digitalRead(PINO_CHAVE) == LOW;
}

bool estaConectado()
{
  if (WiFi.status() != WL_CONNECTED)
    return false;
  if (modoSimulacao())
    return true;
  return mqtt.connected();
}

void verificarWifi()
{
  static unsigned long ultimaTentativa = 0;

  if (redeDesligadaParaTeste())
  {
    if (WiFi.status() == WL_CONNECTED)
      WiFi.disconnect();
    if (wifiEstavaConectado)
    {
      Serial.println("[WIFI] Conexão perdida.");
      mqtt.disconnect();
      wifiEstavaConectado = false;
    }
    return;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    if (!wifiEstavaConectado)
    {
      wifiEstavaConectado = true;
      if (jaConectouAntes)
        Serial.println("[WIFI] Reconexão realizada.");
      else
        Serial.println("[WIFI] Conectado ao Wi-Fi.");
      jaConectouAntes = true;
      if (!ntpIniciado)
      {
        configTime(0, 0, "pool.ntp.org"); // horário UTC pela internet
        ntpIniciado = true;
      }
    }
    return;
  }

  if (wifiEstavaConectado)
  {
    Serial.println("[WIFI] Conexão perdida.");
    mqtt.disconnect();
    wifiEstavaConectado = false;
  }

  // tenta conectar de novo a cada 10 segundos
  if (ultimaTentativa == 0 || millis() - ultimaTentativa > 10000)
  {
    Serial.println("[WIFI] Conectando...");
    WiFi.begin(WIFI_SSID, WIFI_SENHA, 6);
    ultimaTentativa = millis();
  }
}

void verificarMqtt()
{
  static unsigned long ultimaTentativa = 0;
  if (modoSimulacao() || WiFi.status() != WL_CONNECTED)
    return;

  if (mqtt.connected())
  {
    mqtt.loop();
    return;
  }

  // tenta conectar ao broker a cada 5 segundos
  if (ultimaTentativa != 0 && millis() - ultimaTentativa < 5000)
    return;
  ultimaTentativa = millis();

  Serial.println("[MQTT] Conectando ao HiveMQ Cloud...");
  String idCliente = String(DEVICE_ID) + "-" + String(idBoot);
  if (mqtt.connect(idCliente.c_str(), MQTT_USUARIO, MQTT_SENHA))
  {
    Serial.println("[MQTT] Conectado!");
    mqtt.publish(TOPICO_STATUS, "{\"device_id\":\"" DEVICE_ID "\",\"status\":\"online\"}", true, 1);
  }
  else
  {
    Serial.print("[MQTT] Falha na conexão. Erro: ");
    Serial.println(mqtt.lastError());
  }
}

// ---------------- JSON ----------------
String formatarHorario(unsigned long segundos)
{
  time_t t = segundos;
  struct tm data;
  gmtime_r(&t, &data);
  char texto[25];
  strftime(texto, sizeof(texto), "%Y-%m-%dT%H:%M:%SZ", &data);
  return String(texto);
}

String montarJson(Leitura l)
{
  String alerta = verificarAlerta(l.temperatura, l.bpm);
  String json = "{";
  json += "\"device_id\":\"" + String(DEVICE_ID) + "\",";
  json += "\"patient_id\":\"" + String(PATIENT_ID) + "\",";
  json += "\"reading_id\":\"" + String(idBoot) + "-" + String(l.sequencia) + "\",";
  json += "\"sequence\":" + String(l.sequencia) + ",";
  if (l.horario > 0)
    json += "\"timestamp\":\"" + formatarHorario(l.horario) + "\",";
  else
    json += "\"timestamp\":null,";
  json += "\"temperature\":" + String(l.temperatura, 1) + ",";
  json += "\"humidity\":" + String(l.umidade, 1) + ",";
  json += "\"heart_rate\":" + String(l.bpm) + ",";
  json += "\"wifi_connected\":" + String(l.coletadaOffline ? "false" : "true") + ",";
  json += "\"mqtt_connected\":" + String(l.coletadaOffline ? "false" : "true") + ",";
  json += "\"offline\":" + String(l.coletadaOffline ? "true" : "false") + ",";
  json += "\"alert\":" + String(alerta == "NORMAL" ? "false" : "true") + ",";
  json += "\"alert_type\":\"" + alerta + "\"";
  json += "}";
  return json;
}

// ---------------- Coleta e envio ----------------
void coletarLeitura()
{
  Serial.println("[EDGE] Coletando sinais vitais...");

  TempAndHumidity dados = dht.getTempAndHumidity();
  if (isnan(dados.temperature) || isnan(dados.humidity))
  {
    Serial.println("[EDGE] Erro ao ler o DHT22, leitura ignorada.");
    return;
  }

  Leitura l;
  sequencia++;
  l.sequencia = sequencia;
  time_t agora = time(nullptr);
  l.horario = (agora > 1700000000) ? agora : 0; // só usa o horário se o NTP já sincronizou
  l.temperatura = dados.temperature;
  l.umidade = dados.humidity;
  l.bpm = calcularBpm();
  l.coletadaOffline = !estaConectado();

  String alerta = verificarAlerta(l.temperatura, l.bpm);
  Serial.printf("  #%lu  Temp: %.1f C | Umid: %.1f %% | BPM: %d (%s) | %s\n",
                l.sequencia, l.temperatura, l.umidade, l.bpm,
                modoManual ? "botao" : "simulado", alerta.c_str());
  if (alerta != "NORMAL")
    Serial.println("[ALERTA] " + alerta);
  digitalWrite(PINO_LED_VERMELHO, alerta != "NORMAL");

  // toda leitura entra na fila; o envio é feito depois em enviarFila()
  adicionarNaFila(l);

  if (!estaConectado())
  {
    Serial.println("[EDGE] Armazenando leitura localmente.");
    Serial.printf("[EDGE] %d registros pendentes.\n", quantidadeFila);
  }
}

void enviarFila()
{
  if (!estaConectado() || quantidadeFila == 0)
    return;

  if (quantidadeFila > 1 && !sincronizando)
  {
    sincronizando = true;
    Serial.printf("[MQTT] Sincronizando registros (%d pendentes)...\n", quantidadeFila);
  }

  // envia no máximo 5 por vez para não travar o loop
  int enviadas = 0;
  while (quantidadeFila > 0 && enviadas < 5)
  {
    Leitura l = fila[inicioFila]; // a mais antiga primeiro
    String json = montarJson(l);
    bool ok;

    if (modoSimulacao())
    {
      Serial.println("[SIM-MQTT] " + json);
      ok = true;
    }
    else
    {
      // QoS 1: publish só retorna true quando o broker confirma (PUBACK)
      ok = mqtt.publish(TOPICO_SINAIS, json.c_str(), false, 1);
    }

    if (!ok)
    {
      Serial.println("[MQTT] Falha no envio, tento de novo depois.");
      break;
    }

    String alerta = verificarAlerta(l.temperatura, l.bpm);
    if (alerta != "NORMAL" && !modoSimulacao())
    {
      String msgAlerta = "{\"reading_id\":\"" + String(idBoot) + "-" + String(l.sequencia) +
                         "\",\"alert_type\":\"" + alerta + "\"}";
      mqtt.publish(TOPICO_ALERTAS, msgAlerta.c_str(), false, 1);
    }

    removerDaFila(); // só remove depois que o envio foi confirmado
    enviadas++;
  }

  if (sincronizando && quantidadeFila == 0)
  {
    sincronizando = false;
    Serial.println("[MQTT] Sincronização concluída.");
  }
}

// ---------------- Comandos pelo Monitor Serial ----------------
void lerComandos()
{
  if (!Serial.available())
    return;
  String comando = Serial.readStringUntil('\n');
  comando.trim();

  if (comando == "offline")
  {
    comandoOffline = true;
    Serial.println("[TESTE] Rede desligada pelo comando.");
  }
  else if (comando == "online")
  {
    comandoOffline = false;
    Serial.println("[TESTE] Rede liberada.");
  }
  else if (comando.startsWith("bpm "))
  {
    bpmForcado = comando.substring(4).toInt();
    Serial.printf("[TESTE] BPM simulado: %d (0 = normal)\n", bpmForcado);
  }
  else if (comando == "status")
  {
    Serial.printf("Wi-Fi: %s | MQTT: %s | Fila: %d/%d | Descartadas: %lu\n",
                  WiFi.status() == WL_CONNECTED ? "conectado" : "desconectado",
                  estaConectado() ? "ok" : "desconectado",
                  quantidadeFila, TAMANHO_FILA, descartadas);
  }
  else
  {
    Serial.println("Comandos: offline | online | bpm <valor> | status");
  }
}

// ---------------- setup e loop ----------------
void setup()
{
  Serial.begin(115200);
  Serial.println("\n=== CardioIA - Monitoramento Inteligente ===");
  Serial.println("Projeto acadêmico com dados simulados.");

  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_CHAVE, INPUT_PULLUP);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PINO_BOTAO), botaoPressionado, FALLING);

  dht.setup(PINO_DHT, DHTesp::DHT22);
  idBoot = random(100000, 999999);

  Serial.printf("Fila offline: %d leituras (%d bytes)\n", TAMANHO_FILA, (int)sizeof(fila));
  if (modoSimulacao())
  {
    Serial.println("[MQTT] MODO SIMULAÇÃO: configure o HiveMQ no config.h.");
  }
  Serial.println("Comandos: offline | online | bpm <valor> | status");

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(false); // a reconexão é feita por verificarWifi()

  clienteSeguro.setCACert(certificadoHiveMQ);
  mqtt.begin(MQTT_SERVIDOR, MQTT_PORTA, clienteSeguro);
  mqtt.setOptions(30, true, 3000); // keep-alive 30 s, sessão limpa, timeout 3 s
  // Last Will: se o ESP32 cair, o broker avisa que ele ficou offline
  mqtt.setWill(TOPICO_STATUS, "{\"device_id\":\"" DEVICE_ID "\",\"status\":\"offline\"}", true, 1);
}

void loop()
{
  lerComandos();
  verificarWifi();
  verificarMqtt();

  if (millis() - ultimaLeitura >= INTERVALO_LEITURA)
  {
    ultimaLeitura = millis();
    coletarLeitura();
  }

  enviarFila();

  // LED verde: aceso = conectado | piscando = offline guardando na fila
  if (estaConectado())
    digitalWrite(PINO_LED_VERDE, HIGH);
  else
    digitalWrite(PINO_LED_VERDE, (millis() / 500) % 2);

  delay(10);
}
