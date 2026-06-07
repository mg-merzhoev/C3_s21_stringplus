#include "s21_string.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

s21_size_t s21_strlen(const char *str) {
  if (str == S21_NULL) {
    return 0;
  }
  s21_size_t count = 0;
  while (str[count] != '\0') {
    count++;
  }
  return count;
}

void *s21_memchr(const void *str, int c, s21_size_t n) {
  if (str == S21_NULL) {
    return S21_NULL;
  }

  void *result = S21_NULL;

  for (s21_size_t i = 0; i < n && result == S21_NULL; i++) {
    if (((const char *)str)[i] == (char)c) {
      result = (void *)((const char *)str + i);
    }
  }

  return result;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *ptr1 = (const unsigned char *)str1;
  const unsigned char *ptr2 = (const unsigned char *)str2;

  int result = 0;

  for (s21_size_t i = 0; i < n && result == 0; i++) {
    if (ptr1[i] != ptr2[i]) {
      result = ptr1[i] - ptr2[i];
    }
  }

  return result;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  if (dest == S21_NULL || src == S21_NULL) {
    return S21_NULL;
  }

  const unsigned char *ptr_from = (const unsigned char *)src;
  unsigned char *ptr_to = (unsigned char *)dest;

  for (s21_size_t i = 0; i < n; i++) {
    ptr_to[i] = ptr_from[i];
  }
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  if (str == S21_NULL) return S21_NULL;

  for (s21_size_t i = 0; i < n; i++) {
    ((char *)str)[i] = (char)c;
  }

  return str;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  if (dest == S21_NULL || src == S21_NULL) {
    return S21_NULL;
  }

  s21_size_t len_dest = s21_strlen(dest);
  s21_size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[len_dest + i] = src[i];
  }

  dest[len_dest + i] = '\0';

  return dest;
}

char *s21_strchr(const char *str, int c) {
  if (str == S21_NULL) return S21_NULL;

  char *result = S21_NULL;
  s21_size_t len_str = s21_strlen(str);

  for (s21_size_t i = 0; i <= len_str && result == S21_NULL; i++) {
    if (str[i] == (char)c) {
      result = (char *)&str[i];
    }
  }

  return result;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  if (n == 0) return 0;

  int result = 0;

  for (s21_size_t i = 0; i < n && result == 0; i++) {
    if (str1[i] != str2[i]) {
      result = str1[i] - str2[i];
    }
  }

  return result;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  if (dest == S21_NULL || src == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }

  for (; i < n; i++) {
    dest[i] = '\0';
  }
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  if (str1 == S21_NULL || str2 == S21_NULL) return 0;

  s21_size_t len1 = s21_strlen(str1);
  s21_size_t len2 = s21_strlen(str2);
  s21_size_t min_pos = len1;

  for (s21_size_t i = 0; i < len2; i++) {
    for (s21_size_t j = 0; j < len1 && j < min_pos; j++) {
      if (str1[j] == str2[i]) {
        min_pos = j;
      }
    }
  }
  return min_pos;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  if (str1 == S21_NULL || str2 == S21_NULL) {
    return S21_NULL;
  }

  char *result = S21_NULL;

  for (s21_size_t i = 0; str1[i] != '\0' && result == S21_NULL; i++) {
    for (s21_size_t j = 0; str2[j] != '\0' && result == S21_NULL; j++) {
      if (str1[i] == str2[j]) {
        result = (char *)&str1[i];
      }
    }
  }

  return result;
}

char *s21_strrchr(const char *str, int c) {
  if (str == S21_NULL) {
    return S21_NULL;
  }

  char *result = S21_NULL;
  s21_size_t i = 0;

  while (str[i] != '\0') {
    if (str[i] == (char)c) {
      result = (char *)&str[i];
    }
    i++;
  }

  if ((char)c == '\0') {
    result = (char *)&str[i];
  }

  return result;
}

char *s21_strstr(const char *haystack, const char *needle) {
  if (haystack == S21_NULL || needle == S21_NULL) {
    return S21_NULL;
  }

  if (needle[0] == '\0') {
    return (char *)haystack;
  }

  char *result = S21_NULL;

  for (s21_size_t i = 0; haystack[i] != '\0' && result == S21_NULL; i++) {
    s21_size_t j = 0;

    while (haystack[i + j] != '\0' && needle[j] != '\0' &&
           haystack[i + j] == needle[j]) {
      j++;
    }

    if (needle[j] == '\0') {
      result = (char *)&haystack[i];
    }
  }

  return result;
}

char *s21_strtok(char *str, const char *delim) {
  if (delim == S21_NULL) {
    return S21_NULL;
  }

  static char *last = S21_NULL;
  char *result = S21_NULL;

  if (str != S21_NULL) {
    last = str;
  }

  if (last != S21_NULL) {
    while (*last != '\0' && s21_strchr(delim, *last) != S21_NULL) {
      last++;
    }

    if (*last != '\0') {
      result = last;

      while (*last != '\0' && s21_strchr(delim, *last) == S21_NULL) {
        last++;
      }

      if (*last != '\0') {
        *last = '\0';
        last++;
      }
    }
  }

  return result;
}