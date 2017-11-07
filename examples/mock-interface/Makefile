CC = gcc
CXX = g++
C_INCLUDE_PATH = ../../mock-interface
CPLUS_INCLUDE_PATH = $(C_INCLUDE_PATH)

MOCK_PATH = ../../mock-interface
MOCK_SRC = $(MOCK_PATH)/remote_scratchpad.c
MOCK_OBJ = $(MOCK_SRC:.c=.o)

all: simple_rmem simple_scratch_malloc malloc_compaction

simple_rmem: simple_rmem.c $(MOCK_OBJ)
	$(LINK.c) $< -o $@ -I$(C_INCLUDE_PATH) $(MOCK_OBJ)

simple_scratch_malloc: simple_scratch_malloc.cpp $(MOCK_OBJ)
	$(LINK.cpp) $< -o $@ -I$(C_INCLUDE_PATH) $(MOCK_OBJ)

malloc_compaction: malloc_compaction.cpp $(MOCK_OBJ)
	$(LINK.cpp) $< -o $@ -I$(C_INCLUDE_PATH) $(MOCK_OBJ)

$MOCK_OBJ: $(MOCK_SRC)
	$(CC) -c $<
