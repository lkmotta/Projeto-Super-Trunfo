#!/bin/bash

echo "Instalando dependências necessárias para compilar e rodar o projeto..."

read -p "Quer instalar as dependências necessárias? [Y/n] " resposta

if [ "$resposta" = "Y" ] || [ "$resposta" = "y" ] || [ -z "$resposta" ]; then
    # atualizando pacotes e instalando gcc e make
    echo "Atualizando pacotes e verificando se gcc e make estão instalados..."
    sudo apt update && sudo apt install -y gcc make

    # instalando dependências do raylib
    echo "Instalando dependências do Raylib..."

    os_name=$(grep '^NAME=' /etc/os-release | cut -d= -f2 | tr -d '"')

    if [ "$os_name" = "Ubuntu" ]; then
        # UBUNTU
        sudo apt install -y libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
    elif [ "$os_name" = "Fedora" ]; then
        # FEDORA
        sudo dnf install -y alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
    elif [ "$os_name" = "Arch Linux" ]; then
        # ARCH LINUX
        sudo pacman -S --noconfirm alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
    elif [ "$os_name" = "Void Linux" ]; then
        # VOID LINUX
        sudo xbps-install -y alsa-lib-devel libglvnd-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel mesa MesaLib-devel
    else
        echo "Sistema operacional não suportado. Abortando a instalação."
        exit 1
    fi

    echo "Dependências instaladas com sucesso!"
fi

# adicionando o caminho da raylib ao LD_LIBRARY_PATH
echo "Adicionando o caminho da raylib ao LD_LIBRARY_PATH..."
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/raylib/lib

# verificando se tudo foi configurado corretamente
if gcc --version >/dev/null 2>&1 && make --version >/dev/null 2>&1 && [ -d "$(pwd)/raylib/lib" ]; then
    echo "Configuração concluída com sucesso!"
    echo "AVISO: Antes de compilar, não se esqueça do comando make clean para limpar os arquivos antigos."
else
    echo "Erro durante a configuração. Verifique as dependências manualmente, ou clone o repositório novamente."
fi