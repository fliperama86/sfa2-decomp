/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;
void func_800e6bd8_slot0f(Object *obj);
void func_800e6874_slot0f(Object *obj);
void func_800e6a48_slot0f(Object *obj);
void func_800e6db4_slot0f(Object *obj);

void func_800e675c_slot0f(Object *obj) {
    int t;

    obj->pos_x = 0;
    obj->pos_y = 0;
    obj->pos_x = 0x58;
    obj->pos_y = 0x58;
    obj->field_01 = 0;
    obj->field_09 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_0c = 0;
    obj->field_5e = 10;
    obj->field_76 = 0;
    obj->field_04++;
    obj->field_48 = table_8016e5c4[game_state.field_2b8].field_08;
    obj->field_50 = (s32)game_state.cursor;
    game_state.field_308 = (Row *)((u8 *)data_80190468.p + 0x21e);
    obj->field_54 = (s32)game_state.field_308;
    t = ((Object *)((u8 *)data_80190468.p + ((Slot0fObj *)data_80190468.p)->field_21e))->field_15e & 0x3f;
    /* field_5c is stored twice. The first store goes through a plain pointer:
       written as a member store, this compiler moves the load of the second
       value above it. What the original source had here is not known. */
    *(u16 *)((u8 *)obj + 0x5c) = t;
    t = *(u16 *)&game_state.ring_b[0x3f] & 0x3f;
    obj->field_5c = t;
    func_800e6bd8_slot0f(obj);
    game_state.field_2bf = 0;
    func_800e6874_slot0f(obj);
    func_800e6a48_slot0f(obj);
    func_800e6db4_slot0f(obj);
}
