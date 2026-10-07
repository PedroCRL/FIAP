# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="assets\logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>


## 👨‍🎓 Autor do Repositorio: 

- <a href="https://www.linkedin.com/in/pedro-carvalho-cea-149658137/">Pedro Carvalho Rocha Lima</a> 



# CardioIA - Interface (Ir Além 1)

Nesta etapa, desenvolvi um portal front-end do CardioIA utilizando React e Vite. A ideia foi criar uma experiência que simulasse como o usuário interagiria com o sistema na prática. esse portal não tem um back-end real conectado. Por isso, os dados utilizados são simulados em arquivos JSON e ficam armazenados temporariamente no próprio navegador, utilizando memória e localStorage.

## O que tem

- **Login simulado** : fize um login simulado, onde basta preencher usuário e senha. O acesso fica salvo no navegador, então continua ativo mesmo se a página for recarregada.


- **Lista de pacientes**: a lista de pacientes utiliza dados simulados em um arquivo JSON

- **Agendamento de consulta**: criei um formulário para agendar consultas, e os agendamentos ficam armazenados durante a sessão.

- **Dashboard**: contagem de pacientes cadastrados e consultas agendadas.

- **Estilização**: apresenta de forma simples a quantidade de pacientes e consultas agendadas.

Visual: toda a interface foi organizada com CSS Modules, deixando cada tela e componente com seu próprio estilo.

## Como rodar

```bash
cd FASE2/interface
npm install
npm run dev
```

Abre em `http://localhost:5173`. Qualquer usuário/senha entram na tela de login.

