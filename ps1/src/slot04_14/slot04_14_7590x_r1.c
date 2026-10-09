/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern FrameRecord *data_1f8000a8;
extern FrameRecord *data_1f800158;
extern u8 data_801c7644_slot04_14[];
extern u16 data_801c7878_slot04_14[];
extern u16 data_801c7884_slot04_14[];
extern s32 data_801c7890_slot04_14[];
extern s32 data_801c78cc_slot04_14[];

int func_801b7d28_slot04_14(Object *obj, Object *parent);
void func_801b7f1c_slot04_14(Object *obj, u8 index);

void func_801b7590_slot04_14(Object *o) {
    Object *p;
    int a;
    int v;
    int k;
    int t;
    ((Slot04bObj *)o)->field_6c = data_801c7644_slot04_14;
    o->field_04++;
    ((Slot04bObj *)o)->field_b0 = o->field_0d;
    p = o->field_3c;
    o->field_09 = 0;
    o->field_49 = p->field_49;
    o->field_1c = p->field_1c;
    if (o->field_03 != 0) {
        func_801b7d28_slot04_14(o, p);
    }
    k = o->field_03 >> 1;
    t = data_801c7884_slot04_14[k];
    v = data_801c78cc_slot04_14[o->field_ac];
    o->field_50 = v;
    o->pos_y = o->pos_y - t;
    if (o->field_0b != 0) {
        a = data_801c7878_slot04_14[o->field_03 >> 1];
        o->field_4c = data_801c7890_slot04_14[o->field_ac];
    } else {
        a = data_801c7878_slot04_14[o->field_03 >> 1];
        a = -a;
        o->field_4c = -data_801c7890_slot04_14[o->field_ac];
    }
    o->pos_x = a + o->pos_x;
    if (p->side == 0) {
        ((Slot04bObj *)o)->field_8c = (s32)data_1f8000a8;
    } else {
        ((Slot04bObj *)o)->field_8c = (s32)data_1f800158;
    }
    func_801b7f1c_slot04_14(o, o->field_ac);
}
