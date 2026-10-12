/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_80055f3c_slot01;
extern ObjectRef data_80055f44_slot01;
void func_80011740_slot01(void);

void func_80011224_slot01(void) {
    Object *o;
    data_8018f5a0->substep_index++;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x2b;
        o->table_index = 1;
    }
}

void func_80011274_slot01(void) {
    Object *o;
    data_8018f5a0->substep_index++;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x30;
        o->table_index = *(u8 *)&data_80055f3c_slot01;
        o->ot_depth = 8;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1f0;
    }
}

void spawn_hud_object_from_kind_slot01(void) {
    Object *o;
    data_8018f5a0->substep_index++;
    func_80011740_slot01();
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x13;
        if (data_80055f3c_slot01 == 0x12) {
            o->table_index = 0x14;
        } else {
            o->table_index = *(u8 *)&data_80055f3c_slot01;
        }
        o->spawn_x = 0x20;
        o->spawn_y = 0x1e0;
    }
    data_80055f44_slot01.p = o;
}
