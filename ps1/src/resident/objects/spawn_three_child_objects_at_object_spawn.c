/* Reconstruction. Names/roles inferred, not original symbols. */
/* One temporary per block (e, e2, e3) for the copied byte of field_0e. With a
   single temporary reused across the blocks the build chooses other registers
   than the original. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void spawn_three_child_objects_at_object_spawn(Object *object) {
  Object *p[3];
  Object *o;
  int e, e2, e3;
  if (free_block_count >= 3) {
    get_three_pool_objects_sorted((u32 *) p);
    o = p[0];
    o->active_count = 1;
    o->field_02 = 3;
    o->table_index = 1;
    e = object->field_0e;
    o->countdown = 8;
    o->field_20 = 8;
    o->parent = object;
    o->field_0e = e;
    o->image_row = object->image_row + 3;
    o->spawn_flag = 1;
    o->vel_y = 0x1000800;
    o->spawn_x = object->spawn_x;
    o->spawn_y = object->spawn_y;
    o = p[1];
    o->active_count = 1;
    o->field_02 = 3;
    o->table_index = 2;
    e2 = object->field_0e;
    o->countdown = 0x10;
    o->field_20 = 0x10;
    o->parent = object;
    o->field_0e = e2;
    o->image_row = object->image_row + 4;
    o->spawn_flag = 1;
    o->vel_y = 0x1000800;
    o->spawn_x = object->spawn_x;
    o->spawn_y = object->spawn_y;
    o = p[2];
    o->active_count = 1;
    o->field_02 = 3;
    o->table_index = 3;
    e3 = object->field_0e;
    o->countdown = 0x18;
    o->field_20 = 0x18;
    o->parent = object;
    o->field_0e = e3;
    o->image_row = object->image_row + 4;
    o->spawn_flag = 1;
    o->vel_y = 0x1000800;
    o->spawn_x = object->spawn_x;
    o->spawn_y = object->spawn_y;
  }
}
