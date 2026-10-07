import { BrowserRouter, Routes, Route } from 'react-router-dom'
import { AuthProvider, useAuth } from './context/AuthContext'
import { AppointmentsProvider } from './context/AppointmentsContext'
import ProtectedRoute from './components/ProtectedRoute'
import Navbar from './components/Navbar'
import Login from './pages/Login'
import Dashboard from './pages/Dashboard'
import Pacientes from './pages/Pacientes'
import Agendamento from './pages/Agendamento'

// Só mostra a navbar quando o usuário está logado (na tela de login ela
// não faz sentido, já que ainda não existe sessão).
function Layout({ children }) {
  const { estaLogado } = useAuth()
  return (
    <>
      {estaLogado && <Navbar />}
      <main>{children}</main>
    </>
  )
}

export default function App() {
  return (
    // AuthProvider e AppointmentsProvider precisam envolver as rotas
    // pra qualquer página conseguir usar useAuth()/useAppointments()
    <AuthProvider>
      <AppointmentsProvider>
        <BrowserRouter>
          <Layout>
            <Routes>
              {/* única rota pública - as outras passam pelo ProtectedRoute */}
              <Route path="/login" element={<Login />} />

              <Route
                path="/"
                element={
                  <ProtectedRoute>
                    <Dashboard />
                  </ProtectedRoute>
                }
              />
              <Route
                path="/pacientes"
                element={
                  <ProtectedRoute>
                    <Pacientes />
                  </ProtectedRoute>
                }
              />
              <Route
                path="/agendamento"
                element={
                  <ProtectedRoute>
                    <Agendamento />
                  </ProtectedRoute>
                }
              />
            </Routes>
          </Layout>
        </BrowserRouter>
      </AppointmentsProvider>
    </AuthProvider>
  )
}
