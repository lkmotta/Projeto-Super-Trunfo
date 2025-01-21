CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -Iinclude -Iraylib/include
LDFLAGS = -Lraylib/lib -lraylib -lgdi32 -lwinmm

# Definindo para Windows
ifeq ($(OS),Windows_NT)
    EXECUTABLE := trunfo.exe
    DEL_CMD := del /f /q
    RUN_CMD := .\\$(EXECUTABLE)
else
    EXECUTABLE := trunfo
    DEL_CMD := rm -f
    RUN_CMD := ./$(EXECUTABLE)
endif

# Diretorios e arquivos
SRC = src/main.c src/funcaux.c src/filechange.c src/game.c src/interface.c
HEADERS = include/funcaux.h include/filechange.h include/game.h include/interface.h
OBJ = $(SRC:.c=.o)

# Regras de compilacao
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(EXECUTABLE)
	gdb $(RUN_CMD)

run: $(EXECUTABLE)
	$(RUN_CMD)

clean:
	$(DEL_CMD) $(OBJ)

clean_all: clean
	$(DEL_CMD) $(EXECUTABLE)

reset: clean_all
	$(DEL_CMD) assets\\data\\arqbin.dat
