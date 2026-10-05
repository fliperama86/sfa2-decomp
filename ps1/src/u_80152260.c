/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_80152260(Object *object)
{
  PtrTbl39 t;
  t = data_8016da38;
  object->field_7a = 0x60;
  object->field_7c = 0x1e0;
  object->field_04++;
  object->field_0c = 0;
  object->field_0f = 1;
  object->field_0d = 0;
  object->field_81 = 4;
  object->pos_y = 0xf8 - object->pos_y;
  if (object->field_0b == 0)
  {
    object->field_0a = 1;
  }
  func_80130700(object, t.entries[object->field_03][data_8018024c[object->field_03]]);
}
