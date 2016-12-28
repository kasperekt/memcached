CC=clang
CFLAGS=-Wall

SRCDIR=src
OBJDIR=obj
SRC=main.c client.c files.c cache.c
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))
EXE=memcached

FILES_STORAGE_DIR=/Users/tomek/University/temp/memcached_test

all: clean prepare $(EXE)

# Cache size is 1024 * 1024 * 10 = 10485760 (10MB)
# Other values:
# 10MB = 10485760
# 20MB = 20971520
# 30MB = 31457280
run: prepare $(EXE)
	./$(EXE) 0.0.0.0 1234 $(FILES_STORAGE_DIR) 104857600

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

lint:
	splint $(SRCDIR)/*.c +posixlib

prepare:
	if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi

clean:
	rm -r $(OBJDIR)
	rm $(EXE)