CC = gcc
CFLAGS = -Wall -Wextra -std=c17
LDFLAGS = -lm
BUILD_DL_FLAGS = -Wall -Wextra -std=c17 -shared -O3

# Directories
SRC_DIR = .
TEST_DIR = tests
BIN_DIR = bin
BUILD_DIR = build

# Source files
SRC = $(SRC_DIR)/spDa.c
HEADER = $(SRC_DIR)/spDa.h
OBJ = $(SRC_DIR)/spDa.o
DLIB = $(BUILD_DIR)/libspda.so

# Executables
BASIC_TEST = $(BIN_DIR)/basic_test
MAIN_TEST = $(BIN_DIR)/main_test

# Targets
.PHONY: all clean build_lib

all: $(BASIC_TEST) $(MAIN_TEST) $(UTILITY_TEST)

$(BASIC_TEST): $(SRC) $(TEST_DIR)/basic.c $(HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(TEST_DIR)/basic.c -o $@ $(LDFLAGS)

$(MAIN_TEST): $(SRC) $(TEST_DIR)/test.c $(HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(TEST_DIR)/test.c -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

play:
	$(CC) $(CFLAGS) -o ./tests/playground ./tests/playground.c $(SRC)

build_lib: $(BUILD_DIR)
	$(CC) $(BUILD_DL_FLAGS) -o $(DLIB) -fPIC $(SRC)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)
