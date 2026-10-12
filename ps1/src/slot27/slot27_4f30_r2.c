/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_80028128_slot27[];

void set_position_from_other_plus_table_offset_slot27(Object *obj) {
    int *x = data_8019045c;
    int *y = data_80190464;
    *y = (*(Config **)&data_80190458)->table_index * 4;
    *y = (obj->table_index + *y) * 2;
    *x = data_80028128_slot27[data_80190464[0]];
    *y = data_80028128_slot27[data_80190464[0] + 1];
    *x = ref_other.p->pos_x + *x;
    *y = ref_other.p->pos_y + *y;
    *x += obj->vel_x;
    *y += obj->vel_y;
    obj->pos_x = *x;
    obj->pos_y = *y;
}
