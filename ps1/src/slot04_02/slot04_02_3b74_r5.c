/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140598(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, int g);

void func_801b4180_slot04_02(Object *obj) {
    Object *p = obj->other;

    p->field_15b = 1;
    func_80140598(obj, 0, 3, 0x14, 0x1c, 0, 0);
    if ((s16)p->field_5c < 0) {
        obj->field_167 = 0xf;
        game_state.field_6b = 6;
        func_80147000(obj);
    }
}
