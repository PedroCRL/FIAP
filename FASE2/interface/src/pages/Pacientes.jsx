// pacientes.json é importado direto como um array de objetos - o Vite
// deixa importar .json igual um módulo JS normal
import pacientes from '../data/pacientes.json'
import styles from './Pacientes.module.css'

export default function Pacientes() {
  return (
    <div>
      <h1>Pacientes</h1>
      <p className={styles.aviso}>
        Lista simulada (arquivo JSON local), sem integração com back-end real.
      </p>

      {/* overflow-x no wrapper (ver Pacientes.module.css) pra tabela não
          quebrar o layout em telas pequenas, só aparece scroll horizontal */}
      <div className={styles.tabelaWrapper}>
        <table className={styles.tabela}>
          <thead>
            <tr>
              <th>Nome</th>
              <th>Idade</th>
              <th>Telefone</th>
              <th>Última consulta</th>
              <th>Status</th>
            </tr>
          </thead>
          <tbody>
            {pacientes.map((paciente) => (
              <tr key={paciente.id}>
                <td>{paciente.nome}</td>
                <td>{paciente.idade}</td>
                <td>{paciente.telefone}</td>
                <td>{paciente.ultimaConsulta}</td>
                <td>{paciente.status}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  )
}
