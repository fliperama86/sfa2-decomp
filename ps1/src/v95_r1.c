/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* Exact. The digit index is written as digits[7 - i] in the loop (no separate pointer local). */
void func_80156398(TextBuf *text, u32 value)
{
  u8 digits[8];
  int i;
  u8 started;
  for (i = 0; i < 8; i++)
  {
    digits[i] = value & 0xf;
    value >>= 4;
  }

  started = 0;
  for (i = 0; i < 7; i++)
  {
    u8 d = digits[7 - i];
    value = d;
    if (value != 0)
    {
      started = 1;
    }
    if (started)
    {
      text->buf[i] = value + 0x30;
    }
    else
    {
      text->buf[i] = 0x20;
    }
  }

  text->buf[i] = digits[7 - i] + 0x30;
  func_801519b4(text);
}
