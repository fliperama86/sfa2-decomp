/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern FrameRecord *data_1f8000a8;
extern FrameRecord *data_1f800158;
extern BoxTables data_801c0148_slot04_00[];

void func_801b34c4_slot04_00(Object *obj);
void func_801b3164_slot04_00(Object *obj);

void func_801b2f8c_slot04_00(Object *obj) {
    u8 a[6] = { 8, 9, 10, 0xff, 0xff, 0xff };
    int b[6] = { 0x30000, 0x3c000, 0x48000, 0x30000, 0x3c000, 0x48000 };
    int v;
    int x;

    obj->field_04 = obj->field_04 + 1;
    obj->field_09 = 0;
    ref_other.p = obj->field_3c;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    obj->box_tables = data_801c0148_slot04_00;
    if (obj->field_ac >= 6) {
        func_801b34c4_slot04_00(obj);
    }
    obj->field_af = 5;
    obj->field_45 = 0;
    obj->field_50 = 0;
    if (obj->field_ac == 4) {
        obj->field_0d = obj->field_0d + 1;
    }
    obj->field_a0 = a[obj->field_ac >> 1];
    v = b[obj->field_ac >> 1];
    x = 0x59;
    if (obj->field_0b == 0) {
        v = -v;
        x = -0x59;
    }
    obj->field_4c = v;
    obj->pos_x = obj->pos_x + x;
    obj->pos_y = obj->pos_y - 0x36;
    if (ref_other.p->side == 0) {
        obj->frames = data_1f8000a8;
    } else {
        obj->frames = data_1f800158;
    }
    func_80138070(obj, obj->field_ac >> 1);
    func_801b3164_slot04_00(obj);
}
