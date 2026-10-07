import { useReducer, useState } from 'react'
import pacientes from '../data/pacientes.json'
import { useAppointments } from '../context/AppointmentsContext'
import styles from './Agendamento.module.css'

const estadoInicial = { pacienteId: '', data: '', hora: '', motivo: '' }

// useReducer cuida dos campos do formulário (um reducer genérico que
// atualiza um campo por vez, mais um RESET para limpar tudo depois de enviar).
// Dá pra fazer isso com vários useState também, mas como pediram pra usar
// useReducer no formulário, ficou assim.
function formularioReducer(estado, acao) {
  switch (acao.type) {
    case 'SET_CAMPO':
      return { ...estado, [acao.campo]: acao.valor }
    case 'RESET':
      return estadoInicial
    default:
      return estado
  }
}

export default function Agendamento() {
  const [form, dispatch] = useReducer(formularioReducer, estadoInicial)
  // useState aqui só guarda uma mensagem simples pra mostrar na tela
  // (erro de validação ou confirmação de agendamento) - não precisa ser
  // reducer porque é só um campo isolado, não depende do resto do form
  const [mensagem, setMensagem] = useState({ texto: '', tipo: '' })
  const { adicionarConsulta } = useAppointments()

  // usado em todos os inputs/select: pega o "name" do campo e manda pro
  // reducer atualizar só aquele campo
  function handleChange(evento) {
    dispatch({ type: 'SET_CAMPO', campo: evento.target.name, valor: evento.target.value })
  }

  function handleSubmit(evento) {
    evento.preventDefault()

    if (!form.pacienteId || !form.data || !form.hora) {
      setMensagem({ texto: 'Preencha paciente, data e hora.', tipo: 'erro' })
      return
    }

    // o value do <option> vem como string, por isso o String() pra comparar
    const paciente = pacientes.find((p) => String(p.id) === form.pacienteId)

    adicionarConsulta({ ...form, pacienteNome: paciente.nome })
    setMensagem({
      texto: `Consulta agendada para ${paciente.nome} em ${form.data} às ${form.hora}.`,
      tipo: 'sucesso',
    })
    dispatch({ type: 'RESET' })
  }

  return (
    <div>
      <h1>Agendar consulta</h1>
      <p>Agendamento simulado: fica guardado só durante esta sessão do navegador.</p>

      <form className={styles.form} onSubmit={handleSubmit}>
        <label>
          Paciente
          <select name="pacienteId" value={form.pacienteId} onChange={handleChange}>
            <option value="">Selecione...</option>
            {pacientes.map((paciente) => (
              <option key={paciente.id} value={paciente.id}>
                {paciente.nome}
              </option>
            ))}
          </select>
        </label>

        <label>
          Data
          <input type="date" name="data" value={form.data} onChange={handleChange} />
        </label>

        <label>
          Hora
          <input type="time" name="hora" value={form.hora} onChange={handleChange} />
        </label>

        <label>
          Motivo (opcional)
          <input
            type="text"
            name="motivo"
            placeholder="ex: dor no peito"
            value={form.motivo}
            onChange={handleChange}
          />
        </label>

        <button type="submit">Agendar</button>

        {mensagem.texto && (
          <p className={mensagem.tipo === 'erro' ? styles.erro : styles.sucesso}>
            {mensagem.texto}
          </p>
        )}
      </form>
    </div>
  )
}
