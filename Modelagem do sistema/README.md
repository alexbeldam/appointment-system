# 📌 Sistema de Agendamento de Horários  

Trabalho da disciplina PDS2 – UFMG  
Grupo: Alex Beldam Souza, Amanda Santos Rosa, Bernardo Sabino de Souza Alves, Julia Alessandra da Silva, Mirella Almeida Bastos Rodrigues Silva  

---

## 🚀 User Stories  

### 1. Cadastrar Usuário  
**História:**  
Como usuário, quero criar meu login e perfil, para que eu possa acessar o sistema.  

**Critérios de Aceitação:**  
1. Dado que o usuário ainda não tem conta, quando ele preencher nome, e-mail, papel (aluno, professor) e senha válidos, então o sistema deve criar o perfil com sucesso.  
2. Dado que o usuário informe um e-mail já cadastrado, então o sistema deve exibir uma mensagem de erro.  

---

### 2. Cadastrar Horário  
**História:**  
Como professor, quero cadastrar meus horários disponíveis, para que os usuários possam agendar reuniões.  

**Critérios de Aceitação:**  
1. Dado que o professor esteja logado, quando ele selecionar um intervalo de data e hora, então o sistema deve salvar o horário disponível.  
2. Dado que já existe um horário cadastrado no mesmo período, então o sistema deve exibir uma mensagem de conflito.  

---

### 3. Pesquisar Usuário  
**História:**  
Como usuário, quero pesquisar outros usuários pelo ID, para encontrá-los rapidamente.  

**Critérios de Aceitação:**  
1. Dado que o usuário digite um ID válido, então o sistema deve exibir os dados do usuário correspondente.  
2. Dado que o usuário digite um ID inexistente, então o sistema deve informar que não encontrou resultados.  

---

### 4. Agendar Horário  
**História:**  
Como aluno, quero agendar um horário disponível com um professor, para marcar uma reunião.  

**Critérios de Aceitação:**  
1. Dado que o aluno selecione um horário disponível, então o sistema deve registrar o agendamento e notificar o professor.  
2. Dado que o horário já esteja reservado, então o sistema deve impedir o agendamento e exibir mensagem de indisponibilidade.  

---

### 5. Cancelar Agendamento  
**História:**  
Como usuário, quero cancelar um agendamento, para liberar o horário em caso de imprevisto.  

**Critérios de Aceitação:**  
1. Dado que o usuário esteja logado e tenha um agendamento, quando ele clicar em "Cancelar", então o sistema deve remover a reserva.  
2. Dado que o horário já tenha passado, então o sistema não deve permitir o cancelamento.  

---

### 6. Visualizar Histórico  
**História:**  
Como aluno, quero visualizar o histórico dos meus agendamentos, para consultar reuniões passadas.  

**Critérios de Aceitação:**  
1. Dado que o usuário acesse a seção de histórico, então o sistema deve exibir todos os agendamentos concluídos e cancelados.  
2. Dado que o usuário não tenha agendamentos anteriores, então o sistema deve exibir a mensagem “Nenhum histórico encontrado”.  

---

### 7. Atualizar Perfil  
**História:**  
Como usuário, quero atualizar minhas informações de perfil (nome, e-mail), para manter meus dados corretos no sistema.  

**Critérios de Aceitação:**  
1. Dado que o usuário esteja logado, quando ele acessar a tela de perfil, então deve poder editar nome, e-mail e telefone.  
2. Dado que o usuário salve os dados, então o sistema deve atualizar imediatamente as informações.  

---

### 8. Visualizar Agendamentos  
**História:**  
Como professor, quero visualizar o total de agendamentos por dia/semana, para planejar melhor minha agenda.  

**Critérios de Aceitação:**  
1. Dado que o professor acesse o painel de agendamentos, então o sistema deve exibir um resumo tabelado por dia e semana.  
2. Dado que não haja agendamentos, então o sistema deve mostrar “Nenhum agendamento encontrado”.  

---

### 9. Visualizar Menu  
**História:**  
Como usuário, quero ver um menu principal, para escolher entre opções do sistema.  

**Critérios de Aceitação:**  
1. Dado que o usuário abra o sistema, então o terminal deve exibir opções: “1- Cadastrar horário, 2- Agendar, 3- Ver histórico, 4- Sair”.
2. O usuário deve sempre ter a opção de voltar para o menu.

---

### 10. Receber Instruções  
**História:**  
Como usuário, quero ver instruções de como usar o sistema, para entender todas as funcionalidades disponíveis.  

**Critérios de Aceitação:**  
1. Dado que o usuário seleciona a opção “Ajuda/Instruções”, então o terminal deve listar todas as opções do menu com explicação breve.  
2. As instruções devem ser exibidas quando o usuário realizar alguma ação indisponível no sistema.

---

## 🃏 Diagrama de classes

![Diagrama de classes](Diagrama_classes.png)

