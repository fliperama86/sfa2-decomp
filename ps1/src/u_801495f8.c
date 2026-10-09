/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_801495f8(Object *object, int x, int y)
{
  int dx;
  Object *new_var;
  Object *o;
  dx = x;
  o = (Object *)func_8011f1e0();
  if (o != 0)
  {
    o->field_00 = 1;
    o->field_02 = 0x37;
    o->field_03 = 2;
    o->field_1c = object->field_1c;
    o->field_0e = object->field_0e;
    o->field_26 = object->field_26;
    o->field_65 = object->field_65;
    o->field_3c = object;
    o->field_0b = object->field_0b;
    o->field_7a = 0x60;
    o->field_7c = 0x1e0;
    o->field_98 = &data_80172a48;
    o->field_90 = (void *) 0x800fb100;
    o->field_9c = &data_80173c9c;
    if (o->field_0b != 0)
    {
      dx = -x;
    }
    o->pos_x = dx + object->pos_x;
    o->pos_y = object->pos_y - y;
    if (object->field_45 == 0)
    {
      x = 0;
      do
      {
        o = (Object *)func_8011f1e0();
        if (o != 0)
        {
          o->field_00 = 1;
          o->field_02 = 0x37;
          o->field_03 = x;
          o->field_1c = object->field_1c;
          o->field_0e = object->field_0e;
          o->field_26 = object->field_26;
          o->field_0b = object->field_0b;
          o->pos_x = object->pos_x;
          o->pos_y = object->field_70;
          o->field_65 = object->field_65;
          new_var = (o->field_3c = object);
          o->field_7a = 0x60;
          o->field_7c = 0x1e0;
          o->field_98 = &data_80172a48;
          o->field_90 = (void *) 0x800fb100;
          o->field_9c = &data_80173c9c;
        }
        x++;
      }
      while (((u16) x) < 2);
    }
  }
}
