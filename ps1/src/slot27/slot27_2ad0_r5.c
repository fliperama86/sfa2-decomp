/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_80190464[];
extern int data_8019046c[];
extern s16 data_80026a58_slot27[];

void func_80012fd4_slot27(Object *obj) {
    int t;
    int u;
    data_8019045c[0] = 0;
    data_80190464[0] = 1;
    data_8019046c[0] = 0;
    if ((s16)obj->field_5c >= 9) {
        data_8019045c[0] = 1;
        data_80190464[0] = 0;
        data_8019046c[0] = 1;
    }
    ref_first.p = (Object *)obj->field_54;
    ref_second.p = (Object *)obj->field_58;
    ref_first.p->field_62 = data_8019045c[0];
    ref_second.p->field_62 = data_80190464[0];
    if (obj->field_62 != data_8019046c[0]) {
        obj->field_62 = data_8019046c[0];
    }
    data_80190464[0] = (s16)obj->field_5c;
    t = data_80026a58_slot27[data_80190464[0]];
    data_8019045c[0] = t;
    u = data_80026a58_slot27[data_80190464[0] + 1];
    data_80190464[0] = u;
    if (ref_other.p->side != 0) {
        data_8019045c[0] = -t;
    }
    ref_first.p->field_4c = data_8019045c[0];
    data_8019045c[0] = -data_8019045c[0];
    ref_second.p->field_4c = data_8019045c[0];
    ref_first.p->field_50 = data_80190464[0];
    data_80190464[0] = -data_80190464[0];
    ref_second.p->field_50 = data_80190464[0];
}
