#!/bin/bash

echo "Instalando dependências necessárias para compilar e rodar o projeto..."

# atualizando pacotes e instalando gcc e make
sudo apt update && sudo apt install -y gcc make

# adicionando o caminho da raylib ao LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/raylib/lib

# verificando se tudo foi configurado corretamente
if gcc --version >/dev/null 2>&1 && make --version >/dev/null 2>&1 && [ -d "$(pwd)/raylib/lib" ]; then
    echo "Configuração concluída com sucesso!"
    echo "AVISO: Antes de compilar, não se esqueça do comando `make clean` para limpar os arquivos antigos."
else
    echo "Erro durante a configuração. Verifique as dependências manualmente, ou clone o repositório novamente."
fi