/* Addresses of symbols not defined by C units. Names are inferred. */

/* Scratchpad: per-side resource pointers, right side is left + 0xb0. */
sequences_left = 0x1f800064;
box_tables_left = 0x1f800070;
frames_left = 0x1f800088;
box_tables_left_alt = 0x1f8000a0;
sequences_right = 0x1f800114;
box_tables_right = 0x1f800120;
frames_right = 0x1f800138;
box_tables_right_alt = 0x1f800150;

/* Resident data. */
kind_table_162 = 0x80171af8;
side_table_65 = 0x80171b10;
side_table_66 = 0x80171b14;
kind_table_154 = 0x80171b18;
side_table_0d = 0x80171b30;
metrics_left = 0x80189354;
metrics_right = 0x801893bc;
scratch_a = 0x80189424;
scratch_b = 0x80189428;
scratch_c = 0x8018942c;

/* One routine of the character overlays, in the first-side block, the
   second-side block and the shared block. See ps1/docs/overlays.md. */
overlay_shared_kind = 0x801b5a34;
overlay_left_kind = 0x801b5ab8;
overlay_right_kind = 0x801cdab8;

/* Resident data used by object_reset. */
handlers_left = 0x80171864;
shared_handler_15 = 0x801718b8;
shared_handler_16 = 0x801718bc;
shared_handler_17 = 0x801718c0;
/* Same address as shared_handler_17, see object_reset.c. */
shared_handler_17_second = 0x801718c0;
handlers_right = 0x801718c4;
kind_table_f3 = 0x80171924;
game_state = 0x80190108;
player_left = 0x80197fc0;
player_right = 0x80198354;
data_801a27d8 = 0x801a27d8;
data_801a27dc = 0x801a27dc;

/* Resident functions not yet reconstructed. */

/* Added by src.u_setup_pre. */
scratch_fn_left = 0x1f800090;
scratch_fn_right = 0x1f800140;
seqs_60_left = 0x1f800060;
seqs_110_right = 0x1f800110;
seqs_68_left = 0x1f800068;
seqs_118_right = 0x1f800118;
seqs_6c_left = 0x1f80006c;
seqs_11c_right = 0x1f80011c;
seqs_9c_left = 0x1f80009c;
seqs_14c_right = 0x1f80014c;

/* Added by src.u_reset_pre. */
handlers_04 = 0x80171850;
data_801985b8 = 0x801985b8;
data_80198225 = 0x80198225;
data_801a6984 = 0x801a6984;

/* Added by src.u_misc_3f. */
data_80188f44 = 0x80188f44;
ring_left = 0x80188f54;
ring_right = 0x80189154;

/* Added by src.u_misc_4a. */
data_8017cf7c = 0x8017cf7c;
data_8017d698 = 0x8017d698;

/* Added by src.u_state_38. */
data_80176da0 = 0x80176da0;
game_state_second = 0x80190108;

/* Added by src.u_post_setup. */
box_margin = 0x801aa5e6;
kind_table_248 = 0x80171414;
kind_table_29b = 0x80171564;
ref_other = 0x80190460;
ref_first = 0x8019040c;
ref_second = 0x80190410;

/* Added by src.u_pre_swap_a. */
func_80131f7c = 0x80131f7c;

/* Added by src.u_post_swap. */
data_801a8067 = 0x80198067;
data_801a83fb = 0x801983fb;
data_80171b9c = 0x80171b9c;
data_80171bf0 = 0x80171bf0;
data_80171bf4 = 0x80171bf4;
data_80171bf8 = 0x80171bf8;
data_80171bfc = 0x80171bfc;
scratch_call_left = 0x1f8000e0;
scratch_call_right = 0x1f800190;

/* Added by src.u_metrics_post. */
shift_a = 0x8017acdc;
shift_b = 0x8017ace8;
quad_table = 0x8017acf4;

/* Added by src.u_pre_setup. */

/* Added by src.u_state_38_pre. */
data_80188ec4 = 0x80188ec4;
data_80188ecc = 0x80188ecc;
table_80176d70 = 0x80176d70;
table_80176d80 = 0x80176d80;

/* Added by src.u_pre_setup_a. */
scratch_fn_a_left = 0x1f800058;
scratch_fn_a_right = 0x1f800108;
scratch_fn_b_left = 0x1f8000dc;
scratch_fn_b_right = 0x1f80018c;
scratch_fn_c_left = 0x1f8000e8;
scratch_fn_c_right = 0x1f800198;
scratch_fn_d_left = 0x1f8000ec;
scratch_fn_d_right = 0x1f80019c;
motion_speed_table = 0x8016fa8c;
motion_table = 0x8016fb34;

/* Added by src.u_pre_swap_b. */
input_log = 0x801a698c;
data_801a6985 = 0x801a6985;
data_801a6987 = 0x801a6987;
data_801a6966 = 0x801a6966;
data_801a6972 = 0x801a6972;
data_80185ff8 = 0x80185ff8;
data_80185ffc = 0x80185ffc;
table_80185fd8 = 0x80185fd8;
table_8016e664 = 0x8016e664;
data_80171b32 = 0x80171b32;
data_80171b34 = 0x80171b34;
data_80171b36 = 0x80171b36;
table_80171b38 = 0x80171b38;
table_80171b40 = 0x80171b40;
table_80171b48 = 0x80171b48;

/* Added by src.u_s128464. */
data_80197f10 = 0x80197f10;
data_8019046c = 0x8019046c;
handlers_07 = 0x80171998;
data_80190464 = 0x80190464;

/* Added by src.u_s12a5ac. */
handler_table_19a8 = 0x801719a8;
handler_table_19b0 = 0x801719b0;
handler_table_19d0 = 0x801719d0;
handler_table_19d8 = 0x801719d8;
handler_table_19ec = 0x801719ec;
scratch_fn_e_left = 0x1f800094;
scratch_fn_e_right = 0x1f800144;

/* Added by src.u_s12be70. */
data_8017d818 = 0x8017d818;
table_801719f4 = 0x801719f4;
table_80171a00 = 0x80171a00;

/* Added by src.u_s12ce5c. */
table_80171a08 = 0x80171a08;
table_80171a20 = 0x80171a20;
table_80171a30 = 0x80171a30;
table_80171a3c = 0x80171a3c;
table_80171a60 = 0x80171a60;

/* Added by src.u_s12dee4. */
handlers_1a6c = 0x80171a6c;
handlers_1a74 = 0x80171a74;
handlers_1a80 = 0x80171a80;
handlers_1a90 = 0x80171a90;
scratch_call_8c = 0x1f80008c;
scratch_call_e4 = 0x1f8000e4;
scratch_call_13c = 0x1f80013c;
scratch_call_194 = 0x1f800194;
scratch_call_5c = 0x1f80005c;
scratch_call_10c = 0x1f80010c;
scratch_call_98 = 0x1f800098;
scratch_call_148 = 0x1f800148;

/* Added by src.u_s12edac. */
vec_table_80170274 = 0x80170274;
vec_table_80170514 = 0x80170514;
vec_table_80170cf4 = 0x80170cf4;
vec_table_80170f34 = 0x80170f34;
vec_table_80170a54 = 0x80170a54;
vec_table_801707b4 = 0x801707b4;
data_80186000 = 0x80186000;

/* Added by src.u_s1340e4. */
data_80188d4c = 0x80188d4c;
data_80188d50 = 0x80188d50;
data_80188d54 = 0x80188d54;
data_80188d58 = 0x80188d58;
data_80188d5c = 0x80188d5c;
data_80188d60 = 0x80188d60;
data_801721c8 = 0x801721c8;
data_80190474 = 0x80190474;
data_801723c0 = 0x801723c0;
data_80172410 = 0x80172410;
data_801aa544 = 0x801aa544;
data_801aa5d4 = 0x801aa5d4;
data_801aa556 = 0x801aa556;
data_801aa55a = 0x801aa55a;
data_801aa566 = 0x801aa566;
data_801aa56a = 0x801aa56a;
data_801aa56e = 0x801aa56e;
data_801aa572 = 0x801aa572;
data_801aa58c = 0x801aa58c;
data_801aa58e = 0x801aa58e;
data_801aa594 = 0x801aa594;
data_801aa598 = 0x801aa598;
data_801aa5f6 = 0x801aa5f6;
data_801aa5fa = 0x801aa5fa;
data_801aa5fe = 0x801aa5fe;
data_801aa602 = 0x801aa602;

/* Added by src.u_s136668. */
cam_obj = 0x801aa664;
table_80172460 = 0x80172460;
table_801724b0 = 0x801724b0;
table_80172500 = 0x80172500;
table_80172550 = 0x80172550;
table_801725a0 = 0x801725a0;
cam_src_a = 0x801aa5f4;
cam_src_b = 0x801aa5f8;
cam_f12 = 0x801aa676;
cam_f16 = 0x801aa67a;
cam_f22 = 0x801aa686;
cam_f26 = 0x801aa68a;
cam_f2a = 0x801aa68e;
cam_f2e = 0x801aa692;
cam_f48 = 0x801aa6ac;
cam_f4a = 0x801aa6ae;
cam_f50 = 0x801aa6b4;
cam_f54 = 0x801aa6b8;
data_80190568 = 0x80190568;
table_801a27e4 = 0x801a27e4;
table_801a2a84 = 0x801a2a84;
data_801aa4dc = 0x801aa4dc;

