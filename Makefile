# ==============================
# Makefile - Projeto LI3 (G79)
# ==============================

# Compilador
CC = gcc

# pkg-config (GLib) -> isto EXECUTA o comando e mete o output nas flags
PKG_CFLAGS := $(shell pkg-config --cflags glib-2.0)
PKG_LIBS   := $(shell pkg-config --libs glib-2.0)

# Flags de compilação
CFLAGS = -Ofast -march=native -mtune=native -flto \
         -fomit-frame-pointer -funroll-loops \
         -fprefetch-loop-arrays -ffast-math \
         -finline-functions \
         -Wall -Wextra -Werror -std=c11 \
         -Iinclude $(PKG_CFLAGS)

# Flags de link
LDFLAGS = $(PKG_LIBS)

CCOMPILE = $(CC) $(CFLAGS)

# Fontes principais
MAIN1_SRC = src/apresentacao/programa-principal.c
TEST_SRC  = src/apresentacao/programa-testes.c

# Todos os outros .c (comuns)
COMMON_SRCS := $(filter-out $(MAIN1_SRC) $(TEST_SRC), $(shell find src -name "*.c"))
COMMON_OBJS := $(patsubst src/%.c,objs/%.o,$(COMMON_SRCS))

# Objetos principais
MAIN1_OBJ := $(patsubst src/%.c,objs/%.o,$(MAIN1_SRC))
TEST_OBJ  := $(patsubst src/%.c,objs/%.o,$(TEST_SRC))

# Executáveis
COMMON_MAIN = $(COMMON_OBJS) $(MAIN1_OBJ)
COMMON_TEST = $(COMMON_OBJS) $(TEST_OBJ)

# ==============================
# Regras principais
# ==============================

all: programa-principal programa-testes

programa-principal: $(COMMON_MAIN)
	$(CCOMPILE) -o $@ $(COMMON_MAIN) $(LDFLAGS)

programa-testes: $(COMMON_TEST)
	$(CCOMPILE) -o $@ $(COMMON_TEST) $(LDFLAGS)

# Regra genérica de compilação
objs/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CCOMPILE) -c $< -o $@

clean:
	rm -rf objs programa-principal programa-testes
	rm -rf resultados/*

.PHONY: all clean