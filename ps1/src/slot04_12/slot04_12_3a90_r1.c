/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4da0_slot04_12[];
extern ObjectFn data_801c4da8_slot04_12[];
extern s32 data_801c4db8_slot04_12[];
extern u8 data_801c4dc4_slot04_12[];
extern ObjectFn data_801c4dc8_slot04_12[];
extern SequenceStep *data_801c4de8_slot04_12[];
extern u8 data_801c4dec_slot04_12[];
extern u8 data_801c4e28_slot04_12[];
extern ObjectFn data_801c4e30_slot04_12[];
extern ObjectFn data_801c4e40_slot04_12[];
extern ObjectFn data_801c4e54_slot04_12[];
extern SequenceStep **data_801c4f08_slot04_12[];
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern SequenceStep **data_1f8000f0;
extern SequenceStep **data_1f8001a0;

void func_8011f14c(Slab172 *o);
void func_8011ffdc(Object *o);

void func_801b3b0c_slot04_12(Object *obj);
void func_801b3c6c_slot04_12(Object *obj);
void func_801b3cfc_slot04_12(Object *obj);
void func_801b3e58_slot04_12(Object *obj);
void func_801b4064_slot04_12(Object *obj);
void func_801b40d0_slot04_12(Object *obj, Object *p);
void func_801b417c_slot04_12(Object *obj, Object *p);
void func_801b4228_slot04_12(Object *obj);

void func_801b3b0c_slot04_12(Object *obj) {
    data_801c4da8_slot04_12[obj->state_0](obj);
}

void func_801b3b4c_slot04_12(Object *obj) {
    Object *parent;
    u8 i;
    int k;

    parent = obj->parent;
    i = obj->vel_table_index;
    i >>= 1;
    obj->ot_depth = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c4da0_slot04_12;
    obj->field_0c = 0;
    obj->active_state = 0;
    obj->vel_y = 0;
    k = data_801c4db8_slot04_12[i];
    if (obj->facing_side != 0) {
        obj->vel_x = k;
        obj->pos_x = obj->pos_x + 0x58;
    } else {
        k = -k;
        obj->vel_x = k;
        obj->pos_x = obj->pos_x - 0x58;
    }
    obj->pos_y = obj->pos_y - 0x38;
    obj->variant_index_2 = data_801c4dc4_slot04_12[obj->vel_table_index >> 1];
    if (parent->side == 0) {
        ((Slot04bObj *)obj)->data_pointer = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->data_pointer = *(int *)0x1f800158;
    }
    func_80138070(obj, obj->vel_table_index >> 1);
    func_801b3c6c_slot04_12(obj);
}

void func_801b3c6c_slot04_12(Object *obj) {
    data_801c4dc8_slot04_12[obj->state_1](obj);
}

void func_801b3cac_slot04_12(Object *obj) {
    if (obj->state_1 != 0) {
        func_801b3cfc_slot04_12(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->parent->linked_object_3 = 0;
        func_80138070(obj, 3);
    }
}

void func_801b3cfc_slot04_12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}

void func_801b3d3c_slot04_12(Object *obj) {
    Object *p = obj->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    func_8011f14c((Slab172 *)obj);
}

void func_801b3d88_slot04_12(Object *obj) {
    data_801c4e30_slot04_12[obj->state_0](obj);
}

void func_801b3dc8_slot04_12(Object *obj) {
    Object *parent;

    obj->variant_index_2 = 0xff;
    obj->state_0 = obj->state_0 + 1;
    parent = obj->parent;
    obj->ot_depth = 0;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c4e28_slot04_12;
    ((Slot04bObj *)obj)->data_pointer = (int)data_801c4dec_slot04_12;
    obj->active_state = 0;
    obj->health = 0xff;
    func_80130700(obj, data_801c4de8_slot04_12[0]);
    func_801b3e58_slot04_12(obj);
}

void func_801b3e58_slot04_12(Object *obj) {
    data_801c4e40_slot04_12[obj->state_1](obj);
    func_8011ffdc(obj);
}

void func_801b3eac_slot04_12(Object *obj) {
    if (obj->parent->frame->frame_param == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}

void func_801b3ee4_slot04_12(Object *obj) {
    obj->state_0 = 1;
    obj->active_count = 1;
    obj->state_1 = 0;
    obj->state_2 = 0;
    obj->state_3 = 0;
    obj->health = 0xff;
}

void func_801b3f08_slot04_12(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801b3f28_slot04_12(Object *obj) {
    data_801c4e54_slot04_12[obj->state_0](obj);
}

void func_801b3f68_slot04_12(Object *obj) {
    Object *p = obj->parent;

    obj->state_0 = obj->state_0 + 1;
    obj->field_1c = p->field_1c;
    obj->state_3 = p->kind;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->field_0e = p->field_0e;
    obj->variant_index = 0;
    if (obj->table_index == 0) {
        if (p->side == 0) {
            data_801c4f08_slot04_12[0] = data_1f8000b4;
        } else {
            data_801c4f08_slot04_12[1] = data_1f800164;
        }
    } else {
        if (p->side == 0) {
            data_801c4f08_slot04_12[2] = data_1f8000f0;
        } else {
            data_801c4f08_slot04_12[3] = data_1f8001a0;
        }
    }
    func_801b4064_slot04_12(obj);
}

void func_801b4064_slot04_12(Object *obj) {
    Object *p = obj->parent;

    obj->field_01 = 0;
    if (p->kind != obj->state_3) {
        func_801b4228_slot04_12(obj);
    } else if (obj->table_index == 0) {
        func_801b40d0_slot04_12(obj, p);
    } else {
        func_801b417c_slot04_12(obj, p);
    }
}
