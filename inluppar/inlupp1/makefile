#=== Compiler and options ===
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g -std=c99 -I.
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit

#=== Source files ===
SRC_FILES      = hash_table.c linked_list.c iterator.c 
MAIN_SRC	   = frec-count.c
TEST_SRC       = unit_tests.c 
OBJ_FILES      = $(SRC_FILES:.c=.o)

#=== Targets ===
all: unit_tests

#=== Build main program ===
frec-count: $(OBJ_FILES) $(MAIN_SRC)
	$(C_COMPILER) $(C_OPTIONS) $(OBJ_FILES) $(MAIN_SRC) -o $@ $(C_LINK_OPTIONS)

#=== Build object files ===
%.o: %.c
	$(C_COMPILER) $(C_OPTIONS) -c $< -o $@

#=== Link test executable ===
unit_tests: $(OBJ_FILES) $(TEST_SRC)
	$(C_COMPILER) $(C_OPTIONS) $(OBJ_FILES) $(TEST_SRC) -o $@ $(CUNIT_LINK)

#=== Run tests ===
test: unit_tests
	./unit_tests

#=== Run valgrind memory tests ===
memtest: unit_tests
	valgrind --leak-check=full ./unit_tests

#=== Clean build artifacts ===
clean:
	rm -f *.o unit_tests
