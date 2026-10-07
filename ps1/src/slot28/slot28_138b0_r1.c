/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_8004a088_slot28[];
extern SequenceStep *data_8004a094_slot28[];
extern SequenceStep *data_8004a09c_slot28[];
extern u8 data_80048d70_slot28[];
extern u8 data_800490f4_slot28[];
extern Object *data_80051be4_slot28[];
extern ObjectRef data_80051c0c_slot28;
extern Object *data_80051c10_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_80023c70_slot28(void);
void func_80023be0_slot28(Object *obj, int arg);

void func_800238b0_slot28(Object *obj) {
    Object *b;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_80023c70_slot28();
        func_80130768(data_80051c0c_slot28.p, 2, data_8004a088_slot28);
        b = data_80051c10_slot28[0];
        b->pos_x = 0x38;
        b->pos_y = 0x20;
        func_80130768(b, 1, data_8004a094_slot28);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x1d;
            b->field_03 = 3;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->pos_x = 0x1a8;
            b->pos_y = 0xd5;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80048d70_slot28;
            b->field_9c = data_800490f4_slot28;
            b->box_tables = (BoxTables *)data_8004a09c_slot28;
            b->field_01 = 1;
            b->field_0d = 0;
            data_80051be4_slot28[0] = b;
            ((Slot28Obj *)b)->field_47 = 0x20;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x1d;
            b->field_03 = 2;
            b->field_09 = 3;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->pos_x = -0x3d;
            b->pos_y = 0xc1;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80048d70_slot28;
            b->field_9c = data_800490f4_slot28;
            b->box_tables = (BoxTables *)data_8004a09c_slot28;
            b->field_01 = 1;
            b->field_0d = 0;
            data_80051be4_slot28[1] = b;
            ((Slot28Obj *)b)->field_47 = 0x20;
        }
        func_80023be0_slot28(obj, 3);
        func_80128370();
    }
}
