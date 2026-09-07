# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="assets\logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>


## 👨‍🎓 Autor do Repositorio: 

- <a href="https://www.linkedin.com/in/pedro-carvalho-cea-149658137/">Pedro Carvalho Rocha Lima</a> 


## 📜 Descrição FASE 1

O CardioIA é um projeto acadêmico que integra tecnologia, Ciência de Dados e saúde para desenvolver uma plataforma digital inteligente que simula o ecossistema de uma cardiologia moderna.

Ao longo das diferentes fases do curso, o projeto utiliza tecnologias como Machine Learning, Visão Computacional, IoT e agentes inteligentes, aplicadas a dados e processos relacionados à cardiologia.

O objetivo é desenvolver, de forma progressiva, soluções para triagem, diagnóstico, monitoramento, assistência remota e previsões médicas

## PARTE 1

Durante a pesquisa encontrei algumas bases interessantes para integrar o projeto como <a href="https://physionet.org/content/ptb-xl/1.0.3/">PTB-XL no PhysioNet</a>  que gostaria de ter utilizado porem ao verificar os dados nela obtido constatei uma grande complexidade de se utilizar no primeiro momento, até mesmo pois não consegui nela encontrar outras variaveis que foram pedidos no exercicio como "colesteral". 

Resolvi então utilizar uma base mais amigavel que é a <a href="https://archive.ics.uci.edu/dataset/45/heart+disease/">PUCI Machine Learning Repository — Heart Disease</a>  dentro dessa base verifiquei que houve um problema na cleveland.data (originalmente do arquivo baixado) porem dei uma estudada nos dados disponiveis e consegui realizar uma limpeza, tradução e substituição de algumas informações para base ficar mais facil de manipular, para ser mais "rapido" fiz a maipulação da base no proprio excel e salvei em csv.


## PARTE 2

Referene aos textos, utilizei duas fontes encontradas em pesquisa

<a href="https://doi.org/10.5935/abc.20190204">Updated Cardiovascular Prevention Guideline of the Brazilian Society of Cardiology - 2019</a> 

<a href="https://www.scielo.br/j/abc/a/KyjkFNCJn68BRTphGtv9BfQ/?lang=pt">Índices Hematológicos Inflamatórios, Doenças Cardiovasculares e Mortalidade: Uma Revisão Narrativa</a> 



Explicando sobre o porque de um formato TXT no projeto NLP.
O formato .txt é uma boa opção para trabalhar com NLP porque contém apenas o texto, sem toda a formatação e informações extras de um PDF, facilitando o processamento.
Esse conteúdo pode ser usado para treinar ou aprimorar modelos de IA, ajudando-os a entender termos médicos, linguagem científica e a estrutura de artigos acadêmicos. Também pode ser utilizado em tarefas como resumo de textos, classificação de conteúdos e identificação de termos e informações importantes.
Outra aplicação importante no CardioIA é o uso em uma base RAG. Nesse caso, o artigo é dividido em pequenos trechos, transformado em representações numéricas (embeddings) e armazenado em uma base de busca. Assim, quando o usuário fizer uma pergunta, a IA consegue encontrar os trechos mais relevantes e utilizá-los para formular a resposta.

## PARTE 3

Referente as imagens pedidas, foi bem desafiador encontra-las até enquanto analisava e estudava sobre a <a href="https://physionet.org/content/ptb-xl/1.0.3/">PTB-XL no PhysioNet</a> vi que poderia utilizar a biblioteca <a href="https://pypi.org/project/wfdb/">wfdb</a> para realizar a leitura de pontos encontrados por sensores onde iria me retornar um grafico com os pontos medidos e assim me retornaria o ECG montado em um grafico, porem como na atividade pedia imagens em JPG ou PNG pesquisei e encontreia uma base publica no <a href="https://www.kaggle.com/datasets/analiviafr/ecg-images?resource=download">Kaggle</a> onde há uma quantidade interessante de imagens para estudo. 



## Link do Google Drive 

- <a href="https://drive.google.com/drive/folders/1-P5dJOhV7t-pcPovUToFCZklN1UrE3Tx?usp=drive_link/">Google Drive</a> 
 obs: arquivo se encontra zipado devido a grande quantidade de imagens. 

## Entregaveis da Fase 1

- Dados numéricos (simulados ou reais) relacionados a pacientes cardíacos

- Textos médicos ou literários relacionados à saúde cardiovascular

- Imagens médicas que representem exames ou sinais visuais do coração.



## 📁 Estrutura de pastas

Dentre os arquivos e pastas presentes na raiz do projeto, definem-se:

- <b>Assets</b>: imagens utilizadas para formatação do README.

- <b>Base_utilizada</b>: Aqui se encontra a base utilizada antes do tratamento.

- <b>dataset_cardiovascular_300_pacientes.csv</b>: Base em CSV já manipulada.









  