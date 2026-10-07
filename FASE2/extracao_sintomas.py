"""
CardioIA - Fase 2 - Parte 1
Extração de sintomas a partir de relatos em texto, usando um mapa de
conhecimento (CSV) com sintomas, expressões e condições associadas.

Trabalho acadêmico. Os relatos são simulados e os "pesos" usados aqui
são só uma forma simples de ranquear as condições, não representam
probabilidade real nem diagnóstico.
"""

import unicodedata
from pathlib import Path

import pandas as pd

# pasta onde este arquivo está, para os caminhos funcionarem de qualquer lugar
PASTA_ATUAL = Path(__file__).resolve().parent
ARQUIVO_MAPA = PASTA_ATUAL / "dados" / "mapa_conhecimento.csv"
ARQUIVO_FRASES = PASTA_ATUAL / "dados" / "sintomas.txt"


def normalizar(texto):
    """Deixa o texto em minúsculas e sem acento, só para facilitar a busca."""
    texto = str(texto).lower().strip()
    sem_acento = unicodedata.normalize("NFD", texto).encode("ascii", "ignore")
    return sem_acento.decode("utf-8")


def carregar_mapa_conhecimento():
    mapa = pd.read_csv(ARQUIVO_MAPA, encoding="utf-8")

    if mapa.empty:
        raise ValueError("O mapa de conhecimento está vazio.")

    colunas_esperadas = {"sintoma", "expressao_alternativa", "condicao", "peso"}
    if not colunas_esperadas.issubset(mapa.columns):
        raise ValueError(f"Faltam colunas no mapa de conhecimento: {colunas_esperadas}")

    # já deixa pronto a versão normalizada, pra não ficar refazendo isso em todo relato
    mapa["sintoma_norm"] = mapa["sintoma"].map(normalizar)
    mapa["expressao_norm"] = mapa["expressao_alternativa"].map(normalizar)
    return mapa


def carregar_relatos():
    with open(ARQUIVO_FRASES, encoding="utf-8") as arquivo:
        relatos = [linha.strip() for linha in arquivo if linha.strip()]

    if not relatos:
        raise ValueError("O arquivo de relatos está vazio.")

    return relatos


def achar_trecho_original(frase, texto_normalizado, termo):
    """Pega o trecho encontrado, mas mostra com a escrita original do relato
    (com acento e maiúscula), já que a normalização não muda o tamanho do texto."""
    posicao = texto_normalizado.find(termo)
    if posicao == -1:
        return termo
    return frase[posicao:posicao + len(termo)]


def analisar_frase(frase, mapa):
    """Procura no relato as expressões/sintomas do mapa de conhecimento e
    soma os pesos por condição.

    Obs: o mapa tem várias linhas repetindo o mesmo sintoma para a mesma
    condição, só com expressões diferentes (ex: "falta de ar" aparece várias
    vezes ligado a Insuficiência Cardíaca). Se eu somasse todas elas de uma
    vez, o ranking ficaria inflado só porque uma palavra genérica apareceu.
    Por isso guardo apenas a MELHOR evidência de cada (sintoma, condição).
    """
    texto = normalizar(frase)
    melhores = {}

    for _, linha in mapa.iterrows():
        tem_expressao = linha["expressao_norm"] in texto
        tem_sintoma = linha["sintoma_norm"] in texto

        if not tem_expressao and not tem_sintoma:
            continue

        # prioriza a expressão específica; só usa o nome genérico do sintoma
        # quando a expressão específica não apareceu no texto
        termo_usado = linha["expressao_norm"] if tem_expressao else linha["sintoma_norm"]
        chave = (linha["sintoma"], linha["condicao"])

        candidato = {
            "sintoma": linha["sintoma"],
            "trecho": achar_trecho_original(frase, texto, termo_usado),
            "condicao": linha["condicao"],
            "peso": int(linha["peso"]),
            "especifico": tem_expressao,
        }

        if chave not in melhores:
            melhores[chave] = candidato
        else:
            atual = melhores[chave]
            # troca se o novo for mais específico, ou se empatou na
            # especificidade mas o peso é maior
            if candidato["especifico"] and not atual["especifico"]:
                melhores[chave] = candidato
            elif candidato["especifico"] == atual["especifico"] and candidato["peso"] > atual["peso"]:
                melhores[chave] = candidato

    encontrados = list(melhores.values())

    pontuacoes = {}
    for item in encontrados:
        condicao = item["condicao"]
        pontuacoes[condicao] = pontuacoes.get(condicao, 0) + item["peso"]

    ranking = sorted(pontuacoes.items(), key=lambda x: x[1], reverse=True)
    return encontrados, ranking


def exibir_resultado(numero, frase, encontrados, ranking):
    print("=" * 70)
    print(f"RELATO {numero}")
    print(frase)

    print("\nEvidências encontradas:")
    if encontrados:
        for item in encontrados:
            print(f'- "{item["trecho"]}" (sintoma: {item["sintoma"]}) -> {item["condicao"]} (+{item["peso"]})')
    else:
        print("Nenhum sintoma do mapa de conhecimento foi encontrado.")

    print("\nRanking de condições (pontuação heurística, não é diagnóstico):")
    if ranking:
        for posicao, (condicao, pontos) in enumerate(ranking, 1):
            print(f"{posicao}. {condicao}: {pontos} pontos")
    else:
        print("Nenhuma condição associada.")


def main():
    print("CARDIOIA - EXTRAÇÃO DE SINTOMAS (Fase 2 - Parte 1)")
    print("Relatos simulados, apenas para fins acadêmicos.\n")

    mapa = carregar_mapa_conhecimento()
    relatos = carregar_relatos()

    for numero, frase in enumerate(relatos, 1):
        encontrados, ranking = analisar_frase(frase, mapa)
        exibir_resultado(numero, frase, encontrados, ranking)


if __name__ == "__main__":
    main()
