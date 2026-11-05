# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href="https://www.fiap.com.br/"><img src="assets/logo-fiap.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="40%" height="40%"></a>
</p>

<br>

## 👨‍🎓 Autor do Repositório:

- <a href="https://www.linkedin.com/in/pedro-carvalho-cea-149658137/">Pedro Carvalho Rocha Lima</a>

# CardioIA — Monitoramento Inteligente

Nesta fase do projeto CardioIA, a ideia foi desenvolver um protótipo de monitoramento de sinais vitais utilizando IoT, Edge Computing e computação em nuvem.

Utilizamos um ESP32 para simular um dispositivo vestível que coleta informações de temperatura e frequência cardíaca. O sistema consegue processar os dados localmente, identificar possíveis alertas e armazenar as leituras quando a conexão com a internet é interrompida.

Depois que a conexão retorna, as informações são enviadas por MQTT para o HiveMQ Cloud e apresentadas em uma dashboard desenvolvida no Node-RED.

> **Aviso:** Este projeto é um protótipo acadêmico com dados simulados. Não é um dispositivo médico, não realiza diagnósticos e não deve ser utilizado para decisões clínicas.

## O que o projeto faz

O projeto foi dividido em duas partes principais:

1. **Edge Computing:** o ESP32 realiza a leitura dos sensores, calcula a frequência cardíaca, verifica os limites definidos e armazena as informações quando está sem internet.

2. **MQTT e Dashboard:** os dados são enviados para o HiveMQ Cloud por meio do protocolo MQTT. Depois, o Node-RED recebe as informações e apresenta os resultados em gráficos, indicadores e alertas.

O funcionamento geral acontece da seguinte maneira:

ESP32 (sensores) → Processamento local → Fila de armazenamento → MQTT → HiveMQ Cloud → Node-RED → Dashboard.

## 📁 Estrutura de pastas

O projeto foi organizado da seguinte forma:

```text
CardioIA_simplificado/
│
├── README.md
│
├── esp32/
│   ├── src/
│   │   └── main.cpp
│   ├── include/
│   │   ├── config.example.h
│   │   └── config.h
│   ├── diagram.json
│   ├── platformio.ini
│   ├── wokwi.toml
│   └── libraries.txt
│
├── node-red/
│   └── flows.json
│
├── docs/
│   ├── relatorio_edge_computing.md
│   ├── relatorio_mqtt_dashboard.md
│   └── configuracao_hivemq.md
│
└── imagens/
    └── prints da execução
```

- **esp32:** contém os arquivos responsáveis pela programação e simulação do dispositivo.
- **node-red:** contém o fluxo utilizado para receber e processar as mensagens MQTT.
- **docs:** contém os relatórios e as instruções de configuração do projeto.
- **imagens:** espaço destinado aos registros e prints dos testes realizados.

## Parte 1 — Edge Computing

Nesta parte, utilizamos o ESP32 para realizar o processamento das informações diretamente no dispositivo.

Os componentes utilizados foram:

- ESP32 DevKit V1;
- Sensor DHT22;
- Botão para simular os batimentos cardíacos;
- Chave deslizante para simular a queda da internet;
- LED verde para indicar a conexão;
- LED vermelho para indicar alertas.

O sensor DHT22 realiza leituras a cada 2 segundos. Para a frequência cardíaca, utilizamos um botão em que cada toque representa um batimento. Também criamos uma simulação automática de BPM para facilitar os testes.

### Funcionamento do processamento

O ESP32 compara os valores recebidos com os limites definidos no código:

| Condição | Alerta |
|---|---|
| BPM acima de 120 | FC_ALTA |
| BPM abaixo de 50 | FC_BAIXA |
| Temperatura acima de 38 °C | TEMP_ALTA |
| Temperatura abaixo de 35 °C | TEMP_BAIXA |

Quando algum valor ultrapassa esses limites, o LED vermelho acende e o sistema registra o alerta.

### Armazenamento offline

