import styles from './StatCard.module.css'

// Card simples pra mostrar um número com um título embaixo (usado no
// Dashboard pra "Pacientes cadastrados" e "Consultas agendadas").
export default function StatCard({ titulo, valor }) {
  return (
    <div className={styles.card}>
      <span className={styles.valor}>{valor}</span>
      <span className={styles.titulo}>{titulo}</span>
    </div>
  )
}
