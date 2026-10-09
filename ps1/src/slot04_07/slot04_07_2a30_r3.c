/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2df8_slot04_07(Object *obj) {
    obj->field_07++;
    ((Slot04aObj *)obj)->field_1cb = 0;
    ((Slot04aObj *)obj)->field_1cc = 0;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80120554(obj, obj->side, 0x31c);
    func_801307e0(obj, 0x40);
}

void func_801b2e68_slot04_07(Object *obj) {
    if ((u8)obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_801204f4(obj, obj->side, 4);
        obj->field_46 |= 0x3800;
        func_801483a4(obj, 0, 0x5c);
    }
    func_80130efc(obj);
}
