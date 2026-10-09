/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3da8_slot04_08[];

void func_801b3bb8_slot04_08(Object *obj) {
    s16 t = obj->field_3a;
    s16 a;
    s16 n;
    int i;

    if (t & 0x8000) {
        n = obj->field_46;
        n--;
        obj->field_46 = n;
        if (n != 0) {
            func_801307e0(obj, 0x5e);
        } else {
            obj->field_07++;
            func_801307e0(obj, 0x5f);
        }
    } else {
        if (t & 0xff) {
            obj->field_3a = t & ~0xff;
            game_state.field_63 = 0x18;
            i = obj->field_12a >> 1;
            if (obj->field_49 != 0) {
                i += 3;
            }
            a = data_801c3da8_slot04_08[i];
            if ((s16)obj->field_46 != 1) {
                a |= ~0xff;
            }
            func_80140cd8(obj, a, 0);
            if ((s16)obj->other->field_5c < 0) {
                func_80120554(obj, obj->side, 0x319);
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x18;
                    obj->field_255 = 6;
                    game_state.field_6b = 0;
                    func_80147000(obj);
                }
            } else {
                func_80120554(obj, obj->side, 0x319);
            }
            func_80146960(obj);
        }
        func_80130efc(obj);
    }
}
