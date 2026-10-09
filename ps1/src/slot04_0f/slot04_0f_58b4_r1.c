/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801c6144_slot04_0f[];
extern FrameRecord data_801c6150_slot04_0f[];
extern BoxTables data_801c618c_slot04_0f[];
extern ObjectFn data_801c62a4_slot04_0f[];
extern ObjectRef data_80190458;

void func_8011f14c(Slab172 *o);
void func_8011ffdc(Object *o);
void func_801b5958_slot04_0f(Object *obj);
void func_801b5a28_slot04_0f(Object *obj);
void func_801b5b4c_slot04_0f(Object *obj);

void func_801b58b4_slot04_0f(Object *obj) {
    obj->field_09 = 0;
    obj->field_a0 = 0xff;
    obj->field_04 = obj->field_04 + 1;
    obj->field_49 = data_80190458.p->field_49;
    obj->field_1c = data_80190458.p->field_1c;
    obj->box_tables = data_801c618c_slot04_0f;
    obj->field_5c = 0xff;
    obj->field_45 = 0;
    obj->frames = data_801c6150_slot04_0f;
    func_80130768(obj, obj->field_ac >> 1, data_801c6144_slot04_0f);
    obj->field_48 = 0x16;
    func_801b5958_slot04_0f(obj);
}

void func_801b5958_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        data_801c62a4_slot04_0f[obj->field_05](obj);
    }
    func_8011ffdc(obj);
}

void func_801b59c8_slot04_0f(Object *obj) {
    if (data_80190458.p->frame->field_09 == 0) {
        obj->field_04 = obj->field_04 + 1;
    }
    func_80131094(obj);
    func_801b5b4c_slot04_0f(obj);
}

void func_801b5a28_slot04_0f(Object *obj) {
    if (data_80190458.p->frame->field_09 == 0) {
        obj->field_04 = obj->field_04 + 1;
    } else {
        obj->field_04 = 1;
        obj->field_00 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_5c = 0xff;
    }
    func_80131094(obj);
    func_801b5b4c_slot04_0f(obj);
}

void func_801b5aac_slot04_0f(Object *obj) {
    func_801b5a28_slot04_0f(obj);
}

void func_801b5acc_slot04_0f(Object *obj) {
    func_801b5a28_slot04_0f(obj);
}

void func_801b5aec_slot04_0f(Object *obj) {
    func_801b5a28_slot04_0f(obj);
}

void func_801b5b0c_slot04_0f(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801b5b2c_slot04_0f(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}
