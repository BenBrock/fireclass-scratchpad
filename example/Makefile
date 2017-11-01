CC = gcc
CXX = g++
C_INCLUDE_PATH = ../mock-interface
CPLUS_INCLUDE_PATH = $(C_INCLUDE_PATH)

MOCK_PATH = ../mock-interface
MOCK_SRC = $(MOCK_PATH)/remote_scratchpad.c
MOCK_OBJ = $(MOCK_SRC:.c=.o)

all: simple scratch_malloc

simple: simple.c $(MOCK_OBJ)
	$(LINK.c) $< -o $@ -I$(C_INCLUDE_PATH) $(MOCK_OBJ)

scratch_malloc: scratch_malloc.cpp $(MOCK_OBJ)
	$(LINK.cpp) $< -o $@ -I$(C_INCLUDE_PATH) $(MOCK_OBJ)

$MOCK_OBJ: $(MOCK_SRC)
	$(CC) -c $<
