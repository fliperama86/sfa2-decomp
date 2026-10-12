/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c4188_slot04_04[];

int try_start_meter_action_state_8_variant_a_slot04_04(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state != 0) {
                if (obj->field_7e == 0) {
                    if ((u8)try_action_unless_state_bytes_match(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 0xa;
                    }
                }
            } else {
                if ((u8)is_not_state_2_five(obj)) {
                    if ((u8)func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 0xa;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            }
        }
    }
    return r;
}

void clear_slot_byte_runs_unless_kind_4_slot04_04(Object *obj) {
    int z = 0;
    u32 i;
    u8 *p;
    int c;

    p = (u8 *)&obj->slots[2];
    for (i = 0, c = z; i < 9; i++) {
        *p++ = c;
    }
    if (obj->kind != 4) {
        p = (u8 *)&obj->slots[3];
        for (i = 0; i < 9; i++) {
            *p++ = z;
        }
    }
}

void func_801b156c_slot04_04(Object *obj) {
    data_801ad398 = data_801c4188_slot04_04[obj->field_15a](obj);
}
