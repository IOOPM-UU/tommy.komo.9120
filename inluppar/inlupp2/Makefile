CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRC = utils.c ui.c backend.c hash_table.c linked_list.c iterator.c
OBJ = $(SRC:.c=.o)

TARGET = shop

# -------------------------------------------------------------------
# Build the main program
# -------------------------------------------------------------------
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# -------------------------------------------------------------------
# Build unit tests using CUnit, as requested
# -------------------------------------------------------------------
TEST_SRC = unit_tests.c backend.c hash_table.c linked_list.c iterator.c utils.c
TEST_TARGET = unit_tests

tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -lcunit -o $(TEST_TARGET)

# -------------------------------------------------------------------
# Run tests normally
# -------------------------------------------------------------------
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# -------------------------------------------------------------------
# Run tests with Valgrind memory checking
# -------------------------------------------------------------------
memtest: $(TEST_TARGET)
	valgrind --leak-check=full ./$(TEST_TARGET)

# -------------------------------------------------------------------
# Utility rules
# -------------------------------------------------------------------
clean:
	rm -f $(OBJ) $(TARGET) $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)
