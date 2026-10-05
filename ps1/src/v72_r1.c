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
    if (object->field_20f == 0) {
        object->field_208 = 2;
        object->field_04 = 1;
        object->field_06 = 3;
        object->field_209 = 0;
        object->field_05 = 0;
        object->field_07 = 7;
        v = *data_80189460++;
        data_80189464 = v;
        w = v;
        t = w >> 8;
        object->field_210 = t;
        object->field_211 = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->field_212 = t;
        object->field_213 = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->field_214 = t;
        object->field_215 = v;
        func_8014dd14(object);
        object->field_21a = object->field_212;
        object->field_217 = object->field_213;
        if ((object->field_213 & 0xfe) == 4) {
            t = object->field_214 << 8;
            t |= object->field_215;
            object->field_21c = t;
        }
    } else {
        v = *data_80189460++;
        data_80189464 = v;
        w = v;
        t = w >> 8;
        object->field_210 = t;
        object->field_211 = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->field_212 = t;
        object->field_213 = v;
        v = *data_80189460++;
        w = v;
        t = w >> 8;
        object->field_214 = t;
        object->field_215 = v;
        func_8014dd14(object);
        object->field_15a = 3;
        if ((u8)func_8014dcc0(object) != 0) {
        if (*(u32 *)&object->field_04 == 0x20101) {
            object->field_bd = 2;
        }
        object->field_208 = 2;
        object->field_04 = 1;
        object->field_06 = 7;
        object->field_209 = 0;
        object->field_05 = 0;
        object->field_07 = 0;
        object->field_159 = 0;
        object->field_67 = 0;
        object->field_157 = 0;
        object->field_0b = object->field_158;
        object->field_21a = object->field_212;
        object->field_217 = object->field_213;
        if ((object->field_213 & 0xfe) == 4) {
            t = object->field_214 << 8;
            t |= object->field_215;
            object->field_21c = t;
        }
        } else {
            func_8014c914(object);
        }
    }
}
