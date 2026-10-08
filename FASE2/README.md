# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="assets\logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>


## 👨‍🎓 Autor do Repositorio: 

- <a href="https://www.linkedin.com/in/pedro-carvalho-cea-149658137/">Pedro Carvalho Rocha Lima</a> 



# CardioIA — Fase 2

Nesse Fase A ideia é usar IA e dados para criar  um sistema que consiga entender os sintomas de um paciente, relacioná-los a possíveis doenças e indicar seu nível de risco. Os relatos e o dataset usados aqui são **simulados e criados por inteligencia artificial(chatGPT)** — o projeto não é uma ferramenta médica.

## O que o projeto faz

Na Fase 2 conforme proposto pelo exercicio separei em  duas partes:

1. **Extração de sintomas**: lê relatos em texto, procura sintomas conhecidos usando o mapa de conhecimento e sugere possíveis condições associadas, com um ranking simples por pontos.

2. **Classificador de risco**: treina um modelo simples (TF-IDF + Logistic Regression) para classificar as frases como "baixo risco", "médio risco" ou "alto risco".

## 📁 Estrutura de pastas

- <b>dados</b>: Todos dados utilizados para o projeto, conta com os seguintes arquivos dentro dessa pasta: sintomas.txt ,mapa_conhecimento.csv ,dataset_risco.csv.

- <b>Interface </b>: Ir Alem 1 (desafio).

- <b>extracao_sintomas.py </b>: Arquivo parte 1.

- <b>classificacao_risco.py</b>: Arquivo parte 2.

- <b>executar_tudo.py </b>: Arquivo parte que roda o programa por completo, porem se entrada de dados.

## Parte 1 

- Arquivo .txt -> gerei um arquivo com 10 frases utilizado o chat GPT para para que servisse de base para leitura de relatos do modelo

- Pedi tambem para o chat GPT realizar a criação de um arquivo .csv com o mapa de conhecimento para que pudesse ser a base de conhecimento do modelo

Funcionamento do fluxo do programa :

relato do paciente(entrada)  -> normaliza (minúsculo, sem acento, só pra facilitar a busca)  -> procura os sintomas/expressões do mapa de conhecimento no texto  -> soma os pesos das evidências encontradas -> mostra o ranking das condições mais pontuadas


Um detalhe: o mapa de conhecimento tem várias linhas repetindo o mesmo sintoma e a mesma condição com expressões diferentes (ex: "falta de ar" aparece em várias linhas ligado a Insuficiência Cardíaca). Se eu simplesmente somasse o peso de toda linha que "bateu" no texto, o ranking ficaria inflado só por causa de uma palavra genérica repetida. Por isso o código guarda só a melhor evidência para cada par (sintoma, condição) — prioriza a expressão mais específica encontrada e se empatar usa o maior peso

## Parte 2 


frase -> divide em treino/teste (75/25) -> TF-IDF -> Logistic Regression -> classifica em "baixo risco", "médio risco" ou "alto risco"  -> mostra accuracy, precision, recall, f1 e matriz de confusão

## video

https://youtu.be/qvraz-A3aHU

## Como rodar

```bash
cd FASE2
python -m venv .venv
.venv\Scripts\activate        # no Windows
pip install -r requirements.txt
```

Rodar tudo de uma vez:
```bash
python executar_tudo.py
```

Ou separado:
```bash
python extracao_sintomas.py
python classificacao_risco.py
```

O script da Parte 2 salva o gráfico da matriz de confusão em `matriz_confusao.png`, na raiz do projeto.

Depois de treinar e testar o modelo, no arquivo sendo rodado sozinho `classificacao_risco.py` deixa você digitar suas próprias frases no terminal para ver a classificação na hora (aperte Enter vazio para sair desse modo).

## Sobre os pesos do mapa de conhecimento

Usei uma escala de 1 a 3, definida só pra dar ordem de prioridade dentro do projeto:

- 3 = evidência mais forte;

- 2 = evidência moderada;

- 1 = evidência mais fraca/genérica.

Não é uma probabilidade e não tem validação médica é só uma forma simples de ranquear hipóteses no exercício. 


## Entregaveis da Fase 2

- Arquivo .txt com 10 frases completas simulando descrições de sintomas relatados por pacientes.

- Planilha ou arquivo .csv com o mapa de conhecimento (associação entre sintomas e possíveis diagnósticos).

- Código Python (.ipynb ou .py) que faz a leitura do arquivo de frases, identifica os sintomas e sugere diagnósticos com base na ontologia.

- Arquivo .csv com frases e rótulos.

- Código .py com TF-IDF, classificação e avaliação do modelo.

- Repositório público no GitHub contendo todos os arquivos do projeto da Fase 2.

- Um vídeo de até 4 minutos demonstrando o funcionamento completo da solução (pode ser com gravação de tela e explicação por voz ou legenda). O vídeo deve ser postado no YouTube como "não listado" e o link deve ser incluído no README do repositório no GitHub.
