<h1 align="center">
    PROJETO SUPER-TRUNFO
    <p style="font-size: 15px; color: rgb(124, 124, 124);">LIGA DA JUSTIÇA</p>
</h1>

<br>

[![Licença](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Rodando no Linux
#### Dê permissão ao arquivo `install dependencies.sh` para ser executável:

```bash
chmod +x install_dependencies.sh
./install_dependencies.sh
```
##### Serão instalados todos os pacotes necessários para rodar o projeto.

## Rodando no Windows
- Execute o arquivo `verificar_setup_windows.bat`.
- Caso queira modificar o projeto por conta própria, siga as recomendações que aparecerão.

<h2>COMANDOS MAKE</h2>

```bash
mingw32-make            # compilar
mingw32-make run        # EXECUTAR
mingw32-make clean      # limpar arquivos .o
mingw32-make clean_all  # limpar arquivos .o / executável
mingw32-make reset      # limpar tudo
```

### Alertas
- Para compilar NO WINDOWS, siga as orientações do repositório oficial do Raylib: https://github.com/raysan5/raylib/wiki/Working-on-Windows.
- Se estiver utilizando um sistema operacional diferente do Windows, utilize `make` ao invés de `mingw32-make`.
- Verifique se o `mingw-w64` está instalado e configurado corretamente no seu PATH.
- Caso esteja usando o `MinGW` antigo, o projeto não irá compilar.