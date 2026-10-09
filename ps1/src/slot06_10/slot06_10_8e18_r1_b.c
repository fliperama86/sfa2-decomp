/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea968_slot06_10[];
extern ObjectFn data_801ea978_slot06_10[];
extern void (*data_801ea988_slot06_10[])(Object *, Object *);
extern SeqRec data_801ea944_slot06_10[];
extern SequenceStep *data_801ebbc8_slot06_10[];

void func_801e913c_slot06_10(Slot06Cursor *cur);
void func_801e9190_slot06_10(Slot06Cursor *cur, SeqRec *rec);
void func_801e9248_slot06_10(SeqRec *rec, Slot06Cursor *cur);
void func_801e92c8_slot06_10(Slot06Cursor *cur);
void func_801e931c_slot06_10(Slot06Cursor *cur, SeqRec *rec);
void func_801e95c0_slot06_10(Object *obj, int unused, u8 index);
void func_801e960c_slot06_10(Object *obj, Object *other);
void func_801e9678_slot06_10(Object *obj, Object *other);
void func_801e96e8_slot06_10(Object *obj, Object *other);

void func_801e913c_slot06_10(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->field_04 - 1;
    cur->field_04 = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e9190_slot06_10(cur, rec);
    }
}

void func_801e9190_slot06_10(Slot06Cursor *cur, SeqRec *rec) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->field_04 = new_var;
    cur->field_06 = w;
    func_8011fcc0(rec);
}

void func_801e91cc_slot06_10(Object *obj) {
    data_801ea968_slot06_10[obj->field_04](obj);
}

void func_801e920c_slot06_10(Object *obj) {
    obj->field_01 = 0;
    obj->field_04 = obj->field_04 + 1;
    func_801e9248_slot06_10(data_801ea944_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x28));
}

void func_801e9248_slot06_10(SeqRec *rec, Slot06Cursor *cur) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->field_04 = new_var;
    cur->field_06 = w;
}

void func_801e9268_slot06_10(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            func_801e92c8_slot06_10((Slot06Cursor *)((u8 *)obj + 0x28));
        }
    }
}

void func_801e92a8_slot06_10(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e92c8_slot06_10(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->field_04 - 1;
    cur->field_04 = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e931c_slot06_10(cur, rec);
    }
}

void func_801e931c_slot06_10(Slot06Cursor *cur, SeqRec *rec) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->field_04 = new_var;
    cur->field_06 = w;
    func_8011fcc0(rec);
}

void func_801e9358_slot06_10(Object *obj) {
    data_801ea978_slot06_10[obj->field_04](obj);
}

void func_801e9398_slot06_10(Object *obj) {
    obj->field_0f = 1;
    obj->field_04 = 1;
    obj->field_76 = 0x340;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    obj->field_05 = 0;
    obj->field_46 = 0;
    obj->field_0d = 0;
    obj->field_81 = 4;
    obj->pos_y = obj->pos_y + 0x70;
    func_80130700(obj, data_801ebbc8_slot06_10[0]);
}

void func_801e940c_slot06_10(Object *obj) {
    Object *other;
    if ((game_state.field_65 | game_state.field_74) == 0) {
        other = &player_left;
        if (obj->field_03 != 0) {
            other = other + 1;
        }
        data_801ea988_slot06_10[obj->field_05](obj, other);
    }
    func_8011ffdc(obj);
}

void func_801e9490_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801e95c0_slot06_10(obj, (int)other, 0);
        func_801e960c_slot06_10(obj, other);
    } else {
        func_80131094(obj);
    }
}

void func_801e94f4_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801e95c0_slot06_10(obj, (int)other, 3);
        func_801e96e8_slot06_10(obj, other);
        func_801e9678_slot06_10(obj, other);
    } else {
        func_80131094(obj);
    }
}
