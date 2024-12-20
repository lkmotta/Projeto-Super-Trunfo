# Makefile para Projeto Super Trunfo

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =

# Diretorios e arquivos
SRC = main.c funcaux.c filechange.c
HEADERS = funcaux.h filechange.h
OBJ = $(SRC:.c=.o)
TARGET = trunfo.exe

# Regra padrao: compilar o executavel
all: $(TARGET)

# Regra para criar o executavel
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Regra para compilar arquivos .c em .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos .o e do executavel
clean:
	del /f /q *.o

clean_all: clean
	del /f /q $(TARGET)
