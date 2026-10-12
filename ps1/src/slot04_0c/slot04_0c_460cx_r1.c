/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void init_object_from_parent_position_slot04_0c(Object *obj) {
    Object *p = obj->parent;

    obj->state_0++;
    obj->field_0c = p->field_0c;
    obj->field_0e = p->field_0e;
    obj->ot_depth = 4;
    obj->ground_y = p->ground_y + 0x50;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y + 0x2e;
    obj->facing_side = p->facing_side;
    /* The listing has one store of pos_x to itself here. Two arms that each add zero, as the sibling functions offset pos_x by the facing, are an inference that reproduces it. */
    if (obj->facing_side == 0) {
        obj->pos_x -= 0;
    } else {
        obj->pos_x += 0;
    }
    obj->vel_y = 0xfffd8000;
    if (p->side == 0) {
        set_sequence_from_table_5(obj, 1, data_1f8000b4);
    } else {
        set_sequence_from_table_5(obj, 1, data_1f800164);
    }
}
