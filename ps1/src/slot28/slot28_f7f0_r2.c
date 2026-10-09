/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ad8_slot28[];
extern Object *data_80051b00_slot28[];
extern SequenceStep *data_80041498_slot28[];
extern SequenceStep *data_800414a4_slot28[];
extern SequenceStep *data_800414d4_slot28[];
extern u8 data_8003f9fc_slot28[];
extern u8 data_8003feb8_slot28[];

void func_8001f840_slot28(Object *obj) {
    Object *b;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x168;
        data_8018f5a0->field_52++;
        func_80130768(data_80051b00_slot28[0], 2, data_80041498_slot28);
        b = data_80051b00_slot28[1];
        b->pos_x = 0x38;
        b->pos_y = -0x50;
        func_80130768(b, 1, data_800414a4_slot28);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x71;
            b->field_03 = 1;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8003f9fc_slot28;
            b->field_9c = data_8003feb8_slot28;
            b->pos_x = 0xa8;
            b->pos_y = 0x50;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_09 = 2;
            b->field_01 = 0;
            b->field_0d = 0;
            data_80051ad8_slot28[1] = b;
            b->box_tables = (BoxTables *)data_800414d4_slot28;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x71;
            b->field_03 = 2;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8003f9fc_slot28;
            b->field_9c = data_8003feb8_slot28;
            b->pos_x = 0xa8;
            b->pos_y = 0x50;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_01 = 0;
            b->field_0d = 0;
            b->field_09 = 2;
            data_80051ad8_slot28[2] = b;
            b->box_tables = (BoxTables *)data_800414d4_slot28;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x71;
            b->field_03 = 3;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8003f9fc_slot28;
            b->field_9c = data_8003feb8_slot28;
            b->pos_x = 0xa8;
            b->pos_y = 0x50;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_09 = 2;
            b->field_01 = 0;
            b->field_0d = 0;
            data_80051ad8_slot28[3] = b;
            b->box_tables = (BoxTables *)data_800414d4_slot28;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x71;
            b->field_03 = 4;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8003f9fc_slot28;
            b->field_9c = data_8003feb8_slot28;
            b->pos_x = 0xa8;
            b->pos_y = 0x50;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_09 = 2;
            b->field_01 = 0;
            b->field_0d = 0;
            data_80051ad8_slot28[4] = b;
            b->box_tables = (BoxTables *)data_800414d4_slot28;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x71;
            b->field_03 = 5;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8003f9fc_slot28;
            b->field_9c = data_8003feb8_slot28;
            b->pos_x = 0xa8;
            b->pos_y = 0x50;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_09 = 2;
            b->field_01 = 0;
            b->field_0d = 0;
            data_80051ad8_slot28[5] = b;
            b->box_tables = (BoxTables *)data_800414d4_slot28;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x71;
            b->field_03 = 6;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8003f9fc_slot28;
            b->field_9c = data_8003feb8_slot28;
            b->pos_x = 0xa8;
            b->pos_y = 0x50;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_09 = 2;
            b->field_01 = 0;
            b->field_0d = 0;
            data_80051ad8_slot28[6] = b;
            b->box_tables = (BoxTables *)data_800414d4_slot28;
        }
        func_80128280();
    }
}
