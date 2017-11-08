
CPP_FLAGS = -O3 -std=gnu++17

MBU_PATH = ../../../memblade-user
MOCK_PATH = ../../../mock-interface

MOCK_SRC = $(MOCK_PATH)/remote_scratchpad.c
MOCK_OBJ = $(MOCK_SRC:.c=.o)

CC = gcc
CXX = g++
C_INCLUDE_PATH = -I$(MOCK_PATH) -I$(MBU_PATH)
CPLUS_INCLUDE_PATH = $(C_INCLUDE_PATH)

all: assemble

assemble: assemble.cpp $(MOCK_OBJ)
	$(LINK.cpp) $< -o $@ $(CPLUS_INCLUDE_PATH) $(MOCK_OBJ) $(CPP_FLAGS)

$MOCK_OBJ: $(MOCK_SRC)
	$(CC) -c $<
