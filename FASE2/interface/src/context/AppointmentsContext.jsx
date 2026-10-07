import { createContext, useContext, useReducer } from 'react'

// Context separado só pra lista de consultas agendadas, assim o Dashboard
// e a tela de Agendamento conseguem ler/alterar a mesma lista sem precisar
// passar isso tudo por prop desde o App.jsx.
const AppointmentsContext = createContext(null)

// useReducer guarda a lista de consultas agendadas durante a sessão
// (reinicia ao recarregar a página - é só uma simulação em memória,
// não salva no localStorage nem em nenhum servidor).
function reducer(consultas, acao) {
  switch (acao.type) {
    case 'ADICIONAR':
      return [...consultas, acao.consulta]
    default:
      return consultas
  }
}

export function AppointmentsProvider({ children }) {
  const [consultas, dispatch] = useReducer(reducer, [])

  function adicionarConsulta(consulta) {
    // Date.now() como id só pra ter algo único pro key do React na lista
    dispatch({ type: 'ADICIONAR', consulta: { ...consulta, id: Date.now() } })
  }

  return (
    <AppointmentsContext.Provider value={{ consultas, adicionarConsulta }}>
      {children}
    </AppointmentsContext.Provider>
  )
}

export function useAppointments() {
  return useContext(AppointmentsContext)
}
