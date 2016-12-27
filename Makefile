CC=clang
CFLAGS=-Wall

SRCDIR=src
OBJDIR=obj
SRC=main.c client.c files.c
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))
EXE=memcached

all: clean prepare $(EXE)

run: prepare $(EXE)
	./$(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

lint:
	splint $(SRCDIR)/*.c

prepare:
	if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi

clean:
	rm -r $(OBJDIR)
	rm $(EXE)