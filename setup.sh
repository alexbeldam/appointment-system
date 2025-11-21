#!/bin/bash

# Este script configura o ambiente de testes, instruindo o Git a ignorar
# futuras modificações nos arquivos de dados CSV locais (estado do banco mock).

echo "Configurando o ambiente de testes (Linux/macOS)..."

# Lista de arquivos CSV a serem ignorados localmente (após o clone)
FILES=(
    "data/agendamentos.csv"
    "data/alunos.csv"
    "data/horarios.csv"
    "data/professores.csv"
)

# Adiciona a flag assume-unchanged para cada arquivo
for file in "${FILES[@]}"; do
    if [ -f "$file" ]; then
        git update-index --assume-unchanged "$file"
        echo " -> $file: ignorando futuras modificações locais."
    else
        echo "ERRO: Arquivo $file não encontrado. Verifique se você clonou o repositório corretamente."
    fi
done

echo "Configuração inicial concluída. Suas modificações em data/ não serão rastreadas."