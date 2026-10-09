/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Used when both players hold one of three character pairings. */
/* Second name for shared_handler_17. The original loads that address in two
   separate blocks; with a single name this compiler merges them. */

void func_80130678(Object *object, int arg);

void reset_object(Object *object) {
    ObjectFn handler;
    int i;
    /* The original stores this zeroed local, not a constant, then reuses it. */
    int value = 0;

    object->field_45 = 0;
    object->field_73 = 0;
    object->field_157 = 0;
    object->field_15e = 0;
    object->field_15d = 0;
    object->field_163 = 0;
    object->field_159 = 0;
    object->field_15b = 0;
    object->field_15c = 0;
    object->field_167 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        player_right.field_cd = 0;
        object->field_19f = 0;
        object->field_1a2 = 0;
    }
    i = 0;
    object->field_0e = 2;
    object->field_69 = value;
    object->field_67 = value;
    object->field_74 = value;
    object->field_bc = value;
    object->field_262 = value;
    object->field_49 = value;
    object->field_80 = value;
    game_state.cursor = (unsigned int *)&object->field_1a0;
    do {
        i++;
        *game_state.cursor++ = 0;
    } while (i < 0x4d);
    if (object->field_cd) {
        object->field_d8 = 0;
        func_8014a038(object);
    }
    object->field_01 = 1;
    object->field_5c = 0x90;
    object->field_5e = 0x90;
    if (game_state.config->field_a9 != 0 && object->field_f1 != 0) {
        object->field_5c = object->field_fc;
        object->field_5e = object->field_fc;
        object->field_c6 = object->field_fe;
    }
    init_table_fields(object);
    set_start_position(object);
    value = -0x60;
    if (object->side != 0) value = 0x60;
    object->pos_x = object->pos_x + value;
    func_801293d4(object);
    if (player_left.kind == 0x0d && player_right.kind == 0x0d) {
        handler = shared_handler_15;
    } else if (player_left.kind == 0x10 && player_right.kind == 0x10) {
        handler = shared_handler_16;
    } else if (player_left.kind == 0x11 &&
               (player_right.kind == 0x11 || player_right.kind == 0x13)) {
        handler = shared_handler_17;
    } else if (player_left.kind == 0x13 &&
               (player_right.kind == 0x13 || player_right.kind == 0x11)) {
        handler = shared_handler_17_second;
    } else if (object->side == 0) {
        handler = handlers_left[object->kind];
    } else {
        handler = handlers_right[object->kind];
    }
    handler(object);
    player_left.field_02 = 0;
    player_right.field_02 = 1;
    select_box_tables(object);
    func_80138d70(&game_state);
    set_side_field_0d(object);
    func_80129448(object);
    func_80129428(object);
    object->field_0b = 1;
    object->field_158 = 1;
    object->field_0c = 0xff;
    if (object->side != 0) {
        object->field_0b = 0;
        object->field_158 = 0;
        object->field_0c = 0xff;
    }
    build_metrics(object);
    if (game_state.config->field_42 != 0) {
        func_801312b8(object);
    } else {
        object->field_05 = 7;
        object->field_04 = 1;
        if (object->kind != 0x0b) {
            object->field_06 = 0;
            object->field_07 = 0;
        }
        if (object->kind != 0x17) func_80130678(object, 0x22);
    }
    object->field_177 = 1;
    func_8013f3b8(object);
}

void func_801293d4(Object *object) {
    object->field_76 = 0x180;
    object->field_7a = 0x60;
    object->field_78 = 0;
    object->field_7c = 0x1e0;
    object->field_90 = data_801a27d8;
    if (object->side != 0) {
        object->field_76 = 0x1c0;
        object->field_78 = 0;
        object->field_90 = data_801a27dc;
    }
}

void func_80129428(Object *object) {
    func_801378d8(object);
}

void func_80129448(Object *object) {
    object->field_f3 = kind_table_f3[object->kind];
}
