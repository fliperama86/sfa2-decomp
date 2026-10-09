/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

/* Form found by automatic permutation search. */
void func_801497c0(Object *object)
{
  Object *new_object;
  u8 side;
  new_object = (Object *)func_8011f1e0();
  if (new_object != 0)
  {
    new_object->field_00 = 1;
    new_object->field_02 = 0x37;
    new_object->field_03 = 3;
    new_object->field_1c = object->field_1c;
    new_object->field_0e = object->field_0e;
    new_object->field_26 = object->field_26;
    new_object->field_0b = object->field_0b;
    new_object->pos_x = object->pos_x;
    new_object->pos_y = object->pos_y;
    side = object->field_65;
    new_object->field_7a = 0x60;
    new_object->field_7c = 0x1e0;
    new_object->field_3c = object;
    new_object->field_90 = (void *) 0x800fb100;
    new_object->field_98 = data_80172a48;
    new_object->field_9c = data_80173c9c;
    new_object->field_65 = side;
  }
}
