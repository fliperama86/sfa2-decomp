/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void request_sound_command_and_switch_thread(int a) {
    ((SndCtl *)screen_state)->request_arg = a;
    ((SndCtl *)screen_state)->mode = 1;
    func_801577ec(0xff000000);
}

void close_snd_thread_and_switch_main(void) {
    ((SndCtl *)screen_state)->mode = 0;
    func_801575dc();
    func_801577bc(((SndCtl *)screen_state)->thread_handle);
    func_8015786c();
    func_801577ec(0xff000000);
}

void close_voice_thread(int a) {
    u16 *p = (u16 *)(0x801fc200 + a * 0x80);
    if (*p != 0) {
        *p = 0;
        func_801575dc();
        func_801577bc(*(u32 *)(0x801fc204 + a * 0x80));
        func_8015786c();
    }
}

void close_sound_thread_and_switch_to_main(u32 a) {
    ((SndCtl *)screen_state)->mode = 3;
    ((SndCtl *)screen_state)->field_0c = a;
    func_801575dc();
    func_801577bc(((SndCtl *)screen_state)->thread_handle);
    func_8015786c();
    func_801577ec(0xff000000);
}
