#include <stdint.h>

void str2macro(uint8_t *str)
{
  while (*str != '\0') {
    if ((*str == ' ') || (*str == '\'') || (*str == '[') || (*str == ']') || (*str == '!') || (*str == '.'))
      *str = '_';
    *str = tolower(*str);
    str++;
  }
}
