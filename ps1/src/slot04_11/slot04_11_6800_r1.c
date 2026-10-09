/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void *data_801c7b58_slot04_11[];
extern void *data_801c7bd0_slot04_11[];
extern ObjectFn data_801c7bf0_slot04_11[];
extern Slot04_11Rec7bfc data_801c7bfc_slot04_11[];
extern u8 data_801c7c5c_slot04_11[];
extern Object *data_801c7cc0_slot04_11[];
extern ObjectFn data_801c7cd0_slot04_11[];
extern Slot04_11Rec7cf0 data_801c7cf0_slot04_11[];
extern u16 data_801c7d10_slot04_11[];
extern u16 data_801a2944[];
extern u16 data_801a29e4[];

Pooled *func_8011f4a4(void);
Object *func_8011f32c(void);
void func_8011f240(Slab172 *s);

void func_801b68c0_slot04_11(Object *obj);
void func_801b6b58_slot04_11(Object *obj);
void func_801b6ca4_slot04_11(Object *obj);
void func_801b6cf4_slot04_11(Object *obj);
void func_801b6d8c_slot04_11(Object *obj);
void func_801b6e08_slot04_11(Object *obj);
void func_801b6f8c_slot04_11(Object *obj);
Object *func_801b6ffc_slot04_11(Object *obj);
void func_801b70e8_slot04_11(Object *obj);
void func_801b71cc_slot04_11(Object *obj);
void func_801b7208_slot04_11(Object *obj);
void func_801b7214_slot04_11(Object *obj, int idx);
void func_8011f38c(Object *o);

void func_801b6800_slot04_11(Object *obj) {
    Object *s = obj->other;

    func_801b6ca4_slot04_11(obj);
    if ((*(u32 *)&s->field_28c & 0x80ff) != 0x8000 && (game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) == 0) {
        data_801c7bf0_slot04_11[obj->field_05](obj);
        func_801b68c0_slot04_11(obj);
    } else {
        obj->field_04++;
        func_801b6f8c_slot04_11(obj);
    }
}

void func_801b68c0_slot04_11(Object *obj) {
    Slot04_11Rec7cf0 *r = &data_801c7cf0_slot04_11[obj->other->side];
    Object *t = func_801b6ffc_slot04_11(ref_other.p);

    r->c = obj->box_tables;
    r->x = obj->pos_x - t->pos_x - 8;
    r->y = obj->pos_y + t->pos_y;
    func_801519b4((Object *)r);
}

void func_801b694c_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *n;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        obj->field_47 = k;
        func_801b6e08_slot04_11(o);
        func_801204f4(ref_other.p->other, ref_other.p->other->side, 0xf);
        func_801b6cf4_slot04_11(o);
    } else {
        obj->field_46--;
        if (obj->field_46 == 0 && data_801a4fec != 0) {
            obj->field_46 = 0xc;
            o->field_05++;
            n = func_8011f32c();
            if (n != 0) {
                n->field_00 = 1;
                n->field_02 = 0x1e;
                n->field_0e = o->field_0e;
                n->field_48 = 0;
                n->field_3c = o;
                n->field_03 = 0;
                n->field_0b = o->field_0b;
                n->other = o->other;
                o->field_34 = (u32)n;
                n->field_76 = 0x240;
                n->field_7a = 0x60;
                n->field_7c = 0x1e2;
                n->field_98 = data_80172a48;
                n->field_78 = 0;
                n->field_90 = (void *)0x800fb100;
                n->field_9c = data_80173c9c;
                n->field_66 = o->field_66;
            }
        }
    }
}

void func_801b6a98_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        o->field_05 = 0;
        obj->field_47 = k;
        func_801b6e08_slot04_11(o);
        func_801204f4(ref_other.p->other, ref_other.p->other->side, 0xf);
        func_801b6cf4_slot04_11(o);
    } else {
        obj->field_46--;
        if (obj->field_46 == 0) {
            o->field_05++;
            func_801b6d8c_slot04_11(o);
        } else {
            func_801b6b58_slot04_11(o);
        }
    }
}

void func_801b6b58_slot04_11(Object *obj) {
}

