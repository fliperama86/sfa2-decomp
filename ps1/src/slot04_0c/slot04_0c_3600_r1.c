/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bdb88_slot04_0c[];
extern u8 data_801be104_slot04_0c[];
extern ObjectFn data_801be19c_slot04_0c[];
extern u8 data_801be1ac_slot04_0c[];
extern u8 data_801be1b4_slot04_0c[];
extern ObjectFn data_801be1bc_slot04_0c[];
extern ObjectFn data_801be1d0_slot04_0c[];
extern ObjectFn data_801be1d8_slot04_0c[];

Block172 *func_8011f1e0(void);
void func_8011ffdc(Object *object);
void func_80130678(Object *object, u16 arg);
void func_80137b64(Object *object);
void func_80137cc0(Object *object);
void func_80137dc8(Object *object);
void func_80137f00(Object *object);
void func_801b3b58_slot04_0c(Object *object);
void func_801b3b7c_slot04_0c(Object *object);
void func_801b3dbc_slot04_0c(Object *object);
void func_801b3ed0_slot04_0c(Object *object);

void func_801b374c_slot04_0c(Object *object) {
    s16 t = object->field_46;

    if (t != 0) {
        t = t - 1;
        object->field_46 = t;
        if (t == 0) {
            game_state.config->field_4b |= object->side + 1;
        }
    }
    func_80130efc(object);
}

void func_801b37b0_slot04_0c(Object *object) {
    data_801bdb88_slot04_0c[object->field_06](object);
}

void func_801b37f0_slot04_0c(Object *object) {
    object->field_06 = object->field_06 + 1;
    object->field_0b = object->field_158;
    func_80130678(object, 0);
}

void func_801b3824_slot04_0c(Object *object) {
    if (game_state.config->field_5c == 0) {
        object->field_06 = object->field_06 + 1;
    }
    func_80130efc(object);
}

void func_801b386c_slot04_0c(Object *object) {
    int arg = 0x28;

    object->field_46 = 0x78;
    object->field_06 = object->field_06 + 1;
    if (game_state.config->field_a6 != 0) {
        arg = 0x29;
    }
    func_80130678(object, arg);
}

void func_801b38c0_slot04_0c(Object *object) {
    Object *p;
    s16 t;

    if (object->field_3a & 0xff) {
        object->field_3a = object->field_3a & 0xff00;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xe;
            p->field_3c = object;
            p->field_7a = object->field_7a;
            p->field_7c = object->field_7c;
            p->field_0d = object->field_0d;
            p->field_08 = 0x20;
            p->field_66 = object->field_66;
            p->field_90 = object->field_90;
            p->field_98 = object->field_98;
            p->field_9c = object->field_9c;
        }
    }
    t = object->field_46;
    if (t != 0) {
        t = t - 1;
        object->field_46 = t;
        if (t == 0) {
            game_state.config->field_4b |= object->side + 1;
        }
    }
    func_80130efc(object);
}

void func_801b39bc_slot04_0c(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x57; i >= 0; i--) {
        *p++ = z;
    }
}

void func_801b39e0_slot04_0c(Object *object) {
    data_801be19c_slot04_0c[object->field_04](object);
}

void func_801b3a20_slot04_0c(Object *object) {
    Object *parent;
    int speed = 0x30000;

    object->field_a0 = 0xff;
    object->field_04 = object->field_04 + 1;
    parent = object->field_3c;
    object->field_09 = 0;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    ((Slot04bObj *)object)->field_6c = data_801be104_slot04_0c;
    object->field_45 = 0;
    object->field_50 = 0;
    if (object->field_03 == 2) {
        func_801b3ed0_slot04_0c(object);
    }
    if (object->field_0b == 0) {
        object->field_4c = 0xfffd0000;
        object->pos_x = object->pos_x - 0x5a;
    } else {
        object->field_4c = speed;
        object->pos_x = object->pos_x + 0x5a;
    }
    object->pos_y = object->pos_y - 0x44;
    object->field_a0 = data_801be1ac_slot04_0c[object->field_ac >> 1];
    if (parent->side == 0) {
        ((Slot04bObj *)object)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)object)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(object, object->field_ac >> 1);
    func_801b3b58_slot04_0c(object);
    func_801b3b7c_slot04_0c(object);
}

void func_801b3b58_slot04_0c(Object *object) {
    object->field_af = data_801be1b4_slot04_0c[object->field_ac >> 1];
}

void func_801b3b7c_slot04_0c(Object *object) {
    data_801be1bc_slot04_0c[object->field_05](object);
}

void func_801b3bbc_slot04_0c(Object *object) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0 && --object->field_af == 0) {
        object->field_04 = 2;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
        func_801b3dbc_slot04_0c(object);
    } else {
        func_80137b64(object);
    }
}

void func_801b3c34_slot04_0c(Object *object) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        data_801be1d0_slot04_0c[object->field_06](object);
    }
    func_8011ffdc(object);
}

void func_801b3ca4_slot04_0c(Object *object) {
    object->field_06 = object->field_06 + 1;
    func_801b3ed0_slot04_0c(object);
    object->field_46 = 2;
    func_80131094(object);
}

void func_801b3cec_slot04_0c(Object *object) {
    object->field_46 = (s16)object->field_46 - 1;
    if ((s16)object->field_46 == 0) {
        object->field_00 = 1;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
    func_80131094(object);
}

void func_801b3d3c_slot04_0c(Object *object) {
    func_80137cc0(object);
}

void func_801b3d5c_slot04_0c(Object *object) {
    func_801b3b58_slot04_0c(object);
    func_80137dc8(object);
}

void func_801b3d8c_slot04_0c(Object *object) {
    func_801b3b58_slot04_0c(object);
    func_80137f00(object);
}

void func_801b3dbc_slot04_0c(Object *object) {
    data_801be1d8_slot04_0c[object->field_05](object);
    func_8011ffdc(object);
}
