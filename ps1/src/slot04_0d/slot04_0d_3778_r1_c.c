/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2c00_slot04_0d[];
extern u8 data_801c35a0_slot04_0d[];
extern s32 data_801c35a8_slot04_0d[];
extern u8 data_801c35c0_slot04_0d[];
extern ObjectFn data_801c35cc_slot04_0d[];
extern ObjectFn data_801c35dc_slot04_0d[];
extern ObjectFn data_801c35f0_slot04_0d[];

Block172 *func_8011f1e0(void);
void func_80137b64(Object *object);
void func_80137be0(Object *object);
void func_80137cc0(Object *object);
void func_80137dc8(Object *object);
void func_80137f00(Object *object);
void func_801b39c8_slot04_0d(Object *object, GameState *g);
void func_801b3d0c_slot04_0d(Object *object);
void func_801b3d38_slot04_0d(Object *object);
void func_801b3f7c_slot04_0d(Object *object);
void func_801b40bc_slot04_0d(Object *object);

void func_801b3990_slot04_0d(Object *object) {
    func_801b39c8_slot04_0d(object, &game_state);
    func_80130efc(object);
}

void func_801b39c8_slot04_0d(Object *object, GameState *g) {
    if (((Slot04bObj *)object)->field_46 != 0) {
        ((Slot04bObj *)object)->field_46--;
        if (((Slot04bObj *)object)->field_46 == 0) {
            g->field_4b |= 1 << object->side;
        }
    }
}

void func_801b3a14_slot04_0d(Object *object) {
    Object *p;
    int dx;
    u16 e;
    u8 d;

    func_80130efc(object);
    if (object->field_3a & 0xff) {
        object->field_3a = object->field_3a & 0xff00;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x77;
            p->field_3c = object;
            p->field_0c = object->field_0c;
            p->field_0d = object->field_0d;
            p->field_0e = object->field_0e;
            p->field_0b = object->field_0b;
            p->field_1c = object->field_1c;
            e = object->field_1e;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_1e = e;
            d = object->field_0d;
            p->field_02 = 5;
            p->field_08 = 0x20;
            p->field_0d = d;
            p->field_90 = object->field_90;
            p->field_98 = object->field_98;
            p->field_9c = object->field_9c;
            p->field_66 = object->field_66;
            *(s32 *)&p->field_10 = *(s32 *)&object->field_10;
            *(s32 *)&p->field_14 = *(s32 *)&object->field_14;
            p->pos_y = p->pos_y - 0x63;
            dx = -0x3b;
            if (object->field_0b != 0) {
                dx = 0x3b;
            }
            p->pos_x = dx + p->pos_x;
        }
    }
    func_801b39c8_slot04_0d(object, &game_state);
}

void func_801b3b6c_slot04_0d(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x4f; i >= 0; i--) {
        *p++ = z;
    }
}

void func_801b3b90_slot04_0d(Object *object) {
    data_801c35cc_slot04_0d[object->field_04](object);
}

void func_801b3bd0_slot04_0d(Object *object) {
    Object *parent;
    int dx;
    int i;

    parent = object->field_3c;
    object->field_09 = 0;
    object->field_04 = object->field_04 + 1;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    object->field_1e = parent->field_1e;
    ((Slot04bObj *)object)->field_6c = data_801c2c00_slot04_0d;
    if (object->field_03 != 0) {
        func_801b40bc_slot04_0d(object);
    }
    object->field_45 = 0;
    object->field_50 = 0;
    i = object->field_ac >> 1;
    i = data_801c35a8_slot04_0d[i];
    dx = 0x59;
    if (object->field_0b == 0) {
        i = -i;
        dx = -0x59;
    }
    object->field_4c = i;
    object->pos_x = object->pos_x + dx;
    object->pos_y = object->pos_y - 0x36;
    i = object->field_ac >> 1;
    object->field_a0 = data_801c35a0_slot04_0d[i];
    if (parent->side == 0) {
        ((Slot04bObj *)object)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)object)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(object, i);
    func_801b3d0c_slot04_0d(object);
    func_801b3d38_slot04_0d(object);
}

void func_801b3d0c_slot04_0d(Object *object) {
    object->field_46 = *(u16 *)(data_801c35c0_slot04_0d + (object->field_ac & 0xfe)) | (object->field_46 & 0xff00);
}

void func_801b3d38_slot04_0d(Object *object) {
    data_801c35dc_slot04_0d[object->field_05](object);
}

void func_801b3d78_slot04_0d(Object *object) {
    u16 t;

    if ((game_state.field_65 | game_state.field_a8) != 0) {
        goto call;
    }
    if (object->field_03 != 0) {
        if ((s16)object->field_3a >= 0) {
            goto call;
        }
        goto set;
    }
    t = object->field_46 - 1;
    object->field_46 = t;
    if ((u8)t == 0) {
        goto set;
    }
call:
    func_80137b64(object);
    return;
set:
    object->field_04 = 2;
    object->field_05 = 0;
    object->field_06 = 0;
    object->field_07 = 0;
    func_801b3f7c_slot04_0d(object);
}

void func_801b3e10_slot04_0d(Object *object) {
    if ((game_state.field_65 | game_state.field_a8) != 0) {
        func_80137be0(object);
    } else {
        data_801c35f0_slot04_0d[object->field_06](object);
    }
}

void func_801b3e78_slot04_0d(Object *object) {
    object->field_06 = object->field_06 + 1;
    func_801b40bc_slot04_0d(object);
    ((Slot04bObj *)object)->field_46 = 2;
    func_80131094(object);
}

void func_801b3ec0_slot04_0d(Object *object) {
    ((Slot04bObj *)object)->field_46--;
    if (((Slot04bObj *)object)->field_46 == 0) {
        object->field_00 = 1;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
}

void func_801b3efc_slot04_0d(Object *object) {
    func_80137cc0(object);
}

void func_801b3f1c_slot04_0d(Object *object) {
    func_801b3d0c_slot04_0d(object);
    func_80137dc8(object);
}

void func_801b3f4c_slot04_0d(Object *object) {
    func_801b3d0c_slot04_0d(object);
    func_80137f00(object);
}
