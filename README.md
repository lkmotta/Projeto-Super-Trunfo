<h1 align="center">
    PROJETO SUPER-TRUNFO
    <p style="font-size: 15px; color: rgb(124, 124, 124);">LIGA DA JUSTIÇA</p>
</h1>

<br>

[![Licença](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

<h2 style="border:none;color: red;font-weight: bold;">*EM DESENVOLVIMENTO*</h2><br>

<h2>COMANDOS MAKE</h2>

### Compilação
Para compilar o projeto, utilize o comando:
```bash
mingw32-make all
```
ou simplesmente:
```bash
mingw32-make
```

### Execução
Para executar o projeto, utilize o comando:
```bash
trunfo
```

### Limpeza
Para limpar os arquivos objeto gerados durante a compilação, utilize o comando:
```bash
mingw32-make clean
```
Para limpar todos os arquivos objeto e o executável, utilize o comando:
```bash
mingw32-make clean_all
```

### Alertas
- Certifique-se de ter o `mingw32-make` instalado no seu sistema. Caso contrário, instale-o utilizando o [Chocolatey](https://chocolatey.org/) com o comando:
  ```bash
  choco install make
  ```
- Se estiver utilizando um sistema operacional diferente do Windows, utilize `make` ao invés de `mingw32-make`.
- Verifique se o `gcc` está instalado e configurado corretamente no seu PATH.