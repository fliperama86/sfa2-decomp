/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6584_slot04_09(Object *obj, Object *parent, int i);

void func_801b63c4_slot04_09(Object *obj, Object *parent) {
    int n;
    int c;
    int d;
    SequenceStep **t;
    obj->field_01 = 0;
    if (obj->table_index != parent->kind) {
        obj->state_0++;
        return;
    }
    n = parent->frame->frame_param;
    if (n == 0) {
        obj->variant_index = 0;
        return;
    }
    func_801b6584_slot04_09(obj, parent, n);
    obj->field_0c = parent->field_0c;
    obj->image_row = parent->image_row;
    obj->field_01 = 1;
    if (obj->variant_index == n) {
        d = obj->countdown - 1;
        obj->countdown = d;
        if ((s16)d < 0) {
            obj->countdown = 0;
            obj->field_0c = parent->field_0c;
            obj->image_row = parent->image_row;
            c = obj->countdown ^ 1;
            obj->countdown = c;
            if (c & 1) {
                obj->field_0c = 0xff;
                obj->image_row++;
            }
        }
        advance_sequence_checked(obj);
        if (((Slot04aObj *)obj)->mode == 1) {
            obj->facing_side = 0;
            obj->sequence_changed_flag = 1;
        }
    } else {
        obj->variant_index = n;
        if (parent->side == 0) {
            t = data_1f8000b4;
        } else {
            t = data_1f800164;
        }
        func_80130768(obj, n, t);
        if (((Slot04aObj *)obj)->mode == 1) {
            obj->facing_side = 0;
        }
    }
}
