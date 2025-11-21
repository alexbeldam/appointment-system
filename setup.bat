@echo off
REM Este script configura o ambiente de testes, instruindo o Git a ignorar
REM futuras modificacoes nos arquivos de dados CSV locais (estado do banco mock).

echo Configurando o ambiente de testes (Windows)...

REM Lista de arquivos CSV a serem ignorados localmente (apos o clone)
set FILES="data/agendamentos.csv" "data/alunos.csv" "data/horarios.csv" "data/professores.csv"

FOR %%f IN (%FILES%) DO (
    IF EXIST %%f (
        git update-index --assume-unchanged %%f
        echo  -> %%f: ignorando futuras modificacoes locais.
    ) ELSE (
        echo ERRO: Arquivo %%f nao encontrado. Verifique se voce clonou o repositorio corretamente.
    )
)

echo Configuracao inicial concluida. Suas modificacoes em data/ nao serao rastreadas.
pause