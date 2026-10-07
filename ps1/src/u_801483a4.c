/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Object *func_8011f1e0(void);

/* Form found by automatic permutation search. */
void func_801483a4(Object *object, int a_arg, int b_arg)
{
  u16 a = a_arg;
  u16 b = b_arg;
  u8 i;
  Object *new_object;
  u16 side;
  for (i = 0; i <= (object->field_12a >> 1); i++)
  {
    new_object = func_8011f1e0();
    if (new_object != 0)
    {
      new_object->field_00 = 1;
      new_object->field_02 = 7;
      new_object->field_03 = object->field_12a >> 1;
      new_object->field_0c = 0;
      new_object->field_0d = 0;
      new_object->field_45 = 0;
      new_object->field_48 = i;
      new_object->field_0e = object->field_0e;
      side = object->field_1c;
      new_object->field_3c = object;
      new_object->field_7a = 0x60;
      new_object->field_7c = 0x1e0;
      new_object->field_98 = data_80172a48;
      new_object->field_5c = a;
      new_object->field_5e = b;
      new_object->field_90 = (void *) 0x800fb100;
      new_object->field_9c = data_80173c9c;
      new_object->field_1c = side;
    }
  }

}