/* Added by src.u_s137004. */
data_801a27e4 = 0x801a27e4;
data_8018db10 = 0x8018db10;
data_8018daf4 = 0x8018daf4;
data_8018f598 = 0x8018f598;
handlers_267c = 0x8017267c;
data_801a27e4_rows = 0x801a27e4;

/* Added by src.u_s137cc0. */
func_80078e44 = 0x80078e44;
func_8008ef44 = 0x8008ef44;
handlers_2684 = 0x80172684;
handlers_268c = 0x8017268c;
handlers_2694 = 0x80172694;
table_269c = 0x8017269c;
table_6cf0 = 0x80176cf0;
table_6cf8 = 0x80176cf8;
table_6d08 = 0x80176d08;
table_6d10 = 0x80176d10;
seqs_a4_left = 0x1f8000a4;
seqs_154_right = 0x1f800154;
counter_a = 0x80188ec8;
counter_b = 0x80188ecc;
counter_c = 0x80188ec4;

/* Added by src.u_s1387c0. */
table_80188ed0 = 0x80188ed0;
data_80188f30 = 0x80188f30;
data_80188f34 = 0x80188f34;
handlers_7018 = 0x80177018;
handlers_702c = 0x8017702c;
handlers_70a4 = 0x801770a4;
table_8017727c = 0x8017727c;
handlers_72f4 = 0x801772f4;

/* Added by src.u_s13af1c. */
data_80188ed0 = 0x80188ed0;
ref_third = 0x80190478;
kind_pair_table = 0x801773d4;
kind_pair_handlers = 0x801773bc;

/* Added by src.u_s13d0fc. */
table_8017abb8 = 0x8017abb8;
table_8017a8cc = 0x8017a8cc;
data_8016d9a4 = 0x8016d9a4;
data_80188f4c = 0x80188f4c;
data_80188f50 = 0x80188f50;
table_8017abc4 = 0x8017abc4;
table_8017abd8 = 0x8017abd8;
table_8017abe4 = 0x8017abe4;
table_8017abec = 0x8017abec;

/* Added by src.u_s13e574. */
table_8017abf4 = 0x8017abf4;
table_8017aaf8 = 0x8017aaf8;
table_8017ac08 = 0x8017ac08;
table_8017ac14 = 0x8017ac14;
table_8017ac34 = 0x8017ac34;
table_8017ac44 = 0x8017ac44;
table_8017d618 = 0x8017d618;
table_8017d598 = 0x8017d598;

/* Added by src.u_s14147c. */
table_80171748 = 0x80171748;
table_8017179c = 0x8017179c;
table_8017ad54 = 0x8017ad54;
table_8017ad5c = 0x8017ad5c;

/* Added by src.u_s142adc. */
table_8017b550 = 0x8017b550;
table_8017b55c = 0x8017b55c;
table_8017b58c = 0x8017b58c;
table_8017b5c4 = 0x8017b5c4;
table_8017b5d4 = 0x8017b5d4;

/* Added by src.u_s144120. */
handlers_b5f4 = 0x8017b5f4;
table_8017b50c = 0x8017b50c;
handlers_b61c = 0x8017b61c;
handlers_b630 = 0x8017b630;
handlers_b640 = 0x8017b640;
table_8017c7f8 = 0x8017c7f8;
data_80172a48 = 0x80172a48;
data_80173c9c = 0x80173c9c;

/* Added by src.u_s1449fc. */
table_8017b654 = 0x8017b654;
table_8017b668 = 0x8017b668;
table_8017c978 = 0x8017c978;
table_8017c90c = 0x8017c90c;
table_8017c910 = 0x8017c910;
table_8017c960 = 0x8017c960;
table_8017c9c0 = 0x8017c9c0;
table_8017c988 = 0x8017c988;
table_8017c9d0 = 0x8017c9d0;

/* Added by src.u_s1460ec. */
pairs_left = 0x1f800080;
pairs_right = 0x1f800130;
fn_table_8017c9e0 = 0x8017c9e0;
fn_table_8017ca28 = 0x8017ca28;
fn_table_8017cb00 = 0x8017cb00;
data_800fb100 = 0x800fb100;
table_8017cae4 = 0x8017cae4;
data_801a37c4 = 0x801a37c4;

/* Added by src.u_s147a4c. */
seqs_c7f8 = 0x8017c7f8;
fns_cbd0 = 0x8017cbd0;
bytes_cbe0 = 0x8017cbe0;
fns_cbe8 = 0x8017cbe8;
fns_cbf4 = 0x8017cbf4;
fns_cc00 = 0x8017cc00;
fns_cc18 = 0x8017cc18;
offsets_cc28 = 0x8017cc28;

/* Added by src.u_s148588. */
table_8017cd28 = 0x8017cd28;
table_8017cd38 = 0x8017cd38;
table_8017cd40 = 0x8017cd40;
table_8017cd44 = 0x8017cd44;
seqs_8017c7f8 = 0x8017c7f8;
table_8017cebc = 0x8017cebc;
table_8017cef0 = 0x8017cef0;
table_8017cf00 = 0x8017cf00;
table_8017cf10 = 0x8017cf10;
table_8017cf20 = 0x8017cf20;

/* Added by src.u_s1495d8. */
data_8017cf30 = 0x8017cf30;
data_8017cf6c = 0x8017cf6c;
data_8017cf9c = 0x8017cf9c;
data_8017cfc4 = 0x8017cfc4;
data_80189460 = 0x80189460;
data_80189464 = 0x80189464;

/* Added by src.u_s14aa4c. */
table_8017d010 = 0x8017d010;

/* Added by src.u_s14b59c. */
table_8017d030 = 0x8017d030;

/* Added by src.u_s14c00c. */
table_8017d0ac = 0x8017d0ac;
table_8017d798 = 0x8017d798;
table_8017d450 = 0x8017d450;
table_8017d28c = 0x8017d28c;
table_8017d2a8 = 0x8017d2a8;
data_8018946c = 0x8018946c;

/* Added by src.u_s14d108. */
data_8017d2e0 = 0x8017d2e0;

/* Added by src.u_s14d8a4. */
scr_c4_left = 0x1f8000c4;
scr_174_right = 0x1f800174;
scr_d0_left = 0x1f8000d0;
scr_180_right = 0x1f800180;
scr_d4_left = 0x1f8000d4;
scr_184_right = 0x1f800184;
data_8017d320 = 0x8017d320;
data_8017d300 = 0x8017d300;
data_801ad398 = 0x801ad398;
data_80189468 = 0x80189468;
data_8017d340 = 0x8017d340;
data_8017d368 = 0x8017d368;
data_8017d3b4 = 0x8017d3b4;
data_8017d3d4 = 0x8017d3d4;

/* Added by src.u_s14ec80. */
data_8017d918 = 0x8017d918;
func_8015fb30 = 0x8015fb30;
data_801ac620 = 0x801ac620;
ctl_80190948 = 0x80190948;
data_80189478 = 0x80189478;

/* Added by src.u_s14f9f0. */
func_8015c9c0 = 0x8015c9c0;
func_8015cc44 = 0x8015cc44;
func_8015fe50 = 0x8015fe50;
data_8017eb1c = 0x8017eb1c;
data_8017ec7c = 0x8017ec7c;

/* Added by src.u_s150c6c. */
table_8017d9e4 = 0x8017d9e4;
table_8017ec9c = 0x8017ec9c;
data_801900fc = 0x801900fc;
data_8018f5a0 = 0x8018f5a0;
data_80190948 = 0x80190948;
data_80190126 = 0x80190126;
data_8018d204 = 0x8018d204;
table_8018d144 = 0x8018d144;
data_8018d13c = 0x8018d13c;
data_8018d208 = 0x8018d208;
table_8018023c = 0x8018023c;
table_80180274 = 0x80180274;

/* Added by src.u_s152444. */
table_80180284 = 0x80180284;
table_80180294 = 0x80180294;
table_801802a0 = 0x801802a0;
table_801802a8 = 0x801802a8;
table_801802c0 = 0x801802c0;
table_801802cc = 0x801802cc;
data_8018d210 = 0x8018d210;
table_801802f4 = 0x801802f4;
table_8018031c = 0x8018031c;
table_8018049c = 0x8018049c;
table_80180334 = 0x80180334;
table_801803c4 = 0x801803c4;

