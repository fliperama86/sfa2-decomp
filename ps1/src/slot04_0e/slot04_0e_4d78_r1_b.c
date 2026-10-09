/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c60fc_slot04_0e[];
extern ObjectFn data_801c6108_slot04_0e[];
extern ObjectFn data_801c6110_slot04_0e[];
extern ObjectFn data_801c611c_slot04_0e[];

void func_80131468(Object *object);
void func_80146998(Object *object);
Block172 *func_8011f1e0(void);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80130678(Object *object, int arg);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_80138b38(GameState *state, Object *object);

void func_801b4914_slot04_0e(Object *obj);
void func_801b504c_slot04_0e(Object *obj);
void func_801b5088_slot04_0e(Object *obj);
void func_801b577c_slot04_0e(Object *obj);
void func_801b6b40_slot04_0e(Object *obj);

void func_801b5088_slot04_0e(Object *obj) {
    int z = 0;
    u32 *p = (u32 *)((u8 *)obj + 0x2b0);
    u8 i = 0;

    do {
        *p++ = z;
        i++;
    } while (i < 2);
}

void func_801b50b4_slot04_0e(Object *obj) {
    data_801c60fc_slot04_0e[obj->field_07](obj);
}

void func_801b50f4_slot04_0e(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    obj->field_157 = 0;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    func_801307e0(obj, 0x42);
}

void func_801b5144_slot04_0e(Object *obj) {
    Object *p;
    int t;
    u8 d;
    if (*(u8 *)&obj->field_3a == 1) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x3b;
            p->field_03 = 6;
            p->field_09 = 0;
            p->field_02 = 0x14;
            p->field_65 = obj->field_65;
            p->field_3c = obj;
            p->field_0b = obj->field_0b;
            t = 0x33;
            if (obj->field_0b == 0) {
                t = -0x33;
            }
            p->pos_x = t + obj->pos_x;
            p->pos_y = ((Slot04bObj *)obj)->field_70 - 0x3b;
            *(u8 *)&obj->field_3a = 0;
            obj->field_07++;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            d = obj->field_0d;
            p->field_08 = 0x20;
            p->field_0d = d;
            p->field_90 = obj->field_90;
            p->field_66 = obj->field_66;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
        }
    }
    func_80130efc(obj);
}

void func_801b5260_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a > 0) {
        func_80130efc(obj);
    } else {
        obj->field_17b = 0;
        func_801312b8(obj);
    }
}

void func_801b52a0_slot04_0e(Object *obj) {
    data_801c6108_slot04_0e[obj->field_07](obj);
}

void func_801b52e0_slot04_0e(Object *obj) {
    ((Slot04bObj *)obj)->field_27b = 0x19;
    obj->field_157 = 0;
    obj->field_07++;
    func_80146998(obj);
    func_801307e0(obj, 0x40);
}

void func_801b532c_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
        func_801b504c_slot04_0e(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b5380_slot04_0e(Object *obj) {
    data_801c6110_slot04_0e[obj->field_07](obj);
}

void func_801b53c0_slot04_0e(Object *obj) {
    ((Slot04bObj *)obj)->field_27b = 0x19;
    obj->field_157 = 0;
    obj->field_07++;
    func_80146998(obj);
    func_801307e0(obj, 0x41);
}

void func_801b540c_slot04_0e(Object *obj) {
    func_80130efc(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_45 = 1;
        obj->field_4c = 0xc0000;
        obj->field_54 = -0x2000;
        obj->field_50 = 0x40000;
        obj->field_58 = -0x8000;
        obj->field_07++;
        *(u8 *)&obj->field_3a = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
    }
}

void func_801b5498_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->field_70 < obj->pos_y) {
        obj->field_45 = 0;
        obj->field_10 = 0;
        obj->field_14 = 0;
        obj->field_07++;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801209c4(obj);
        obj->field_0b = obj->field_158;
        func_80131468(obj);
    }
}

void func_801b550c_slot04_0e(Object *obj) {
    data_801c611c_slot04_0e[obj->field_07](obj);
}

void func_801b554c_slot04_0e(Object *obj) {
    int a;

    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    a = 0x2f;
    if (obj->field_45 == 0) {
        a = 0x2b;
    }
    func_80130678(obj, a);
}

void func_801b55b8_slot04_0e(Object *obj) {
    s16 a;
    int b;

    if (((Slot04bObj *)obj)->field_3b != 0) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->field_46 = 1;
        obj->field_07++;
        if (obj->field_45 == 0) {
            a = -0x3b;
            b = 0x51;
        } else {
            a = -0xf;
            b = 0x54;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b6b40_slot04_0e(obj);
    }
    func_80130efc(obj);
}

void func_801b5660_slot04_0e(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        obj->field_07++;
        ((Slot04bObj *)obj)->field_47 = 0x14;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b56e8_slot04_0e(Object *obj) {
    obj->field_27c++;
    func_80130efc(obj);
    ((Slot04bObj *)obj)->field_46--;
    if (((Slot04bObj *)obj)->field_46 != 0) {
        func_801b577c_slot04_0e(obj);
    } else {
        if (obj->field_45 != 0) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 3;
            obj->field_07 = 1;
        }
        func_80142c70(obj);
    }
}
