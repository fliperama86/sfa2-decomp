/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051890_slot28;
extern SequenceStep *data_8002bde4_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_80015640_slot28(Object *obj);
void func_80015678_slot28(Object *obj);
void func_8001581c_slot28(Object *obj, int arg);

void func_80015374_slot28(Object *obj) {
    HudState *h;
    Object *p;
    Object *b;
    s16 t;
    if (obj->field_f0 == 0) {
        t = 0x3c0;
        data_8018f5a0->field_60 = t;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_80015678_slot28(obj);
        p = (Object *)func_8011f1e0();
        b = p;
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x76;
            b->pos_x = 0xf0;
            b->pos_y = 0x70;
            b->field_09 = 2;
            b->field_03 = 1;
            b->field_0c = 0;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            func_80015640_slot28(b);
        }
        p = (Object *)func_8011f1e0();
        b = p;
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x76;
            b->field_03 = 2;
            b->pos_x = 0x90;
            b->pos_y = 0x68;
            b->field_0c = 0;
            b->field_09 = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            func_80015640_slot28(b);
        }
        p = (Object *)func_8011f1e0();
        b = p;
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x76;
            b->field_03 = 2;
            b->pos_x = 0xa0;
            b->pos_y = 0x50;
            b->field_0c = 0;
            b->field_09 = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            func_80015640_slot28(b);
        }
        p = (Object *)func_8011f1e0();
        b = p;
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x76;
            b->field_03 = 2;
            b->pos_x = 0x58;
            b->pos_y = 0x58;
            b->field_0c = 0;
            b->field_09 = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            func_80015640_slot28(b);
        }
        p = (Object *)func_8011f1e0();
        b = p;
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x76;
            b->field_03 = 2;
            b->pos_x = 0x70;
            b->pos_y = 0x60;
            b->field_0c = 0;
            b->field_09 = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            func_80015640_slot28(b);
        }
        p = (Object *)func_8011f1e0();
        b = p;
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x76;
            b->field_03 = 3;
            b->pos_x = 0x89;
            b->pos_y = 0xa0;
            b->field_0c = 0;
            b->field_09 = 0;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            func_80015640_slot28(b);
            data_80051890_slot28.p = b;
        }
        p = (Object *)func_8011f1e0();
        b = p;
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0xa5;
            b->pos_x = 0xf0;
            b->pos_y = 0x70;
            b->field_03 = 0;
            b->field_0c = 0;
            b->field_09 = 3;
            b->field_01 = 1;
            func_80015640_slot28(b);
            func_80130768(b, 6, data_8002bde4_slot28);
        }
        t = 2;
        func_8001581c_slot28(obj, t);
        func_80128370();
    }
}
