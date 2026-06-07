#include "s21_string.h"

typedef struct {
  int minus;
  int plus;
  int space;
  int width;
  int precision;
  int is_long;
  int is_short;
} s21_spec;

static void s21_itoa(long long num, char *buffer, int *pos) {
  if (num < 0) {
    buffer[(*pos)++] = '-';
    num = -num;
  }

  if (num >= 10) {
    s21_itoa(num / 10, buffer, pos);
  }
  buffer[(*pos)++] = (num % 10) + '0';
}

static void s21_uitoa(unsigned long long num, char *buffer, int *pos) {
  if (num >= 10) {
    s21_uitoa(num / 10, buffer, pos);
  }
  buffer[(*pos)++] = (num % 10) + '0';
}

static void s21_ftoa_int_part(long long num, char *buffer, int *pos) {
  if (num == 0) {
    buffer[(*pos)++] = '0';
  } else {
    char temp[64];
    int temp_pos = 0;
    unsigned long long abs_num = (num < 0) ? -num : num;
    s21_uitoa(abs_num, temp, &temp_pos);

    for (int i = 0; i < temp_pos; i++) {
      buffer[(*pos)++] = temp[i];
    }
  }
}

static void s21_ftoa_frac_part(long long frac, int precision, char *buffer,
                               int *pos) {
  if (precision > 0) {
    buffer[(*pos)++] = '.';

    char temp[64];
    int temp_pos = 0;

    for (int i = 0; i < precision; i++) {
      temp[temp_pos++] = (frac % 10) + '0';
      frac /= 10;
    }

    for (int i = temp_pos - 1; i >= 0; i--) {
      buffer[(*pos)++] = temp[i];
    }
  }
}

static void s21_ftoa(double num, int precision, char *buffer, int *pos) {
  if (precision < 0) {
    precision = 6;
  }

  double abs_num = (num < 0) ? -num : num;
  long long integer_part = (long long)abs_num;
  double fractional_part = abs_num - integer_part;

  long long multiplier = 1;
  for (int i = 0; i < precision; i++) {
    multiplier *= 10;
  }

  long long fractional_int = (long long)(fractional_part * multiplier + 0.5);

  if (fractional_int >= multiplier) {
    fractional_int = 0;
    integer_part++;
  }

  s21_ftoa_int_part(integer_part, buffer, pos);
  s21_ftoa_frac_part(fractional_int, precision, buffer, pos);
}

static void s21_parse_flags(const char **f, s21_spec *spec) {
  while (**f == '-' || **f == '+' || **f == ' ') {
    if (**f == '-') {
      spec->minus = 1;
    } else if (**f == '+') {
      spec->plus = 1;
    } else if (**f == ' ') {
      spec->space = 1;
    }
    (*f)++;
  }
}

static void s21_parse_width(const char **f, s21_spec *spec) {
  if (**f >= '0' && **f <= '9') {
    spec->width = 0;
    while (**f >= '0' && **f <= '9') {
      spec->width = spec->width * 10 + (**f - '0');
      (*f)++;
    }
  }
}

static void s21_parse_precision(const char **f, s21_spec *spec) {
  if (**f == '.') {
    (*f)++;
    spec->precision = 0;
    while (**f >= '0' && **f <= '9') {
      spec->precision = spec->precision * 10 + (**f - '0');
      (*f)++;
    }
  }
}

static void s21_parse_length(const char **f, s21_spec *spec) {
  if (**f == 'l') {
    spec->is_long = 1;
    (*f)++;
  } else if (**f == 'h') {
    spec->is_short = 1;
    (*f)++;
  }
}

static void s21_put_char(char c, s21_spec spec, char **str) {
  int padding = (spec.width > 1) ? spec.width - 1 : 0;

  if (!spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }

  *(*str)++ = c;

  if (spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }
}

static void s21_put_string(const char *s, s21_spec spec, char **str) {
  const char *src = (s != S21_NULL) ? s : "(null)";
  int len = 0;
  int src_len = s21_strlen(src);
  len = (spec.precision >= 0 && spec.precision < src_len) ? spec.precision
                                                          : src_len;

  int padding = (spec.width > len) ? spec.width - len : 0;

  if (!spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }

  for (int i = 0; i < len; i++) {
    *(*str)++ = src[i];
  }

  if (spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }
}

static void s21_put_int(long long num, s21_spec spec, char **str) {
  char buffer[128];
  int pos = 0;
  int sign_char = 0;

  if (num < 0) {
    sign_char = '-';
    num = -num;
  } else if (spec.plus) {
    sign_char = '+';
  } else if (spec.space) {
    sign_char = ' ';
  }

  if (!(num == 0 && spec.precision == 0)) {
    s21_itoa(num, buffer, &pos);
  }

  int num_len = pos;
  int sign_len = (sign_char != 0) ? 1 : 0;
  int zeros = (spec.precision > num_len) ? spec.precision - num_len : 0;
  int total_len = num_len + sign_len + zeros;
  int padding = (spec.width > total_len) ? spec.width - total_len : 0;

  if (!spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }

  if (sign_char != 0) {
    *(*str)++ = sign_char;
  }

  for (int i = 0; i < zeros; i++) {
    *(*str)++ = '0';
  }

  for (int i = 0; i < num_len; i++) {
    *(*str)++ = buffer[i];
  }

  if (spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }
}