/* Added by src.u_s153854. */
table_80180454 = 0x80180454;
table_80180ecc = 0x80180ecc;
table_80181380 = 0x80181380;
table_80180384 = 0x80180384;
table_80180348 = 0x80180348;
data_80180ed8 = 0x80180ed8;
table_80180f9c = 0x80180f9c;
table_80180fa4 = 0x80180fa4;
data_8018fef8 = 0x8018fef8;
table_8018134c = 0x8018134c;
data_80181360 = 0x80181360;
table_80181530 = 0x80181530;
data_8018d268 = 0x8018d268;
table_80181650 = 0x80181650;

/* Added by src.u_s156398. */
table_8018179c = 0x8018179c;
data_80181930 = 0x80181930;
data_8018192c = 0x8018192c;
table_80181934 = 0x80181934;
table_801817e0 = 0x801817e0;
table_8018191c = 0x8018191c;
data_801817c0 = 0x801817c0;
data_801817d0 = 0x801817d0;

/* Added by src.u_s1577bc. */
gpu_ops = 0x8018198c;
gpu_printf = 0x80181990;
data_80181994 = 0x80181994;
gpu_dbg_level = 0x80181996;
data_80181997 = 0x80181997;
data_80181a44 = 0x80181a44;
data_80181a50 = 0x80181a50;
str_8016db54 = 0x8016db54;
str_8016dbb0 = 0x8016dbb0;
str_8016dbc4 = 0x8016dbc4;
str_8016dbfc = 0x8016dbfc;
str_8016dc08 = 0x8016dc08;
str_8016dc14 = 0x8016dc14;
str_8016dc20 = 0x8016dc20;
str_8016dc2c = 0x8016dc2c;
str_8016dc44 = 0x8016dc44;
str_8016dc5c = 0x8016dc5c;
str_8016dc70 = 0x8016dc70;
func_80158a84 = 0x80158a84;
data_80181a48 = 0x80181a48;
data_80181a4c = 0x80181a4c;

/* Added by src.u_s158894. */
dev_8018198c = 0x8018198c;
data_80181a00 = 0x80181a00;
gpu = 0x8018198c;
hw = 0x80181a64;
data_8018d2bc = 0x8018d2bc;
func_801599f0 = 0x801599f0;
data_80181aac = 0x80181aac;
data_80181ab0 = 0x80181ab0;
data_8018d3bc = 0x8018d3bc;
data_8018d3c0 = 0x8018d3c0;
data_8018d3c4 = 0x8018d3c4;
data_8018d3c8 = 0x8018d3c8;
data_8018d3cc = 0x8018d3cc;
func_80181990 = 0x80181990;
data_8016de3c = 0x8016de3c;

/* Added by src.u_s15bfe8. */
func_8015e268 = 0x8015e268;
func_8015d584 = 0x8015d584;
func_8015d7f8 = 0x8015d7f8;
func_8015eb28 = 0x8015eb28;
func_8015e880 = 0x8015e880;
func_8015dab8 = 0x8015dab8;
data_80181990 = 0x80181990;
data_80181b34 = 0x80181b34;
data_80181b38 = 0x80181b38;
data_80181b3c = 0x80181b3c;
data_80181b44 = 0x80181b44;
data_80181b54 = 0x80181b54;
data_80181df8 = 0x80181df8;
data_80181dfc = 0x80181dfc;
data_80181e00 = 0x80181e00;
data_80181e04 = 0x80181e04;
data_8016dea4 = 0x8016dea4;
data_8016dec0 = 0x8016dec0;
data_8016dedc = 0x8016dedc;
data_8016dee8 = 0x8016dee8;
data_8016def4 = 0x8016def4;
data_8016e0f4 = 0x8016e0f4;

/* Added by src.u_s15e200. */
table_80181e14 = 0x80181e14;
data_80182eec = 0x80182eec;
data_80182ef0 = 0x80182ef0;
data_80182ef4 = 0x80182ef4;
data_80182ef8 = 0x80182ef8;
data_80181e64 = 0x80181e64;
data_80181e66 = 0x80181e66;
data_80181e96 = 0x80181e96;
data_80181e98 = 0x80181e98;
data_80182f00 = 0x80182f00;
data_80182f04 = 0x80182f04;
func_8015f7ac = 0x8015f7ac;
func_8015f930 = 0x8015f930;
table_80182f2c = 0x80182f2c;
data_80182f4c = 0x80182f4c;
data_80182f50 = 0x80182f50;
func_8015fad8 = 0x8015fad8;
data_8016e1ec = 0x8016e1ec;
table_80182f08 = 0x80182f08;

/* Added by src.u_s160fbc. */
data_801a693e = 0x801a693e;
data_801a694c = 0x801a694c;
data_801a694d = 0x801a694d;
table_80182f78 = 0x80182f78;
data_801a694b = 0x801a694b;
data_801a6956 = 0x801a6956;
data_801a27e0 = 0x801a27e0;
func_8015ffc0 = 0x8015ffc0;
func_80160e14 = 0x80160e14;
data_801a6952 = 0x801a6952;
data_80197f04 = 0x80197f04;
table_801ac628 = 0x801ac628;
func_80162f14 = 0x80162f14;
func_80162388 = 0x80162388;
table_8018d3f4 = 0x8018d3f4;
table_8018d574 = 0x8018d574;
func_80161340 = 0x80161340;
func_80161884 = 0x80161884;
func_80163234 = 0x80163234;

/* Added by src.u_s118900. */
data_801ac310 = 0x801ac310;
data_80183ad0 = 0x80183ad0;
data_80183bb0 = 0x80183bb0;
data_80183da0 = 0x80183da0;
data_80183dc8 = 0x80183dc8;
data_80185c84 = 0x80185c84;
data_80185cc4 = 0x80185cc4;
data_80185ce4 = 0x80185ce4;
data_80183c90 = 0x80183c90;
data_801846fc = 0x801846fc;
data_80184700 = 0x80184700;
data_801a696a = 0x801a696a;
data_801a6976 = 0x801a6976;

/* Added by src.u_s11b678. */
data_801ac888 = 0x801ac888;
table_8016e6b8 = 0x8016e6b8;
data_801a27d4 = 0x801a27d4;
data_801a27cc = 0x801a27cc;
data_801a89f4 = 0x801a89f4;
table_80197f20 = 0x80197f20;
data_801904d8 = 0x801904d8;
data_801a6960 = 0x801a6960;
data_801a89b0 = 0x801a89b0;
data_801a4fec = 0x801a4fec;
data_801a68f0 = 0x801a68f0;
data_801ac618 = 0x801ac618;
data_801abefc = 0x801abefc;
data_801a4fe4 = 0x801a4fe4;
data_801ad348 = 0x801ad348;
data_801a6980 = 0x801a6980;

/* Added by src.u_s11f674. */
handlers_by_side = 0x8016e6d8;
data_801aa624 = 0x801aa624;
data_801ad354 = 0x801ad354;
data_8018db00 = 0x8018db00;
data_8018db04 = 0x8018db04;
data_80190100 = 0x80190100;
data_80190104 = 0x80190104;
data_8018f59c = 0x8018f59c;
data_8018f5e0 = 0x8018f5e0;
table_80185cf4 = 0x80185cf4;
table_80185e50 = 0x80185e50;
data_8019036c = 0x8019036c;
data_801903ac = 0x801903ac;

/* Added by src.u_s11ffdc. */
func_801650bc = 0x801650bc;
func_8016529c = 0x8016529c;
func_80164bbc = 0x80164bbc;
stack_801ad354 = 0x801ad354;
stack_8018db00 = 0x8018db00;
stack_8018db04 = 0x8018db04;
count_8018f59c = 0x8018f59c;
count_80190100 = 0x80190100;
count_80190104 = 0x80190104;
data_801953d0 = 0x801953d0;
data_80190a44 = 0x80190a44;
data_80197ed0 = 0x80197ed0;
table_8016e6e0 = 0x8016e6e0;
table_8016e6f0 = 0x8016e6f0;
table_8016e720 = 0x8016e720;
ptr_80197ee8 = 0x80197ee8;

/* Added by src.u_s120f40. */
func_80010058 = 0x80010058;
func_80010b2c = 0x80010b2c;
func_80013834 = 0x80013834;
func_8001365c = 0x8001365c;
func_800136b0 = 0x800136b0;
func_801e1d0c = 0x801e1d0c;
func_801e1b64 = 0x801e1b64;
func_801e1d6c = 0x801e1d6c;
table_8016e738 = 0x8016e738;
table_8016e760 = 0x8016e760;
table_8016e774 = 0x8016e774;
data_801ac61c = 0x801ac61c;
data_801a89f0 = 0x801a89f0;
data_801ae02c = 0x801ae02c;
data_80190a40 = 0x80190a40;

/* Added by src.u_s1221e8. */
table_8016e7bc = 0x8016e7bc;
table_8016e7d0 = 0x8016e7d0;
table_8016e7e0 = 0x8016e7e0;
table_8016e7e8 = 0x8016e7e8;
table_8016e6f8 = 0x8016e6f8;

