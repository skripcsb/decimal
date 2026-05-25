CFLAGS = -Wall -Wextra -Werror -std=c11 -g
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

UNAME_S := $(shell uname -s)

CC = gcc
GCOV_CMD = gcov
ifeq ($(UNAME_S), Linux)
	TEST_FLAGS = -lcheck -lm -lpthread -lsubunit
else
	TEST_FLAGS = -lcheck -lm -lpthread
endif

AR = ar
ARFLAGS = rcs

SOURCES = $(wildcard s21_decimal_*.c)
OBJECTS = $(SOURCES:.c=.o)

TEST_SOURCES = $(wildcard tests/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

LIBRARY = s21_decimal.a

TEST_EXEC = test_s21_decimal

.PHONY: all clean test gcov_report

all: s21_decimal.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

s21_decimal.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_FLAGS)

gcov_report:
	@echo "Running tests with coverage flags"
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SOURCES) $(TEST_SOURCES) -o $(TEST_EXEC)_gcov $(TEST_FLAGS)
	-./$(TEST_EXEC)_gcov
	@echo "Creating coverage report"
	for file in $(SOURCES); do \
		base_name=$$(basename $$file .c); \
		if [ -f $(TEST_EXEC)_gcov-$$base_name.gcno ]; then \
			cp $(TEST_EXEC)_gcov-$$base_name.gcno $$base_name.gcno; \
		fi; \
		if [ -f $(TEST_EXEC)_gcov-$$base_name.gcda ]; then \
			cp $(TEST_EXEC)_gcov-$$base_name.gcda $$base_name.gcda; \
		fi; \
	done
	$(GCOV_CMD) $(SOURCES)
	lcov -t "s21_decimal" -o s21_decimal.info -c -d .
	lcov -o s21_decimal_filtered.info -r s21_decimal.info "tests/*"
	genhtml -o report s21_decimal_filtered.info
	@echo "========================================"
	@echo "Coverage report is done!"
	@echo "file://$(shell pwd)/report/index.html"

clean:
	rm -f *.o *.a *.d *.gcno *.gcda *.gcov *.info $(TEST_EXEC) $(TEST_EXEC)_gcov $(TEST_OBJECTS)
	rm -rf report