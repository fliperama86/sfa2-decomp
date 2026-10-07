/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801495f8(Object *object, int x, int y);
void func_80130678(Object *object, u16 index);
void func_801b4b8c_slot04_11(Object *obj);
void func_801b511c_slot04_11(Object *obj);
void func_801b5174_slot04_11(Object *obj);
void func_801b51a8_slot04_11(Object *obj);
void func_801b5874_slot04_11(Object *obj);
int func_801b57a8_slot04_11(Object *obj);

extern ObjectFn data_801c77c0_slot04_11[];
extern ObjectFn data_801c77c8_slot04_11[];
extern s32 data_801c77d4_slot04_11[];
extern ObjectFn data_801c77f4_slot04_11[];

void func_801b4ac8_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4b0c_slot04_11(Object *obj) {
    if (obj->kind == 0x13) {
        func_801b4b8c_slot04_11(obj);
    } else if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4b8c_slot04_11(Object *obj) {
    data_801c77c0_slot04_11[obj->field_07](obj);
}

void func_801b4bcc_slot04_11(Object *obj) {
    obj->field_4c = 0x50000;
    obj->field_54 = -0x3000;
    obj->field_50 = 0;
    obj->field_58 = 0;
    obj->field_07++;
}

void func_801b4bf4_slot04_11(Object *obj) {
    int a;
    u16 b;

    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
        if ((obj->field_3a & 0x80) == 0) {
            func_801b57a8_slot04_11(obj);
        }
        a = obj->field_3a & 0x7f;
        if (a != 0) {
            b = a;
            obj->field_3a = obj->field_3a & 0xff80;
            if (obj->field_0b == 0) {
                b = -a;
            }
            obj->pos_x = b + obj->pos_x;
        }
    }
}

void func_801b4cb4_slot04_11(Object *obj) {
    data_801c77c8_slot04_11[obj->field_07](obj);
}

void func_801b4cf4_slot04_11(Object *obj) {
    int k;

    obj->field_07++;
    k = obj->kind != 0x11;
    obj->field_4c = data_801c77d4_slot04_11[(k << 2) + 0];
    obj->field_54 = data_801c77d4_slot04_11[(k << 2) + 1];
    obj->field_50 = data_801c77d4_slot04_11[(k << 2) + 2];
    obj->field_58 = data_801c77d4_slot04_11[(k << 2) + 3];
    obj->field_45 = 1;
    func_80130efc(obj);
}

void func_801b4d84_slot04_11(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0 && func_801b57a8_slot04_11(obj) < 0 && obj->pos_y >= (s16)((Slot04bObj *)obj)->field_70) {
        obj->field_07++;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    }
    func_80130efc(obj);
}

void func_801b4e14_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4e74_slot04_11(Object *obj) {
    data_801c77f4_slot04_11[obj->field_07](obj);
}

void func_801b4eb4_slot04_11(Object *obj) {
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

void func_801b4f20_slot04_11(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->field_46 = 1;
        obj->field_07++;
        func_801b511c_slot04_11(obj);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b51a8_slot04_11(obj);
        func_801204f4(obj, obj->side, 7);
    }
    func_80130efc(obj);
}

void func_801b4fb4_slot04_11(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        ((Slot04bObj *)obj)->field_47 = 0x14;
        obj->field_07++;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b503c_slot04_11(Object *obj) {
    obj->field_27c++;
    func_80130efc(obj);
    ((Slot04bObj *)obj)->field_46--;
    if (((Slot04bObj *)obj)->field_46 == 0) {
        if (obj->field_45 != 0) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 3;
            obj->field_07 = 1;
        }
        func_80142c70(obj);
    } else {
        if (((Slot04bObj *)obj)->field_134 != 0) {
            if (((Slot04bObj *)obj)->field_27d == 0) {
                ((Slot04bObj *)obj)->field_27d = obj->field_27c;
            }
        } else if (((Slot04bObj *)obj)->field_47 != 0) {
            ((Slot04bObj *)obj)->field_47--;
        }
        func_80142fe8(obj);
    }
}

void func_801b511c_slot04_11(Object *obj) {
    int a;
    int b;

    if (obj->kind == 0x13) {
        func_801b5174_slot04_11(obj);
    } else {
        a = -3;
        b = 0x4f;
        if (obj->field_45 != 0) {
            a = -0xd;
            b = 0x44;
        }
        func_801495f8(obj, a, b);
    }
}

void func_801b5174_slot04_11(Object *obj) {
    if (obj->field_45 != 0) {
        func_801495f8(obj, -0xe, 0x47);
    } else {
        func_801495f8(obj, 8, 0x47);
    }
}
