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

### 2. Autenticar Usuário  
**História:**  
Como usuário, quero realizar login no sistema, para que eu possa acessar minhas funcionalidades de acordo com meu perfil.  

**Critérios de Aceitação:**  
1. Dado que o usuário tenha uma conta válida, quando ele informar e-mail e senha corretos, então o sistema deve autenticar o usuário e permitir o acesso.  
2. Dado que o usuário informe credenciais inválidas, então o sistema deve exibir uma mensagem de erro informando que e-mail ou senha estão incorretos.  

---

### 3. Deletar Usuário  
**História:**  
Como usuário, quero excluir meu perfil, para que eu possa remover meus dados do sistema.  

**Critérios de Aceitação:**  
1. Dado que o usuário esteja autenticado, quando solicitar a exclusão de sua conta, então o sistema deve remover o perfil e todos os dados associados.  
2. Dado que o usuário tente excluir uma conta inexistente, então o sistema deve exibir uma mensagem informando que o usuário não foi encontrado.  

---

### 4. Cadastrar Horário Disponível  
**História:**  
Como professor, quero cadastrar horários disponíveis, para que alunos possam agendar atendimentos.  

**Critérios de Aceitação:**  
1. Dado que o professor esteja autenticado, quando cadastrar uma data, hora e duração válidas, então o sistema deve registrar o horário como disponível.  
2. Dado que o professor tente cadastrar um horário que conflite com outro já existente, então o sistema deve impedir o cadastro e exibir uma mensagem de aviso.  

---

### 5. Deletar Horário  
**História:**  
Como professor, quero excluir um horário cadastrado, para que ele não esteja mais disponível para agendamento.  

**Critérios de Aceitação:**  
1. Dado que o horário ainda não tenha sido agendado, quando o professor solicitar a exclusão, então o sistema deve removê-lo com sucesso.  
2. Dado que o horário já tenha um agendamento, então o sistema deve impedir a exclusão e exibir uma mensagem informando que há agendamentos vinculados.  

---

### 6. Listar Todos os Horários do Professor  
**História:**  
Como professor, quero visualizar todos os meus horários cadastrados, para que eu possa gerenciar minhas disponibilidades.  

**Critérios de Aceitação:**  
1. Dado que o professor esteja autenticado, quando acessar a lista de horários, então o sistema deve exibir todos os horários cadastrados com data e hora.  
2. Dado que o professor não tenha horários cadastrados, então o sistema deve exibir uma mensagem informando que não há horários disponíveis.  

---

### 7. Avaliar Agendamentos Pendentes  
**História:**  
Como professor, quero avaliar se vou confirmar ou recusar agendamentos pendentes, para controlar meus atendimentos.  

**Critérios de Aceitação:**  
1. Dado que o professor tenha agendamentos pendentes, quando ele confirmar ou recusar, então o sistema deve atualizar o status do agendamento.  
2. Dado que o professor tente avaliar um agendamento inexistente ou já confirmado/recusado, então o sistema deve exibir uma mensagem de erro.  

---

### 8. Agendar Horário  
**História:**  
Como aluno, quero agendar um horário disponível, para que eu possa marcar um atendimento com o professor.  

**Critérios de Aceitação:**  
1. Dado que o aluno esteja autenticado, quando selecionar um horário disponível e confirmar o agendamento, então o sistema deve registrar o agendamento com sucesso.  
2. Dado que o aluno tente agendar um horário já ocupado, então o sistema deve impedir o agendamento e exibir uma mensagem de erro.  

---

### 9. Cancelar Agendamento  
**História:**  
Como aluno, quero cancelar um agendamento, para que eu possa liberar o horário e ajustar minha agenda.  

**Critérios de Aceitação:**  
1. Dado que o aluno tenha um agendamento ativo, quando solicitar o cancelamento, então o sistema deve remover o agendamento e liberar o horário.  
2. Dado que o aluno tente cancelar um agendamento inexistente ou já cancelado, então o sistema deve exibir uma mensagem de erro.  

---

### 10. Listar Agendamentos  
**História:**  
Como aluno, quero visualizar todos os meus agendamentos, para que eu possa acompanhar meus horários e status.  

**Critérios de Aceitação:**  
1. Dado que o aluno esteja autenticado, quando acessar a lista de agendamentos, então o sistema deve exibir todos os agendamentos com data, hora, status e nome do professor.  
2. Dado que o aluno não tenha agendamentos, então o sistema deve exibir uma mensagem informando que não há agendamentos registrados.  

---

## 🃏 Diagrama de classes

![Diagrama de classes](Diagrama_classes.png)

