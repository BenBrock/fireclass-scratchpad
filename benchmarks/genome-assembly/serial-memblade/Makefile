CC = gcc
CXX = g++
C_INCLUDE_PATH = ../../../mock-interface
CPLUS_INCLUDE_PATH = $(C_INCLUDE_PATH)

CPP_FLAGS = -O3 -std=gnu++14

MOCK_PATH = ../../../mock-interface
MOCK_SRC = $(MOCK_PATH)/remote_scratchpad.c
MOCK_OBJ = $(MOCK_SRC:.c=.o)

all: assemble

assemble: assemble.cpp $(MOCK_OBJ)
	$(LINK.cpp) $< -o $@ -I$(CPLUS_INCLUDE_PATH) $(MOCK_OBJ) $(CPP_FLAGS)

$MOCK_OBJ: $(MOCK_SRC)
	$(CC) -c $<
