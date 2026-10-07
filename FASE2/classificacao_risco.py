"""
CardioIA - Fase 2 - Parte 2
Classificador de risco (baixo risco / médio risco / alto risco) usando
TF-IDF + Logistic Regression.

Trabalho acadêmico com dados simulados. O resultado do modelo não é um
diagnóstico nem deve ser usado para decisão clínica.
"""

from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix, ConfusionMatrixDisplay
from sklearn.model_selection import train_test_split

PASTA_ATUAL = Path(__file__).resolve().parent
DATASET_PATH = PASTA_ATUAL / "dados" / "dataset_risco.csv"


def carregar_dados():
    df = pd.read_csv(DATASET_PATH, encoding="utf-8")

    if df.empty:
        raise ValueError("O dataset está vazio.")

    if not {"frase", "situacao"}.issubset(df.columns):
        raise ValueError("O dataset precisa ter as colunas 'frase' e 'situacao'.")

    return df


def treinar_modelo(df):
    X = df["frase"]
    y = df["situacao"]

    # separa treino e teste, mantendo a mesma proporção de baixo/alto risco
    # nos dois conjuntos (stratify) e com random_state fixo pra dar sempre o
    # mesmo resultado quando rodar de novo
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.25, random_state=42, stratify=y)

    # TF-IDF transforma cada frase em um vetor de números, dando mais peso
    # para palavras que aparecem pouco no geral (e por isso são mais
    # "informativas"). ngram_range=(1,2) usa palavras sozinhas e também
    # pares de palavras seguidas, tipo "dor no" e "no peito"
    vectorizer = TfidfVectorizer(lowercase=True, ngram_range=(1, 2))

    X_train_tfidf = vectorizer.fit_transform(X_train)
    X_test_tfidf = vectorizer.transform(X_test)

    modelo = LogisticRegression(random_state=42, max_iter=1000)
    modelo.fit(X_train_tfidf, y_train)

    return modelo, vectorizer, X_test, y_test, X_test_tfidf


def avaliar_modelo(modelo, y_test, X_test_tfidf):
    y_pred = modelo.predict(X_test_tfidf)
    acuracia = accuracy_score(y_test, y_pred)

    print("\n--- AVALIAÇÃO DO MODELO ---")
    print(f"Acurácia: {acuracia:.2%}\n")

    print("Relatório de classificação (precision, recall, f1-score):")
    print(classification_report(y_test, y_pred, zero_division=0))

    rotulos = ["baixo risco", "médio risco", "alto risco"]
    matriz = confusion_matrix(y_test, y_pred, labels=rotulos)
    print("Matriz de confusão:")
    print(matriz)

    # salva a matriz em imagem (não usei plt.show() pra não ficar esperando
    # eu fechar uma janela pra terminar a execução)
    ConfusionMatrixDisplay(confusion_matrix=matriz, display_labels=rotulos).plot()
    plt.title("CardioIA - Matriz de Confusão")
    plt.tight_layout()
    caminho_imagem = PASTA_ATUAL / "matriz_confusao.png"
    plt.savefig(caminho_imagem)
    print(f"\nGráfico salvo em: {caminho_imagem}")


def testar_novas_frases(modelo, vectorizer):
    novas_frases = [
        "Estou sentindo uma dor forte no peito acompanhada de falta de ar e suor frio.",
        "Estou apenas um pouco cansado depois de uma noite mal dormida.",
        "Tenho palpitações, batimentos irregulares e sensação de desmaio.",
        "Sinto uma palpitação leve e passageira de vez em quando, sem outro sintoma junto.",
    ]

    X_novas = vectorizer.transform(novas_frases)
    previsoes = modelo.predict(X_novas)
    probabilidades = modelo.predict_proba(X_novas)

    print("\n--- TESTE COM FRASES NOVAS ---")
    for frase, previsao, probs in zip(novas_frases, previsoes, probabilidades):
        prob_da_classe_prevista = max(probs)
        print(f"\nRelato: {frase}")
        print(f"Classificação: {previsao}")
        print(f"Probabilidade estimada pelo modelo: {prob_da_classe_prevista:.2%}")

    print(
        "\nObs: essa probabilidade é só a saída matemática do predict_proba "
        "do modelo, não é uma probabilidade real."
    )


def classificar_frase_usuario(modelo, vectorizer):
    """Deixa digitar frases e ver a classificação na hora."""
    print("\n--- CLASSIFIQUE UMA FRASE SUA ---")
    print("Digite um relato e aperte Enter (Enter vazio para sair).")

    while True:
        try:
            frase = input("\nRelato: ").strip()
        except EOFError:
            # acontece se o script for rodado sem terminal interativo
            # (ex.: dentro de outro script ou de forma automatizada)
            break

        if not frase:
            break

        X_novo = vectorizer.transform([frase])
        previsao = modelo.predict(X_novo)[0]
        prob = max(modelo.predict_proba(X_novo)[0])

        print(f"Classificação: {previsao}")
        print(f"Probabilidade estimada pelo modelo: {prob:.2%}")

    print(
        "\nObs: essa probabilidade é só a saída matemática do predict_proba "
        "do modelo, não é uma probabilidade médica real."
    )


def main():
    print("CARDIOIA - CLASSIFICADOR DE RISCO (Fase 2 - Parte 2)\n")

    df = carregar_dados()
    print(f"Total de frases no dataset: {len(df)}")
    print("Distribuição das classes:")
    print(df["situacao"].value_counts())

    modelo, vectorizer, X_test, y_test, X_test_tfidf = treinar_modelo(df)
    print("\nModelo treinado (TF-IDF + Logistic Regression).")

    avaliar_modelo(modelo, y_test, X_test_tfidf)
    testar_novas_frases(modelo, vectorizer)
    classificar_frase_usuario(modelo, vectorizer)


if __name__ == "__main__":
    main()
