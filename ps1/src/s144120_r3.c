/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014448c(Object *object) {
    object->field_06 = 7;
    func_80144ee4(object, 0xd);
}

void func_801444b4(Object *object) {
    if (*(u8 *)&object->field_3a != 0) {
        object->field_06 = object->field_06 + 1;
        object->field_3a = object->field_3a & 0xff00;
        func_80120554(0, 0, 0x20c);
    }
}
