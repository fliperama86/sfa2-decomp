/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190458;
extern Slot04_0fRec6194 data_801c6194_slot04_0f[];

Block172 *pop_block_from_pool(void);

void spawn_pooled_block_with_table_velocity_slot04_0f(Object *obj) {
    Object *p;
    u16 t;
    int a;
    int x;
    int y;
    int dx;
    int dy;

    if (((game_state.config->side_offset + data_80190458.p->side) & 3) == 0) {
        t = obj->step_flags;
        a = -3;
        if ((t & 0xff) != 0) {
            if (t & 0x80) {
                a = 3;
            }
            obj->variant_index = (obj->variant_index + a) & 0x3f;
            p = (Object *)pop_block_from_pool();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 0x25;
                p->parent = data_80190458.p;
                p->field_0c = data_80190458.p->field_0c;
                p->spawn_x = 0x60;
                p->spawn_y = 0x1e0;
                p->image_row = obj->image_row;
                p->field_90 = data_80190458.p->field_90;
                p->data_ptr_a = data_80190458.p->data_ptr_a;
                p->data_ptr_b = data_80190458.p->data_ptr_b;
                p->mode_id = 0x20;
                p->field_02 = 4;
                p->sound_side = obj->sound_side;
                p->field_0e = data_80190458.p->field_0e;
                p->pos_x = data_80190458.p->pos_x;
                p->pos_y = data_80190458.p->pos_y - 0x3a;
                a = 0x28;
                if (data_80190458.p->facing_side == 0) {
                    a = -0x28;
                }
                p->pos_x += a;
                p->ot_depth = 6;
                p->field_1c = data_80190458.p->field_1c;
                p->facing_side = data_80190458.p->facing_side;
                a = obj->variant_index & 0x1f;
                y = data_801c6194_slot04_0f[a].b;
                x = data_801c6194_slot04_0f[a].a;
                y = -y;
                x <<= 8;
                y <<= 8;
                dx = x << 3;
                dy = y << 4;
                if (data_80190458.p->facing_side == 0) {
                    x = -x;
                    dx = -dx;
                }
                p->vel_x = x;
                p->vel_y = y;
                *(s32 *)&p->pos_x_frac += dx;
                *(s32 *)&p->pos_y_frac += dy;
            }
        }
    }
}
