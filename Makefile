# libgeom - Biblioteca de Geometria Computacional em C

# Configuração
LIB_NAME	:= geom

CC		 	:= gcc
CFLAGS		:= -std=c11 -Wall -Wextra -pedantic -O2 -Iinclude
DEPFLAGS	:= -MMD -MP
LDLIBS		:= -lm

PREFIX		?= /usr/local
LIBDIR		:= $(PREFIX)/lib
INCLUDEDIR	:= $(PREFIX)/include/$(LIB_NAME)

# Diretórios
SRC_DIR		:= src
INC_DIR		:= include
APP_DIR		:= app
TEST_DIR	:= tests
BUILD_DIR	:= build
LIB_DIR		:= lib
BIN_DIR		:= bin

# Arquivos
HEADERS		:= $(wildcard $(INC_DIR)/*.h)

LIB_SRCS	:= $(wildcard $(SRC_DIR)/*.c)
LIB_OBJS	:= $(LIB_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/lib/%.o)

APP_SRCS	:= $(wildcard $(APP_DIR)/*.c)
APP_OBJS	:= $(APP_SRCS:$(APP_DIR)/%.c=$(BUILD_DIR)/app/%.o)

TEST_SRCS	:= $(wildcard $(TEST_DIR)/test_*.c)
TEST_OBJS	:= $(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/tests/%.o)
TEST_BINS	:= $(TEST_SRCS:$(TEST_DIR)/%.c=$(BIN_DIR)/tests/%)

STATIC_LIB	:= $(LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB	:= $(LIB_DIR)/lib$(LIB_NAME).so

APP_STATIC	:= $(BIN_DIR)/app_static
APP_SHARED	:= $(BIN_DIR)/app_shared

# Regra .PHONY para evitar que os comandos sejam interpretados como arquivos
.PHONY: all run run-static run-shared test install uninstall clean help

# Regra .SECONDARY para evitar que os objetos dos testes sejam apagados por serem intermediários
.SECONDARY: $(TEST_OBJS)

# Alvo padrão
all: $(STATIC_LIB) $(SHARED_LIB) $(APP_STATIC) $(APP_SHARED)

# Regra para compilar os objetos da biblioteca (lembrando do -fPIC para a biblioteca dinâmica)
$(BUILD_DIR)/lib/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPFLAGS) -fPIC -c $< -o $@

# Regra para biblioteca estática
$(STATIC_LIB): $(LIB_OBJS)
	@mkdir -p $(@D)
	ar rcs $@ $^

# Regra para biblioteca dinâmica
$(SHARED_LIB): $(LIB_OBJS)
	@mkdir -p $(@D)
	$(CC) -shared $^ $(LDLIBS) -o $@

# Programa do cliente
$(BUILD_DIR)/app/%.o: $(APP_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Link estático do programa do cliente
$(APP_STATIC): $(APP_OBJS) $(STATIC_LIB)
	@mkdir -p $(@D)
	$(CC) $(APP_OBJS) $(STATIC_LIBS) $(LDLIBS) -o $@

# Link dinâmico do programa do cliente
$(APP_SHARED): $(APP_OBJS) $(SHARED_LIB)
	@mkdir -p $(@D)
	# aqui eu uso -Wl,-rpath para não precisar mexer em LD_LIBRARY_PATH na hora de rodar o programa
	$(CC) $(APP_OBJS) -L$(LIB_DIR) -l$(LIB_NAME) -Wl,-rpath,'$$ORIGIN/../$(LIB_DIR)' $(LDLIBS) -o $@

run: run-static run-shared

# Roda programa do cliente com linking estático estática
run-static: $(APP_STATIC)
	@echo "Rodando programa cliente com linking estático"
	@./$<

# Roda programa do cliente com linking dinâmico
run-shared: $(APP_SHARED)
	@echo "Rodando programa cliente com linking dinâmico"
	@./$<

# Regra para compilar os objetos dos testes
$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) -I$(TEST_DIR) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Regra para linkar o executável dos testes
$(BIN_DIR)/tests/%: $(BUILD_DIR)/tests/%.o $(STATIC_LIB)
	@mkdir -p $(@D)
	$(CC) $< $(STATIC_LIB) $(LDLIBS) -o $@

test: $(TEST_BINS)
	@failed=0; \
	for t in $(TEST_BINS); do \
		echo "--- Rodando teste $$t"; \
		if ./$$t; then \
			echo "O teste $$t passou"; \
		else \
			echo "O teste $$t falhou"; \
			failed=$$((failed + 1)); \
		fi; \
		done; \
	echo "=============================================="; \
	if [ $$failed -eq 0 ]; then \
		echo "RESULTADO: SUCESSO em todos os testes"; \
	else \
		echo "RESULTADO: FALHA em $$failed testes"; \
		exit 1; \
	fi

# Instala (copia) as libs para um diretório com as permissões e proprietários corretos 
install: $(STATIC_LIB) $(SHARED_LIB)
	install -d $(LIBDIR) $(INCLUDEDIR)
	install -m 644 $(STATIC_LIB) $(LIBDIR)
	install -m 755 $(SHARED_LIB) $(LIBDIR)
	install -m 644 $(HEADERS) $(INCLUDEDIR)
	@echo "Instalado em $(PREFIX)"

# Desinstala (deleta) as libs do diretório em questão
uninstall:
	rm -f $(LIBDIR)/$(notdir $(STATIC_LIB)) $(LIBDIR)/$(notdir $(SHARED_LIB))
	rm -rf $(INCLUDEDIR)

# Limpeza dos artefatos criados
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(BIN_DIR)

help:
	@echo "Alvos principais:"
	@echo "  make             Compila a biblioteca (estática e dinâmica) e a aplicação do cliente"
	@echo "  make run         Executa a aplicação ligada à biblioteca estática e à dinâmica"
	@echo "  make test        Compila e executa os testes automatizados"
	@echo "  make install     Instala a biblioteca e os headers em PREFIX (padrão: $(PREFIX))"
	@echo "  make clean       Remove todos os artefatos gerados"
	@echo ""
	@echo "Alvos extras:"
	@echo "  make run-static  Executa apenas a aplicação ligada à biblioteca estática"
	@echo "  make run-shared  Executa apenas a aplicação ligada à biblioteca dinâmica"
	@echo "  make uninstall   Remove os arquivos instalados por \"make install\""
	@echo "  make help        Mostra esta lista de alvos"
	@echo ""
	@echo "Instalação sem privilégios de administrador:"
	@echo '  make install PREFIX=$$HOME/.local'