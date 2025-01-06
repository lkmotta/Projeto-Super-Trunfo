# Makefile para Projeto Super Trunfo

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS =

# Diretorios e arquivos
SRC = src/main.c src/funcaux.c src/filechange.c src/game.c
HEADERS = include/funcaux.h include/filechange.h include/game.h
OBJ = $(SRC:.c=.o)
TARGET = trunfo

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
	ARQBIN = assets\data\arqbin.dat
	RM = cmd /C "for %%i in (src\*.o) do del /f /q %%i"
	RM_TARGET = cmd /C del /f /q $(TARGET)
	RM_ARQBIN = cmd /C del /f /q $(ARQBIN)
	TARGET := trunfo.exe
	RUN_CMD = $(TARGET)
else
	ARQBIN = assets/data/arqbin.dat
	RM = rm -f
	RM_TARGET = rm -f $(TARGET)
	RM_ARQBIN = rm -f $(ARQBIN)
	RUN_CMD = ./$(TARGET)
endif

# Regras
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM)

clean_all: clean
	$(RM_TARGET)

reset: clean_all
	$(RM_ARQBIN)

run: $(TARGET)
	$(RUN_CMD)