/* Added by src.u_s123968. */
data_80197f1c = 0x80197f1c;
table_8016f4f0 = 0x8016f4f0;
table_8016e81c = 0x8016e81c;
table_8016e804 = 0x8016e804;
table_8016e99c = 0x8016e99c;
table_801aa4d8 = 0x801aa4d8;
table_801ac318 = 0x801ac318;
data_801a6986 = 0x801a6986;
data_801a6988 = 0x801a6988;
data_80198072 = 0x80198072;
data_80198406 = 0x80198406;

/* Added by src.u_s1257d0. */
data_8016ea70 = 0x8016ea70;
data_8016f508 = 0x8016f508;
data_8016f538 = 0x8016f538;
data_8019032e = 0x8019032e;
data_80190111 = 0x80190111;
data_8016f5a4 = 0x8016f5a4;
data_8019032d = 0x8019032d;
data_801fc248 = 0x801fc248;

/* Added by src.u_s126adc. */
table_8016f5bc = 0x8016f5bc;
table_8016f5f4 = 0x8016f5f4;
table_8016f5fc = 0x8016f5fc;

/* Added by src.u_s15c6b0. */
data_801904d4 = 0x801904d4;
data_8018dae8 = 0x8018dae8;
data_8018dadc = 0x8018dadc;
data_8018dae4 = 0x8018dae4;
data_8018daf0 = 0x8018daf0;
data_8018312c = 0x8018312c;
data_80190944 = 0x80190944;
func_80164f28 = 0x80164f28;
func_8016541c = 0x8016541c;
func_80169398 = 0x80169398;
func_801658ec = 0x801658ec;
func_8016b364 = 0x8016b364;
func_8016a878 = 0x8016a878;
data_801906e4 = 0x8018f6e4;
data_801906e8 = 0x8018f6e8;

/* Added by src.u_s1682d0. */
data_8018f6e4 = 0x8018f6e4;
data_80190470 = 0x80190470;
func_801691d8 = 0x801691d8;
func_801634c4 = 0x801634c4;
func_8016a56c = 0x8016a56c;
data_801a8994 = 0x801a8994;
data_801adfe0 = 0x801adfe0;
table_801adfe8 = 0x801adfe8;
table_801ae0d8 = 0x801ae0d8;
data_80183138 = 0x80183138;
data_801831e8 = 0x801831e8;

/* Added by src.u_s16b2c8. */
data_80183134 = 0x80183134;
data_80183150 = 0x80183150;
data_80183158 = 0x80183158;
data_8018315c = 0x8018315c;
data_80183160 = 0x80183160;
data_80183164 = 0x80183164;
data_80183168 = 0x80183168;
data_8018316c = 0x8018316c;
data_80183170 = 0x80183170;
data_80183190 = 0x80183190;
data_80183194 = 0x80183194;
data_801831ec = 0x801831ec;
data_801835fc = 0x801835fc;
data_80183600 = 0x80183600;
data_80183604 = 0x80183604;
data_80183608 = 0x80183608;
func_8016c5a8 = 0x8016c5a8;
func_8016bec8 = 0x8016bec8;
func_8016d4cc = 0x8016d4cc;
func_8016c20c = 0x8016c20c;

/* Added by src.u_m12028c. */
data_80185fae = 0x80185fae;
data_8016e800 = 0x8016e800;
data_801900f4 = 0x801900f4;
data_801900f8 = 0x801900f8;

/* Added by src.u_m126130. */
table_8016f550 = 0x8016f550;
data_80181534 = 0x80181534;
table_8016f658 = 0x8016f658;

/* Added by src.u_m12af5c. */
motion_recs = 0x8016fb54;

/* Added by src.u_m12e168. */
table_80170244 = 0x80170244;
data_801ac6a8 = 0x801ac6a8;
data_801ac870 = 0x801ac870;
data_801ac878 = 0x801ac878;
data_80171c5c = 0x80171c5c;
data_80171c5d = 0x80171c5d;
data_80171c5e = 0x80171c5e;
data_80171c5f = 0x80171c5f;
table_80172144 = 0x80172144;

/* Added by src.u_m133df4. */
data_80188d64 = 0x80188d64;
data_80188d65 = 0x80188d65;
data_80188d68 = 0x80188d68;
data_80188d69 = 0x80188d69;
data_801aa5ce = 0x801aa5ce;
data_801aa65e = 0x801aa65e;
data_801aa6ee = 0x801aa6ee;
data_80190562 = 0x80190562;
data_801725f0 = 0x801725f0;
data_8017262c = 0x8017262c;
data_801aa548 = 0x801aa548;
data_801aa5d8 = 0x801aa5d8;
data_801aa668 = 0x801aa668;
data_801904dc = 0x801904dc;

/* Added by src.u_m138290. */
units_2c20 = 0x801ad3e0;
handlers_6bf0 = 0x80176bf0;
handlers_6c70 = 0x80176c70;
table_6d30 = 0x80176d30;

/* Added by src.u_m13c970. */

/* Added by src.u_m13eb28. */
table_8017ab5c = 0x8017ab5c;
table_8017ab6c = 0x8017ab6c;
table_8016d9ac = 0x8016d9ac;
data_8016d9ac = 0x8016d9ac;

/* Added by src.u_m141f28. */
table_8017ac54 = 0x8017ac54;
table_8017ac7c = 0x8017ac7c;
seq_table_8017b50c = 0x8017b50c;
table_8017ca64 = 0x8017ca64;
table_8017ca38 = 0x8017ca38;
bytes_8017cb18 = 0x8017cb18;
table_8017cbac = 0x8017cbac;
data_80189458 = 0x80189458;
data_8018945c = 0x8018945c;

/* Added by src.u_m1474b8. */
seqs_8017cbac = 0x8017cbac;
data_8019045c = 0x8019045c;
table_8017cd54 = 0x8017cd54;
table_8017cdb4 = 0x8017cdb4;

/* Added by src.u_m14a4f4. */
scr_c8_left = 0x1f8000c8;
scr_178_right = 0x1f800178;
scr_cc_left = 0x1f8000cc;
scr_17c_right = 0x1f80017c;

/* Added by src.u_m14dd14. */
data_8017e9d4 = 0x8017e9d4;
func_8014f918 = 0x8014f918;

/* Added by src.u_m14ff20. */
func_80166144 = 0x80166144;
data_8017eb44 = 0x8017eb44;
func_8015cb08 = 0x8015cb08;
data_8017eb34 = 0x8017eb34;
table_8017f2d8 = 0x8017f2d8;
data_8017f2ec = 0x8017f2ec;
table_8017f2fc = 0x8017f2fc;
table_8017f3a0 = 0x8017f3a0;
data_8018a4bc = 0x8018a4bc;
data_8018ccbc = 0x8018ccbc;
data_8016da38 = 0x8016da38;
data_8018024c = 0x8018024c;

/* Added by src.u_m152b40. */
table_80180338 = 0x80180338;
table_801802dc = 0x801802dc;
data_801ac624 = 0x801ac624;
table_80180304 = 0x80180304;
table_8018030c = 0x8018030c;

/* Added by src.u_m15393c. */
table_8018033c = 0x8018033c;
table_80180344 = 0x80180344;
table_80180f1c = 0x80180f1c;
table_80180f5c = 0x80180f5c;
table_80181510 = 0x80181510;

/* Added by src.u_m11907c. */
data_801a89a8 = 0x801a89a8;
data_801a6968 = 0x801a6968;
data_801a696c = 0x801a696c;
data_801a6974 = 0x801a6974;
data_801a6978 = 0x801a6978;
data_8019047c = 0x8019047c;
data_8016e6a0 = 0x8016e6a0;
data_801901be = 0x801901be;
data_801a27d0 = 0x801a27d0;
data_801a4fe8 = 0x801a4fe8;
data_801a695c = 0x801a695c;
data_801a697c = 0x801a697c;
data_801aa540 = 0x801aa540;
data_801ad3dc = 0x801ad3dc;
data_801ae118 = 0x801ae118;
data_801987cc = 0x801987cc;
data_8018db18 = 0x8018db18;
data_8019056c = 0x8019056c;

/* Added by src.u_m1566e8. */
table_8016e5c4 = 0x8016e5c4;
table_8016e614 = 0x8016e614;
data_80181998 = 0x80181998;
data_8018199a = 0x8018199a;
table_801818d0 = 0x801818d0;
str_8016dbd8 = 0x8016dbd8;
str_8016dbe4 = 0x8016dbe4;
str_8016dbf8 = 0x8016dbf8;
data_80181a78 = 0x80181a78;
data_80181a7c = 0x80181a7c;
data_80181a80 = 0x80181a80;
data_80181a84 = 0x80181a84;
func_8015a2b8 = 0x8015a2b8;
data_80181a64 = 0x80181a64;
data_80181a68 = 0x80181a68;
str_8016dcec = 0x8016dcec;
str_8016dcf8 = 0x8016dcf8;
str_8016dd04 = 0x8016dd04;

