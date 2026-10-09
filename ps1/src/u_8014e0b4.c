/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8014e25c(Object *object);

/* Form found by automatic permutation search. */
int func_8014e0b4(Object *object)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[4];
  int flags;
  Object *other;
  FrameRecord *frame;
  int a;
  int b;
  char new_var;
  int c;
  if (((s16) object->field_24a) == 0)
  {
    return 0;
  }
  flags = (s16) object->field_24a;
  if (object->field_20a == 3)
  {
    return 0;
  }
  data_8018946c = 0;
  if (flags & 1)
  {
    if ((u8)func_8014e25c(object) != 0)
    {
      goto done;
    }
  }
  data_8018946c = 1;
  if (flags & 2)
  {
    if (((*((u32 *) (&object->field_04))) == 0x20101) && (object->field_21e < 0x40))
    {
      other = object->other;
      frame = other->frame;
      b = frame->box_b;
      new_var = b;
      a = frame->box_a;
      c = frame->box_c;
      if (((a != 0) || (new_var != 0)) || (c != 0))
      {
        object->field_0b = object->field_158;
        object->field_69 = 0;
        object->field_6a = 0;
        b = 5;
        object->field_249 = b;
        object->field_170 = 0;
        goto done;
      }
    }
  }
  data_8018946c = 2;
  if (flags & 4)
  {
    if (object->field_21e < 0x60)
    {
      if (func_8014e368(object) != 0)
      {
        goto done;
      }
    }
  }
  data_8018946c = 3;
  if (flags & 8)
  {
    if (func_8014e310(object) != 0)
    {
      other = object->other;
      if (other->field_163 != 0)
      {
        goto done;
      }
      if (((*((u32 *) (&other->field_04))) & 0xffffff) == 0x90001)
      {
        done:
        object->field_244 = data_8018946c;

        return 1;
      }
    }
  }
  return 0;
}
