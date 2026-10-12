/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80036694_slot28[];
extern SequenceStep *data_8003669c_slot28[];
extern SequenceStep *data_800366a8_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
extern ObjectRef data_800519c0_slot28;
extern Object *data_800519c4_slot28[];
extern ObjectRef data_800519c8_slot28;
extern Object *data_80051998_slot28[];
extern ObjectRef data_8005199c_slot28;
void func_8001a958_slot28(Object *o, int arg);
void func_8001a988_slot28(Object *o);
void func_8001a9e8_slot28(void);

void func_8001a69c_slot28(Object *obj) {
    Object *s;
    if (data_80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->pass_index++;
        data_8018f5a0->countdown_timer = 0x1e0;
        func_8001a9e8_slot28();
        func_80130768(data_800519c0_slot28.p, 1, data_80036694_slot28);
        s = data_800519c4_slot28[0];
        s->pos_y = -0x60;
        func_80130768(s, 0, data_8003669c_slot28);
        s = data_800519c8_slot28.p;
        s->field_01 = 1;
        s->pos_y = 0x20;
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        do {
            s = (Object *)func_8011f1e0();
            if (s != 0) {
                func_8001a988_slot28(s);
                s->pos_x = 0xc0;
                s->pos_y = 0x90;
                s->countdown = 0x40;
                s->spawn_x = 0x60;
                s->spawn_y = 0x1e0;
                s->image_row = 0;
                s->ot_depth = 3;
                func_80130768(s, 5, data_800366a8_slot28);
                data_80051998_slot28[0] = s;
            }
        } while (0);
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        do {
            s = (Object *)func_8011f1e0();
            if (s != 0) {
                s->field_02 = 0x33;
                s->table_index = 2;
                s->field_90 = (void *)0x80060000;
                s->data_ptr_a = data_80035430_slot28;
                s->data_ptr_b = data_800357d8_slot28;
                s->pos_x = 0xb8;
                s->pos_y = 0x64;
                s->vel_y = 0x8000;
                s->accel_y = -0x400;
                s->spawn_x = 0x60;
                s->ot_depth = 3;
                s->active_count = 1;
                s->field_01 = 0;
                s->countdown = 0;
                s->spawn_y = 0x1e0;
                s->image_row = 0;
                ((Slot28Obj *)s)->sequence_ptr = data_800366a8_slot28;
                data_8005199c_slot28.p = s;
            }
        } while (0);
        func_8001a958_slot28(obj, 4);
        func_80128370();
    }
}