/* Added by src.u_m15bd0c. */
str_8016de24 = 0x8016de24;
data_80181e08 = 0x80181e08;
data_80181e0c = 0x80181e0c;
data_80181e11 = 0x80181e11;
data_80181e12 = 0x80181e12;
data_80181e13 = 0x80181e13;
str_8018d3d8 = 0x8018d3d8;
str_8018d3d0 = 0x8018d3d0;
func_8015d048 = 0x8015d048;
func_8015e4bc = 0x8015e4bc;
data_80181e50 = 0x80181e50;
data_80181e54 = 0x80181e54;
data_80181e58 = 0x80181e58;
data_80181e5c = 0x80181e5c;
data_80181e60 = 0x80181e60;
data_80181e14 = 0x80181e14;
data_80181e18 = 0x80181e18;
data_80181e1c = 0x80181e1c;
data_80181e24 = 0x80181e24;
data_80181e28 = 0x80181e28;
data_80181e34 = 0x80181e34;
data_80181e38 = 0x80181e38;

/* Added by src.u_m15f158. */
func_8015f234 = 0x8015f234;
data_80181ea0 = 0x80181ea0;
data_80197f14 = 0x80197f14;
data_801a6943 = 0x801a6943;
data_801a6948 = 0x801a6948;
func_80162a54 = 0x80162a54;
data_80190a3c = 0x80190a3c;
table_8018d58c = 0x8018d58c;
data_8018daf8 = 0x8018daf8;
data_8018dafc = 0x8018dafc;
data_801ae11c = 0x801ae11c;
data_801ae120 = 0x801ae120;
data_80197efc = 0x80197efc;
data_80197f0c = 0x80197f0c;
data_801a693d = 0x801a693d;
data_801a6942 = 0x801a6942;
table_801903cc = 0x801903cc;
table_80190418 = 0x80190418;
table_80190480 = 0x80190480;
func_80166388 = 0x80166388;

/* Added by src.u_m166638. */
data_801a6934 = 0x801a6934;
str_8016e500 = 0x8016e500;
func_80168a24 = 0x80168a24;
table_801ad39c = 0x801ad39c;
data_80183198 = 0x80183198;
data_8018319c = 0x8018319c;
data_801831b4 = 0x801831b4;
data_801831a4 = 0x801831a4;
data_801831a8 = 0x801831a8;
data_801831aa = 0x801831aa;
data_801831ac = 0x801831ac;
data_801831b0 = 0x801831b0;
data_801831f4 = 0x801831f4;
data_801831f8 = 0x801831f8;
table_8018360c = 0x8018360c;
func_8016b788 = 0x8016b788;

/* Added by src.u_l121058. */
data_8016e68b = 0x8016e68b;
func_801b0000 = 0x801b0000;
func_80010000 = 0x80010000;
func_801b2194 = 0x801b2194;
func_801b4a88 = 0x801b4a88;
data_801a6938 = 0x801a6938;
data_80197fc4 = 0x80197fc4;
data_80198358 = 0x80198358;
func_801e0020 = 0x801e0020;
table_801aa4e8 = 0x801aa4e8;
data_801aa53c = 0x801aa53c;

/* Added by src.u_l122c5c. */
data_801abf08 = 0x801abf08;

/* Added by src.u_l124bbc. */
table_8016e820 = 0x8016e820;
data_80185fb4 = 0x80185fb4;
data_80185fb8 = 0x80185fb8;
data_801ae028 = 0x801ae028;
table_8016e828 = 0x8016e828;
data_80190949 = 0x80190949;
data_8016e858 = 0x8016e858;
data_8016e863 = 0x8016e863;

/* Added by src.u_l128814. */
table_801717f0 = 0x801717f0;
table_8016fa38 = 0x8016fa38;
handler_table_1960 = 0x80171960;

/* Added by src.u_l12a3f4. */
scratch_call_54 = 0x1f800054;
scratch_call_d8 = 0x1f8000d8;
scratch_call_104 = 0x1f800104;
scratch_call_188 = 0x1f800188;

/* Added by src.u_l12bd00. */
pairs_80170074 = 0x80170074;
bytes_801700b0 = 0x801700b0;
data_8017d898 = 0x8017d898;
table_80171194 = 0x80171194;
table_80171174 = 0x80171174;

/* Added by src.u_l12e38c. */
data_80171a9c = 0x80171a9c;
data_80171aa4 = 0x80171aa4;
func_80130af0 = 0x80130af0;
strips = 0x8018ff34;
strips2 = 0x80188ddc;

/* Added by src.u_l134624. */
data_80198098 = 0x80198098;
data_8019842c = 0x8019842c;
data_801987c8 = 0x801987c8;
data_80188d6c = 0x80188d6c;
data_80188ebc = 0x80188ebc;
data_80188ec0 = 0x80188ec0;
data_8018db14 = 0x8018db14;

/* Added by src.u_l138164. */
table_801726bc = 0x801726bc;
table_80172980 = 0x80172980;
table_801729e4 = 0x801729e4;
data_80188f0c = 0x80188f0c;
data_80188f10 = 0x80188f10;
data_80188f14 = 0x80188f14;
data_80188f18 = 0x80188f18;

/* Added by src.u_l13e028. */

/* Added by src.u_l1409d8. */
data_80189430 = 0x80189430;
table_801777d4 = 0x801777d4;
table_80177bd4 = 0x80177bd4;
table_80179834 = 0x80179834;
table_8017984c = 0x8017984c;
ptr_8019040c = 0x8019040c;
table_8017ac1c = 0x8017ac1c;

/* Added by src.round15u. */
data_80189434 = 0x80189434;
table_8017c9f0 = 0x8017c9f0;
table_8017ca10 = 0x8017ca10;
table_8017ca20 = 0x8017ca20;
table_8017cac4 = 0x8017cac4;
data_80189454 = 0x80189454;
data_8017ceb4 = 0x8017ceb4;
data_8017ceb8 = 0x8017ceb8;
table_8017d4d0 = 0x8017d4d0;
table_8017d534 = 0x8017d534;
data_8017d4d0 = 0x8017d4d0;
data_8017d524 = 0x8017d524;
data_8017d528 = 0x8017d528;
data_8017d52c = 0x8017d52c;
data_8017d530 = 0x8017d530;
data_8017d534 = 0x8017d534;
data_8017ec60 = 0x8017ec60;
data_8018947c = 0x8018947c;
table_80180340 = 0x80180340;
data_8018d269 = 0x8018d269;
data_8018d26a = 0x8018d26a;
data_8018d26c = 0x8018d26c;
data_8018d270 = 0x8018d270;
data_8018d274 = 0x8018d274;
table_801815d4 = 0x801815d4;
table_80181638 = 0x80181638;
table_8016e604 = 0x8016e604;
table_8016e654 = 0x8016e654;
data_8016e685 = 0x8016e685;
data_8016e687 = 0x8016e687;
data_8016e688 = 0x8016e688;
table_8016e730 = 0x8016e730;
table_8016e7ac = 0x8016e7ac;
func_80164140 = 0x80164140;
data_8016e8fc = 0x8016e8fc;
data_8016e92c = 0x8016e92c;
data_8016e95c = 0x8016e95c;
data_8016e98c = 0x8016e98c;
data_80185fac = 0x80185fac;
data_80185fbc = 0x80185fbc;
data_80185fc0 = 0x80185fc0;
data_80185fc4 = 0x80185fc4;
data_8016e801 = 0x8016e801;
data_8016e802 = 0x8016e802;
data_8016e803 = 0x8016e803;
data_801a6989 = 0x801a6989;
data_8019011f = 0x8019011f;
table_8016e810 = 0x8016e810;
table_8016e818 = 0x8016e818;
data_80198226 = 0x80198226;
data_801985ba = 0x801985ba;
table_8017d718 = 0x8017d718;
boxes_74_left = 0x1f800074;
boxes_124_right = 0x1f800124;
table_80188d08 = 0x80188d08;
data_80188d28 = 0x80188d28;
table_80172318 = 0x80172318;
table_80172328 = 0x80172328;
data_80172380 = 0x80172380;
data_80190137 = 0x80190137;
data_80190138 = 0x80190138;
data_80198069 = 0x80198069;
data_801980b0 = 0x801980b0;
data_801983fd = 0x801983fd;
data_80198444 = 0x80198444;
table_80176fc0 = 0x80176fc0;
table_80176fe0 = 0x80176fe0;
table_80177000 = 0x80177000;

