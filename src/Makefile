CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic

UNAME = $(shell uname -s)
ifeq ($(UNAME), Linux)
	CHECK_FLAGS = -lcheck -lm -lrt -lpthread -lsubunit
endif
ifeq ($(UNAME), Darwin)
	CHECK_FLAGS = -lcheck
endif

SOURCES = s21_string.c s21_strerror.c s21_sprintf.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean test s21_string.a gcov_report style format

all: s21_string.a

s21_string.a: $(OBJECTS)
	ar rcs $@ $^

%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

test: s21_string.a
	$(CC) $(CFLAGS) tests/test_s21_string.c $< -o tests/s21_test $(CHECK_FLAGS)
	./tests/s21_test

gcov_report:
	mkdir -p report
	$(CC) $(CFLAGS) --coverage $(SOURCES) tests/test_s21_string.c -o s21_test $(CHECK_FLAGS)
	./s21_test
	gcovr --html --html-details -o report/coverage_report.html --exclude tests/test_s21_string.c
	rm -f *.gcda *.gcno s21_test

clean:
	rm -f $(OBJECTS) s21_string.a
	rm -f tests/s21_test
	rm -rf report

style:
	clang-format -n --style=file *.c *.h tests/*.c

format:
	clang-format -i --style=file *.c *.h tests/*.c