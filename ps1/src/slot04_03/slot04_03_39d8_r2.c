/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c098c_slot04_03[];
extern s32 data_801c09a4_slot04_03[];
extern u16 data_801c09b0_slot04_03[];

void call_handler_by_state_1_slot04_03(Object *obj, Object *unused);

/* The call of call_handler_by_state_1_slot04_03 passes one argument although the function takes two: the original does not set the second argument register before it. Written with a second parameter passed on, this function is 12 bytes longer than the original. */
void func_801b3ae8_slot04_03(Object *obj, Object *unused) {
    u8 a2;
    int a1;

    obj->variant_index_2 = 0xff;
    obj->state_0++;
    obj->ot_depth = 0;
    ref_other.p = obj->parent;
    obj->field_49 = ref_other.p->field_49;
    a2 = 3;
    obj->field_1c = ref_other.p->field_1c;
    ((Slot04aObj *)obj)->data_pointer = data_801c098c_slot04_03;
    obj->active_state = 0;
    obj->vel_y = 0;
    obj->vel_x = 0x50000;
    if (obj->table_index == 0) {
        obj->vel_x = data_801c09a4_slot04_03[obj->vel_table_index >> 1];
        a2 = 0;
    }
    a1 = data_801c09b0_slot04_03[obj->ground_y];
    if (obj->facing_side == 0) {
        a1 = -a1;
        obj->vel_x = -obj->vel_x;
    }
    if (ref_other.p->side == 0) {
        ((Slot04aObj *)obj)->scratchpad_value = *(s32 *)0x1f8000a8;
    } else {
        ((Slot04aObj *)obj)->scratchpad_value = *(s32 *)0x1f800158;
    }
    obj->pos_x = a1 + obj->pos_x;
    obj->pos_y = obj->pos_y - 0x44;
    func_80138070(obj, a2 + (obj->vel_table_index >> 1));
    ((void (*)(Object *))call_handler_by_state_1_slot04_03)(obj);
}
