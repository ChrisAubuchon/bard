#include <stdint.h>

void decryptName(uint8_t *src, uint8_t *dest)
{
  int i;

  for (i = 0; i < 16; i++) {
    *dest = *src++ & 0x7f;
    if (*dest == 0x7f) {
      *dest = '\0';
      break;
    }
    dest++;
  }
}

void str2macro(uint8_t *str)
{
  while (*str != '\0') {
    if ((*str == ' ') || (*str == '\'') || (*str == '[') || (*str == ']') || (*str == '!') || (*str == '.'))
      *str = '_';
    *str = tolower(*str);
    str++;
  }
}
