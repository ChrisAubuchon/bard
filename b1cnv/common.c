#include <b1lib.h>

void getSavingThrow(uint8_t type, uint8_t *lo, uint8_t *hi)
{
  *lo = ((type == 0) ? 32 : type) >> 3;
  *hi = (((type == 0) ? 32 : type) >> 3) + 7;
}
