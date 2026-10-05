/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8011f70c(Object *object) {
    if (object->field_2a2 != 0) {
        object->field_16a = 1;
        object->field_2a2--;
        object->field_7e = object->field_7e - 1;
        if (object->field_7e & 0x80) {
            object->field_225 = 0;
        }
        object->field_182 = object->field_182 + 1;
        if ((s16)object->field_182 >= 0xae) {
            object->field_182 = 0xad;
        }
    }
}

void func_8011f774(Object *object) {
    if (object->field_2a1 != 0) {
        object->field_2a1--;
        if (object->side != 0) {
            game_state.cursor = table_80185cf4;
        } else {
            game_state.cursor = table_80185e50;
        }
        object->field_180 = object->field_180 + 1;
        if ((s16)object->field_180 >= 0xae) {
            object->field_180 = 0xad;
        }
    }
}

void func_8011f7e8(Object *object) {
    SeqRec *rec;
    int v = object->field_38 - 1;
    object->field_38 = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->field_3a < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_8011f914(object, rec);
    }
}

void func_8011f83c(Object *object) {
    SeqRec *rec;
    int v = object->field_38 - 1;
    object->field_38 = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->field_3a < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_8011f96c(object, rec);
    }
}

void func_8011f890(Object *object) {
    SeqRec *rec;
    int v = object->field_38 - 1;
    object->field_38 = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->field_3a < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_8011fa50(object, rec);
    }
}

void func_8011f8e4(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    func_8011f914(object, *p);
}

void func_8011f914(Object *object, SeqRec *rec) {
    func_8011f994(object, rec, data_801aa594);
}

void func_8011f93c(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    func_8011f96c(object, *p);
}

void func_8011f96c(Object *object, SeqRec *rec) {
    func_8011f994(object, rec, data_801aa624);
}
