# CardioIA — Monitoramento Inteligente

Projeto da FIAP sobre IoT na saúde. Fizemos um protótipo de dispositivo vestível com ESP32 que mede temperatura e batimentos cardíacos (simulados), guarda as leituras quando a internet cai e depois envia tudo por MQTT para o HiveMQ Cloud. Os dados aparecem numa dashboard no Node-RED com gráficos e alertas.

> **Aviso:** é um protótipo acadêmico com dados simulados. Não é um dispositivo médico, não faz diagnóstico e não deve ser usado para decisões clínicas.

## Como funciona

```
DHT22 + botão → ESP32 (processa e guarda na fila) → MQTT/TLS → HiveMQ Cloud → Node-RED → Dashboard
```

1. O ESP32 lê o DHT22 a cada 2 segundos. A temperatura do DHT22 representa a temperatura corporal.
2. Cada toque no botão é um batimento. O BPM é calculado com os batimentos dos últimos 15 segundos × 4. Se ninguém tocar no botão, o código simula um BPM automaticamente.
3. O ESP32 verifica se os valores estão fora dos limites e acende o LED vermelho em caso de alerta.
4. Toda leitura entra numa fila (até 1000 leituras). Se a internet estiver funcionando, ela é enviada na hora; se não, fica guardada até a conexão voltar.
5. O Node-RED recebe os dados, ignora mensagens repetidas e mostra tudo na dashboard.

## Arquivos

```
CardioIA_simplificado/
├── README.md
├── esp32/
│   ├── src/main.cpp              código do ESP32
│   ├── include/config.example.h  modelo de configuração (sem senhas)
│   ├── include/config.h          configuração local (não vai para o GitHub)
│   ├── diagram.json              circuito do Wokwi
│   ├── platformio.ini
│   ├── wokwi.toml
│   └── libraries.txt             bibliotecas para o wokwi.com
├── node-red/flows.json           fluxo para importar no Node-RED
├── docs/
│   ├── relatorio_edge_computing.md
│   ├── relatorio_mqtt_dashboard.md
│   └── configuracao_hivemq.md
└── imagens/                      prints da execução
```

## Circuito

| Componente | Pino do ESP32 |
|---|---|
| DHT22 (SDA) | GPIO15 |
| Botão (batimento) | GPIO4 e GND |
| Chave deslizante (simula queda do Wi-Fi) | GPIO18 e GND |
| LED verde + resistor 220 Ω (conexão) | GPIO26 |
| LED vermelho + resistor 220 Ω (alerta) | GPIO27 |

## Como rodar

### ESP32 no Wokwi

**Pelo VS Code:**

1. Instale as extensões **PlatformIO IDE** e **Wokwi Simulator**.
2. Em **File → Open Folder**, abra a pasta **`esp32`** (não a pasta do projeto inteiro: o PlatformIO e o Wokwi procuram o `platformio.ini` e o `wokwi.toml` na raiz da pasta aberta).
3. Compile: clique no ✓ da barra inferior do PlatformIO ou rode `pio run` no terminal. O arquivo `.pio/build/esp32dev/firmware.bin` deve ser criado.
4. Na primeira vez, aperte `F1` → **Wokwi: Request a New License** e confirme no navegador (é gratuita).
5. Aperte `F1` → **Wokwi: Start Simulator** (ou abra o `diagram.json` e clique no ▶).

**Pelo site wokwi.com:**

1. Crie um projeto ESP32.
2. Cole o `diagram.json`.
3. Cole o `main.cpp` no `sketch.ino`.
4. Crie o arquivo `config.h` e o arquivo `libraries.txt` com o conteúdo das pastas do projeto.

Sem configurar o HiveMQ, o código roda em **modo simulação**: as mensagens aparecem no Monitor Serial com `[SIM-MQTT]`.

### HiveMQ Cloud

Siga o passo a passo em [docs/configuracao_hivemq.md](docs/configuracao_hivemq.md) e coloque o endereço, o usuário e a senha no `esp32/include/config.h`.

### Node-RED

1. Instale o Node-RED (`npm install -g node-red`) e rode `node-red`.
2. Em <http://localhost:1880>: Menu → Manage palette → instale `@flowfuse/node-red-dashboard`.
3. Menu → Import → selecione `node-red/flows.json`.
4. Abra o nó **HiveMQ Cloud** e coloque o endereço do cluster, a porta 8883, o TLS ligado, o usuário e a senha.
5. Clique em Deploy e abra <http://localhost:1880/dashboard>.

## Testando

| O que fazer | O que deve acontecer |
|---|---|
| Clicar no DHT22 e colocar 38,5 °C | Aparece `TEMP_ALTA` no Serial, o LED vermelho acende e a dashboard mostra o alerta |
| Digitar `bpm 130` no Serial | Alerta de frequência cardíaca alta (`bpm 0` volta ao normal) |
| Tocar no botão uma vez por segundo durante 15 s | BPM perto de 60 |
| Mover a chave para o outro lado (ou digitar `offline`) | `[WIFI] Conexão perdida.` e as leituras são guardadas: `[EDGE] N registros pendentes.` |
| Voltar a chave (ou digitar `online`) | `[WIFI] Reconexão realizada.`, `[MQTT] Sincronizando registros...` e `[MQTT] Sincronização concluída.` |
| Digitar `status` | Mostra Wi-Fi, MQTT e o tamanho da fila |

Se a simulação começar dizendo que a rede está desligada, mova a chave para o outro lado.

### Checklist

- [ ] Simulação inicia no Wokwi
- [ ] Leituras do DHT22 e BPM aparecem no Serial
- [ ] Alertas funcionam (LED e Serial)
- [ ] Sem Wi-Fi, as leituras ficam na fila
- [ ] Quando a conexão volta, a fila é enviada
- [ ] Conectou no HiveMQ (mensagens visíveis no Web Client)
- [ ] Node-RED recebe os dados
- [ ] Gráficos, gauges e alertas atualizam na dashboard

## Bibliotecas

- `MQTT` (Joel Gaehwiler / 256dpi): escolhemos essa porque ela confirma o envio com QoS 1
- `DHT sensor library for ESPx`
- Node-RED Dashboard 2.0 (`@flowfuse/node-red-dashboard`)