void func_801b6b60_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 == k) {
        func_801b6b58_slot04_11(o);
    } else {
        o->field_05 = 0;
        obj->field_47 = k;
        func_801b6e08_slot04_11(o);
        func_801204f4(ref_other.p->other, ref_other.p->other->side, 0xf);
        func_801b6cf4_slot04_11(o);
    }
}

void func_801b6be8_slot04_11(Object *obj) {
    obj->field_04++;
    ref_other.p = obj->field_3c;
    if (ref_other.p->field_3c == obj) {
        ref_other.p->field_3c = 0;
    }
    if (obj->field_34 != 0) {
        ref_other.p = (Object *)obj->field_34;
        if (obj == ref_other.p->field_3c && ref_other.p->field_00 != 0) {
            ref_other.p->field_04++;
            obj->field_34 = 0;
        }
    }
}

void func_801b6c84_slot04_11(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b6ca4_slot04_11(Object *obj) {
    int d = data_801c7bfc_slot04_11[ref_other.p->kind].b;

    obj->pos_x = ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y - d - 0x18;
}

void func_801b6cf4_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_11Rec7cf0 *r;

    obj->field_46 = 5;
    r = &data_801c7cf0_slot04_11[o->other->side];
    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    o->box_tables = data_801c7b58_slot04_11[obj->field_47];
}

void func_801b6d54_slot04_11(u8 *src, u8 *dst) {
    if (*src != 0) {
        do {
            *dst = *src;
            src++;
            dst++;
        } while (*src != 0);
        *dst = 0;
    }
}

void func_801b6d8c_slot04_11(Object *obj) {
    Slot04_11Rec7cf0 *r = &data_801c7cf0_slot04_11[ref_other.p->side];

    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    obj->box_tables = data_801c7bd0_slot04_11[0];
    func_801b6f8c_slot04_11(obj);
}

int func_801b6dec_slot04_11(u16 index) {
    return data_801c7c5c_slot04_11[index];
}

void func_801b6e08_slot04_11(Object *obj) {
    u16 *s = data_801c7d10_slot04_11 + ref_other.p->side * 16;
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
        job->src = (u8 *)(data_801c7d10_slot04_11 + ref_other.p->side * 16);
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

void func_801b6f28_slot04_11(Object *obj) {
    Object *p = ref_other.p;
    Rect rect;

    rect.x = p->side * 32 + 0x110;
    rect.y = 0x1e0;
    rect.w = 0x10;
    rect.h = 1;
    func_80158028(&rect, (u8 *)(data_801c7d10_slot04_11 + p->side * 16));
}

void func_801b6f8c_slot04_11(Object *obj) {
    if (game_state.field_65 == 0) {
        if (ref_other.p->side == 0) {
            func_80136e90();
        } else {
            func_80136ed0();
        }
        func_8013786c(ref_other.p);
    }
}

void func_801b6ff4_slot04_11(Object *obj) {
}

Object *func_801b6ffc_slot04_11(Object *obj) {
    return data_801c7cc0_slot04_11[obj->field_0e];
}

void func_801b7020_slot04_11(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801c7cd0_slot04_11[obj->field_04](obj);
}

void func_801b7070_slot04_11(Object *obj) {
    obj->field_04++;
    obj->field_1c = ref_other.p->field_1c;
    obj->field_0c = ref_other.p->field_0c;
    obj->field_0e = ref_other.p->field_0e;
    obj->field_48 = 0;
    obj->field_01 = 0;
    func_801b70e8_slot04_11(obj);
}

void func_801b70e8_slot04_11(Object *obj) {
    Object *p;
    u8 f;

    obj->field_01 = 0;
    if (ref_other.p->kind != 0x11 && ref_other.p->kind != 0x13) {
        func_801b71cc_slot04_11(obj);
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
            func_801b7214_slot04_11(obj, f);
        }
    } else {
        func_801b7208_slot04_11(obj);
    }
}

void func_801b71cc_slot04_11(Object *obj) {
    obj->field_04++;
}

void func_801b71e0_slot04_11(Object *obj) {
    ref_other.p->field_28 = 0;
    func_8011f38c(obj);
}

void func_801b7208_slot04_11(Object *obj) {
    obj->field_48 = 0;
    obj->field_01 = 0;
}
