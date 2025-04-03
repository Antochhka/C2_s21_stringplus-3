CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lcheck -lm -lsubunit
AR = ar
ARFLAGS = rcs

TARGET = s21_string.a
TEST_EXEC = test

SRC = $(wildcard core/*.c) utils.c
FILTERED_SRC = $(filter-out utils.c, $(SRC))
OBJ = $(SRC:.c=.o)


all: checkmk $(TARGET) $(TEST_EXEC)

checkmk: 
	checkmk test.check > test.c

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: core/%.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: utils.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_EXEC): test.o $(TARGET)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	./$(TEST_EXEC)

test.o: test.c s21_string.h
	$(CC) $(CFLAGS) -c $<

gcov_report: $(TEST_EXEC)
	$(CC) $(CFLAGS) --coverage test.c $(SRC) -o core/s21_str_tst_gcv $(LDFLAGS)
	./core/s21_str_tst_gcv
	lcov -t "s21_string_test_report" -c --directory core -o coverage.info $(addprefix --include=, $(FILTERED_SRC))
	genhtml coverage.info --output-directory coverage_html --legend

format:
	clang-format -i **/*.c *.c

clean:
	rm -f *.o *.a $(TEST_EXEC) ./core/s21_str_tst_gcv coverage.info test.c
	rm -rf coverage_html *.gcno *.gcda
	rm core/*.o core/*.gcno core/*.gcda
