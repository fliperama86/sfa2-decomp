/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4404_slot04_04[];

void func_801b36f8_slot04_04(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b373c_slot04_04(Object *obj) {
    data_801c4404_slot04_04[obj->field_07](obj);
}
