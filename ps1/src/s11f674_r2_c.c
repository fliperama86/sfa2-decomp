/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8011fc00(Object *object) {
    SeqRec *rec;
    int v = object->field_38 - 1;
    object->field_38 = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->field_3a < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_8011fc84(object, rec);
    }
}

void func_8011fc54(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    func_8011fc84(object, *p);
}
