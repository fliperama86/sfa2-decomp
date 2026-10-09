/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b6e94_slot04_sel[];

void func_801b1f20_slot04_sel(Object *obj, Slot04SelRec *r) {
    if (obj->kind == 2 && (*data_801b6e94_slot04_sel[obj->side + 6] & 0x100) != 0) {
        u8 t = r->field_18 + 1;
        r->field_18 = t;
        if (t > 0xb4) {
            r->field_18 = 0xb4;
        }
    } else {
        r->field_18 = 0;
    }
    if (obj->kind == 4 && (*data_801b6e94_slot04_sel[obj->side + 6] & 0x100) != 0) {
        u8 t = r->field_17 + 1;
        r->field_17 = t;
        if (t > 0xb4) {
            r->field_17 = 0xb4;
        }
    } else {
        r->field_17 = 0;
    }
}
