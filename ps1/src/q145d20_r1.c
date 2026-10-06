/* Reconstruction. Names/roles inferred, not original symbols. */
/* One temporary per block (e, e2, e3) for the copied byte of field_0e. With a
   single temporary reused across the blocks the build chooses other registers
   than the original. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80145d20(Object *object) {
  Object *p[3];
  Object *o;
  int e, e2, e3;
  if (data_80197f10 >= 3) {
    func_801460ec((u32 *) p);
    o = p[0];
    o->field_00 = 1;
    o->field_02 = 3;
    o->field_03 = 1;
    e = object->field_0e;
    o->field_46 = 8;
    o->field_20 = 8;
    o->field_3c = object;
    o->field_0e = e;
    o->field_0d = object->field_0d + 3;
    o->field_44 = 1;
    o->field_50 = 0x1000800;
    o->field_7a = object->field_7a;
    o->field_7c = object->field_7c;
    o = p[1];
    o->field_00 = 1;
    o->field_02 = 3;
    o->field_03 = 2;
    e2 = object->field_0e;
    o->field_46 = 0x10;
    o->field_20 = 0x10;
    o->field_3c = object;
    o->field_0e = e2;
    o->field_0d = object->field_0d + 4;
    o->field_44 = 1;
    o->field_50 = 0x1000800;
    o->field_7a = object->field_7a;
    o->field_7c = object->field_7c;
    o = p[2];
    o->field_00 = 1;
    o->field_02 = 3;
    o->field_03 = 3;
    e3 = object->field_0e;
    o->field_46 = 0x18;
    o->field_20 = 0x18;
    o->field_3c = object;
    o->field_0e = e3;
    o->field_0d = object->field_0d + 4;
    o->field_44 = 1;
    o->field_50 = 0x1000800;
    o->field_7a = object->field_7a;
    o->field_7c = object->field_7c;
  }
}
