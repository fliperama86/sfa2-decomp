/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Declared as an array so the load is treated as a member access and orders after the field_06 store. */
extern u16 box_margin[];


void func_80144830(Object *object)
{
  u16 margin;
  u8 t = object->field_06;
  object->field_06 = t + 1;
  margin = box_margin[0];
  object->pos_y = 0x70;
  object->field_01 = 2;
  object->field_98 = data_80172a48;
  object->field_9c = data_80173c9c;
  object->field_7a = 0x60;
  object->field_90 = (void *) 0x800fb100;
  object->pos_x = margin + 0xc0;
  func_80130768(object, 0x41, table_8017c7f8);
  func_801448c4(object);
}
