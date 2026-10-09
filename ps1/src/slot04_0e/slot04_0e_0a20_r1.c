/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);
void func_80152df4(Object *object);
void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);
Block172 *func_8011f1e0(void);
Pooled *func_8011f4a4(void);
int func_80141618(Object *object);
int func_801412a4(Object *object);

void func_801b0bd8_slot04_0e(Object *obj);
void func_801b1270_slot04_0e(Object *obj);
void func_801b12b0_slot04_0e(Object *obj);
void func_801b1488_slot04_0e(Object *obj);
void func_801b16d0_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);
void func_801b6808_slot04_0e(Object *obj);
void func_801b687c_slot04_0e(Object *obj);

extern u8 data_801a27e4[];
extern u16 data_801a2824[];
extern u16 data_801c5da4_slot04_0e[];
extern ObjectFn data_801c5efc_slot04_0e[];
extern ObjectFn data_801c5f0c_slot04_0e[];
extern ObjectFn data_801c5f28_slot04_0e[];
extern ObjectFn data_801c5f34_slot04_0e[];

void func_801b0a20_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a == 3) {
        obj->field_3a = 0;
        func_801204f4(o, o->field_02, 0x15);
    }
    func_80130efc(o);
}


int func_801b0a68_slot04_0e(Object *a, Object *b) {
    *(s32 *)&b->field_14 = *(s32 *)&b->field_14 - a->field_50;
    return a->field_50 += a->field_58;
}

void func_801b0a94_slot04_0e(Object *object, int arg) {
    u16 index = arg;
    u16 fi;

    if (object->side == 0) {
        object->sequence = seqs_68_left[index];
    } else {
        object->sequence = seqs_118_right[index];
    }
    object->field_38 = object->sequence->duration;
    object->field_3a = object->sequence->flags;
    fi = object->sequence->frame_index;
    object->field_80 = 1;
    object->frame = object->frames + fi;
}

void func_801b0b10_slot04_0e(Object *object) {
    Object *o = object;
    u16 fi;

    *(s16 *)&o->field_38 = (s16)o->field_38 - 1;
    if ((s16)o->field_38 == 0) {
        if ((s16)o->field_3a < 0) {
            o->sequence = o->sequence + o->sequence->loop_offset;
        } else {
            o->sequence = o->sequence + 1;
        }
        o->field_38 = o->sequence->duration;
        o->field_3a = o->sequence->flags;
        fi = o->sequence->frame_index;
        o->field_80 = 1;
        o->frame = o->frames + fi;
    }
}

void func_801b0bb8_slot04_0e(Object *object) {
    func_801378d8(object);
}

void func_801b0bd8_slot04_0e(Object *obj) {
    Rect r;
    Job *job;
    u8 *src;

    src = data_801a27e4 + (obj->field_0d << 5);
    r.x = (obj->field_02 << 5) + 0x110;
    r.y = (game_state.field_1d & 1) + 0x1e0;
    r.w = 0x10;
    r.h = 1;
    func_80158028(&r, src);
    job = (Job *)func_8011f4a4();
    if (job) {
        r.x = 0x60;
        r.y = obj->field_0d + 0x1e0;
        r.w = 0x10;
        r.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = r;
        job->src = src;
    }
}

void func_801b0cc8_slot04_0e(void) {
}

void func_801b0cd0_slot04_0e(Object *obj) {
    u16 i;

    for (i = 0; i < 0x10; i++) {
        data_801a2824[(obj->field_0d << 4) + i] = data_801c5da4_slot04_0e[i];
    }
    func_80137220(0, 6);
}

void func_801b0d44_slot04_0e(Object *object) {
    data_801c5efc_slot04_0e[object->field_06](object);
}

void func_801b0d84_slot04_0e(Object *object) {
    object->field_06 = object->field_06 + 1;
    object->field_0b = object->field_158;
    func_80130678(object, 0);
}

void func_801b0db8_slot04_0e(Object *object) {
    if (game_state.field_5c == 0) {
        object->field_06 = object->field_06 + 1;
    }
    func_80130efc(object);
}

