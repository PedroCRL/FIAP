// CardioIA - arquivo de configuração (MODELO)
// Copie este arquivo para "config.h" e coloque os dados do seu HiveMQ.
// O config.h não deve ir para o GitHub (está no .gitignore).

#pragma once

// Identificação (dados fictícios)
#define DEVICE_ID  "ESP32_CARDIOIA_001"
#define PATIENT_ID "PACIENTE_001"

// Wi-Fi do Wokwi
#define WIFI_SSID  "Wokwi-GUEST"
#define WIFI_SENHA ""

// HiveMQ Cloud
// Enquanto MQTT_SERVIDOR tiver "SEU-CLUSTER", o código roda em modo simulação
// e mostra as mensagens só no Monitor Serial.
#define MQTT_SERVIDOR "SEU-CLUSTER.s1.eu.hivemq.cloud"
#define MQTT_PORTA    8883
#define MQTT_USUARIO  "seu_usuario"
#define MQTT_SENHA    "sua_senha"

// Tópicos
#define TOPICO_SINAIS  "cardioia/pacientes/paciente001/sinais"
#define TOPICO_ALERTAS "cardioia/pacientes/paciente001/alertas"
#define TOPICO_STATUS  "cardioia/pacientes/paciente001/status"

// Limites dos alertas (valores da atividade, não são critérios médicos)
#define LIMITE_BPM_ALTO   120
#define LIMITE_BPM_BAIXO  50
#define LIMITE_TEMP_ALTA  38.0
#define LIMITE_TEMP_BAIXA 35.0
