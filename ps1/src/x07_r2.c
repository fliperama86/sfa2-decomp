/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80168d4c(u8 *a, int b);
int func_80166144(u8 *a, int b, int c);

void func_8014ff20(Stream *stream)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[8];
  u32 v;
  u32 n;
  u8 **t;
  u8 *p;
  if (stream->field_24 == 0)
  {
    t = data_8017eb1c[stream->field_0b];
    v = *(u32 *)stream->field_40;
    n = v & 0xff;
    n = n + ((v & 0xff00) >> 6);
    p = t[n];
    stream->field_38 = p;
    stream->field_44 = p;
  }
  func_8015003c(stream);
  stream->field_24++;
  if (stream->field_30 == 0)
  {
    u8 *r = stream->field_40;
    u32 k = r[0];
    n = r[1];
    if (n == 0)
    {
      data_80190a44[k + 4] = func_80168d4c(stream->field_44, data_80190a44[k]);
    }
    else
    {
      data_80190a44[k + 8] = func_80166144(stream->field_44, data_80190a44[k], ((s16 *)r)[6]);
    }
  }
}
