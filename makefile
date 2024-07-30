CC = gcc
CFLAGS = -Wall -Wextra 
LDFLAGS = -lm

BUILDFLAGS = -Wall -Wextra -c -fPIC

# Directories
SRC_DIR = .
TEST_DIR = tests
BIN_DIR = bin

# Source files
SRC = $(SRC_DIR)/spDa.c
HEADER = $(SRC_DIR)/spDa.h
OBJ = $(SRC_DIR)/spDa.o

# Test executables
BASIC_TEST = $(BIN_DIR)/basic_test
MAIN_TEST = $(BIN_DIR)/main_test

# Targets
.PHONY: all clean test

all: $(BASIC_TEST) $(MAIN_TEST)

$(BASIC_TEST): $(SRC) $(TEST_DIR)/basic.c $(HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(TEST_DIR)/basic.c -o $@ $(LDFLAGS)

$(MAIN_TEST): $(SRC) $(TEST_DIR)/test.c $(HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(TEST_DIR)/test.c -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $@

build:
	$(CC) -shared -o libspda.so -fPIC $(SRC)

test: $(BASIC_TEST) $(MAIN_TEST)
	./$(BASIC_TEST)
	./$(MAIN_TEST)

clean:
	rm -rf $(BIN_DIR)