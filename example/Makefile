CC = gcc
C_INCLUDE_PATH = ../mock-interface

MOCK_PATH = ../mock-interface
MOCK_SRC = $(MOCK_PATH)/remote_scratchpad.c
MOCK_OBJ = $(MOCK_SRC:.c=.o)

all: simple

simple: simple.c $(MOCK_OBJ)
	$(LINK.c) $< -o $@ -I$(C_INCLUDE_PATH) $(MOCK_OBJ)

$MOCK_OBJ: $(MOCK_SRC)
	$(CC) -c $<
