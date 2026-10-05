/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80130678(Object *object, int arg);

void func_8012f384(Object *object) {
    u8 flag;

    object->field_07 = 5;
    flag = object->field_164 != 1;
    func_80120554(object, object->side, 0x314);
    object->field_0b = flag;
    func_80130678(object, 0x30);
}

void func_8012f3e0(Object *object) {
    object->field_07 = 4;
    func_80130678(object, 0x1c);
}
