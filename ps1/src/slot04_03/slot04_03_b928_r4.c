/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0f44_slot04_03(Object *obj);


/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0da4_slot04_03(Object *obj) {
    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->substate_index = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0e24_slot04_03(Object *obj) {
    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        func_801b0f44_slot04_03(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0eac_slot04_03(Object *obj) {
    if ((s16)obj->meter >= 0x30 && obj->link_flags == 0 && (u8)func_80141788(obj)) {
        func_801b0f44_slot04_03(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142718(obj);
    }
}

