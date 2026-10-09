/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ad8_slot28[];
extern Object *data_80051b00_slot28[];
extern SequenceStep *data_80041498_slot28[];
extern SequenceStep *data_800414b0_slot28[];
extern SequenceStep *data_800414b8_slot28[];
void func_800200ec_slot28(void);
void func_8002008c_slot28(Object *obj);
void func_8002005c_slot28(Object *obj, int arg);

void func_8001fcc0_slot28(Object *obj) {
    Object *b;
    if (*(u8 *)0x80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52++;
        func_800200ec_slot28();
        func_80130768(data_80051b00_slot28[0], 0, data_80041498_slot28);
        b = data_80051b00_slot28[1];
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->field_09 = 5;
        b = data_80051b00_slot28[2];
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        func_80130768(b, 1, data_800414b0_slot28);
        b->field_09 = 4;
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            int two = 2;
            func_8002008c_slot28(b);
            b->pos_x = 0x98;
            b->pos_y = 0x90;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_09 = two;
            func_80130768(b, 0, data_800414b8_slot28);
            data_80051ad8_slot28[0] = b;
            b->field_09 = two;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            func_8002008c_slot28(b);
            b->pos_x = 0x98;
            b->pos_y = 0x90;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_09 = 2;
            func_80130768(b, 1, data_800414b8_slot28);
            data_80051ad8_slot28[1] = b;
            b->field_09 = 3;
        }
        func_8002005c_slot28(obj, 3);
        func_80128370();
    }
}
