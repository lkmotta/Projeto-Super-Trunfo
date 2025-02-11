# compilador e flags
CC = gcc
CFLAGS =
LDFLAGS =

# diretorios e arquivos
SRC = src/main.c src/funcaux.c src/filechange.c src/game.c src/interface.c
HEADERS = include/funcaux.h include/filechange.h include/game.h include/interface.h
OBJ = $(SRC:.c=.o)
EXECUTAVEL = trunfo

# detectando sistema operacional
ifeq ($(OS),Windows_NT)
	CFLAGS += -Wall -Wextra -O2 -g -Iinclude -Iraylib/include
	LDFLAGS += -Lraylib/libwin -lraylib -lgdi32 -lwinmm
	EXECUTAVEL := trunfo.exe
	RUN_CMD = $(EXECUTAVEL)
	ARQBIN = assets\data\arqbin.dat
	HISTORICO = assets\data\historico.dat
	RM = cmd /C "del /f /q src*.o"
	RM_EXECUTAVEL = cmd /C del /f /q $(EXECUTAVEL)
	RM_ARQBIN = cmd /C del /f /q $(ARQBIN)
	RM_HISTORICO = cmd /C del /f /q $(HISTORICO)
else
    CFLAGS = -Wall -Wextra -O2 -Iinclude -Iraylib/include -m64
	LDFLAGS = -Lraylib/lib -lraylib -lm -lpthread -ldl -lrt -lX11
	RUN_CMD = ./$(EXECUTAVEL)
	ARQBIN = assets/data/arqbin.dat
	HISTORICO = assets/data/historico.dat
	RM = rm -f $(OBJ)
	RM_EXECUTAVEL = rm -f $(EXECUTAVEL)
	RM_ARQBIN = rm -f $(ARQBIN)
	RM_HISTORICO = rm -f $(HISTORICO)
endif

# regras
all: $(EXECUTAVEL)

$(EXECUTAVEL): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM)

clean_all: clean
	$(RM_EXECUTAVEL)

reset: clean_all
	$(RM_ARQBIN)
	$(RM_HISTORICO)

run: $(EXECUTAVEL)
	$(RUN_CMD)