void func_801b0df4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->field_06 = o->field_06 + 1;
    if (game_state_second.field_a6 == 0) {
        arg = 0x28;
    }
    func_80130678(o, arg);
}

void func_801b0e40_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 t;

    if (obj->field_47 != 0) {
        t = obj->field_47 - 1;
        obj->field_47 = t;
        if (t == 0) {
            game_state_second.field_4b = game_state.field_4b | (1 << o->side);
        }
    }
    data_801c5f0c_slot04_0e[o->field_07](o);
    func_80130efc(o);
}

void func_801b0edc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    int x;

    if (obj->field_3a == 1) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x3b;
            p->field_03 = 7;
            p->field_09 = 0;
            p->field_02 = 0x14;
            p->field_65 = o->field_65;
            p->field_3c = o;
            p->field_0b = o->field_0b;
            x = 0x33;
            if (o->field_0b == 0) {
                x = -0x33;
            }
            p->pos_x = x + o->pos_x;
            p->pos_y = obj->field_70 - 0x3b;
            obj->field_3a = 0;
            o->field_07 = o->field_07 + 1;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = o->field_0d;
            p->field_08 = 0x20;
            p->field_90 = o->field_90;
            p->field_66 = o->field_66;
            p->field_98 = o->field_98;
            p->field_9c = o->field_9c;
        }
    }
}

void func_801b0ff0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a == 2) {
        obj->field_3a = 0;
        o->field_07 = o->field_07 + 1;
        func_801204f4(o, o->side, 0xc);
        func_801b6808_slot04_0e(o);
    }
}

void func_801b1044_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a == 3) {
        obj->field_3a = 0;
        o->field_4c = 0;
        o->field_54 = 0;
        o->field_07 = o->field_07 + 1;
        func_80152df4(o);
        func_80120554(o, o->field_02, 0x321);
        o->field_50 = 0x5c000;
        o->field_58 = -0x5400;
        o->field_45 = 1;
        func_801308c4(o, 0x13);
        func_801b0bd8_slot04_0e(o);
    }
}

void func_801b10d4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_801b4914_slot04_0e(o);
    if (o->field_70 >= o->pos_y) {
        func_801b0bd8_slot04_0e(o);
    } else {
        obj->field_46 = 4;
        o->field_45 = 0;
        o->field_14 = 0;
        o->field_07 = o->field_07 + 1;
        o->pos_y = obj->field_70;
        func_801308c4(o, 0x14);
        func_8013786c(o);
        func_801b687c_slot04_0e(o);
    }
}

void func_801b1160_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_46--;
    if (obj->field_46 == 0) {
        o->field_45 = 1;
        o->field_50 = 0x28000;
        o->field_58 = -0x4800;
        o->field_07 = o->field_07 + 1;
        func_801308c4(o, 0x15);
    }
}

void func_801b11c4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_801b4914_slot04_0e(o);
    if (o->field_70 < o->pos_y) {
        o->field_07 = o->field_07 + 1;
        o->field_45 = 0;
        o->field_14 = 0;
        o->pos_y = obj->field_70;
        func_801308c4(o, 0x16);
    }
}

void func_801b1228_slot04_0e(Object *obj) {
}

void func_801b1230_slot04_0e(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b16d0_slot04_0e(obj);
    } else {
        func_801b1270_slot04_0e(obj);
    }
}

void func_801b1270_slot04_0e(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b1488_slot04_0e(obj);
    } else {
        func_801b12b0_slot04_0e(obj);
    }
}

void func_801b12b0_slot04_0e(Object *obj) {
    data_801c5f28_slot04_0e[obj->field_12a >> 1](obj);
}

void func_801b12f4_slot04_0e(Object *obj) {
    data_801c5f34_slot04_0e[obj->field_07](obj);
}

void func_801b1334_slot04_0e(Object *obj) {
    obj->field_07++;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && func_8013f8c4(obj, -0x21, 7) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b13bc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->field_3a >= 0) {
        if ((obj->field_3a & 0x80) != 0 && (u8)func_80141618(o) != 0 || (u8)func_801412a4(o) != 0) {
            o->field_07 = 0;
        }
        func_80130efc(o);
    } else {
        func_801312b8(o);
    }
}
