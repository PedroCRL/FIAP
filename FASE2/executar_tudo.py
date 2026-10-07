"""
CardioIA - Fase 2
Roda as duas partes do trabalho em sequência: extração de sintomas e
depois o classificador de risco.
"""

import classificacao_risco
import extracao_sintomas


def main():
    extracao_sintomas.main()
    print("\n")
    classificacao_risco.main()


if __name__ == "__main__":
    main()
