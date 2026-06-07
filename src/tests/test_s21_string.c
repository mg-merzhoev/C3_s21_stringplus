#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

#define BUFFER_SIZE 1024

// ==================== ТЕСТЫ ДЛЯ s21_strlen ====================

START_TEST(test_strlen_normal) {
  const char *str = "Hello, world!";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strlen_empty) {
  const char *str = "";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strlen_null) {
  // Стандартная strlen не обрабатывает NULL, тестируем только свою реализацию
  ck_assert_int_eq(s21_strlen(S21_NULL), 0);
}
END_TEST

START_TEST(test_strlen_whitespace) {
  const char *str = "   \t\n\r";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strlen_special) {
  const char *str = "Hello\x00World";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strlen_long) {
  char str[1000];
  for (int i = 0; i < 999; i++) str[i] = 'a';
  str[999] = '\0';
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_memset ====================

START_TEST(test_memset_normal) {
  char buf1[50] = "Hello World";
  char buf2[50] = "Hello World";
  s21_memset(buf1, '*', 5);
  memset(buf2, '*', 5);
  ck_assert_mem_eq(buf1, buf2, 50);
}
END_TEST

START_TEST(test_memset_zero) {
  char buf1[50] = "Hello";
  char buf2[50] = "Hello";
  s21_memset(buf1, '*', 0);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_memset_full) {
  char buf1[50];
  char buf2[50];
  s21_memset(buf1, 'A', 50);
  memset(buf2, 'A', 50);
  ck_assert_mem_eq(buf1, buf2, 50);
}
END_TEST

START_TEST(test_memset_null) {
  ck_assert_ptr_eq(s21_memset(S21_NULL, 'A', 10), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_memchr ====================

START_TEST(test_memchr_first) {
  char str[] = "Hello World";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 11), memchr(str, 'H', 11));
}
END_TEST

START_TEST(test_memchr_middle) {
  char str[] = "Hello World";
  ck_assert_ptr_eq(s21_memchr(str, 'o', 11), memchr(str, 'o', 11));
}
END_TEST

START_TEST(test_memchr_last) {
  char str[] = "Hello World";
  ck_assert_ptr_eq(s21_memchr(str, 'd', 11), memchr(str, 'd', 11));
}
END_TEST

START_TEST(test_memchr_not_found) {
  char str[] = "Hello World";
  ck_assert_ptr_eq(s21_memchr(str, 'x', 11), memchr(str, 'x', 11));
}
END_TEST

START_TEST(test_memchr_zero_n) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 0), memchr(str, 'H', 0));
}
END_TEST

START_TEST(test_memchr_null) {
  ck_assert_ptr_eq(s21_memchr(S21_NULL, 'a', 5), S21_NULL);
}
END_TEST

