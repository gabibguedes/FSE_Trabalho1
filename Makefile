# CC = /Users/gabibs/.armv7-unknown-linux-gnueabihf/bin/armv7-unknown-linux-gnueabihf-gcc
CC = gcc
# LDFLAGS = -lwiringPi -lpthread -lncurses
LDFLAGS =
BLDDIR = .
INCDIR = $(BLDDIR)/inc
SRCDIR = $(BLDDIR)/src
OBJDIR = $(BLDDIR)/obj
CFLAGS = -c -Wall -I$(INCDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = bin/bin

all: clean $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJDIR)/*.o -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJDIR)/*.o $(EXE)

run:
	$(EXE)
