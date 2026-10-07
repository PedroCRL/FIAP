import { useState } from 'react'
import { useNavigate } from 'react-router-dom'
import { useAuth } from '../context/AuthContext'
import styles from './Login.module.css'

export default function Login() {
  // useState simples pra cada campo do formulário (não precisa de
  // useReducer aqui, são só 2 campos + 1 mensagem de erro)
  const [usuario, setUsuario] = useState('')
  const [senha, setSenha] = useState('')
  const [erro, setErro] = useState('')
  const { login } = useAuth()
  const navigate = useNavigate()

  function handleSubmit(evento) {
    evento.preventDefault()

    if (!usuario.trim() || !senha.trim()) {
      setErro('Preencha usuário e senha.')
      return
    }

    // Login simulado: não existe verificação real de senha, qualquer
    // usuário/senha não vazios entra. Serve só para demonstrar o fluxo
    // de autenticação com Context API + token no localStorage.
    login(usuario.trim())
    navigate('/')
  }

  return (
    <div className={styles.container}>
      <form className={styles.card} onSubmit={handleSubmit}>
        <h1>CardioIA</h1>
        <p className={styles.aviso}>
          Login simulado para fins acadêmicos — qualquer usuário e senha entram.
          Dados fictícios, projeto sem back-end real.
        </p>

        <label>
          Usuário
          <input
            value={usuario}
            onChange={(evento) => setUsuario(evento.target.value)}
            placeholder="ex: joao.silva"
          />
        </label>

        <label>
          Senha
          <input
            type="password"
            value={senha}
            onChange={(evento) => setSenha(evento.target.value)}
            placeholder="qualquer senha"
          />
        </label>

        {erro && <p className={styles.erro}>{erro}</p>}

        <button type="submit">Entrar</button>
      </form>
    </div>
  )
}