Um dos pontos importantes foi permitir que o dispositivo continuasse funcionando mesmo sem internet.

Para isso, criamos uma fila circular na memória RAM do ESP32, com capacidade para até 1.000 leituras.

Quando a conexão cai, as informações continuam sendo coletadas e armazenadas. Assim que a internet retorna, o dispositivo começa a enviar os registros pendentes, mantendo a ordem das leituras.

Escolhemos essa estratégia para evitar a perda de informações durante possíveis falhas de conexão.

## Parte 2 — MQTT e HiveMQ Cloud

Depois de desenvolver o processamento local, implementamos a comunicação entre o ESP32 e a nuvem utilizando o protocolo MQTT.

Criamos uma conta no HiveMQ Cloud e configuramos um broker para receber as mensagens enviadas pelo dispositivo.

A comunicação funciona por meio de três tópicos principais:

- `cardioia/pacientes/paciente001/sinais`: recebe todas as leituras.
- `cardioia/pacientes/paciente001/alertas`: recebe os registros com alertas.
- `cardioia/pacientes/paciente001/status`: informa se o dispositivo está online ou offline.

As informações são transmitidas no formato JSON, contendo dados como temperatura, umidade, frequência cardíaca, horário e identificação de cada leitura.

Também utilizamos o **QoS 1**, que permite confirmar o recebimento das mensagens pelo broker. Dessa forma, o ESP32 só remove uma leitura da fila depois de receber a confirmação.

Outro recurso implementado foi a identificação de mensagens duplicadas, evitando que uma mesma leitura seja contabilizada mais de uma vez no Node-RED.

A comunicação utiliza TLS na porta 8883 e autenticação por usuário e senha.

## Parte 3 — Dashboard no Node-RED

Na última parte, desenvolvemos uma dashboard utilizando o Node-RED Dashboard 2.0.

A ideia foi criar uma interface simples para acompanhar os dados recebidos pelo ESP32.

A dashboard foi dividida em quatro grupos:

1. **Paciente e conexão:** apresenta a identificação do paciente e o estado da conexão Wi-Fi e MQTT.
2. **Sinais vitais:** mostra a frequência cardíaca, temperatura e umidade.
3. **Alertas:** apresenta as situações identificadas e a quantidade de registros recebidos.
4. **Histórico:** exibe gráficos com as alterações dos sinais vitais ao longo do tempo.

Também implementamos notificações automáticas. Quando algum valor ultrapassa os limites configurados, a dashboard apresenta uma mensagem de alerta e destaca a situação em vermelho.

O Node-RED também realiza a validação das mensagens recebidas e identifica registros repetidos antes de atualizar os indicadores.

## Circuito

A ligação dos componentes no ESP32 foi organizada da seguinte maneira:

| Componente | Pino do ESP32 |
|---|---|
| DHT22 | GPIO15 |
| Botão (batimento) | GPIO4 e GND |
| Chave deslizante | GPIO18 e GND |
| LED verde | GPIO26 |
| LED vermelho | GPIO27 |

Os LEDs utilizam resistores de 220 Ω para proteção.

## Como rodar

### ESP32 no Wokwi

**Pelo VS Code:**

1. Instale as extensões PlatformIO IDE e Wokwi Simulator.
2. Abra a pasta `esp32` no VS Code.
3. Compile o projeto pelo PlatformIO ou execute `pio run` no terminal.
4. Solicite uma licença gratuita do Wokwi pelo comando `F1` → Wokwi: Request a New License.
5. Inicie a simulação pelo comando `F1` → Wokwi: Start Simulator.

**Pelo site Wokwi:**

1. Crie um projeto ESP32.
2. Utilize o arquivo `diagram.json` para montar o circuito.
3. Copie o código do `main.cpp` para o `sketch.ino`.
4. Adicione os arquivos de configuração e as bibliotecas necessárias.

Sem configurar o HiveMQ, o programa funciona em modo de simulação e apresenta as mensagens no Monitor Serial.

