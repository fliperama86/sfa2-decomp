/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b61c0_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int n);
void func_801b6340_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int n);
void func_801205c4(int a, unsigned c);
extern u8 data_801b8c91_slot04_sel;
extern u8 data_801b8c92_slot04_sel;
extern u8 data_801b8c93_slot04_sel;
extern u8 data_801b8c94_slot04_sel;
extern u8 data_801b8ddc_slot04_sel[];
extern u8 data_801b8de4_slot04_sel[];
extern u8 data_801b8de8_slot04_sel[];
extern u8 data_801b8dec_slot04_sel[];
extern u8 data_801b8df0_slot04_sel[];
extern u8 data_801b9d70_slot04_sel;
extern u8 data_801b9d74_slot04_sel;
extern Slot04SelRec8eac data_801b8eac_slot04_sel;
extern Slot04SelRec8eac data_801b8edc_slot04_sel;
extern Slot04SelRec8eac data_801b8f00_slot04_sel;
extern Slot04SelRec8eac data_801b8f30_slot04_sel;

void func_801b5b0c_slot04_sel(Object *obj, Slot04SelRec9d78 *rec) {
    Slot04SelRec8eac *a = &data_801b8eac_slot04_sel;
    Slot04SelRec8eac *b = &data_801b8edc_slot04_sel;
    Slot04SelRec8eac *c = &data_801b8f00_slot04_sel;
    Slot04SelRec8eac *d = &data_801b8f30_slot04_sel;

    if (rec->field_0e == 0 && ((game_state.field_07 >> obj->side) & 1)) {
        data_801b9d70_slot04_sel = data_801b8c91_slot04_sel;
        data_801b9d74_slot04_sel = rec->field_0e;
        func_801b61c0_slot04_sel(obj, rec, 6);
        data_801b8c91_slot04_sel = data_801b9d70_slot04_sel;
        rec->field_0e = data_801b9d74_slot04_sel;
        if (data_801b8c91_slot04_sel == 4) {
            data_801b9d70_slot04_sel = data_801b8c92_slot04_sel;
            func_801b6340_slot04_sel(obj, rec, 2);
            data_801b8c92_slot04_sel = data_801b9d70_slot04_sel;
        }
        if (data_801b8c91_slot04_sel == 5) {
            data_801b9d70_slot04_sel = data_801b8c93_slot04_sel;
            func_801b6340_slot04_sel(obj, rec, 1);
            data_801b8c93_slot04_sel = data_801b9d70_slot04_sel;
            game_state.field_56 = data_801b8df0_slot04_sel[data_801b9d70_slot04_sel];
        }
        if (data_801b8c91_slot04_sel == 6) {
            data_801b9d70_slot04_sel = data_801b8c94_slot04_sel;
            func_801b6340_slot04_sel(obj, rec, 2);
            data_801b8c94_slot04_sel = data_801b9d70_slot04_sel;
        }
        if (rec->field_0e != 0) {
            func_801205c4(obj->side, 1);
        }
    }
    a->field_0c[data_801b8ddc_slot04_sel[0]] = 0x1a;
    a->field_0c[data_801b8ddc_slot04_sel[1]] = 0x1a;
    a->field_0c[data_801b8ddc_slot04_sel[2]] = 0x1a;
    a->field_0c[data_801b8ddc_slot04_sel[3]] = 0x1a;
    a->field_0c[data_801b8ddc_slot04_sel[4]] = 0x1a;
    a->field_0c[data_801b8ddc_slot04_sel[5]] = 0x1a;
    a->field_0c[data_801b8ddc_slot04_sel[6]] = 0x1a;
    a->field_0c[data_801b8ddc_slot04_sel[data_801b8c91_slot04_sel]] = 0x10;
    b->field_0c[data_801b8de4_slot04_sel[0]] = 0x1b;
    b->field_0c[data_801b8de4_slot04_sel[1]] = 0x1b;
    b->field_0c[data_801b8de4_slot04_sel[2]] = 0x1b;
    b->field_0c[data_801b8de4_slot04_sel[data_801b8c92_slot04_sel]] = 0x1a;
    c->field_0c[data_801b8de8_slot04_sel[0]] = 0x1b;
    c->field_0c[data_801b8de8_slot04_sel[1]] = 0x1b;
    c->field_0c[data_801b8de8_slot04_sel[data_801b8c93_slot04_sel]] = 0x1a;
    d->field_0c[data_801b8dec_slot04_sel[0]] = 0x1b;
    d->field_0c[data_801b8dec_slot04_sel[1]] = 0x1b;
    d->field_0c[data_801b8dec_slot04_sel[2]] = 0x1b;
    d->field_0c[data_801b8dec_slot04_sel[data_801b8c94_slot04_sel]] = 0x1a;
}