/* Added by src.u_y1189c4. */
data_801abef8 = 0x801abef8;
data_801ac314 = 0x801ac314;
data_801a89ac = 0x801a89ac;
scratch_word_00 = 0x1f800000;
scratch_word_04 = 0x1f800004;
scratch_word_10 = 0x1f800010;
func_801578fc = 0x801578fc;
data_801fc0a2 = 0x801fc0a2;
data_801fc0a4 = 0x801fc0a4;
data_801fc0a5 = 0x801fc0a5;
data_801fc0a6 = 0x801fc0a6;
data_801fc0a7 = 0x801fc0a7;
data_801fc18a = 0x801fc18a;
data_801fc18c = 0x801fc18c;
data_801fc18d = 0x801fc18d;
data_801fc18e = 0x801fc18e;
data_801fc18f = 0x801fc18f;
data_8018e598 = 0x8018e598;
data_8018f5e4 = 0x8018f5e4;
data_801adfe4 = 0x801adfe4;
func_80158470 = 0x80158470;
data_8019093c = 0x8019093c;
data_80190940 = 0x80190940;
data_801a6964 = 0x801a6964;
data_801a6970 = 0x801a6970;
data_801abf0c = 0x801abf0c;
data_801903b0 = 0x801903b0;
scratch_word_08 = 0x1f800008;
scratch_word_0c = 0x1f80000c;
scratch_word_14 = 0x1f800014;
scratch_word_18 = 0x1f800018;

/* Added by src.u_y1199ec. */
data_80183ffc = 0x80183ffc;
data_801841bc = 0x801841bc;
data_8018437c = 0x8018437c;
data_80183910 = 0x80183910;
data_801839f0 = 0x801839f0;
data_80184704 = 0x80184704;

/* Added by src.u_y11bc84. */
data_80183d74 = 0x80183d74;
data_80183dec = 0x80183dec;
data_80183d9c = 0x80183d9c;
data_80183dc4 = 0x80183dc4;

/* Added by src.u_y11db78. */
data_801abf10 = 0x801abf10;
data_801ac30c = 0x801ac30c;
data_801ad358 = 0x801ad358;
table_801900f8 = 0x801900f8;
data_801aa5ea = 0x801aa5ea;
data_801a87c8 = 0x801a87c8;
data_80185c04 = 0x80185c04;

/* Added by src.u_y11fcc0. */
data_80190414 = 0x80190414;
data_80190458 = 0x80190458;

/* Added by src.u_y146ba0. */
data_8017ceec = 0x8017ceec;
data_8017cecc = 0x8017cecc;

/* Added by src.u_y14e890. */
data_8018d264 = 0x8018d264;
table_801811ed = 0x801811ed;
data_801812f4 = 0x801812f4;
data_80189470 = 0x80189470;
table_8017d96c = 0x8017d96c;
table_8017d9bc = 0x8017d9bc;

/* Added by src.u_z11b708. */
func_801e9080 = 0x801e9080;
func_801e9f90 = 0x801e9f90;
func_801e99c8 = 0x801e99c8;
func_801e9d04 = 0x801e9d04;
func_801e99b4 = 0x801e99b4;
func_801e8dc8 = 0x801e8dc8;
func_801e9798 = 0x801e9798;
func_801e9970 = 0x801e9970;
func_801e9b54 = 0x801e9b54;
func_801e9840 = 0x801e9840;
func_801e8df0 = 0x801e8df0;
func_801e96fc = 0x801e96fc;
func_801ea640 = 0x801ea640;
func_801e9f20 = 0x801e9f20;
func_801e9d14 = 0x801e9d14;
func_801e9bb0 = 0x801e9bb0;
func_801e9ef4 = 0x801e9ef4;
func_801e8fec = 0x801e8fec;
func_801e9114 = 0x801e9114;

/* Added by src.u_z11bf70. */
data_80185504 = 0x80185504;

/* Added by src.u_z11c724. */
data_80184380 = 0x80184380;
data_8019019a = 0x8019019a;

/* Added by src.u_z13245c. */
data_80186004 = 0x80186004;

/* Added by src.u_z133848. */
data_80188d44 = 0x80188d44;
data_80188d48 = 0x80188d48;

/* Added by src.u_z134234. */
data_8019016c = 0x8019016c;
data_80188d04 = 0x80188d04;
data_8019808e = 0x8019808e;
data_80198422 = 0x80198422;
data_801ac86c = 0x801ac86c;
data_801ac874 = 0x801ac874;
data_801ac87c = 0x801ac87c;
data_801ac880 = 0x801ac880;
data_801ac884 = 0x801ac884;
data_80190014 = 0x80190014;
data_80188e4c = 0x80188e4c;

/* Added by src.u_z13b0c4. */
data_80188f20 = 0x80188f20;
table_8017736c = 0x8017736c;
table_80177390 = 0x80177390;
table_80197ef8 = 0x80197ef8;

/* Added by src.u_z1452ec. */
data_801a4ff0 = 0x801a4ff0;

