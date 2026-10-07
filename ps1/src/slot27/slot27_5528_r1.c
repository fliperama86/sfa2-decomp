/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_800284e0_slot27[];
extern GameState *data_800284f8_slot27;
Block172 *func_8011f1e0(void);
void func_80015838_slot27(Object *obj);
void func_800158c0_slot27(Object *obj);
void func_80015950_slot27(Object *obj);

void func_80015528_slot27(Object *obj) {
    obj->field_09 = 0x10;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->field_7a = 0x20;
    obj->field_7c = 0x1e0;
    obj->field_0d = 0x18;
    obj->field_90 = (void *)0x80038000;
    obj->field_01 = 0;
    obj->field_04++;
    if (obj->field_03 == 0) {
        func_800158c0_slot27(obj);
        func_80015950_slot27(obj);
        func_80130768(obj, 0, data_800284e0_slot27);
    } else if (obj->field_03 == 1) {
        func_800158c0_slot27(obj);
        obj->field_04 = 2;
        obj->pos_x = data_800284f8_slot27->field_d2;
        obj->pos_y = data_800284f8_slot27->field_d4;
        func_80015838_slot27(obj);
        func_80015950_slot27(obj);
        func_80130768(obj, 1, data_800284e0_slot27);
    } else if (obj->field_03 >= 2) {
        obj->field_04 = 4;
        func_80130768(obj, obj->field_03, data_800284e0_slot27);
    }
}