START_TEST(test_memchr_binary) {
  unsigned char data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  ck_assert_ptr_eq(s21_memchr(data, 0x03, 5), memchr(data, 0x03, 5));
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_memcmp ====================

START_TEST(test_memcmp_equal) {
  char str1[] = "Hello";
  char str2[] = "Hello";
  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
}
END_TEST

START_TEST(test_memcmp_different) {
  char str1[] = "Hello";
  char str2[] = "Hella";
  int s21_result = s21_memcmp(str1, str2, 5);
  int std_result = memcmp(str1, str2, 5);
  ck_assert((s21_result < 0 && std_result < 0) ||
            (s21_result > 0 && std_result > 0) ||
            (s21_result == 0 && std_result == 0));
}
END_TEST

START_TEST(test_memcmp_partial) {
  char str1[] = "Hello";
  char str2[] = "Help!";
  ck_assert_int_eq(s21_memcmp(str1, str2, 3), memcmp(str1, str2, 3));
}
END_TEST

START_TEST(test_memcmp_zero_n) {
  char str1[] = "Hello";
  char str2[] = "World";
  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_memcpy ====================

START_TEST(test_memcpy_normal) {
  char src[] = "Hello World";
  char dest1[50] = {0};
  char dest2[50] = {0};
  s21_memcpy(dest1, src, 11);
  memcpy(dest2, src, 11);
  ck_assert_mem_eq(dest1, dest2, 50);
}
END_TEST

START_TEST(test_memcpy_partial) {
  char src[] = "Hello World";
  char dest1[50] = {0};
  char dest2[50] = {0};
  s21_memcpy(dest1, src, 5);
  memcpy(dest2, src, 5);
  ck_assert_mem_eq(dest1, dest2, 5);
}
END_TEST

START_TEST(test_memcpy_zero) {
  char src[] = "Hello";
  char dest1[50] = {0};
  char dest2[50] = {0};
  s21_memcpy(dest1, src, 0);
  memcpy(dest2, src, 0);
  ck_assert_mem_eq(dest1, dest2, 50);
}
END_TEST

START_TEST(test_memcpy_null) {
  char dest[50] = {0};
  ck_assert_ptr_eq(s21_memcpy(S21_NULL, dest, 10), S21_NULL);
  ck_assert_ptr_eq(s21_memcpy(dest, S21_NULL, 10), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strncat ====================

START_TEST(test_strncat_normal) {
  char dest1[50] = "Hello ";
  char dest2[50] = "Hello ";
  const char *src = "World!!!";
  s21_strncat(dest1, src, 5);
  strncat(dest2, src, 5);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_strncat_zero) {
  char dest1[50] = "Hello";
  char dest2[50] = "Hello";
  const char *src = "World";
  s21_strncat(dest1, src, 0);
  strncat(dest2, src, 0);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_strncat_more) {
  char dest1[50] = "Hi";
  char dest2[50] = "Hi";
  const char *src = " There!";
  s21_strncat(dest1, src, 10);
  strncat(dest2, src, 10);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_strncat_null_dest) {
  ck_assert_ptr_eq(s21_strncat(S21_NULL, "test", 3), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strchr ====================

START_TEST(test_strchr_first) {
  const char *str = "Hello World";
  ck_assert_ptr_eq(s21_strchr(str, 'H'), strchr(str, 'H'));
}
END_TEST

START_TEST(test_strchr_middle) {
  const char *str = "Hello World";
  ck_assert_ptr_eq(s21_strchr(str, 'o'), strchr(str, 'o'));
}
END_TEST

START_TEST(test_strchr_last) {
  const char *str = "Hello World";
  ck_assert_ptr_eq(s21_strchr(str, 'd'), strchr(str, 'd'));
}
END_TEST

START_TEST(test_strchr_not_found) {
  const char *str = "Hello";
  ck_assert_ptr_eq(s21_strchr(str, 'x'), strchr(str, 'x'));
}
END_TEST

START_TEST(test_strchr_null_char) {
  const char *str = "Hello";
  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));
}
END_TEST

START_TEST(test_strchr_null) {
  ck_assert_ptr_eq(s21_strchr(S21_NULL, 'a'), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strncmp ====================

START_TEST(test_strncmp_equal) {
  const char *str1 = "Hello";
  const char *str2 = "Hello";
  ck_assert_int_eq(s21_strncmp(str1, str2, 5), strncmp(str1, str2, 5));
}
END_TEST

START_TEST(test_strncmp_different) {
  const char *str1 = "Hello";
  const char *str2 = "Hella";
  int s21_result = s21_strncmp(str1, str2, 5);
  int std_result = strncmp(str1, str2, 5);
  ck_assert((s21_result < 0 && std_result < 0) ||
            (s21_result > 0 && std_result > 0));
}
END_TEST

START_TEST(test_strncmp_partial) {
  const char *str1 = "Hello";
  const char *str2 = "Help!";
  ck_assert_int_eq(s21_strncmp(str1, str2, 3), strncmp(str1, str2, 3));
}
END_TEST

START_TEST(test_strncmp_zero) {
  const char *str1 = "Hello";
  const char *str2 = "World";
  ck_assert_int_eq(s21_strncmp(str1, str2, 0), strncmp(str1, str2, 0));
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strncpy ====================

START_TEST(test_strncpy_normal) {
  char dest1[20] = "XXXXXXXXXX";
  char dest2[20] = "XXXXXXXXXX";
  const char *src = "Hello";
  s21_strncpy(dest1, src, 5);
  strncpy(dest2, src, 5);
  ck_assert_mem_eq(dest1, dest2, 20);
}
END_TEST

START_TEST(test_strncpy_less) {
  char dest1[20] = "XXXXXXXXXX";
  char dest2[20] = "XXXXXXXXXX";
  const char *src = "HelloWorld";
  s21_strncpy(dest1, src, 5);
  strncpy(dest2, src, 5);
  ck_assert_mem_eq(dest1, dest2, 20);
}
END_TEST

START_TEST(test_strncpy_more) {
  char dest1[20] = "XX";
  char dest2[20] = "XX";
  const char *src = "Hello";
  s21_strncpy(dest1, src, 7);
  strncpy(dest2, src, 7);
  ck_assert_mem_eq(dest1, dest2, 20);
}
END_TEST

START_TEST(test_strncpy_null) {
  char dest[20];
  ck_assert_ptr_eq(s21_strncpy(S21_NULL, "test", 3), S21_NULL);
  ck_assert_ptr_eq(s21_strncpy(dest, S21_NULL, 3), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strcspn ====================

START_TEST(test_strcspn_normal) {
  const char *str1 = "Hello World";
  const char *str2 = " ";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

START_TEST(test_strcspn_multiple) {
  const char *str1 = "Hello World";
  const char *str2 = "lo";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

START_TEST(test_strcspn_none) {
  const char *str1 = "Hello";
  const char *str2 = "xyz";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

START_TEST(test_strcspn_empty) {
  const char *str1 = "Hello";
  const char *str2 = "";
  ck_assert_int_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}
END_TEST

START_TEST(test_strcspn_null) {
  ck_assert_int_eq(s21_strcspn(S21_NULL, "abc"), 0);
  ck_assert_int_eq(s21_strcspn("abc", S21_NULL), 0);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strerror ====================

START_TEST(test_strerror_valid) {
  for (int i = 0; i < 10; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(test_strerror_invalid) {
  ck_assert_str_eq(s21_strerror(1000), strerror(1000));
}
END_TEST

START_TEST(test_strerror_negative) {
  ck_assert_str_eq(s21_strerror(-1), strerror(-1));
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strpbrk ====================

START_TEST(test_strpbrk_found) {
  const char *str1 = "Hello World";
  const char *str2 = "lo";
  ck_assert_ptr_eq(s21_strpbrk(str1, str2), strpbrk(str1, str2));
}
END_TEST

START_TEST(test_strpbrk_first) {
  const char *str1 = "Hello World";
  const char *str2 = "Wo";
  ck_assert_ptr_eq(s21_strpbrk(str1, str2), strpbrk(str1, str2));
}
END_TEST

START_TEST(test_strpbrk_not_found) {
  const char *str1 = "Hello";
  const char *str2 = "xyz";
  ck_assert_ptr_eq(s21_strpbrk(str1, str2), strpbrk(str1, str2));
}
END_TEST

START_TEST(test_strpbrk_null) {
  ck_assert_ptr_eq(s21_strpbrk(S21_NULL, "abc"), S21_NULL);
  ck_assert_ptr_eq(s21_strpbrk("abc", S21_NULL), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strrchr ====================

START_TEST(test_strrchr_normal) {
  const char *str = "Hello World";
  ck_assert_ptr_eq(s21_strrchr(str, 'o'), strrchr(str, 'o'));
}
END_TEST

START_TEST(test_strrchr_last) {
  const char *str = "Hello World";
  ck_assert_ptr_eq(s21_strrchr(str, 'l'), strrchr(str, 'l'));
}
END_TEST

START_TEST(test_strrchr_not_found) {
  const char *str = "Hello";
  ck_assert_ptr_eq(s21_strrchr(str, 'x'), strrchr(str, 'x'));
}
END_TEST

START_TEST(test_strrchr_null_char) {
  const char *str = "Hello";
  ck_assert_ptr_eq(s21_strrchr(str, '\0'), strrchr(str, '\0'));
}
END_TEST

START_TEST(test_strrchr_null) {
  ck_assert_ptr_eq(s21_strrchr(S21_NULL, 'a'), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strstr ====================

START_TEST(test_strstr_normal) {
  const char *haystack = "Hello World";
  const char *needle = "World";
  ck_assert_ptr_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_strstr_middle) {
  const char *haystack = "Hello World";
  const char *needle = "lo W";
  ck_assert_ptr_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_strstr_not_found) {
  const char *haystack = "Hello World";
  const char *needle = "xyz";
  ck_assert_ptr_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_strstr_empty) {
  const char *haystack = "Hello";
  const char *needle = "";
  ck_assert_ptr_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_strstr_null) {
  ck_assert_ptr_eq(s21_strstr(S21_NULL, "abc"), S21_NULL);
  ck_assert_ptr_eq(s21_strstr("abc", S21_NULL), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_strtok ====================

START_TEST(test_strtok_normal) {
  char str1[] = "Hello,World,How,Are,You";
  char str2[] = "Hello,World,How,Are,You";
  const char *delim = ",";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 && token2) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(S21_NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

START_TEST(test_strtok_multiple_delims) {
  char str1[] = "Hello  World\tHow\nAre,You";
  char str2[] = "Hello  World\tHow\nAre,You";
  const char *delim = " \t\n,";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 && token2) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(S21_NULL, delim);
    token2 = strtok(S21_NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

START_TEST(test_strtok_null_delim) {
  char str[] = "Hello World";
  ck_assert_ptr_eq(s21_strtok(str, S21_NULL), S21_NULL);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ s21_sprintf ====================

START_TEST(test_sprintf_string) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "Hello %s", "World");
  sprintf(buf2, "Hello %s", "World");
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_char) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "Char: %c", 'A');
  sprintf(buf2, "Char: %c", 'A');
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_int) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "Int: %d", 42);
  sprintf(buf2, "Int: %d", 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_unsigned) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "Unsigned: %u", 42);
  sprintf(buf2, "Unsigned: %u", 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_float) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "Float: %.2f", 3.14159);
  sprintf(buf2, "Float: %.2f", 3.14159);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_percent) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "Percent: %%");
  sprintf(buf2, "Percent: %%");
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_minus_flag) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "|%-10s|", "Hi");
  sprintf(buf2, "|%-10s|", "Hi");
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_plus_flag) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%+d %+d", 42, -42);
  sprintf(buf2, "%+d %+d", 42, -42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_space_flag) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "% d % d", 42, -42);
  sprintf(buf2, "% d % d", 42, -42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_combined_flags) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%-+10d", 42);
  sprintf(buf2, "%-+10d", 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_width_int) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "|%10d|%-10d|", 42, 42);
  sprintf(buf2, "|%10d|%-10d|", 42, 42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_width_string) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "|%10s|%-10s|", "Hi", "Hi");
  sprintf(buf2, "|%10s|%-10s|", "Hi", "Hi");
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_width_char) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "|%5c|%-5c|", 'A', 'B');
  sprintf(buf2, "|%5c|%-5c|", 'A', 'B');
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_precision_int) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%.5d %.0d", 42, 0);
  sprintf(buf2, "%.5d %.0d", 42, 0);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_precision_float) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%.2f %.0f %.6f", 3.14159, 3.14159, 3.14159);
  sprintf(buf2, "%.2f %.0f %.6f", 3.14159, 3.14159, 3.14159);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_precision_string) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%.3s %.10s", "Hello", "Hello");
  sprintf(buf2, "%.3s %.10s", "Hello", "Hello");
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_long) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  long val = 1234567890L;
  s21_sprintf(buf1, "Long: %ld %lu", val, val);
  sprintf(buf2, "Long: %ld %lu", val, val);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_short) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  short val = 32000;
  s21_sprintf(buf1, "Short: %hd", val);
  sprintf(buf2, "Short: %hd", val);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_null_string) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%s", S21_NULL);
  sprintf(buf2, "%s", "(null)");
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_unknown_specifier) {
  char buf1[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "Unknown: %q", 42);
  ck_assert_str_eq(buf1, "Unknown: %q");
}
END_TEST

START_TEST(test_sprintf_multiple) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%d %s %c %.2f %%", 42, "Hello", 'A', 3.14);
  sprintf(buf2, "%d %s %c %.2f %%", 42, "Hello", 'A', 3.14);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_negative_numbers) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%d %+d % d", -42, -42, -42);
  sprintf(buf2, "%d %+d % d", -42, -42, -42);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

