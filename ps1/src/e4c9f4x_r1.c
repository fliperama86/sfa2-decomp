/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* One local, s, holds the result of the first test and later the shifted
   halfword: with the test written in the if and s used for the shift only,
   this function differs from the original in 6 instruction slots. The
   halfword is read into t and copied to v: with v = *p; it differs in 5.
   The shifted value goes through t to the store: with
   object->field_20e = s >> 24; it differs in 5. The pointer is read back
   from data_80189460: with p = (u16 *)object->field_22c; it differs in 5. */
void func_8014c9f4(Object *object) {
    u16 *p;
    int t;
    int v;
    int s;
    s = object->field_20a == 0;
    if (s) {
        func_8014d9ac(object);
    }
    object->field_20a = 1;
    func_8014da18(object);
    data_80189460 = (u16 *)object->field_22c;
    object->field_238 = (s32)data_80189460;
    p = data_80189460;
    data_80189460 = p + 1;
    t = *p;
    v = t;
    s = v << 16;
    t = s >> 24;
    object->field_20e = t;
    object->field_20f = v;
}