### HiveMQ Cloud

Para configurar a comunicação:

1. Crie uma conta no HiveMQ Cloud.
2. Crie um cluster gratuito.
3. Copie o endereço do broker.
4. Configure o usuário e a senha de acesso.
5. Preencha as informações no arquivo `esp32/include/config.h`.

O passo a passo completo está disponível em `docs/configuracao_hivemq.md`.

### Node-RED

1. Instale o Node-RED utilizando `npm install -g node-red`.
2. Inicie o programa com o comando `node-red`.
3. Acesse `http://localhost:1880`.
4. Instale o pacote `@flowfuse/node-red-dashboard`.
5. Importe o arquivo `node-red/flows.json`.
6. Configure o endereço do HiveMQ, a porta 8883, TLS e as credenciais.
7. Clique em Deploy.
8. Acesse `http://localhost:1880/dashboard`.

## Testes realizados

Durante o desenvolvimento, realizamos alguns testes para verificar o funcionamento do sistema:

| Teste | Resultado esperado |
|---|---|
| Alterar a temperatura do DHT22 para 38,5 °C | Identificar TEMP_ALTA e acender o LED vermelho |
| Digitar `bpm 130` no Monitor Serial | Identificar frequência cardíaca alta |
| Digitar `bpm 0` | Retornar à simulação automática |
| Pressionar o botão continuamente | Calcular o BPM pelos batimentos registrados |
| Desligar a conexão Wi-Fi | Continuar coletando e armazenando as leituras |
| Restaurar a conexão | Enviar os registros pendentes |
| Digitar `status` | Mostrar o estado da conexão e o tamanho da fila |

Esses testes ajudaram a verificar se o dispositivo continuava funcionando durante a ausência de internet e se os dados eram enviados corretamente após a reconexão.

## Limitações e melhorias futuras

Durante o desenvolvimento, identificamos alguns pontos que podem ser melhorados:

- Utilizar SPIFFS para manter os registros mesmo após reiniciar o ESP32.
- Implementar uma confirmação do Node-RED após o processamento das mensagens.
- Utilizar o horário original da coleta nos gráficos.
- Integrar um banco de dados para armazenar o histórico.
- Implementar login e controle de acesso na dashboard.
- Utilizar sensores reais para medir os sinais vitais.

## Bibliotecas e ferramentas utilizadas

- **PlatformIO:** compilação e gerenciamento do projeto ESP32.
- **Wokwi:** simulação do circuito eletrônico.
- **MQTT (256dpi / Joel Gaehwiler):** comunicação com o broker utilizando QoS 1.
- **DHT Sensor Library for ESPx:** leitura do sensor DHT22.
- **HiveMQ Cloud:** recebimento e distribuição das mensagens MQTT.
- **Node-RED:** processamento e integração dos dados.
- **Node-RED Dashboard 2.0:** criação da interface de monitoramento.

## Conclusão

Com o desenvolvimento dessa etapa, conseguimos entender melhor como funciona a integração entre dispositivos IoT, processamento local e serviços em nuvem.

O principal aprendizado foi perceber a importância do Edge Computing, pois o ESP32 consegue continuar coletando, processando e armazenando informações mesmo quando não existe conexão com a internet.

Também aprendemos a utilizar o MQTT para transmitir os dados e o Node-RED para organizar as informações em uma dashboard.

De maneira geral, conseguimos desenvolver um protótipo funcional que atende aos objetivos propostos pela atividade e demonstra como essas tecnologias podem ser utilizadas em soluções de monitoramento na área da saúde.

## Entregáveis da fase

- Código-fonte do ESP32.
- Circuito desenvolvido e testado no Wokwi.
- Configuração do HiveMQ Cloud.
- Fluxo de integração do Node-RED.
- Dashboard de monitoramento com gráficos e alertas.
- Relatórios sobre Edge Computing e MQTT.
- Prints demonstrando o funcionamento do projeto.