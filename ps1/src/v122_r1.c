/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search, then cleaned by hand. */
/* The search's variant assigned the object to a local that nothing read. */
int func_80128814(Object *object)
{
  s16 a;
  s16 base;
  u8 h;
  if (object->field_278 != 0)
  {
    return 0;
  }
  if (object->field_279 != 0)
  {
    return 0;
  }
  if (object->field_06 == 0)
  {
    return 0x60000;
  }
  if (object->kind == 0xf)
  {
    if ((object->frame->field_0c & 0x80) == 0)
    {
      return 0;
    }
    base = (((ObjectView *) object)->field_33b) ? (0xc) : (0);
    a = base;
    if (object->field_128 != 0)
    {
      a = base + 6;
    }
    if (object->field_129 != 0)
    {
      a += 3;
    }
    h = object->field_12a;
    ref_first.p = (Object *) table_801717f0;
    return ((s32 *) ref_first.p)[(s16) (a + (h >> 1))];
  }
  else
  {
    int v;
    u8 k;
    FrameRecord *fr;
    ref_first.p = (Object *) table_8016fa38[object->kind];
    base = (object->field_128) ? (6) : (0);
    a = base;
    if (object->field_129 != 0)
    {
      a = 3;
      a = base + a;
    }
    k = object->field_12a;
    fr = object->frame;
    v = ((s32 *) ref_first.p)[(s16) (a + (k >> 1))];
    k = fr->field_0c;
    k = k & 0x80;
    if (k != 0)
    {
      v = 0;
    }
    return v;
  }
}
