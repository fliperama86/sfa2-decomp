/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c284c_slot04_0b[];
extern u8 data_801c2eb8_slot04_0b[];
extern s32 data_801c2ebc_slot04_0b[];
extern u8 data_801c2ee0_slot04_0b[];

void call_handler_by_state_1_slot04_0b(Object *o, Object *unused);
void select_box_data_pointer_by_health_slot04_0b(Object *o);

/* The call of call_handler_by_state_1_slot04_0b passes one argument although the function takes two: the original does not set the second argument register before it. Written with a second parameter passed on, this function is 12 bytes longer than the original. */
void func_801b3850_slot04_0b(Object *o, Object *second) {
    s32 vx;
    s32 dx;
    int hi;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    o->variant_index_2 = 0xff;
    o->state_0++;
    o->ot_depth = 0;
    ref_other.p = o->parent;
    o->field_49 = ref_other.p->field_49;
    o->field_1c = ref_other.p->field_1c;
    ((Slot04aObj *)o)->data_pointer = data_801c284c_slot04_0b;
    if (o->vel_table_index >= 0xc) {
        o->health = data_801c2eb8_slot04_0b[ref_other.p->direction_index >> 1];
        select_box_data_pointer_by_health_slot04_0b(o);
    }
    o->active_state = 0;
    o->vel_y = 0;
    if (o->vel_table_index == 4) {
        o->image_row++;
    }
    vx = data_801c2ebc_slot04_0b[o->vel_table_index >> 1];
    hi = o->countdown_2 & 0xfe;
    dx = *(s16 *)(data_801c2ee0_slot04_0b + hi);
    o->pos_y = o->pos_y - *(u16 *)(data_801c2ee0_slot04_0b + hi + 2);
    if (o->facing_side == 0) {
        vx = -vx;
        dx = -dx;
    }
    o->vel_x = vx;
    o->pos_x = o->pos_x + dx;
    if (ref_other.p->side == 0) {
        ((Slot04aObj *)o)->scratchpad_value = *(int *)0x1f8000a8;
    } else {
        ((Slot04aObj *)o)->scratchpad_value = *(int *)0x1f800158;
    }
    func_80138070(o, o->vel_table_index >> 1);
    ((void (*)(Object *))call_handler_by_state_1_slot04_0b)(o);
}
