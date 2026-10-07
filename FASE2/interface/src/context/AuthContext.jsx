import { createContext, useContext, useEffect, useState } from 'react'

// Context de autenticação: guarda quem está logado e deixa qualquer
// componente saber isso sem precisar passar "usuario" por prop em tudo.
const AuthContext = createContext(null)

const CHAVE_TOKEN = 'cardioia_token'
const CHAVE_USUARIO = 'cardioia_usuario'

export function AuthProvider({ children }) {
  const [usuario, setUsuario] = useState(null)
  // começa "carregando" porque precisa checar o localStorage antes de
  // decidir se mostra a tela de login ou não
  const [carregando, setCarregando] = useState(true)

  // roda uma vez quando o app abre: se já tinha um login salvo de uma
  // visita anterior, continua logado em vez de pedir pra entrar de novo
  useEffect(() => {
    const token = localStorage.getItem(CHAVE_TOKEN)
    const usuarioSalvo = localStorage.getItem(CHAVE_USUARIO)

    if (token && usuarioSalvo) {
      setUsuario(usuarioSalvo)
    }

    setCarregando(false)
  }, [])

  function login(nomeUsuario) {
    // token "fake" só para parecer um JWT (não é um JWT de verdade, não
    // tem assinatura nem validação - é só uma string pra simular o formato)
    const tokenFalso = `${btoa(nomeUsuario)}.fake.jwt`

    localStorage.setItem(CHAVE_TOKEN, tokenFalso)
    localStorage.setItem(CHAVE_USUARIO, nomeUsuario)
    setUsuario(nomeUsuario)
  }

  function logout() {
    localStorage.removeItem(CHAVE_TOKEN)
    localStorage.removeItem(CHAVE_USUARIO)
    setUsuario(null)
  }

  const valor = {
    usuario,
    estaLogado: Boolean(usuario),
    carregando,
    login,
    logout,
  }

  return <AuthContext.Provider value={valor}>{children}</AuthContext.Provider>
}

// hook pra usar o contexto sem precisar importar useContext + AuthContext
// em cada componente que precisa saber se o usuário está logado
export function useAuth() {
  return useContext(AuthContext)
}