/* Added by src.u_z151a04. */
data_8018d140 = 0x8018d140;
data_8018d20c = 0x8018d20c;
func_801680f4 = 0x801680f4;
func_80169344 = 0x80169344;
func_8016936c = 0x8016936c;
func_8016a6e8 = 0x8016a6e8;
func_8016a70c = 0x8016a70c;
SsUtReverbOn = 0x801681dc;
SsUtReverbOff = 0x801681fc;
SsUtSetReverbDepth = 0x801682d0;
SsUtSetReverbFeedback = 0x80168368;
SsUtSetReverbDelay = 0x801683a8;
_SsSndNextSep = 0x801683e8;
SsUtGetProgAtr = 0x80168908;
SpuVmDamperOff = 0x80168a14;
_SsSndReplay = 0x80169abc;
_SsSndPlay = 0x80169b24;
_SsSndPause = 0x8016a0cc;
SsVabTransBody = 0x8016a49c;
SpuFree = 0x8016cc18;
_SpuDataCallback = 0x8016ced0;
SpuIsTransferCompleted = 0x8016cf70;
SpuSetTransferMode = 0x8016d06c;
SpuSetTransferStartAddr = 0x8016d0a0;
SpuWrite = 0x8016d0dc;
_spu_setInTransfer = 0x8016d13c;
SpuWritePartly = 0x8016d440;
GPU_printf = 0x80181990;
D_800334FC = 0x80183130;
_spu_tsa = 0x80183134;
_spu_RXX = 0x80183138;
D_80033508 = 0x8018313c;
D_8003350C = 0x80183140;
D_80033510 = 0x80183144;
D_80033518 = 0x8018314c;
_spu_transMode = 0x80183150;
_spu_mem_mode = 0x80183158;
_spu_mem_mode_plus = 0x8018315c;
_spu_mem_mode_unit = 0x80183160;
_spu_mem_mode_unitM = 0x80183164;
_spu_inTransfer = 0x80183168;
_spu_transferCallback = 0x8018316c;
_spu_IRQCallback = 0x80183170;
D_80033550 = 0x80183184;
D_80033554 = 0x80183188;
D_80033558 = 0x8018318c;
D_80033098 = 0x80183190;
_spu_trans_mode = 0x80183194;
_spu_rev_flag = 0x80183198;
_spu_rev_reserve_wa = 0x8018319c;
_spu_rev_offsetaddr = 0x801831b4;
_spu_EVdma = 0x801831ec;
D_800330F8 = 0x801831fc;
_spu_isCalled = 0x801835fc;
D_8003355C = 0x80183600;
D_80033560 = 0x80183604;
_spu_memList = 0x80183608;
_spu_rev_startaddr = 0x8018360c;
_svm_rattr = 0x8018f6e4;
_SsMarkCallback = 0x8018f6f8;
_svm_vab_pg = 0x801903cc;
_svm_vab_vh = 0x80190418;
_svm_vab_tn = 0x80190480;
_snd_ev_flag = 0x80190a3c;
kMaxPrograms = 0x80197efc;
_svm_pg = 0x80197f04;
_svm_vh = 0x80197f0c;
_svm_tn = 0x80197f14;
spuVmMaxVoice = 0x801a27e0;
_snd_openflag = 0x801a6934;
_svm_cur = 0x801a693c;
_svm_vab_used = 0x801a8994;
VBLANK_MINUS = 0x801abf00;
_ss_score = 0x801ac628;
_snd_seq_s_max = 0x801ad34c;
_snd_seq_t_max = 0x801ad350;
_svm_vab_total = 0x801ad39c;
_svm_vab_count = 0x801adfe0;
_svm_vab_start = 0x801adfe8;
_svm_brr_start_addr = 0x801ae0d8;
func_80164db4 = 0x80164db4;
func_80168150 = 0x80168150;
func_8016821c = 0x8016821c;
func_8016b2c8 = 0x8016b2c8;
func_8016ccb4 = 0x8016ccb4;
func_8016cef4 = 0x8016cef4;
func_8016d7cc = 0x8016d7cc;
func_8016cc08 = 0x8016cc08;
func_8015a540 = 0x8015a540;
func_8016d900 = 0x8016d900;
func_8015f13c = 0x8015f13c;
func_80159900 = 0x80159900;
func_80164970 = 0x80164970;
func_8015fd14 = 0x8015fd14;
func_801578ac = 0x801578ac;
func_8015f6d4 = 0x8015f6d4;
func_8015fc78 = 0x8015fc78;
func_8015c7f4 = 0x8015c7f4;
func_8015f714 = 0x8015f714;
func_8015bcfc = 0x8015bcfc;
func_8015f570 = 0x8015f570;
func_8016c940 = 0x8016c940;
func_8016d8e0 = 0x8016d8e0;
func_8016d8f0 = 0x8016d8f0;
func_801577fc = 0x801577fc;
func_8015783c = 0x8015783c;
func_8015a530 = 0x8015a530;
func_80164a78 = 0x80164a78;
func_80159950 = 0x80159950;
func_8015782c = 0x8015782c;
func_8015761c = 0x8015761c;
func_8015a550 = 0x8015a550;
func_801590bc = 0x801590bc;
func_801575cc = 0x801575cc;
func_801577cc = 0x801577cc;
func_8015fa5c = 0x8015fa5c;
func_8015786c = 0x8015786c;
func_801575dc = 0x801575dc;
func_8015f724 = 0x8015f724;
func_8015f158 = 0x8015f158;
func_8015785c = 0x8015785c;
func_8015f6f4 = 0x8015f6f4;
func_8015f620 = 0x8015f620;
func_8015f6e4 = 0x8015f6e4;
func_80161078 = 0x80161078;
func_8015f704 = 0x8015f704;
func_80157e08 = 0x80157e08;
func_8015780c = 0x8015780c;
func_8016482c = 0x8016482c;
func_8015f75c = 0x8015f75c;
func_8015c5f0 = 0x8015c5f0;
func_8015bf34 = 0x8015bf34;
func_8016452c = 0x8016452c;
func_80158150 = 0x80158150;
func_8015bf70 = 0x8015bf70;
func_8015808c = 0x8015808c;
func_801576e8 = 0x801576e8;
func_8015efc0 = 0x8015efc0;
func_8015eff0 = 0x8015eff0;
func_8015f020 = 0x8015f020;
func_8015f050 = 0x8015f050;
func_8015f084 = 0x8015f084;
func_8015f0b4 = 0x8015f0b4;
func_8015f0e4 = 0x8015f0e4;
func_8015f114 = 0x8015f114;
func_8015f6a8 = 0x8015f6a8;
func_8015f9d8 = 0x8015f9d8;
func_8015fa04 = 0x8015fa04;
func_8015fb04 = 0x8015fb04;
func_8015bebc = 0x8015bebc;
func_8015bfac = 0x8015bfac;
func_8015bfe8 = 0x8015bfe8;
func_80166274 = 0x80166274;
func_80167290 = 0x80167290;
func_8016730c = 0x8016730c;
func_80166f48 = 0x80166f48;
func_80167c34 = 0x80167c34;
func_80168038 = 0x80168038;
func_80160fbc = 0x80160fbc;
func_801621bc = 0x801621bc;
func_80162308 = 0x80162308;
func_801625a0 = 0x801625a0;
func_80163604 = 0x80163604;
func_80163678 = 0x80163678;
func_801636cc = 0x801636cc;
func_80163740 = 0x80163740;
func_80164644 = 0x80164644;
func_80164b14 = 0x80164b14;
func_80164b68 = 0x80164b68;
SpuVmAlloc = 0x8015ffc0;
SpuSetNoiseVoice = 0x8016b340;
D_80032F10 = 0x80182f74;
D_80032F14 = 0x80182f78;
_svm_sreg_buf = 0x8018d3f4;
_svm_sreg_dirty = 0x8018d574;
_svm_voice = 0x8018d58c;
D_8003BD08 = 0x8018da0c;
_svm_envx_hist = 0x8018da10;
_svm_okon1 = 0x8018daf8;
_svm_okon2 = 0x8018dafc;
_svm_orev1 = 0x8018db08;
_svm_orev2 = 0x8018db0c;
_svm_damper = 0x80190470;
_svm_stereo_mono = 0x80190944;
_svm_auto_kof_mode = 0x801aa4d4;
_svm_okof1 = 0x801ae11c;
_svm_okof2 = 0x801ae120;
func_8015c588 = 0x8015c588;
func_8015c6b0 = 0x8015c6b0;
func_8015c49c = 0x8015c49c;
func_8016507c = 0x8016507c;
func_8016509c = 0x8016509c;
func_80165850 = 0x80165850;
func_80165884 = 0x80165884;
func_801658b8 = 0x801658b8;
func_80168d4c = 0x80168d4c;
func_8015c824 = 0x8015c824;
func_8015c890 = 0x8015c890;
func_8015cd98 = 0x8015cd98;
func_8015cdbc = 0x8015cdbc;
func_8015cde0 = 0x8015cde0;
func_8015bdd4 = 0x8015bdd4;
func_8015bd0c = 0x8015bd0c;
func_8015bdec = 0x8015bdec;
func_80157b6c = 0x80157b6c;
func_80157c84 = 0x80157c84;
func_80157c94 = 0x80157c94;
func_80157d00 = 0x80157d00;
func_80157d9c = 0x80157d9c;
func_80157f30 = 0x80157f30;
func_80157fc4 = 0x80157fc4;
func_80158028 = 0x80158028;
func_80158208 = 0x80158208;
func_801582a0 = 0x801582a0;
func_80158300 = 0x80158300;
func_801588cc = 0x801588cc;
func_80158900 = 0x80158900;
func_8015893c = 0x8015893c;
func_801589c0 = 0x801589c0;
func_80158a2c = 0x80158a2c;
func_8015999c = 0x8015999c;
func_801599cc = 0x801599cc;
func_80166750 = 0x80166750;
func_80166638 = 0x80166638;
func_801631f8 = 0x801631f8;
func_801633ac = 0x801633ac;
func_8016310c = 0x8016310c;
SetRCnt = 0x8015764c;
StartRCnt = 0x80157720;
ResetRCnt = 0x80157784;
get_mode = 0x80158d28;
get_cs = 0x80158d84;
get_ce = 0x80158e50;
get_ofs = 0x80158f1c;
get_tw = 0x80158f64;
_addque2 = 0x801599f0;
get_alarm = 0x8015a2b8;
GetVideoMode = 0x8015f74c;
SpuVmSetVol = 0x80163794;
_SsTrapIntrVSync = 0x801657b0;
_SsSeqCalledTbyT_1per2 = 0x801657e4;
_SsContDataEntry = 0x80167388;
SpuVmDamperOn = 0x801688f4;
SpuInitHot = 0x8016a7c4;
_spu_init = 0x8016b788;
SpuInit = 0x8016cc94;
SpuInitMalloc = 0x8016d018;
_spu_getInTransfer = 0x8016d168;
D_8002D3B8 = 0x80181930;
D_8002C220 = 0x8018194c;
D_8002C260 = 0x8018198c;
gs = 0x80181994;
D_8002C214 = 0x80181a14;
D_8002C228 = 0x80181a28;
D_8002C23C = 0x80181a3c;
D_8002C250 = 0x80181a50;
GPU_DATA = 0x80181a64;
GPU_STATUS = 0x80181a68;
DMA2_MADR = 0x80181a6c;
DMA2_BCR = 0x80181a70;
DMA2_CHCR = 0x80181a74;
DMA6_MADR = 0x80181a78;
DMA6_BCR = 0x80181a7c;
DMA6_CHCR = 0x80181a80;
DPCR = 0x80181a84;
D_80032A24 = 0x80181ab4;
CD_unk14 = 0x80181e14;
_spu_rev_attr = 0x801831a0;
_spu_keystat = 0x801831e8;
_spu_clear_a = 0x801831f4;
_spu_clear_b = 0x801831f8;
D_80039260 = 0x8018d3d0;
D_80039268 = 0x8018d3d8;
D_80039270 = 0x8018d3e0;
Alarm = 0x8018d3e8;
_ss_spu_vm_rec = 0x8018da54;
_snd_tick_vsync = 0x8018dadc;
_snd_tick_unk = 0x8018dae0;
_snd_tick_irq = 0x8018dae4;
_snd_tick_half = 0x8018dae8;
_snd_video_mode = 0x8018daec;
_snd_tick_prev_cb = 0x8018daf0;
_snd_tick_flag = 0x801904d4;
_svm_vcf = 0x801a6930;
_snd_tick_mode = 0x801a89a4;
func_8015a284 = 0x8015a284;
func_8015a504 = 0x8015a504;
_exeque = 0x80159cec;
_qin = 0x80181a98;
_qout = 0x80181a9c;
D_intr_reset = 0x80181aa8;
D_80039254 = 0x80181aac;
D_80039258 = 0x80181ab0;
ctlbuf = 0x8018d2bc;
D_80037F54 = 0x801aa6f8;

