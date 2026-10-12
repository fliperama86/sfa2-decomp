/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e93d8_slot06_00(Object *object) {
    Slot06Layer *layer = (Slot06Layer *)data_801aa544;
    s16 d;

    d = layer->ref_x;
    d -= layer->field_22;
    d -= d >> 2;
    d += object->pos_x_frac;
    object->pos_x = d;
    d = layer->field_0e;
    d -= layer->field_26;
    d -= d >> 2;
    d -= object->pos_y_frac + 0xff08;
    object->pos_y = 0xf8 - d;
    func_80120028(object);
}
