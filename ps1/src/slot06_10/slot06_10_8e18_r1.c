/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea958_slot06_10[];
extern SeqRec data_801ea868_slot06_10[];
extern SeqRec data_801ea8a0_slot06_10[];
extern SeqRec data_801ea8d8_slot06_10[];

void func_801e8f14_slot06_10(SeqRec *rec, Slot06Cursor *cur);
void func_801e9018_slot06_10(Slot06Pal *pal);
void func_801e906c_slot06_10(Slot06Pal *pal);
void func_801e90a8_slot06_10(Slot06Pal *pal, SeqRec *rec);
void func_801e913c_slot06_10(Slot06Cursor *cur);

void func_801e8e18_slot06_10(Object *obj) {
    data_801ea958_slot06_10[obj->state_0](obj);
}

void func_801e8e58_slot06_10(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    obj->init_param_b = 0x10;
    obj->table_base_index = 0x11;
    obj->init_param_e = 0x12;
    obj->init_param_a = 0x2e0;
    obj->sequence_index = 0x200;
    obj->size_param = 0x180;
    obj->field_01 = 0;
    obj->offset_b = 0x50;
    obj->init_param_c = 0x50;
    obj->init_param_d = 0x50;
    obj->step_count = obj->step_count + 1;
    func_801e906c_slot06_10((Slot06Pal *)((u8 *)obj + 0x30));
    func_801e906c_slot06_10((Slot06Pal *)((u8 *)obj + 0x40));
    func_801e906c_slot06_10((Slot06Pal *)((u8 *)obj + 0x50));
    func_801e8f14_slot06_10(data_801ea868_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x28));
    func_801e8f14_slot06_10(data_801ea8a0_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x6c));
    func_801e8f14_slot06_10(data_801ea8d8_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x84));
}

void func_801e8f14_slot06_10(SeqRec *rec, Slot06Cursor *cur) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->remaining_count = new_var;
    cur->step_word = w;
}

void func_801e8f34_slot06_10(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            func_801e9018_slot06_10((Slot06Pal *)((u8 *)obj + 0x30));
            func_801e9018_slot06_10((Slot06Pal *)((u8 *)obj + 0x40));
            if (((Slot06Obj *)obj)->field_46 != 0) {
                ((Slot06Obj *)obj)->pos_x = 0x340;
                ((Slot06Obj *)obj)->pos_y = 0x80;
                *(u16 *)&((Slot06Obj *)obj)->field_46 = *(u16 *)&((Slot06Obj *)obj)->field_46 & 0xff00;
            }
            func_801e9018_slot06_10((Slot06Pal *)((u8 *)obj + 0x50));
            if (((Slot06Obj *)obj)->field_56 != 0) {
                ((Slot06Obj *)obj)->pos_x = 0x1c0;
                ((Slot06Obj *)obj)->pos_y = 0x80;
                *(u16 *)&((Slot06Obj *)obj)->field_56 = *(u16 *)&((Slot06Obj *)obj)->field_56 & 0xff00;
            }
            func_801e913c_slot06_10((Slot06Cursor *)((u8 *)obj + 0x28));
            func_801e913c_slot06_10((Slot06Cursor *)((u8 *)obj + 0x6c));
            func_801e913c_slot06_10((Slot06Cursor *)((u8 *)obj + 0x84));
        }
    }
}

void func_801e8ff8_slot06_10(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9018_slot06_10(Slot06Pal *pal) {
    SeqRec *rec;
    int v = pal->countdown - 1;
    pal->countdown = v;
    if ((s16)v == 0) {
        rec = pal->rec;
        rec++;
        if ((s16)pal->step_value < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e90a8_slot06_10(pal, rec);
    }
}

void func_801e906c_slot06_10(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    func_801e90a8_slot06_10(pal, (SeqRec *)table_8017c910[pal->seq_rec_index]);
}
