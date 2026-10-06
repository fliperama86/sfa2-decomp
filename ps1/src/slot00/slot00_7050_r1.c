/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern BoxTables *data_80079814_slot00[];
extern BoxTables *data_80079834_slot00[];
extern u8 data_80079860_slot00[];

void func_80077050_slot00(Object *obj) {
    if (obj->field_ad == 0) {
        obj->box_tables = data_80079814_slot00[obj->field_ae];
    } else {
        int i = obj->field_ac * 2;
        i += (s16)obj->field_5c >> 1;
        obj->box_tables = data_80079834_slot00[i];
    }
}

void func_800770b8_slot00(Object *obj) {
    ref_other.p = obj->field_3c;
    if (obj->field_0b != 0) {
        obj->pos_x = data_80079860_slot00[(s16)obj->field_46] + ref_other.p->pos_x;
    } else {
        obj->pos_x = ref_other.p->pos_x - data_80079860_slot00[(s16)obj->field_46];
    }
    obj->pos_y = ref_other.p->pos_y - 0x42;
}
