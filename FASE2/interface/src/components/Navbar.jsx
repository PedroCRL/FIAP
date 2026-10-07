import { NavLink } from 'react-router-dom'
import { useAuth } from '../context/AuthContext'
import styles from './Navbar.module.css'

export default function Navbar() {
  const { usuario, logout } = useAuth()

  // deixa o link da página atual destacado (NavLink já avisa se está
  // ativo ou não através do isActive)
  function linkClasse({ isActive }) {
    return isActive ? styles.ativo : ''
  }

  return (
    <nav className={styles.nav}>
      <span className={styles.logo}>CardioIA</span>

      <div className={styles.links}>
        {/* o "end" no link da Home é necessário, senão ele fica marcado
            como ativo em qualquer página (toda URL começa com "/") */}
        <NavLink to="/" end className={linkClasse}>Painel</NavLink>
        <NavLink to="/pacientes" className={linkClasse}>Pacientes</NavLink>
        <NavLink to="/agendamento" className={linkClasse}>Agendar consulta</NavLink>
      </div>

      <div className={styles.usuario}>
        <span>{usuario}</span>
        <button onClick={logout}>Sair</button>
      </div>
    </nav>
  )
}
