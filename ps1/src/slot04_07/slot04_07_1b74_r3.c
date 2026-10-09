/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);
int func_80140cd8(Object *obj, int a, int b);

void func_801b1e40_slot04_07(Object *obj) {
    int k;

    if (((Slot04aObj *)obj)->field_3a != 0) {
        func_80130efc(obj);
    } else if (func_801b41c4_slot04_07(obj) >= 0 || (k = obj->field_70) > obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = k;
        obj->field_14 = 0;
        obj->field_45 = 0;
        game_state.field_63 = 0x40;
        func_801204f4(obj, obj->side, 8);
        k = obj->field_49 != 0 ? 0x14 : 0x11;
        if ((u8)func_80140cd8(obj, k, 0) != 0) {
            func_80120554(obj, obj->side, 0x319);
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x17;
                obj->field_255 = 6;
                game_state.field_6b = 3;
                func_80147000(obj);
            }
        } else {
            func_80120554(obj, obj->side, 0x319);
        }
        func_801307e0(obj, 0x22);
    }
}
