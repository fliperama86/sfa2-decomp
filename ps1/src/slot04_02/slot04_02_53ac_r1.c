/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6518_slot04_02[];
extern u8 data_801c6520_slot04_02[];
int func_80140cd8(Object *object, int a, int b);

void func_801b53ac_slot04_02(Object *obj) {
    int i;
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0 && obj->pos_y >= obj->field_70) {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_80146960(obj);
        i = obj->field_12a & 0xfe;
        game_state.field_63 = data_801c6518_slot04_02[i];
        if ((u8)func_80140cd8(obj, *(s16 *)(data_801c6520_slot04_02 + i), 0) != 0) {
            func_80120554(obj, obj->side, 0x319);
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x12;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            }
        } else {
            func_80120554(obj, obj->side, 0x319);
        }
        func_801307e0(obj, 0x59);
    } else {
        func_80130efc(obj);
    }
}
