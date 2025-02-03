[![Super-Trunfo](assets/img/telas/tela_inicial_cpuwin(800x600).png)](Super-Trunfo)

## Feito com: 
[![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)](#)
[![RayLib](https://img.shields.io/badge/RAYLIB-FFFFFF?style=for-the-badge&logo=raylib&logoColor=black)](https://github.com/raysan5/raylib/)

## Rodando no Linux
#### Dê permissão ao arquivo `install dependencies.sh` para ser executável:

```bash
chmod +x install_dependencies.sh
./install_dependencies.sh
```
##### Serão instalados todos os pacotes necessários para rodar o projeto.

## Rodando no Windows
- Instale o [mingw-w64](https://winlibs.com/) em C:/ e <strong>adicione ao PATH do sistema</strong>
##### O jogo irá compilar e rodar normalmente.
<br>

## COMANDOS MAKE

```bash
make            # compilar
make run        # EXECUTAR
make clean      # limpar arquivos .o
make clean_all  # limpar arquivos .o / executável
make reset      # limpar tudo
```

### Alertas
- <b>Projeto ainda não finalizado!</b>
- Se o comando `make` não funciona, tente `mingw32-make`.
- Verifique se o `mingw-w64` está instalado e configurado corretamente no seu PATH.
- Caso esteja usando o `MinGW` antigo, o projeto não irá compilar.
