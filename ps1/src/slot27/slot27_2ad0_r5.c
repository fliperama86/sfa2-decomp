/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_80026a58_slot27[];

void set_linked_objects_velocity_from_health_slot27(Object *obj) {
    int t;
    int u;
    data_8019045c[0] = 0;
    data_80190464[0] = 1;
    data_8019046c[0] = 0;
    if ((s16)obj->health >= 9) {
        data_8019045c[0] = 1;
        data_80190464[0] = 0;
        data_8019046c[0] = 1;
    }
    ref_first.p = (Object *)((ObjectWords *)obj)->word_54;
    ref_second.p = (Object *)((ObjectWords *)obj)->word_58;
    ref_first.p->field_62 = data_8019045c[0];
    ref_second.p->field_62 = data_80190464[0];
    if (obj->field_62 != data_8019046c[0]) {
        obj->field_62 = data_8019046c[0];
    }
    data_80190464[0] = (s16)obj->health;
    t = data_80026a58_slot27[data_80190464[0]];
    data_8019045c[0] = t;
    u = data_80026a58_slot27[data_80190464[0] + 1];
    data_80190464[0] = u;
    if (ref_other.p->side != 0) {
        data_8019045c[0] = -t;
    }
    ref_first.p->vel_x = data_8019045c[0];
    data_8019045c[0] = -data_8019045c[0];
    ref_second.p->vel_x = data_8019045c[0];
    ref_first.p->vel_y = data_80190464[0];
    data_80190464[0] = -data_80190464[0];
    ref_second.p->vel_y = data_80190464[0];
}
