CC = gcc
CFLAGS = -Wall -Wextra -std=c17
LDFLAGS = -lm

BUILD_DL_FLAGS = -Wall -Wextra -shared -fPIC

# Directories
SRC_DIR = .
TEST_DIR = tests
BIN_DIR = bin

# Source files
SRC = $(SRC_DIR)/spDa.c
HEADER = $(SRC_DIR)/spDa.h
OBJ = $(SRC_DIR)/spDa.o

# Executables
BASIC_TEST = $(BIN_DIR)/basic_test
MAIN_TEST = $(BIN_DIR)/main_test
UTILITY_TEST = $(BIN_DIR)/utility_test

# Targets
.PHONY: all clean

all: $(BASIC_TEST) $(MAIN_TEST) $(UTILITY_TEST)

$(BASIC_TEST): $(SRC) $(TEST_DIR)/basic.c $(HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(TEST_DIR)/basic.c -o $@ $(LDFLAGS)

$(MAIN_TEST): $(SRC) $(TEST_DIR)/test.c $(HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(TEST_DIR)/test.c -o $@ $(LDFLAGS)

$(UTILITY_TEST): $(SRC) $(TEST_DIR)/utility.c $(HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(TEST_DIR)/utility.c -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $@

play:
	$(CC) $(CFLAGS) -o ./tests/playground ./tests/playground.c $(SRC)


build:
	$(CC) $(BUILD_DL_FLAGS) -o libspda.so $(SRC)

clean:
	rm -rf $(BIN_DIR)
