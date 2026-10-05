/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: NOT EXACT (2 residuals, same in both functions): original keeps the first loop bound
   copied (move a0,v0) after the blez, and puts the flag pointer in t5 and the block
   counter in t4; mine has them swapped. Everything else matches. */
/* Form found by automatic permutation search. */
void func_8011e5c8(unsigned char n, unsigned char *src, unsigned int dst, unsigned int key)
{
  unsigned short flags[4];
  unsigned short i;
  unsigned short j;
  unsigned short count;
  unsigned char value;
  int blocks;
  int nb;
  unsigned short *fp;
  int left;
  int old;
  unsigned int m;
  unsigned int bits;
  m = n;
  nb = (((int) (m - 1)) >> 4) + 1;
  for (i = 0; ((int) i) < ((((int) (m - 1)) >> 4) + 1); i++)
  {
    flags[i] = *((unsigned short *) src);
    src += 2;
  }

  fp = flags;
  blocks = m >> 4;
  m &= 0xf;
  left = blocks;
  left = left - 1;
  if (blocks != 0)
  {
    do
    {
      bits = *(fp++);
      for (i = 0; i < 0x10; i++, bits >>= 1)
      {
        if (bits & 1)
        {
          value = *(src++);
          value = (value >> 4) | ((value & 0xf) << 4);
          count = *(src++);
          for (j = 0; j < count; j++)
          {
            *((unsigned char *) ((dst++) ^ key)) = value;
          }

        }
        else
        {
          value = *(src++);
          value = (value >> 4) | ((value & 0xf) << 4);
          *((unsigned char *) ((dst++) ^ key)) = value;
        }
      }

      old = left;
      left--;
    }
    while (((unsigned short) old) != 0);
  }
  bits = *fp;
  for (i = 0; i < m; i++, bits >>= 1)
  {
    if (bits & 1)
    {
      value = *(src++);
      value = (value >> 4) | ((value & 0xf) << 4);
      count = *(src++);
      for (j = 0; j < count; j++)
      {
        *((unsigned char *) ((dst++) ^ key)) = value;
      }

    }
    else
    {
      value = *(src++);
      value = (value >> 4) | ((value & 0xf) << 4);
      *((unsigned char *) ((dst++) ^ key)) = value;
    }
  }

}