static void s21_put_uint(unsigned long long num, s21_spec spec, char **str) {
  char buffer[128];
  int pos = 0;

  if (!(num == 0 && spec.precision == 0)) {
    s21_uitoa(num, buffer, &pos);
  }

  int num_len = pos;
  int zeros = (spec.precision > num_len) ? spec.precision - num_len : 0;
  int total_len = num_len + zeros;
  int padding = (spec.width > total_len) ? spec.width - total_len : 0;

  if (!spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }

  for (int i = 0; i < zeros; i++) {
    *(*str)++ = '0';
  }

  for (int i = 0; i < num_len; i++) {
    *(*str)++ = buffer[i];
  }

  if (spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }
}

static void s21_put_float(double num, s21_spec spec, char **str) {
  char buffer[512];
  int pos = 0;
  int sign_char = 0;
  double abs_num = (num < 0) ? -num : num;

  if (num < 0) {
    sign_char = '-';
  } else if (spec.plus) {
    sign_char = '+';
  } else if (spec.space) {
    sign_char = ' ';
  }

  int precision = (spec.precision >= 0) ? spec.precision : 6;
  s21_ftoa(abs_num, precision, buffer, &pos);

  int num_len = pos;
  int sign_len = (sign_char != 0) ? 1 : 0;
  int total_len = num_len + sign_len;
  int padding = (spec.width > total_len) ? spec.width - total_len : 0;

  if (!spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }

  if (sign_char != 0) {
    *(*str)++ = sign_char;
  }

  for (int i = 0; i < num_len; i++) {
    *(*str)++ = buffer[i];
  }

  if (spec.minus) {
    for (int i = 0; i < padding; i++) {
      *(*str)++ = ' ';
    }
  }
}

static void s21_handle_c(va_list *args, s21_spec spec, char **str) {
  char arg = (char)va_arg(*args, int);
  s21_put_char(arg, spec, str);
}

static void s21_handle_d(va_list *args, s21_spec spec, char **str) {
  long long num;

  if (spec.is_long) {
    num = va_arg(*args, long);
  } else if (spec.is_short) {
    num = (short)va_arg(*args, int);
  } else {
    num = va_arg(*args, int);
  }

  s21_put_int(num, spec, str);
}

static void s21_handle_u(va_list *args, s21_spec spec, char **str) {
  unsigned long long num;

  if (spec.is_long) {
    num = va_arg(*args, unsigned long);
  } else if (spec.is_short) {
    num = (unsigned short)va_arg(*args, unsigned int);
  } else {
    num = va_arg(*args, unsigned int);
  }

  s21_put_uint(num, spec, str);
}

static void s21_handle_f(va_list *args, s21_spec spec, char **str) {
  double num = va_arg(*args, double);
  s21_put_float(num, spec, str);
}

static void s21_handle_s(va_list *args, s21_spec spec, char **str) {
  char *arg = va_arg(*args, char *);
  s21_put_string(arg, spec, str);
}

static void s21_init_spec(s21_spec *spec) {
  spec->minus = 0;
  spec->plus = 0;
  spec->space = 0;
  spec->width = 0;
  spec->precision = -1;
  spec->is_long = 0;
  spec->is_short = 0;
}

int s21_sprintf(char *str, const char *format, ...) {
  if (str == S21_NULL || format == S21_NULL) {
    return 0;
  }

  va_list args;
  va_start(args, format);
  char *start = str;

  for (const char *f = format; *f != '\0'; f++) {
    if (*f != '%') {
      *str++ = *f;
    } else {
      f++;

      s21_spec spec;
      s21_init_spec(&spec);

      s21_parse_flags(&f, &spec);
      s21_parse_width(&f, &spec);
      s21_parse_precision(&f, &spec);
      s21_parse_length(&f, &spec);

      switch (*f) {
        case 'c':
          s21_handle_c(&args, spec, &str);
          break;
        case 'd':
        case 'i':
          s21_handle_d(&args, spec, &str);
          break;
        case 'u':
          s21_handle_u(&args, spec, &str);
          break;
        case 'f':
          s21_handle_f(&args, spec, &str);
          break;
        case 's':
          s21_handle_s(&args, spec, &str);
          break;
        case '%':
          *str++ = '%';
          break;
        default:
          *str++ = '%';
          *str++ = *f;
          break;
      }
    }
  }

  *str = '\0';
  va_end(args);

  return str - start;
}