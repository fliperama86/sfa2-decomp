/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80125144(Object *o) {
    LogRec *rec;
    LogRec *row;
    if (o->field_80 != 0 && game_state.field_b6 == 0) {
        data_801900f8[o->field_02]++;
        data_801900f4[o->field_02] = o->field_0b;
    }
    row = table_801ac318[o->field_02];
    table_801aa4d8[o->field_02] = (table_801aa4d8[o->field_02] + 1) & 0x1f;
    rec = &row[table_801aa4d8[o->field_02]];
    rec->a = data_801900f8[o->field_02];
    rec->b = *(u8 *)&data_801900f4[o->field_02];
    rec->c = o->sequence->frame_index;
    rec->d = o->pos_x;
    rec->e = o->pos_y;
}
