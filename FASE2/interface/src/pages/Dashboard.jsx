import pacientes from '../data/pacientes.json'
import { useAppointments } from '../context/AppointmentsContext'
import StatCard from '../components/StatCard'
import styles from './Dashboard.module.css'

export default function Dashboard() {
  // pacientes.length não muda (vem de um JSON fixo), mas consultas.length
  // muda conforme o usuário agenda coisas na outra tela
  const { consultas } = useAppointments()

  return (
    <div>
      <h1>Painel</h1>
      <p>Visão geral simulada do CardioIA. Dados fictícios, só para fins acadêmicos.</p>

      <div className={styles.grid}>
        <StatCard titulo="Pacientes cadastrados" valor={pacientes.length} />
        <StatCard titulo="Consultas agendadas" valor={consultas.length} />
      </div>

      <div className={styles.lista}>
        <h2>Últimas consultas agendadas</h2>
        {consultas.length === 0 ? (
          <p className={styles.vazio}>Nenhuma consulta agendada ainda.</p>
        ) : (
          <ul>
            {consultas.map((consulta) => (
              <li key={consulta.id}>
                {consulta.pacienteNome} — {consulta.data} às {consulta.hora}
                {consulta.motivo ? ` (${consulta.motivo})` : ''}
              </li>
            ))}
          </ul>
        )}
      </div>
    </div>
  )
}
