/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ccc_slot28[];
extern Object *data_80051cd0_slot28[];
extern Object *data_80051ca4_slot28[];
extern SequenceStep *data_800502c8_slot28[];
extern SequenceStep *data_800502d4_slot28[];
extern SequenceStep *data_800502e8_slot28[];
extern u8 data_8004ead4_slot28[];
extern u8 data_8004ef00_slot28[];
void func_800272ec_slot28(void);
void func_8002725c_slot28(Object *o, int arg);

void func_80026f70_slot28(Object *obj) {
    HudState *h;
    Object *p;
    if (data_80190949 != 0) {
        if (obj->field_f0 == 0) {
            h = data_8018f5a0;
            h->countdown_timer = 0x258;
            h->pass_index++;
            func_800272ec_slot28();
            func_80130768(data_80051ccc_slot28[0], 2, data_800502c8_slot28);
            p = data_80051cd0_slot28[0];
            p->pos_x = 0x68;
            p->pos_y = 0x10;
            p->ot_depth = 4;
            p->field_01 = 1;
            func_80130768(p, 3, data_800502d4_slot28);
            p->vel_y = 0x2000;
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                p->active_count = 1;
                p->table_index = 0;
                p->field_01 = 1;
                p->field_02 = 0x73;
                p->field_90 = (void *)0x80060000;
                p->data_ptr_a = data_8004ead4_slot28;
                p->data_ptr_b = data_8004ef00_slot28;
                p->spawn_x = 0x60;
                p->spawn_y = 0x1e0;
                p->box_tables = (BoxTables *)data_800502e8_slot28;
                p->image_row = 0;
                p->ot_depth = 2;
                data_80051ca4_slot28[0] = p;
            }
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                p->active_count = 1;
                p->table_index = 1;
                p->field_01 = 1;
                p->field_02 = 0x73;
                p->field_90 = (void *)0x80060000;
                p->data_ptr_a = data_8004ead4_slot28;
                p->data_ptr_b = data_8004ef00_slot28;
                p->spawn_x = 0x60;
                p->spawn_y = 0x1e0;
                p->box_tables = (BoxTables *)data_800502e8_slot28;
                p->image_row = 0;
                p->ot_depth = 2;
                data_80051ca4_slot28[1] = p;
            }
            func_8002725c_slot28(obj, 4);
            func_80128370();
        }
    }
}
