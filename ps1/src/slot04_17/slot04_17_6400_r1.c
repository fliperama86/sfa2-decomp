/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void *data_801ce868_slot04_17[];
extern void *data_801ce8e0_slot04_17[];
extern Slot04_17Rece90c data_801ce90c_slot04_17[];
extern u8 data_801ce96c_slot04_17[];
extern Object *data_801ce9d0_slot04_17[];
extern ObjectFn data_801ce9e0_slot04_17[];
extern Slot04_17Recec3c data_801cec3c_slot04_17[];
extern u16 data_801cec5c_slot04_17[];
extern u16 data_801a2944[];
extern u16 data_801a29e4[];

Pooled *func_8011f4a4(void);
void func_8011f240(Slab172 *s);
void func_8011f38c(Object *o);

void func_801b6430_slot04_17(Object *obj);
void func_801b654c_slot04_17(Object *obj);
void func_801b65bc_slot04_17(Object *obj);
void func_801b6654_slot04_17(Object *obj);
void func_801b66d0_slot04_17(Object *obj);
void func_801b6854_slot04_17(Object *obj);
void func_801b69b0_slot04_17(Object *obj);
void func_801b6a94_slot04_17(Object *obj);
void func_801b6ad0_slot04_17(Object *obj);
void func_801b6adc_slot04_17(Object *obj, int index);

void func_801b6430_slot04_17(Object *obj) {
}

void func_801b6438_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 == k) {
        func_801b6430_slot04_17(o);
    } else {
        o->field_05 = 0;
        obj->field_47 = k;
        func_801b66d0_slot04_17(o);
        func_801204f4(ref_other.p->other, ref_other.p->other->side, 0xf);
        func_801b65bc_slot04_17(o);
    }
}

void func_801b64c0_slot04_17(Object *obj) {
    obj->field_04++;
    ref_other.p = obj->field_3c;
    if (ref_other.p->field_3c == obj) {
        ref_other.p->field_3c = 0;
    }
    if (obj->field_34 != 0) {
        ref_other.p = (Object *)obj->field_34;
        if (obj == ref_other.p->field_3c) {
            ref_other.p->field_04++;
            obj->field_34 = 0;
        }
    }
}

void func_801b654c_slot04_17(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b656c_slot04_17(Object *obj) {
    int d = data_801ce90c_slot04_17[ref_other.p->kind].b;

    obj->pos_x = ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y - d - 0x18;
}

void func_801b65bc_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_17Recec3c *r;

    obj->field_46 = 5;
    r = &data_801cec3c_slot04_17[o->other->side];
    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    o->box_tables = data_801ce868_slot04_17[obj->field_47];
}

void func_801b661c_slot04_17(u8 *src, u8 *dst) {
    if (*src != 0) {
        do {
            *dst = *src;
            src++;
            dst++;
        } while (*src != 0);
        *dst = 0;
    }
}

void func_801b6654_slot04_17(Object *obj) {
    Slot04_17Recec3c *r = &data_801cec3c_slot04_17[ref_other.p->side];

    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    obj->box_tables = data_801ce8e0_slot04_17[0];
    func_801b6854_slot04_17(obj);
}

int func_801b66b4_slot04_17(u16 index) {
    return data_801ce96c_slot04_17[index];
}

void func_801b66d0_slot04_17(Object *obj) {
    u16 *s = data_801cec5c_slot04_17 + ref_other.p->side * 16;
    Job *job;
    Rect rect;
    u16 *p;
    u16 *q;
    int i;

    job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x60;
        rect.y = ref_other.p->field_0d + 0x1e0;
        rect.w = 0x10;
        rect.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = (u8 *)(data_801cec5c_slot04_17 + ref_other.p->side * 16);
        i = 0;
        p = data_801a2944;
        if (ref_other.p->side == 0) {
            p = data_801a29e4;
        }
        q = p + 0xa00;
        do {
            *p = *s;
            p++;
            *q = *s;
            s++;
            q++;
            i++;
        } while (i < 0x10);
    }
}

void func_801b67f0_slot04_17(Object *obj) {
    Object *p = ref_other.p;
    Rect rect;

    rect.x = p->side * 32 + 0x110;
    rect.y = 0x1e0;
    rect.w = 0x10;
    rect.h = 1;
    func_80158028(&rect, (u8 *)(data_801cec5c_slot04_17 + p->side * 16));
}

void func_801b6854_slot04_17(Object *obj) {
    if (game_state.field_65 == 0) {
        if (ref_other.p->side == 0) {
            func_80136e90();
        } else {
            func_80136ed0();
        }
        func_8013786c(ref_other.p);
    }
}

void func_801b68bc_slot04_17(Object *obj) {
}

Object *func_801b68c4_slot04_17(Object *obj) {
    return data_801ce9d0_slot04_17[obj->field_0e];
}

void func_801b68e8_slot04_17(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801ce9e0_slot04_17[obj->field_04](obj);
}

void func_801b6938_slot04_17(Object *obj) {
    obj->field_04++;
    obj->field_1c = ref_other.p->field_1c;
    obj->field_0c = ref_other.p->field_0c;
    obj->field_0e = ref_other.p->field_0e;
    obj->field_48 = 0;
    obj->field_01 = 0;
    func_801b69b0_slot04_17(obj);
}

void func_801b69b0_slot04_17(Object *obj) {
    Object *p;
    u8 f;

    obj->field_01 = 0;
    if (ref_other.p->kind != 0x11 && ref_other.p->kind != 0x13) {
        func_801b6a94_slot04_17(obj);
    }
    p = ref_other.p;
    f = p->frame->field_09;
    if (f != 0) {
        obj->pos_x = p->pos_x;
        obj->pos_y = ref_other.p->pos_y;
        obj->field_0b = ref_other.p->field_0b;
        obj->field_01 = 1;
        if (obj->field_48 == f) {
            func_80131094(obj);
        } else {
            func_801b6adc_slot04_17(obj, f);
        }
    } else {
        func_801b6ad0_slot04_17(obj);
    }
}

void func_801b6a94_slot04_17(Object *obj) {
    obj->field_04++;
}

void func_801b6aa8_slot04_17(Object *obj) {
    ref_other.p->field_28 = 0;
    func_8011f38c(obj);
}

void func_801b6ad0_slot04_17(Object *obj) {
    obj->field_48 = 0;
    obj->field_01 = 0;
}
