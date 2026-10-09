/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4da0_slot04_12[];
extern ObjectFn data_801c4da8_slot04_12[];
extern s32 data_801c4db8_slot04_12[];
extern u8 data_801c4dc4_slot04_12[];
extern ObjectFn data_801c4dc8_slot04_12[];
extern SequenceStep *data_801c4de8_slot04_12[];
extern u8 data_801c4dec_slot04_12[];
extern u8 data_801c4e28_slot04_12[];
extern ObjectFn data_801c4e30_slot04_12[];
extern ObjectFn data_801c4e40_slot04_12[];
extern ObjectFn data_801c4e54_slot04_12[];
extern SequenceStep **data_801c4f08_slot04_12[];
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern SequenceStep **data_1f8000f0;
extern SequenceStep **data_1f8001a0;

void func_8011f14c(Slab172 *o);
void func_8011ffdc(Object *o);

void func_801b3b0c_slot04_12(Object *obj);
void func_801b3c6c_slot04_12(Object *obj);
void func_801b3cfc_slot04_12(Object *obj);
void func_801b3e58_slot04_12(Object *obj);
void func_801b4064_slot04_12(Object *obj);
void func_801b40d0_slot04_12(Object *obj, Object *p);
void func_801b417c_slot04_12(Object *obj, Object *p);
void func_801b4228_slot04_12(Object *obj);

void func_801b3b0c_slot04_12(Object *obj) {
    data_801c4da8_slot04_12[obj->field_04](obj);
}

void func_801b3b4c_slot04_12(Object *obj) {
    Object *parent;
    u8 i;
    int k;

    parent = obj->field_3c;
    i = obj->field_ac;
    i >>= 1;
    obj->field_09 = 0;
    obj->field_04 = obj->field_04 + 1;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->field_6c = data_801c4da0_slot04_12;
    obj->field_0c = 0;
    obj->field_45 = 0;
    obj->field_50 = 0;
    k = data_801c4db8_slot04_12[i];
    if (obj->field_0b != 0) {
        obj->field_4c = k;
        obj->pos_x = obj->pos_x + 0x58;
    } else {
        k = -k;
        obj->field_4c = k;
        obj->pos_x = obj->pos_x - 0x58;
    }
    obj->pos_y = obj->pos_y - 0x38;
    obj->field_a0 = data_801c4dc4_slot04_12[obj->field_ac >> 1];
    if (parent->side == 0) {
        ((Slot04bObj *)obj)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(obj, obj->field_ac >> 1);
    func_801b3c6c_slot04_12(obj);
}

void func_801b3c6c_slot04_12(Object *obj) {
    data_801c4dc8_slot04_12[obj->field_05](obj);
}

void func_801b3cac_slot04_12(Object *obj) {
    if (obj->field_05 != 0) {
        func_801b3cfc_slot04_12(obj);
    } else {
        obj->field_05 = obj->field_05 + 1;
        obj->field_3c->field_14c = 0;
        func_80138070(obj, 3);
    }
}

void func_801b3cfc_slot04_12(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = obj->field_04 + 1;
    }
    func_80131094(obj);
}

void func_801b3d3c_slot04_12(Object *obj) {
    Object *p = obj->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)obj);
}

void func_801b3d88_slot04_12(Object *obj) {
    data_801c4e30_slot04_12[obj->field_04](obj);
}

void func_801b3dc8_slot04_12(Object *obj) {
    Object *parent;

    obj->field_a0 = 0xff;
    obj->field_04 = obj->field_04 + 1;
    parent = obj->field_3c;
    obj->field_09 = 0;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->field_6c = data_801c4e28_slot04_12;
    ((Slot04bObj *)obj)->field_8c = (int)data_801c4dec_slot04_12;
    obj->field_45 = 0;
    obj->field_5c = 0xff;
    func_80130700(obj, data_801c4de8_slot04_12[0]);
    func_801b3e58_slot04_12(obj);
}

void func_801b3e58_slot04_12(Object *obj) {
    data_801c4e40_slot04_12[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801b3eac_slot04_12(Object *obj) {
    if (obj->field_3c->frame->field_09 == 0) {
        obj->field_04 = obj->field_04 + 1;
    }
}

void func_801b3ee4_slot04_12(Object *obj) {
    obj->field_04 = 1;
    obj->field_00 = 1;
    obj->field_05 = 0;
    obj->field_06 = 0;
    obj->field_07 = 0;
    obj->field_5c = 0xff;
}

void func_801b3f08_slot04_12(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801b3f28_slot04_12(Object *obj) {
    data_801c4e54_slot04_12[obj->field_04](obj);
}

void func_801b3f68_slot04_12(Object *obj) {
    Object *p = obj->field_3c;

    obj->field_04 = obj->field_04 + 1;
    obj->field_1c = p->field_1c;
    obj->field_07 = p->kind;
    obj->field_0c = p->field_0c;
    obj->field_0d = p->field_0d;
    obj->field_0e = p->field_0e;
    obj->field_48 = 0;
    if (obj->field_03 == 0) {
        if (p->side == 0) {
            data_801c4f08_slot04_12[0] = data_1f8000b4;
        } else {
            data_801c4f08_slot04_12[1] = data_1f800164;
        }
    } else {
        if (p->side == 0) {
            data_801c4f08_slot04_12[2] = data_1f8000f0;
        } else {
            data_801c4f08_slot04_12[3] = data_1f8001a0;
        }
    }
    func_801b4064_slot04_12(obj);
}

void func_801b4064_slot04_12(Object *obj) {
    Object *p = obj->field_3c;

    obj->field_01 = 0;
    if (p->kind != obj->field_07) {
        func_801b4228_slot04_12(obj);
    } else if (obj->field_03 == 0) {
        func_801b40d0_slot04_12(obj, p);
    } else {
        func_801b417c_slot04_12(obj, p);
    }
}
