/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (2 slots): the second load of box->field_11 is scheduled after the load of
   field_7e here and before it in the original. */
/* Form found by automatic permutation search. */
void func_80139a78(Object *a, Object *b)
{
  Box32 *box;
  u8 t;
  s16 i;
  if (a->field_08 == 0)
  {
    box = &a->wide_boxes[a->frame->active];
  }
  else
  {
    box = ptr_8019040c;
  }
  if (a->field_08 == 0)
  {
    a->field_29d = 0;
  }
  i = box->field_0d;
  if (a->field_45 != 0)
  {
    a->field_75 = table_80176fe0[i];
  }
  else
  {
    a->field_75 = table_80176fc0[i];
  }
  if (a->field_08 == 0)
  {
    a->field_296 = table_80177000[a->kind];
  }
  a->field_67 = 1;
  data_80188f34 = 0;
  b->field_04 = 1;
  b->field_05 = 1;
  b->field_06 = 0;
  b->field_07 = 0;
  b->field_62 = box->field_09 & 0xf;
  if (b->field_295 == 0)
  {
    b->field_295 = a->field_49;
  }
  if (((box->field_0d != 0x1a) && ((box->field_08 & 0x80) == 0)) && (a->field_45 != 0))
  {
    b->field_62 = 0;
  }
  b->field_63 = box->field_0a;
  if (a->field_49 != 0)
  {
    b->field_63 = box->field_19;
  }
  b->field_72 = a->field_0b;
  if (box->field_0b & 0x80)
  {
    b->field_72 = a->pos_x < b->pos_x;
  }
  else
  {
    b->field_72 = box->field_0b ^ b->field_72;
  }
  if (a->field_66 != 0)
  {
    b->field_265 = box->field_12;
  }
  else
  {
    b->field_264 = box->field_12;
  }
  if (a->field_08 != 8)
  {
    if (a->field_66 != 0)
    {
      b->field_289 = b->field_289 + 1;
    }
    else
    {
      b->field_288 = b->field_288 + 1;
    }
  }
  if (a->field_74 == 0)
  {
    i = box->field_11;
    b->field_6b = box->field_11 & 0x1f;
    t = i;
    if (a->field_7e != 0)
    {
      t = t >> 2;
    }
    a->field_6b = t;
  }
  if ((((s16) b->field_5c) < 0) || (b->frame->field_06 == 0))
  {
    func_8013a154(a, b, box);
  }
  else
  {
    func_80139d04(a, b, box);
  }
}
