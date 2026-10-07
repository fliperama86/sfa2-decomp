/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
void func_80120554(Object *o, int b, unsigned c);

void func_801b3a90_slot04_0a(Object *obj) {
    obj->field_07++;
    if (obj->field_cd != 0 ? (u16)(box_margin[0] + 0xc0) < obj->pos_x : (obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 0;
    } else {
        obj->field_0b = 1;
    }
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_80141f28(obj, 3);
    func_801307e0(obj, 0x18);
}
