/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_80026a08_slot27[];
extern int data_80190464[];
extern int data_8019046c[];
Block172 *func_8011f1e0(void);
void func_80013b74_slot27(Object *obj);

void func_8001276c_slot27(Object *obj) {
    s32 *p0;
    s32 *p1;
    s32 *p2;
    s32 *p3;

    obj->field_06++;
    obj->field_46 = 0x258;
    ref_other.p->field_d8 = 0;
    obj->field_45 = 0xff;
    ref_second.p = ref_other.p;
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00++;
        ref_other.p->field_02 = 0x22;
        ref_other.p->field_03 = 0;
        ref_other.p->other = ref_second.p;
        ref_other.p->field_3c = obj;
        obj->field_54 = (s32)ref_other.p;
    }
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00++;
        ref_other.p->field_02 = 0x22;
        ref_other.p->field_03 = 1;
        ref_other.p->other = ref_second.p;
        ref_other.p->field_3c = obj;
        obj->field_58 = (s32)ref_other.p;
    }
    p0 = (s32 *)&data_8019045c;
    p1 = data_80190464;
    p2 = data_8019046c;
    p3 = (s32 *)&data_8019047c;
    *p0 = 0;
    *p1 = -2;
    *p2 = 0;
    ref_other.p = ref_second.p;
    *p3 = 1;
    if (ref_other.p->field_d8 != 0) {
        *p0 = 0x10;
        *p1 = 2;
        *p2 = 1;
        *p3 = 0;
    }
    obj->field_5c = *p0;
    obj->field_5e = *p1;
    ref_first.p = (Object *)obj->field_54;
    ref_second.p = (Object *)obj->field_58;
    ref_first.p->field_62 = *p2;
    ref_second.p->field_62 = *p3;
    *p0 = *p0 / 16;
    obj->field_62 = *p0;
    obj->field_63 = *p0;
    *p0 = 2;
    *p0 = ref_other.p->field_d8 + 2;
    func_80013b74_slot27(obj);
    *p0 = ref_other.p->side * 2;
    data_80190464[0] = data_80026a08_slot27[data_8019045c[0]];
    data_8019046c[0] = data_80026a08_slot27[data_8019045c[0] + 1];
    obj->field_4c = data_80190464[0];
    obj->field_50 = data_8019046c[0];
    data_80190464[0] = data_80190464[0] << 3;
    data_8019046c[0] = data_8019046c[0] - data_80190464[0];
    obj->pos_x = data_8019046c[0];
    obj->pos_y = 0x3c;
}
