/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_players_and_hud_for_slot0f_scene_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    HudState *h = data_8018f5a0;
    Object *left = &player_left;
    Object *right = left + 1;
    h->step_index += 1;
    obj->field_32 = 0;
    obj->field_33 = 0;
    obj->field_1b = 0;
    obj->handler_index_b = 0;
    obj->handler_index = 0;
    obj->field_05 = 0;
    obj->field_54 = 0;
    obj->field_66 = 0;
    obj->field_f0 = 0;
    obj->field_84 = 0;
    obj->field_12c = 0;
    obj->field_12e = 0;
    func_8011eb14();
    obj->field_42 = 1;
    obj->countdown_2 = 0x13;
    obj->countdown_b = 0x3b;
    obj->left_ptr = left;
    obj->right_ptr = right;
    left->field_01 = 1;
    left->active_count = 1;
    right->field_01 = 1;
    right->active_count = 1;
    left->other = right;
    right->other = left;
    left->field_d4 = 0;
    left->field_02 = 0;
    left->side = 0;
    right->field_d4 = 1;
    right->field_02 = 1;
    right->side = 1;
    right->mode_flag_2 = 1;
    left->mode_flag_2 = 1;
    left->state_3 = 0;
    left->state_2 = 0;
    left->state_1 = 0;
    left->state_0 = 0;
    right->state_3 = 0;
    right->state_2 = 0;
    right->state_1 = 0;
    right->state_0 = 0;
    right->meter = 0x90;
    left->meter = 0x90;
    right->kind = ((u8 *)&data_801ac624)[0];
    left->kind = ((u8 *)&data_801ac624)[0];
    obj->field_40 = ((u16 *)&data_801ac624)[1];
    update_player_inputs_and_pick_handler();
    data_80190568 = 1;
    data_801aa544[0].active_flag = 1;
    data_801aa5d4[0] = 1;
    cam_obj[0] = 1;
    queue_image_upload_job(0, 6);
    queue_image_upload_job(1, 0);
    queue_image_upload_job(2, 1);
    queue_image_upload_job(3, 2);
    queue_image_upload_job(4, 7);
    request_sound_command_and_switch_thread(1);
    func_80136dc4();
    func_801285e0();
    clear_four_arrays_and_call_kind_handlers();
    set_capped_value_and_ot_indices((ObjectView *)obj);
    obj->field_2c = 0xff;
    init_hud_primitives();
    data_801a6938 = 0;
}
