CC = gcc
CFLAGS =
LDFLAGS =

# Definindo para Windows
ifeq ($(OS),Windows_NT)
    EXECUTABLE := trunfo.exe
	CFLAGS += -Wall -Wextra -O2 -g -Iinclude -Iraylib/include
	LDFLAGS += -Lraylib/libwin -lraylib -lgdi32 -lwinmm
    DEL_CMD := del /f /q
    RUN_CMD := .\\$(EXECUTABLE)
	ARQBIN = assets\\data\\arqbin.dat
	HISTORICO = assets\\data\\historico.dat
else
    EXECUTABLE := trunfo
    CFLAGS = -Wall -Wextra -O2 -Iinclude -Iraylib/include -m64
	LDFLAGS = -Lraylib/lib -lraylib -lm -lpthread -ldl -lrt -lX11
	DEL_CMD := rm -f
    RUN_CMD := ./$(EXECUTABLE)
	ARQBIN = assets/data/arqbin.dat
	HISTORICO = assets/data/historico.dat
endif

# Diretórios e arquivos
SRC = src/main.c src/funcaux.c src/filechange.c src/game.c src/interface.c
HEADERS = include/funcaux.h include/filechange.h include/game.h include/interface.h
OBJ = $(SRC:.c=.o)

# Regras de compilação
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
	$(DEL_CMD) $(ARQBIN)
	$(DEL_CMD) $(HISTORICO)
