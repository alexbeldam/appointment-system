<p align="center">
  <img src="https://api.iconify.design/tabler/calendar-check.svg?color=%23C8102E" alt="Agenda com Check Vermelho UFMG" width="80" />
</p>

<h1 align="center"><strong>Sistema de Agendamento CLI</strong></h1>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/C%2B%2B17-4D99F0?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Makefile-3E3E3E?style=for-the-badge&logo=make&logoColor=white" />
  <img src="https://img.shields.io/badge/Doxygen-D51F2A?style=for-the-badge&logo=doxygen&logoColor=white" />
  <img src="https://img.shields.io/badge/SOLID-8B4513?style=for-the-badge" />
</p>

<p align="center">
  <a href="#sobre">Sobre</a> • 
  <a href="#primeiros-passos">Primeiros Passos</a> • 
  <a href="#demonstracao-cli">Demonstração CLI</a> •
  <a href="#uso-e-funcionalidades">Uso e Funcionalidades</a> • 
  <a href="#documentacao">Documentação</a> •
  <a href="#colaboradores">Colaboradores</a>
</p>

<p align="center">
  <b>Sistema de agendamento de reuniões/aulas desenvolvido em C++ puro, utilizando o padrão MVC e Injeção de Dependência.</b>
</p>

---

<h2 id="sobre">📌 Sobre</h2>

Este projeto é um sistema de agendamento interativo baseado em **Console (CLI)**, implementado em C++17. Ele simula a interação entre **Alunos** e **Professores** para marcação de horários de disponibilidade.

### Arquitetura e Padrões

A arquitetura do projeto é fortemente baseada em:

- **MVC (Model-View-Controller):** Separação clara entre as camadas de Entidade (`model`), Lógica de Negócio (`service`, `controller`) e Interface (`view`).
- **Injeção de Dependência (DI):** Utilizada para conectar serviços, controladores e a persistência, mantendo o código altamente desacoplado e testável.
- **Barramento de Eventos (`EventBus`):** Permite a comunicação assíncrona/desacoplada entre os componentes (ex: o login de um usuário notifica outros serviços).
- **Persistência Mock:** Os dados são armazenados em arquivos `.csv` (simulando um banco de dados simples) com um mecanismo de **Cache** e **Invalidação** (via `FileObserver`).

---

<h2 id="primeiros-passos">🚀 Primeiros Passos</h2>

### Pré-requisitos

Para compilar e executar o projeto, você precisará de:

- **Compilador C++:** Suporte para **C++17** (G++, Clang, MSVC).
- **Make:** Para utilizar o `Makefile`.
- **Git:** Para clonar o repositório.

### ⚠️ Credenciais de Teste

> A senha padrão para **TODOS** os usuários (Alunos e Professores) cadastrados nos arquivos CSV iniciais do repositório é: **`senha`**

### ⚙️ Configuração Inicial (Setup)

O projeto usa arquivos CSV na pasta `data/` como banco de dados mock. Para evitar que suas alterações de teste subam acidentalmente, use o script de configuração adequado:

1.  **Clone o repositório:**

    ```bash
    git clone https://github.com/julia-alessandra/PDS2-20252-TM1-RedeSocial.git
    cd PDS2-20252-TM1-RedeSocial
    ```

2.  **Configure o ambiente de dados:** 
    - **Linux/macOS:** Dê permissão de execução e execute:
      
    ```bash
    chmod +x setup.sh
    ./setup.sh
    ``` 
    - **Windows:** Execute o script no diretório raiz:
      
    ```bash
    ./setup.bat
    ```

> Este passo garante que suas modificações nos arquivos `data/*.csv` não serão rastreadas pelo Git, mantendo o repositório limpo.

### Compilando e Executando

O projeto usa um `Makefile` para gerenciar a compilação:

1.  **Compilar:**

    ```bash
    make
    ```

2.  **Executar:**
    - **Linux/macOS:** `./programa`
    - **Windows:** `programa.exe`

---

<h2 id="demonstracao-cli">🖥️ Demonstração CLI: Fluxo de Usuário</h2>

Esta seção simula o layout visual e as opções de navegação apresentadas ao usuário em cada Interface de Usuário (UI).

### 1. Menu Inicial (`AuthUI`)

É a tela de entrada que permite a criação de conta ou acesso ao sistema.

```text
*-------------------------------*
|       AGENDAMENTO DE          |
|           HORARIOS            |
|           .---.               |
|          /   / \              |
|         |   o   |             |
|          \ | /                |
|           '---'               |
*-------------------------------*

MENU PRINCIPAL:
1 - Fazer Login
2 - Fazer cadastro
0 - Sair do programa
Escolha uma opcao:
```

### 2. Menu Aluno (`AlunoUI`)

Tela principal para usuários logados como **Aluno**.

