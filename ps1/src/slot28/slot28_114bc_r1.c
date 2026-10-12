/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051b8c_slot28;
extern Object *data_80051b90_slot28[];
extern ObjectRef data_80051b94_slot28;
extern ObjectRef data_80051b9c_slot28;
extern ObjectRef data_80051ba0_slot28;
extern Object *data_80051b64_slot28[];
extern SequenceStep *data_80045c3c_slot28[];
extern SequenceStep *data_80045c40_slot28[];
extern SequenceStep *data_80045c4c_slot28[];
extern SequenceStep *data_80045c6c_slot28[];
extern u8 data_80044620_slot28[];
extern u8 data_800449d0_slot28[];
extern SequenceStep *data_80045c58_slot28[];
void func_80021d94_slot28(Object *obj);
void func_80021dd0_slot28(void);

void func_800214bc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->countdown_timer = 0x258;
    h->step_index_2++;
    func_80151020(0x401);
    func_80021dd0_slot28();
    func_801280f0();
    data_80190568 = one;
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b8c_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        func_80130768(b, 1, data_80045c40_slot28);
        b->field_01 = one;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051ba0_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        func_80130768(b, 0, data_80045c3c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b90_slot28[0] = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->spawn_x = 0x10;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 5;
        func_80130768(b, 0, data_80045c4c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b94_slot28.p = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->spawn_x = 0x20;
        b->spawn_y = 0x1e0;
        b->field_01 = 0;
        b->image_row = 0;
        b->ot_depth = 4;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b9c_slot28.p = b;
        b->spawn_y = 0x1e0;
        b->pos_x = 0x28;
        b->spawn_x = 0;
        b->image_row = 0;
        b->table_index = one;
        b->vel_y = 0xb8;
        b->ot_depth = 0;
        func_80130768(b, 0, data_80045c6c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_02 = 0x7a;
        b->ot_depth = 2;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->pos_x = 0xb8;
        b->pos_y = 0xa0;
        b->field_90 = (void *)0x80060000;
        b->data_ptr_a = data_80044620_slot28;
        b->data_ptr_b = data_800449d0_slot28;
        b->active_count = one;
        b->table_index = 0;
        b->field_01 = one;
        b->image_row = 0;
        b->box_tables = (BoxTables *)data_80045c58_slot28;
        data_80051b64_slot28[0] = b;
    }
    func_8012818c();
}