START_TEST(test_sprintf_boundary_values) {
  char buf1[BUFFER_SIZE] = {0};
  char buf2[BUFFER_SIZE] = {0};
  s21_sprintf(buf1, "%d %u", INT_MAX, UINT_MAX);
  sprintf(buf2, "%d %u", INT_MAX, UINT_MAX);
  ck_assert_str_eq(buf1, buf2);
}
END_TEST

// ==================== MAIN ====================

int main(void) {
  Suite *s = suite_create("s21_string");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strlen_normal);
  tcase_add_test(tc_core, test_strlen_empty);
  tcase_add_test(tc_core, test_strlen_null);
  tcase_add_test(tc_core, test_strlen_whitespace);
  tcase_add_test(tc_core, test_strlen_special);
  tcase_add_test(tc_core, test_strlen_long);

  tcase_add_test(tc_core, test_memset_normal);
  tcase_add_test(tc_core, test_memset_zero);
  tcase_add_test(tc_core, test_memset_full);
  tcase_add_test(tc_core, test_memset_null);

  tcase_add_test(tc_core, test_memchr_first);
  tcase_add_test(tc_core, test_memchr_middle);
  tcase_add_test(tc_core, test_memchr_last);
  tcase_add_test(tc_core, test_memchr_not_found);
  tcase_add_test(tc_core, test_memchr_zero_n);
  tcase_add_test(tc_core, test_memchr_null);
  tcase_add_test(tc_core, test_memchr_binary);

  tcase_add_test(tc_core, test_memcmp_equal);
  tcase_add_test(tc_core, test_memcmp_different);
  tcase_add_test(tc_core, test_memcmp_partial);
  tcase_add_test(tc_core, test_memcmp_zero_n);

  tcase_add_test(tc_core, test_memcpy_normal);
  tcase_add_test(tc_core, test_memcpy_partial);
  tcase_add_test(tc_core, test_memcpy_zero);
  tcase_add_test(tc_core, test_memcpy_null);

  tcase_add_test(tc_core, test_strncat_normal);
  tcase_add_test(tc_core, test_strncat_zero);
  tcase_add_test(tc_core, test_strncat_more);
  tcase_add_test(tc_core, test_strncat_null_dest);

  tcase_add_test(tc_core, test_strchr_first);
  tcase_add_test(tc_core, test_strchr_middle);
  tcase_add_test(tc_core, test_strchr_last);
  tcase_add_test(tc_core, test_strchr_not_found);
  tcase_add_test(tc_core, test_strchr_null_char);
  tcase_add_test(tc_core, test_strchr_null);

  tcase_add_test(tc_core, test_strncmp_equal);
  tcase_add_test(tc_core, test_strncmp_different);
  tcase_add_test(tc_core, test_strncmp_partial);
  tcase_add_test(tc_core, test_strncmp_zero);

  tcase_add_test(tc_core, test_strncpy_normal);
  tcase_add_test(tc_core, test_strncpy_less);
  tcase_add_test(tc_core, test_strncpy_more);
  tcase_add_test(tc_core, test_strncpy_null);

  tcase_add_test(tc_core, test_strcspn_normal);
  tcase_add_test(tc_core, test_strcspn_multiple);
  tcase_add_test(tc_core, test_strcspn_none);
  tcase_add_test(tc_core, test_strcspn_empty);
  tcase_add_test(tc_core, test_strcspn_null);

  tcase_add_test(tc_core, test_strerror_valid);
  tcase_add_test(tc_core, test_strerror_invalid);
  tcase_add_test(tc_core, test_strerror_negative);

  tcase_add_test(tc_core, test_strpbrk_found);
  tcase_add_test(tc_core, test_strpbrk_first);
  tcase_add_test(tc_core, test_strpbrk_not_found);
  tcase_add_test(tc_core, test_strpbrk_null);

  tcase_add_test(tc_core, test_strrchr_normal);
  tcase_add_test(tc_core, test_strrchr_last);
  tcase_add_test(tc_core, test_strrchr_not_found);
  tcase_add_test(tc_core, test_strrchr_null_char);
  tcase_add_test(tc_core, test_strrchr_null);

  tcase_add_test(tc_core, test_strstr_normal);
  tcase_add_test(tc_core, test_strstr_middle);
  tcase_add_test(tc_core, test_strstr_not_found);
  tcase_add_test(tc_core, test_strstr_empty);
  tcase_add_test(tc_core, test_strstr_null);

  tcase_add_test(tc_core, test_strtok_normal);
  tcase_add_test(tc_core, test_strtok_multiple_delims);
  tcase_add_test(tc_core, test_strtok_null_delim);

  tcase_add_test(tc_core, test_sprintf_string);
  tcase_add_test(tc_core, test_sprintf_char);
  tcase_add_test(tc_core, test_sprintf_int);
  tcase_add_test(tc_core, test_sprintf_unsigned);
  tcase_add_test(tc_core, test_sprintf_float);
  tcase_add_test(tc_core, test_sprintf_percent);
  tcase_add_test(tc_core, test_sprintf_minus_flag);
  tcase_add_test(tc_core, test_sprintf_plus_flag);
  tcase_add_test(tc_core, test_sprintf_space_flag);
  tcase_add_test(tc_core, test_sprintf_combined_flags);
  tcase_add_test(tc_core, test_sprintf_width_int);
  tcase_add_test(tc_core, test_sprintf_width_string);
  tcase_add_test(tc_core, test_sprintf_width_char);
  tcase_add_test(tc_core, test_sprintf_precision_int);
  tcase_add_test(tc_core, test_sprintf_precision_float);
  tcase_add_test(tc_core, test_sprintf_precision_string);
  tcase_add_test(tc_core, test_sprintf_long);
  tcase_add_test(tc_core, test_sprintf_short);
  tcase_add_test(tc_core, test_sprintf_null_string);
  tcase_add_test(tc_core, test_sprintf_unknown_specifier);
  tcase_add_test(tc_core, test_sprintf_multiple);
  tcase_add_test(tc_core, test_sprintf_negative_numbers);
  tcase_add_test(tc_core, test_sprintf_boundary_values);

  suite_add_tcase(s, tc_core);

  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}