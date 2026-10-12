/* Reconstruction. Names/roles inferred, not original symbols.
 * Exact. The combine of fields 214/215 goes through the u16 temporary t (t = hi << 8; t |= lo),
 * which puts the `or` result in $v0 as the original does. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014b340(Object *object) {
    u16 v;
    s16 w;
    u16 t;
    if (object->script_arg == 0) {
        object->field_208 = 2;
        object->state_0 = 1;
        object->state_2 = 3;
        object->field_209 = 0;
        object->state_1 = 0;
        object->state_3 = 7;
        v = *data_80189460++;
        data_80189464 = v;
        w = v;
        t = w >> 8;
        object->limit_hi = t;
        object->limit_lo = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->script_byte_hi = t;
        object->script_byte_lo = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->limit_hi_2 = t;
        object->y_margin = v;
        func_8014dd14(object);
        object->direction_flag = object->script_byte_hi;
        object->saved_mode_bits = object->script_byte_lo;
        if ((object->script_byte_lo & 0xfe) == 4) {
            t = object->limit_hi_2 << 8;
            t |= object->y_margin;
            object->countdown_3 = t;
        }
    } else {
        v = *data_80189460++;
        data_80189464 = v;
        w = v;
        t = w >> 8;
        object->limit_hi = t;
        object->limit_lo = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->script_byte_hi = t;
        object->script_byte_lo = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->limit_hi_2 = t;
        object->y_margin = v;
        func_8014dd14(object);
        object->field_15a = 3;
        if ((u8)func_8014dcc0(object) != 0) {
        if (*(u32 *)&object->state_0 == 0x20101) {
            object->effect_request = 2;
        }
        object->field_208 = 2;
        object->state_0 = 1;
        object->state_2 = 7;
        object->field_209 = 0;
        object->state_1 = 0;
        object->state_3 = 0;
        object->active_flag_3 = 0;
        object->active_flag = 0;
        object->field_157 = 0;
        object->facing_side = object->saved_facing;
        object->direction_flag = object->script_byte_hi;
        object->saved_mode_bits = object->script_byte_lo;
        if ((object->script_byte_lo & 0xfe) == 4) {
            t = object->limit_hi_2 << 8;
            t |= object->y_margin;
            object->countdown_3 = t;
        }
        } else {
            func_8014c914(object);
        }
    }
}
