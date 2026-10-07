import { Navigate } from 'react-router-dom'
import { useAuth } from '../context/AuthContext'

// Componente "guarda-rota": só mostra o conteúdo de dentro (children) se
// tiver usuário logado no AuthContext. Se não tiver, manda pra tela de
// login em vez de mostrar a página.
export default function ProtectedRoute({ children }) {
  const { estaLogado, carregando } = useAuth()

  // enquanto ainda está checando o localStorage, não mostra nada (evita
  // o usuário ver a tela de login "piscar" rapidinho antes de logar)
  if (carregando) {
    return null
  }

  if (!estaLogado) {
    return <Navigate to="/login" replace />
  }

  return children
}
