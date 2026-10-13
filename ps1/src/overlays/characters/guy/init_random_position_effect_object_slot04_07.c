/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_random_velocities_from_table_slot04_07(Object *obj);

void init_random_position_effect_object_slot04_07(Object *obj) {
    Object *o;
    s32 *p;
    int a;
    int v;
    int w;

    obj->ot_depth = 2;
    obj->state_0 = obj->state_0 + 1;
    o = obj->other;
    obj->active_state = 0;
    obj->facing_side = o->side;
    obj->field_0e = o->field_0e;
    obj->scroll_factor = o->scroll_factor;
    obj->field_1c = o->field_1c;
    obj->field_0c = o->field_0c;
    obj->image_row = o->image_row;
    obj->parent = o;
    v = get_random_byte() & 0x1f;
    v -= 0x10;
    a = -0x30;
    if (obj->facing_side != 0) {
        a = 0x1b0;
    }
    a += *(s16 *)box_margin;
    v += a;
    w = v << 16;
    *(s32 *)&obj->health = w;
    *(s32 *)&obj->box_tables = w;
    v = get_random_byte() & 0x3f;
    v = 0xb8 - v;
    w = v << 16;
    *(s32 *)&obj->field_60 = w;
    *(s32 *)&obj->ground_y = w;
    set_random_velocities_from_table_slot04_07(obj);
    if (o->side == 0) {
        p = (s32 *)seqs_b4_left;
    } else {
        p = (s32 *)seqs_164_right;
    }
    v = p[5];
    *(s32 *)&obj->field_20 = v;
    *(s32 *)&obj->pair_value = p[6];
}
