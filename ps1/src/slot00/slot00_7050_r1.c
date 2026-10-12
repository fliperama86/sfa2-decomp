/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern BoxTables *data_80079814_slot00[];
extern BoxTables *data_80079834_slot00[];
extern u8 data_80079860_slot00[];

void func_80077050_slot00(Object *obj) {
    if (obj->active_flag_2 == 0) {
        obj->box_tables = data_80079814_slot00[obj->table_index_3];
    } else {
        int i = obj->vel_table_index * 2;
        i += (s16)obj->health >> 1;
        obj->box_tables = data_80079834_slot00[i];
    }
}

void func_800770b8_slot00(Object *obj) {
    ref_other.p = obj->parent;
    if (obj->facing_side != 0) {
        obj->pos_x = data_80079860_slot00[(s16)obj->countdown] + ref_other.p->pos_x;
    } else {
        obj->pos_x = ref_other.p->pos_x - data_80079860_slot00[(s16)obj->countdown];
    }
    obj->pos_y = ref_other.p->pos_y - 0x42;
}