```text
*-------------------------------*
|       AGENDAMENTO DE          |
|           HORARIOS            |
|           .---.               |
|          /   / \              |
|         |   o   |             |
|          \ | /                |
|           '---'               |
*-------------------------------*

MENU ALUNO:
1 - Agendar Horário
2 - Listar meus agendamentos
3 - Cancelar agendamento
4 - Atualizar Perfil
5 - Deletar perfil
6 - Logout
0 - Sair do programa
Escolha uma opcao:
```

### 2. Menu Professor (`ProfessorUI`)

Tela principal para usuários logados como **Professor**.

```text
*-------------------------------*
|       AGENDAMENTO DE          |
|           HORARIOS            |
|           .---.               |
|          /   / \              |
|         |   o   |             |
|          \ | /                |
|           '---'               |
*-------------------------------*

MENU PROFESSOR:
1 - Gerenciar Agendamentos Pendentes
2 - Cancelar Agendamento
3 - Cadastrar horário disponível
4 - Listar meus horários
5 - Excluir um horário
6 - Excluir todos meus horários
7 - Atualizar Perfil
8 - Deletar Perfil
9 - Logout
0 - Sair do programa
Escolha uma opcao:
```

---

<h2 id="uso-e-funcionalidades">💻 Uso e Funcionalidades</h2>

Ao iniciar o programa, você será apresentado ao **Menu de Autenticação (`AuthUI`)**, permitindo:

| Usuário       | Funcionalidade Principal                                                                  | Controllers Envolvidos                                              |
| :------------ | :---------------------------------------------------------------------------------------- | :------------------------------------------------------------------ |
| **Geral**     | Login e Cadastro (Aluno/Professor)                                                        | `LoginController`, `AlunoController`, `ProfessorController`         |
| **Aluno**     | Agendar Horários, Visualizar Agendamentos (Pendentes/Confirmados), Gerenciar Perfil.      | `AlunoController`, `AgendamentoController`, `ProfessorController`   |
| **Professor** | Cadastrar e Excluir Horários, Avaliar Agendamentos (Confirmar/Recusar), Gerenciar Perfil. | `ProfessorController`, `HorarioController`, `AgendamentoController` |

---

<h2 id="documentacao">📚 Documentação (Doxygen)</h2>

A documentação detalhada das classes, métodos e arquitetura pode ser gerada a partir dos comentários Doxygen presentes no código.

📄 **Histórias de Usuário**: Para o planejamento e detalhes das funcionalidades, consulte o [README da pasta docs](docs/README.md).

### Pré-requisito para Geração

Você precisa ter o **Doxygen** e o **Graphviz** instalados.

| Sistema Operacional | Comando de Instalação                                                                               |
| :------------------ | :-------------------------------------------------------------------------------------------------- |
| **Fedora**          | `sudo dnf install doxygen graphviz`                                                                 |
| **Debian/Ubuntu**   | `sudo apt install doxygen graphviz`                                                                 |
| **Windows**         | Instale os binários via [Chocolatey](https://chocolatey.org/) ou baixe diretamente do site oficial. |

### Gerando a Documentação

Basta usar a regra `doc` do `Makefile`:

```bash
make doc
```

### Visualizando a Documentação

A documentação será gerada na pasta docs/html_doc/. Abra o arquivo index.html no seu navegador:

```bash
# Exemplo Linux/macOS
xdg-open docs/html_doc/html/index.html

# Exemplo Windows (use no CMD ou PowerShell)
start docs\html_doc\html\index.html
```

<h2 id="colaboradores">🤝 Colaboradores</h2>

<p align="center">
Um grande obrigado a todas as pessoas que contribuíram para este projeto.
</p>

<table align="center">
  <tr>
    <td align="center">
      <a href="https://github.com/alexbeldam">
        <img src="https://github.com/alexbeldam.png" width="100px" height="100px" alt="Foto do Alex"/><br>
        <sub><b>Alex</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/amanda1411-coder">
        <img src="https://github.com/amanda1411-coder.png" width="100px" height="100px" alt="Foto da Amanda"/><br>
        <sub><b>Amanda</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/bernardo-sabino">
        <img src="https://github.com/bernardo-sabino.png" width="100px" height="100px" alt="Foto do Bernardo"/><br>
        <sub><b>Bernardo</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/julia-alessandra">
        <img src="https://github.com/julia-alessandra.png" width="100px" height="100px"alt="Foto da Júlia"/><br>
        <sub><b>Júlia</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/MirellaABRS">
        <img src="https://github.com/MirellaABRS.png" width="100px" height="100px" alt="Foto da Mirella"/><br>
        <sub><b>Mirella</b></sub>
      </a>
    </td>
  </tr>
</table>

---

<p align="center">
Feito com 🧠 para o trabalho prático de PDS2 da UFMG
</p>