/* Added by src.u_z1350c0. */
data_80188d10 = 0x80188d10;
data_80188d2c = 0x80188d2c;
data_80188d14 = 0x80188d14;
data_80188d30 = 0x80188d30;
data_80188d18 = 0x80188d18;
data_80188d34 = 0x80188d34;
data_80188d1c = 0x80188d1c;
data_80188d38 = 0x80188d38;
data_80188d20 = 0x80188d20;
data_80188d3c = 0x80188d3c;
data_80188d24 = 0x80188d24;
data_80188d40 = 0x80188d40;
table_80171c57 = 0x80171c57;
table_80171c5b = 0x80171c5b;
table_80171c60 = 0x80171c60;

/* Added by src.u_z1550b0. */
data_801812b4 = 0x801812b4;
data_801812c4 = 0x801812c4;
data_801812d4 = 0x801812d4;
data_80181324 = 0x80181324;
data_80181334 = 0x80181334;
data_8018d250 = 0x8018d250;
data_8018d254 = 0x8018d254;
data_8018d258 = 0x8018d258;
data_8018d25c = 0x8018d25c;
data_8018125e = 0x8018125e;
data_8018128a = 0x8018128a;
data_80181094 = 0x80181094;
data_80181138 = 0x80181138;
data_80181344 = 0x80181344;
func_8015c77c = 0x8015c77c;
func_8015c7a4 = 0x8015c7a4;
func_8015c7cc = 0x8015c7cc;
CdSyncCallback = 0x8015c978;
CdReadyCallback = 0x8015c990;
CdReadCallback = 0x8015c9a8;
func_8015a424 = 0x8015a424;
func_8015c6ec = 0x8015c6ec;

/* Added by src.u_v18. */

/* Added by src.u_v19. */

/* Added by src.u_v22. */
func_8015ed74 = 0x8015ed74;
func_8015ec84 = 0x8015ec84;
func_8015e02c = 0x8015e02c;
func_8015e788 = 0x8015e788;
func_8015df94 = 0x8015df94;
func_8015c09c = 0x8015c09c;
func_8015c0ec = 0x8015c0ec;
func_8015c100 = 0x8015c100;
func_8015c150 = 0x8015c150;
func_8015c23c = 0x8015c23c;
func_8015c3f0 = 0x8015c3f0;
func_8016c850 = 0x8016c850;
func_8016c8d8 = 0x8016c8d8;
func_8016c988 = 0x8016c988;
_spu_timeout_msg = 0x8016e564;
func_8015df0c = 0x8015df0c;
func_8015e200 = 0x8015e200;
func_8015e10c = 0x8015e10c;

/* Added by src.u_z15a9e4. */
str_8016dd14 = 0x8016dd14;
str_8016dd28 = 0x8016dd28;
str_8016dd50 = 0x8016dd50;
str_8016dd68 = 0x8016dd68;
str_8016dd80 = 0x8016dd80;

/* Added by src.u_z15ab7c. */
str_8016dd98 = 0x8016dd98;
str_8016dda0 = 0x8016dda0;
str_8016dda8 = 0x8016dda8;
str_8016ddb0 = 0x8016ddb0;
str_8016ddb8 = 0x8016ddb8;
str_8016ddbc = 0x8016ddbc;
str_8016ddc0 = 0x8016ddc0;
str_8016ddc8 = 0x8016ddc8;
str_8016ddd0 = 0x8016ddd0;
str_8016ddd8 = 0x8016ddd8;
str_8016dde0 = 0x8016dde0;
str_8016dde8 = 0x8016dde8;
str_8016ddf0 = 0x8016ddf0;
str_8016ddf4 = 0x8016ddf4;
str_8016ddf8 = 0x8016ddf8;
str_8016de00 = 0x8016de00;
str_8016de08 = 0x8016de08;
_spu_addrMode = 0x80183154;
func_8014e890 = 0x8014e890;
func_80158e50 = 0x80158e50;
func_8013ec50 = 0x8013ec50;
func_80158d84 = 0x80158d84;
func_8013245c = 0x8013245c;
func_8013a3a8 = 0x8013a3a8;
func_8013ef28 = 0x8013ef28;
func_80151324 = 0x80151324;
func_80131ab4 = 0x80131ab4;
func_801347b4 = 0x801347b4;
func_80158d28 = 0x80158d28;
func_8015764c = 0x8015764c;
func_8013eb28 = 0x8013eb28;
func_801364a0 = 0x801364a0;
func_8013c6ac = 0x8013c6ac;
func_8013172c = 0x8013172c;
func_8011bf70 = 0x8011bf70;
func_801452ec = 0x801452ec;
func_8013b558 = 0x8013b558;
func_8011db78 = 0x8011db78;
func_80124304 = 0x80124304;
func_80158fe8 = 0x80158fe8;
func_8013ed90 = 0x8013ed90;
func_8011c724 = 0x8011c724;
func_80141534 = 0x80141534;
func_80120604 = 0x80120604;
func_8013bfa4 = 0x8013bfa4;
func_8013f474 = 0x8013f474;
func_8013e028 = 0x8013e028;
func_8015fd24 = 0x8015fd24;
func_80157784 = 0x80157784;
func_80120cf0 = 0x80120cf0;
func_80135a10 = 0x80135a10;
func_8011eb4c = 0x8011eb4c;
func_8014362c = 0x8014362c;
func_80133108 = 0x80133108;
func_80141cec = 0x80141cec;
func_80136744 = 0x80136744;
func_80137e38 = 0x80137e38;
func_8012fd80 = 0x8012fd80;
func_8015057c = 0x8015057c;
func_8013f8c4 = 0x8013f8c4;
func_8011a880 = 0x8011a880;
func_80142030 = 0x80142030;
func_8011cf98 = 0x8011cf98;
func_8011df3c = 0x8011df3c;
func_80139928 = 0x80139928;
func_80153d7c = 0x80153d7c;
func_80158f1c = 0x80158f1c;
func_801397d0 = 0x801397d0;
func_8011acbc = 0x8011acbc;
func_80119444 = 0x80119444;
func_80134234 = 0x80134234;
func_80140b5c = 0x80140b5c;
func_80140cd8 = 0x80140cd8;
func_8011d74c = 0x8011d74c;
func_8014c9f4 = 0x8014c9f4;
func_8013b0c4 = 0x8013b0c4;
func_80152124 = 0x80152124;
func_80157720 = 0x80157720;
func_801189c4 = 0x801189c4;
func_80138d70 = 0x80138d70;
func_8013db48 = 0x8013db48;
func_8014ff20 = 0x8014ff20;
func_8011f06c = 0x8011f06c;
func_80132cf0 = 0x80132cf0;
func_8013902c = 0x8013902c;
func_801199ec = 0x801199ec;
func_801511c8 = 0x801511c8;
func_8014c4a8 = 0x8014c4a8;
func_8011a018 = 0x8011a018;
func_8011fa50 = 0x8011fa50;
func_8014a94c = 0x8014a94c;
func_8013f3e8 = 0x8013f3e8;
func_801427d8 = 0x801427d8;
func_8012f9b0 = 0x8012f9b0;
func_80151a04 = 0x80151a04;
func_8013fc18 = 0x8013fc18;
func_8013e1e4 = 0x8013e1e4;
func_80152ad8 = 0x80152ad8;
func_80119694 = 0x80119694;

/* Added by src.u_slot2a_0020. */
data_801e1c8c_slot2a = 0x801e1c8c;
data_801e25c8_slot2a = 0x801e25c8;
data_801e25cc_slot2a = 0x801e25cc;
data_801e25d0_slot2a = 0x801e25d0;
data_801e25d4_slot2a = 0x801e25d4;
data_801e5160_slot2a = 0x801e5160;
data_801e5164_slot2a = 0x801e5164;
data_801e52c8_slot2a = 0x801e52c8;
func_801e0958_slot2a = 0x801e0958;
func_801e114c_slot2a = 0x801e114c;
func_801e12fc_slot2a = 0x801e12fc;
func_801e16a8_slot2a = 0x801e16a8;

/* Added by src.u_slot2a_03d0. */
data_801e52c4_slot2a = 0x801e52c4;
data_801e2250_slot2a = 0x801e2250;

/* Added by src.u_slot2a_05f4. */
data_801e25d8_slot2a = 0x801e25d8;
table_801e225c_slot2a = 0x801e225c;
data_801e25dc_slot2a = 0x801e25dc;

/* Added by src.u_slot2a_0850. */
data_801e21f4_slot2a = 0x801e21f4;
data_801e21fc_slot2a = 0x801e21fc;

/* Added by src.u_slot2a_0e48. */
data_801e226c_slot2a = 0x801e226c;
data_801e226e_slot2a = 0x801e226e;
data_801e2270_slot2a = 0x801e2270;
data_801e2272_slot2a = 0x801e2272;
data_801e22b4_slot2a = 0x801e22b4;
data_801e2fdc_slot2a = 0x801e2fdc;
data_801e515c_slot2a = 0x801e515c;

/* Added by src.u_slot2a_114c. */
data_801e2300_slot2a = 0x801e2300;
data_801e230c_slot2a = 0x801e230c;
