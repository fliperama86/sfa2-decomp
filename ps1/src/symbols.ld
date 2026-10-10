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
SpuWrite = 0x8016d0dc;
_spu_setInTransfer = 0x8016d13c;
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
_addque2 = 0x801599f0;
get_alarm = 0x8015a2b8;
GetVideoMode = 0x8015f74c;
SpuVmSetVol = 0x80163794;
_SsContDataEntry = 0x80167388;
SpuInitHot = 0x8016a7c4;
_spu_init = 0x8016b788;
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
func_80151324 = 0x80151324;
func_80131ab4 = 0x80131ab4;
func_801347b4 = 0x801347b4;
func_80158d28 = 0x80158d28;
func_8015764c = 0x8015764c;
func_8013eb28 = 0x8013eb28;
func_801364a0 = 0x801364a0;
func_8013c6ac = 0x8013c6ac;
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
func_80157784 = 0x80157784;
func_80120cf0 = 0x80120cf0;
func_80135a10 = 0x80135a10;
func_8011eb4c = 0x8011eb4c;
func_8014362c = 0x8014362c;
func_80141cec = 0x80141cec;
func_80136744 = 0x80136744;
func_8012fd80 = 0x8012fd80;
func_8013f8c4 = 0x8013f8c4;
func_8011a880 = 0x8011a880;
func_80142030 = 0x80142030;
func_8011cf98 = 0x8011cf98;
func_8011df3c = 0x8011df3c;
func_80139928 = 0x80139928;
func_80158f1c = 0x80158f1c;
func_801397d0 = 0x801397d0;
func_8011acbc = 0x8011acbc;
func_80119444 = 0x80119444;
func_80134234 = 0x80134234;
func_80140b5c = 0x80140b5c;
func_80140cd8 = 0x80140cd8;
func_8011d74c = 0x8011d74c;
func_8013b0c4 = 0x8013b0c4;
func_80152124 = 0x80152124;
func_80157720 = 0x80157720;
func_801189c4 = 0x801189c4;
func_80138d70 = 0x80138d70;
func_8013db48 = 0x8013db48;
func_8011f06c = 0x8011f06c;
func_8013902c = 0x8013902c;
func_8014c4a8 = 0x8014c4a8;
func_8011a018 = 0x8011a018;
func_8011fa50 = 0x8011fa50;
func_8014a94c = 0x8014a94c;
func_8013f3e8 = 0x8013f3e8;
func_801427d8 = 0x801427d8;
func_8012f9b0 = 0x8012f9b0;
func_8013fc18 = 0x8013fc18;
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

/* Called by the module image slot16. Both are in the character block that is
   loaded. */
func_801b606c = 0x801b606c;
func_801b63fc = 0x801b63fc;

/* Added by src.u_slot16_bc00. */
data_8007e868_slot16 = 0x8007e868;

/* Added by src.u_slot16_bd50. */
data_8007e870_slot16 = 0x8007e870;

/* Added by src.u_slot16_be8c. */
data_8007e878_slot16 = 0x8007e878;

/* Added by src.u_n1193fc. */
func_800100a4 = 0x800100a4;
func_80010e60 = 0x80010e60;
data_80185fc8 = 0x80185fc8;
data_80185fcc = 0x80185fcc;
data_80185fd0 = 0x80185fd0;
data_80185fd4 = 0x80185fd4;
data_8016e9a8 = 0x8016e9a8;
data_8016ea60 = 0x8016ea60;
data_8016e9b8 = 0x8016e9b8;
func_801254f4 = 0x801254f4;

/* Added by src.u_n125b18. */
table_8016f520 = 0x8016f520;
table_80171988 = 0x80171988;
table_8017193c = 0x8017193c;

/* Added by src.u_n12b8dc. */
table_8017ab88 = 0x8017ab88;

/* Added by src.u_n13d520. */

/* Added by src.u_n145b60. */
data_80190468 = 0x80190468;
data_8017cf60 = 0x8017cf60;

/* Added by src.u_n14a648. */
data_8017ec44 = 0x8017ec44;
data_8018d260 = 0x8018d260;
data_80181044 = 0x80181044;
data_80181060 = 0x80181060;
data_801811aa = 0x801811aa;
data_80181206 = 0x80181206;
data_8018124a = 0x8018124a;
func_801545cc = 0x801545cc;

/* Added by src.u_n154998. */
data_801812e4 = 0x801812e4;
data_80181304 = 0x80181304;
data_80181314 = 0x80181314;

/* Added by src.u_n156220. */
data_80181668 = 0x80181668;
data_80181684 = 0x80181684;
data_801816a0 = 0x801816a0;
data_801816bc = 0x801816bc;
data_801816d4 = 0x801816d4;
data_801816e4 = 0x801816e4;
data_80181708 = 0x80181708;
data_8018171c = 0x8018171c;
data_8018172c = 0x8018172c;
data_8018173c = 0x8018173c;
data_80181758 = 0x80181758;
data_80181774 = 0x80181774;
data_8018178c = 0x8018178c;
func_80165d34 = 0x80165d34;
func_801682d0 = 0x801682d0;
func_80168368 = 0x80168368;
func_801683a8 = 0x801683a8;
func_801683e8 = 0x801683e8;
func_80168908 = 0x80168908;
func_80168f18 = 0x80168f18;
func_80168f50 = 0x80168f50;
func_80168e18 = 0x80168e18;
func_801691ac = 0x801691ac;
func_80169abc = 0x80169abc;
func_8016a0cc = 0x8016a0cc;
func_8016a3b0 = 0x8016a3b0;
func_8016a440 = 0x8016a440;
func_8016a49c = 0x8016a49c;
func_8016a730 = 0x8016a730;
func_8016b340 = 0x8016b340;
func_8016cc18 = 0x8016cc18;
func_8016ce4c = 0x8016ce4c;
func_8016cd84 = 0x8016cd84;
func_8016ced0 = 0x8016ced0;
func_8016cf70 = 0x8016cf70;
func_8016d018 = 0x8016d018;
func_8016d06c = 0x8016d06c;
func_8016d0a0 = 0x8016d0a0;
func_8016d440 = 0x8016d440;
func_8015cfc8 = 0x8015cfc8;
func_8015cec4 = 0x8015cec4;
func_80158f64 = 0x80158f64;
func_801657b0 = 0x801657b0;
func_801657e4 = 0x801657e4;
func_80164ec0 = 0x80164ec0;
_snd_tick_toggle = 0x8018312c;
func_8015c938 = 0x8015c938;
func_8015c958 = 0x8015c958;
func_8015c978 = 0x8015c978;
func_8015ce04 = 0x8015ce04;
func_80169b24 = 0x80169b24;
func_80165790 = 0x80165790;
func_801656ac = 0x801656ac;
func_801656cc = 0x801656cc;
func_801681fc = 0x801681fc;
func_801681dc = 0x801681dc;
func_80168a14 = 0x80168a14;
func_801688f4 = 0x801688f4;
func_8016582c = 0x8016582c;
func_80165840 = 0x80165840;
func_8016cc94 = 0x8016cc94;
func_8016d168 = 0x8016d168;

/* Added by src.u_slot0b_00c4. */
func_801e0a78_slot0b = 0x801e0a78;
func_801e0ce8_slot0b = 0x801e0ce8;
func_801e0ea8_slot0b = 0x801e0ea8;
func_801e1d6c_slot0b = 0x801e1d6c;
data_801e1fdc_slot0b = 0x801e1fdc;
data_801e766c_slot0b = 0x801e766c;
data_801e46bc_slot0b = 0x801e46bc;
data_801e4704_slot0b = 0x801e4704;
data_801e46cc_slot0b = 0x801e46cc;
data_801e46d4_slot0b = 0x801e46d4;
data_801e46b4_slot0b = 0x801e46b4;
data_801e4614_slot0b = 0x801e4614;

/* Added by src.u_slot0b_05a0. */
data_801e4710_slot0b = 0x801e4710;
data_801e471c_slot0b = 0x801e471c;
data_801e46b0_slot0b = 0x801e46b0;
data_801e4728_slot0b = 0x801e4728;

/* Added by src.u_slot0b_09f4. */
data_801e4748_slot0b = 0x801e4748;
table_801e1fec_slot0b = 0x801e1fec;

/* Added by src.u_slot0b_0ea8. */
table_801e4cd0_slot0b = 0x801e4cd0;
data_801e7a2c_slot0b = 0x801e7a2c;
table_801e4ce0_slot0b = 0x801e4ce0;
data_801e4b40_slot0b = 0x801e4b40;
table_801e47a8_slot0b = 0x801e47a8;

/* Added by src.u_slot0b_131c. */
data_801e4c30_slot0b = 0x801e4c30;
data_801e4cc0_slot0b = 0x801e4cc0;
data_801e4cec_slot0b = 0x801e4cec;

/* Added by src.u_slot0b_17a4. */
data_801e7648_slot0b = 0x801e7648;
data_801e4cfc_slot0b = 0x801e4cfc;
data_801e4d50_slot0b = 0x801e4d50;
data_801e4d5c_slot0b = 0x801e4d5c;
data_801e4d78_slot0b = 0x801e4d78;
data_801ae02e = 0x801ae02e;

/* Added by src.u_slot0b_1c00. */
data_801e7630_slot0b = 0x801e7630;
data_801e7668_slot0b = 0x801e7668;
data_8019d7cc = 0x8019d7cc;

/* Added by src.u_slot12_0058. */
func_80011688_slot12 = 0x80011688;
func_80012a0c_slot12 = 0x80012a0c;
func_80012c34_slot12 = 0x80012c34;
func_80013224_slot12 = 0x80013224;
func_8001606c_slot12 = 0x8001606c;
func_80016af0_slot12 = 0x80016af0;
data_80017560_slot12 = 0x80017560;
data_80017568_slot12 = 0x80017568;
data_80017574_slot12 = 0x80017574;
data_8002d56c_slot12 = 0x8002d56c;
data_8002d568_slot12 = 0x8002d568;
data_8002d578_slot12 = 0x8002d578;
data_8002d574_slot12 = 0x8002d574;
data_8002d57c_slot12 = 0x8002d57c;

/* Added by src.u_slot12_0830. */
data_800175b4_slot12 = 0x800175b4;
data_80028a98_slot12 = 0x80028a98;

/* Added by src.u_slot12_1010. */
data_800175cc_slot12 = 0x800175cc;
data_80028aa0_slot12 = 0x80028aa0;
data_80028b40_slot12 = 0x80028b40;
data_801a2fe4 = 0x801a2fe4;
data_80017540_slot12 = 0x80017540;
data_80028b44_slot12 = 0x80028b44;

/* Added by src.u_slot12_17ac. */
data_80022dc4_slot12 = 0x80022dc4;
table_80022dcc_slot12 = 0x80022dcc;
table_80022dd8_slot12 = 0x80022dd8;
data_80022de8_slot12 = 0x80022de8;
table_80022e08_slot12 = 0x80022e08;
table_80022e14_slot12 = 0x80022e14;
data_80028b48_slot12 = 0x80028b48;
data_800175e0_slot12 = 0x800175e0;
data_80017630_slot12 = 0x80017630;
table_80017870_slot12 = 0x80017870;

/* Added by src.u_slot12_1f04. */
data_80029348_slot12 = 0x80029348;
data_8002934c_slot12 = 0x8002934c;
data_8002d570_slot12 = 0x8002d570;
data_8002a390_slot12 = 0x8002a390;
data_80021b34_slot12 = 0x80021b34;
data_80022e20_slot12 = 0x80022e20;
data_80022e30_slot12 = 0x80022e30;
data_80022c08_slot12 = 0x80022c08;
data_80029350_slot12 = 0x80029350;
data_80029b70_slot12 = 0x80029b70;
data_80018d80_slot12 = 0x80018d80;

/* Added by src.u_slot12_281c. */
data_80022e80_slot12 = 0x80022e80;
data_80022e8c_slot12 = 0x80022e8c;
data_80022e98_slot12 = 0x80022e98;
data_80022e48_slot12 = 0x80022e48;
data_8002a394_slot12 = 0x8002a394;
data_8002a794_slot12 = 0x8002a794;
data_80019054_slot12 = 0x80019054;
data_80019548_slot12 = 0x80019548;
data_8001d4ec_slot12 = 0x8001d4ec;
data_80023170_slot12 = 0x80023170;
data_801ae02d = 0x801ae02d;
data_801ae066 = 0x801ae066;
data_8002b620_slot12 = 0x8002b620;
data_8002b624_slot12 = 0x8002b624;
data_8002b628_slot12 = 0x8002b628;
data_8002b62c_slot12 = 0x8002b62c;
data_8002b6b0_slot12 = 0x8002b6b0;
data_800230a0_slot12 = 0x800230a0;
data_80023120_slot12 = 0x80023120;
data_80022ea0_slot12 = 0x80022ea0;

/* Added by src.u_slot12_3180. */
data_8002b614_slot12 = 0x8002b614;
data_8002b618_slot12 = 0x8002b618;
data_8002ab94_slot12 = 0x8002ab94;
data_800243bc_slot12 = 0x800243bc;
data_800243c8_slot12 = 0x800243c8;
data_80022eb0_slot12 = 0x80022eb0;

/* Added by src.u_slot12_3a3c. */
data_8002317c_slot12 = 0x8002317c;
data_8002413c_slot12 = 0x8002413c;
data_800243d4_slot12 = 0x800243d4;
data_80024414_slot12 = 0x80024414;
data_80024450_slot12 = 0x80024450;
data_8002445c_slot12 = 0x8002445c;
data_8002b6f0_slot12 = 0x8002b6f0;
data_8002b710_slot12 = 0x8002b710;
data_8002b730_slot12 = 0x8002b730;
data_8002b9b0_slot12 = 0x8002b9b0;
data_8002bc30_slot12 = 0x8002bc30;
data_8002bc34_slot12 = 0x8002bc34;
data_801a3de4 = 0x801a3de4;
data_801a29e4 = 0x801a29e4;

/* Added by src.u_slot12_4268. */
data_80024470_slot12 = 0x80024470;
table_800281f0_slot12 = 0x800281f0;
table_80028148_slot12 = 0x80028148;

/* Added by src.u_slot12_4a30. */
data_80028200_slot12 = 0x80028200;
data_800282a8_slot12 = 0x800282a8;
data_800282c4_slot12 = 0x800282c4;
data_800282d4_slot12 = 0x800282d4;
data_800282dc_slot12 = 0x800282dc;
data_800280a0_slot12 = 0x800280a0;
data_80024488_slot12 = 0x80024488;
data_8002bc38_slot12 = 0x8002bc38;
data_80028474_slot12 = 0x80028474;
data_8002bc78_slot12 = 0x8002bc78;
data_8002bc7c_slot12 = 0x8002bc7c;
data_8002bc8c_slot12 = 0x8002bc8c;
data_8002bc94_slot12 = 0x8002bc94;
data_8002bdd4_slot12 = 0x8002bdd4;

/* Added by src.u_slot12_51e4. */
data_80028484_slot12 = 0x80028484;
data_800284ac_slot12 = 0x800284ac;
data_800284c4_slot12 = 0x800284c4;
data_800284cc_slot12 = 0x800284cc;
data_800284d8_slot12 = 0x800284d8;
data_800284e4_slot12 = 0x800284e4;
data_800284e8_slot12 = 0x800284e8;
data_8002bc80_slot12 = 0x8002bc80;
data_8002bc82_slot12 = 0x8002bc82;
data_8002bc84_slot12 = 0x8002bc84;
data_8002bc85_slot12 = 0x8002bc85;
data_8002bc86_slot12 = 0x8002bc86;
data_8002bc87_slot12 = 0x8002bc87;
data_8002bc88_slot12 = 0x8002bc88;
data_8002bc90_slot12 = 0x8002bc90;
data_801903c0 = 0x801903c0;
data_801903c2 = 0x801903c2;
data_801903c4 = 0x801903c4;
data_801903c5 = 0x801903c5;

/* Added by src.u_slot12_59bc. */
data_8002bca4_slot12 = 0x8002bca4;
data_8002bfb4_slot12 = 0x8002bfb4;
data_80028444_slot12 = 0x80028444;
data_80028500_slot12 = 0x80028500;
data_800284ec_slot12 = 0x800284ec;
data_8002bcc4_slot12 = 0x8002bcc4;
data_8002bf64_slot12 = 0x8002bf64;
data_8002bcb4_slot12 = 0x8002bcb4;
data_8002bf14_slot12 = 0x8002bf14;

/* Added by src.u_slot12_61ec. */
data_80028508_slot12 = 0x80028508;
data_800287a8_slot12 = 0x800287a8;
data_8002874c_slot12 = 0x8002874c;
data_800287a0_slot12 = 0x800287a0;
data_800287a4_slot12 = 0x800287a4;
data_800287b4_slot12 = 0x800287b4;
data_8002bfe8_slot12 = 0x8002bfe8;
data_8002c128_slot12 = 0x8002c128;
data_8002c268_slot12 = 0x8002c268;
data_80028a04_slot12 = 0x80028a04;
data_8002c3a8_slot12 = 0x8002c3a8;
data_8002c528_slot12 = 0x8002c528;
data_80028970_slot12 = 0x80028970;

/* Added by src.u_slot12_6a7c. */
data_800287ec_slot12 = 0x800287ec;
data_8002891c_slot12 = 0x8002891c;
data_80028a10_slot12 = 0x80028a10;
data_80028a18_slot12 = 0x80028a18;
data_80028a28_slot12 = 0x80028a28;
data_80028a30_slot12 = 0x80028a30;
data_80028a40_slot12 = 0x80028a40;
data_8002c3c8_slot12 = 0x8002c3c8;
data_8002c4c8_slot12 = 0x8002c4c8;
data_8002c508_slot12 = 0x8002c508;

/* Added by src.u_slot12_71cc. */
data_80028a58_slot12 = 0x80028a58;
data_80028a60_slot12 = 0x80028a60;
data_80028a84_slot12 = 0x80028a84;
data_80028a88_slot12 = 0x80028a88;
data_8002d528_slot12 = 0x8002d528;

/* Added by src.u_x23. */
data_8017ff68 = 0x8017ff68;

/* Added by src.u_slot2a_0958x. */

/* Added by src.u_slot00_5e00. */
func_80077808_slot00 = 0x80077808;
func_80078628_slot00 = 0x80078628;
func_80078d78_slot00 = 0x80078d78;
data_80079140_slot00 = 0x80079140;
data_80079148_slot00 = 0x80079148;
data_80079150_slot00 = 0x80079150;
data_80079158_slot00 = 0x80079158;
data_80079164_slot00 = 0x80079164;
data_8007916c_slot00 = 0x8007916c;
data_80075e00_slot00 = 0x80075e00;

/* Added by src.u_slot00_642c. */
data_800797b0_slot00 = 0x800797b0;
data_800797c0_slot00 = 0x800797c0;
data_800797d4_slot00 = 0x800797d4;
data_800797dc_slot00 = 0x800797dc;
data_1f8000a8 = 0x1f8000a8;
data_1f800158 = 0x1f800158;

/* Added by src.u_slot00_6a80. */
data_800797e4_slot00 = 0x800797e4;
data_800797ec_slot00 = 0x800797ec;
data_800797f4_slot00 = 0x800797f4;
data_800797fc_slot00 = 0x800797fc;
data_8007980c_slot00 = 0x8007980c;
data_1f8000ac = 0x1f8000ac;
data_1f80015c = 0x1f80015c;

/* Added by src.u_slot00_7050. */
data_80079814_slot00 = 0x80079814;
data_80079834_slot00 = 0x80079834;
data_80079860_slot00 = 0x80079860;
data_80079878_slot00 = 0x80079878;
data_800798d0_slot00 = 0x800798d0;
data_800798fc_slot00 = 0x800798fc;
data_80079908_slot00 = 0x80079908;
data_80079938_slot00 = 0x80079938;
data_80079948_slot00 = 0x80079948;

/* Added by src.u_slot00_7640. */
data_80079fec_slot00 = 0x80079fec;
data_80079ff8_slot00 = 0x80079ff8;
data_8007a008_slot00 = 0x8007a008;
data_8007a018_slot00 = 0x8007a018;
data_8007a028_slot00 = 0x8007a028;
data_8007a13c_slot00 = 0x8007a13c;
data_8007a140_slot00 = 0x8007a140;
data_8007a010_slot00 = 0x8007a010;

/* Added by src.u_slot00_7cd8. */
data_8007a038_slot00 = 0x8007a038;
data_8007a044_slot00 = 0x8007a044;
data_8007a06c_slot00 = 0x8007a06c;
data_8007a07c_slot00 = 0x8007a07c;
data_8007a0ac_slot00 = 0x8007a0ac;
data_8007a064_slot00 = 0x8007a064;

/* Added by src.u_slot00_8340. */
data_8007a0d8_slot00 = 0x8007a0d8;
data_8007a104_slot00 = 0x8007a104;
data_8007a110_slot00 = 0x8007a110;

/* Added by src.u_slot00_8964. */
data_8007a11c_slot00 = 0x8007a11c;
data_8007a124_slot00 = 0x8007a124;
data_8007a12c_slot00 = 0x8007a12c;

/* Added by src.u_slot2b_7000. */
func_80077868_slot2b = 0x80077868;
func_80079144_slot2b = 0x80079144;
func_800793d4_slot2b = 0x800793d4;
func_800796e4_slot2b = 0x800796e4;
data_80079804_slot2b = 0x80079804;
data_80079810_slot2b = 0x80079810;
data_8007981c_slot2b = 0x8007981c;
data_80079828_slot2b = 0x80079828;
data_80079834_slot2b = 0x80079834;
data_80079840_slot2b = 0x80079840;
data_80079848_slot2b = 0x80079848;
data_80079854_slot2b = 0x80079854;

/* Added by src.u_slot2b_7660. */
data_8007985c_slot2b = 0x8007985c;
data_800798f8_slot2b = 0x800798f8;
data_80079900_slot2b = 0x80079900;
data_80079910_slot2b = 0x80079910;
data_8007991c_slot2b = 0x8007991c;
data_80079920_slot2b = 0x80079920;
data_80079938_slot2b = 0x80079938;
data_8007ef24_slot2b = 0x8007ef24;

/* Added by src.u_slot2b_7c38. */
data_8007994c_slot2b = 0x8007994c;
data_80079954_slot2b = 0x80079954;
data_8007995c_slot2b = 0x8007995c;
data_80079968_slot2b = 0x80079968;
data_80079a00_slot2b = 0x80079a00;
data_80079a08_slot2b = 0x80079a08;
data_80079a18_slot2b = 0x80079a18;
data_8007ef28_slot2b = 0x8007ef28;
data_8007ef2c_slot2b = 0x8007ef2c;

/* Added by src.u_slot2b_82c0. */
data_80079a2c_slot2b = 0x80079a2c;
data_8007eb48_slot2b = 0x8007eb48;
data_8007ed88_slot2b = 0x8007ed88;
data_8007ed98_slot2b = 0x8007ed98;
data_8007eda8_slot2b = 0x8007eda8;
data_8007ef30_slot2b = 0x8007ef30;
data_8007ef34_slot2b = 0x8007ef34;

/* Added by src.u_slot2b_88f0. */
data_8007edc8_slot2b = 0x8007edc8;
data_8007ede8_slot2b = 0x8007ede8;
data_8007edf4_slot2b = 0x8007edf4;
data_8007ee00_slot2b = 0x8007ee00;
data_8007ee08_slot2b = 0x8007ee08;

/* Added by src.u_slot2b_8f8c. */
data_8007ee14_slot2b = 0x8007ee14;
data_8007ee54_slot2b = 0x8007ee54;
data_8017c850 = 0x8017c850;

/* Added by src.u_slot2b_95f8x. */
data_8007ee64_slot2b = 0x8007ee64;

/* Added by src.u_slot2a_0118x. */
data_801e1bdc_slot2a = 0x801e1bdc;
data_801e2318_slot2a = 0x801e2318;

/* Added by src.u_slot0f_f378. */
func_800e0850_slot0f = 0x800e0850;
func_800e0a1c_slot0f = 0x800e0a1c;
func_800e0b48_slot0f = 0x800e0b48;
func_800e0f2c_slot0f = 0x800e0f2c;
func_800e3478_slot0f = 0x800e3478;
func_800e4ab0_slot0f = 0x800e4ab0;
func_800e68e8_slot0f = 0x800e68e8;
func_800e80ec_slot0f = 0x800e80ec;
data_800e83a8_slot0f = 0x800e83a8;
data_800e83b4_slot0f = 0x800e83b4;
data_800e83c8_slot0f = 0x800e83c8;
data_800e83e0_slot0f = 0x800e83e0;
func_800df654_slot0f = 0x800df654;
func_800df750_slot0f = 0x800df750;

/* Added by src.u_slot0f_fa04. */
data_800e83e8_slot0f = 0x800e83e8;
data_800e83f0_slot0f = 0x800e83f0;
data_800e83fc_slot0f = 0x800e83fc;
data_800e8404_slot0f = 0x800e8404;
data_800f8574_slot0f = 0x800f8574;

/* Added by src.u_slot0f_000c. */
data_800e841c_slot0f = 0x800e841c;
data_800e8428_slot0f = 0x800e8428;

/* Added by src.u_slot0f_0560. */
data_800f01fc_slot0f = 0x800f01fc;
data_800e84f0_slot0f = 0x800e84f0;
data_800e8480_slot0f = 0x800e8480;
data_800e8490_slot0f = 0x800e8490;
data_800f8576_slot0f = 0x800f8576;
data_8016e686 = 0x8016e686;
data_800df0f0_slot0f = 0x800df0f0;
data_800df0f4_slot0f = 0x800df0f4;
data_800df0f8_slot0f = 0x800df0f8;
data_800df0fc_slot0f = 0x800df0fc;
data_800df100_slot0f = 0x800df100;
data_800df118_slot0f = 0x800df118;
data_800df11a_slot0f = 0x800df11a;
data_800e8504_slot0f = 0x800e8504;
data_8018ff14 = 0x8018ff14;
data_8018ff18 = 0x8018ff18;

/* Added by src.u_slot0f_0bf4. */
data_800f80a4_slot0f = 0x800f80a4;
data_800df11c_slot0f = 0x800df11c;
_card_clear = 0x8016d8a0;

/* Added by src.u_slot0f_119c. */
data_800f857c_slot0f = 0x800f857c;
data_800e8778_slot0f = 0x800e8778;

/* Added by src.u_slot0f_171c. */
data_800e8790_slot0f = 0x800e8790;
data_800e8794_slot0f = 0x800e8794;
data_800e87a8_slot0f = 0x800e87a8;
data_800e87bc_slot0f = 0x800e87bc;

/* Added by src.u_slot0f_1dac. */
data_800e8508_slot0f = 0x800e8508;
data_800e87cc_slot0f = 0x800e87cc;
data_800e87dc_slot0f = 0x800e87dc;
data_800e87ec_slot0f = 0x800e87ec;
data_800e87fc_slot0f = 0x800e87fc;
data_800e8568_slot0f = 0x800e8568;
data_800e8598_slot0f = 0x800e8598;
data_800e85b8_slot0f = 0x800e85b8;
data_800e85f8_slot0f = 0x800e85f8;
data_800e8608_slot0f = 0x800e8608;
data_800e8638_slot0f = 0x800e8638;
data_800e8648_slot0f = 0x800e8648;
data_800e8658_slot0f = 0x800e8658;
data_800e8668_slot0f = 0x800e8668;
data_800e8678_slot0f = 0x800e8678;
data_800e8688_slot0f = 0x800e8688;
data_800e8698_slot0f = 0x800e8698;
data_800e86a8_slot0f = 0x800e86a8;
data_800e86b8_slot0f = 0x800e86b8;
data_800e86c8_slot0f = 0x800e86c8;
data_800e8518_slot0f = 0x800e8518;

/* Added by src.u_slot0f_2348. */
data_800e8548_slot0f = 0x800e8548;
data_800e8578_slot0f = 0x800e8578;
data_800e85c8_slot0f = 0x800e85c8;
data_800e8558_slot0f = 0x800e8558;
data_800e85a8_slot0f = 0x800e85a8;
data_800e8588_slot0f = 0x800e8588;
data_800e8618_slot0f = 0x800e8618;
data_800e86d8_slot0f = 0x800e86d8;
data_800df2f8_slot0f = 0x800df2f8;
data_800e971c_slot0f = 0x800e971c;
data_800f0200_slot0f = 0x800f0200;
data_800f0204_slot0f = 0x800f0204;
data_800f0208_slot0f = 0x800f0208;
data_800f8578_slot0f = 0x800f8578;
data_800e929c_slot0f = 0x800e929c;
data_800e9724_slot0f = 0x800e9724;

/* Added by src.u_slot0f_2964. */
data_800f8554_slot0f = 0x800f8554;
data_800f8555_slot0f = 0x800f8555;
data_800f8556_slot0f = 0x800f8556;
data_800f8557_slot0f = 0x800f8557;
data_800f8558_slot0f = 0x800f8558;
data_800f8559_slot0f = 0x800f8559;
data_800f855a_slot0f = 0x800f855a;
data_800f855b_slot0f = 0x800f855b;
data_800f855c_slot0f = 0x800f855c;
data_800e9540_slot0f = 0x800e9540;
data_800e95c0_slot0f = 0x800e95c0;
data_800e9600_slot0f = 0x800e9600;
data_800e94b0_slot0f = 0x800e94b0;
data_800e94ec_slot0f = 0x800e94ec;
data_800e950c_slot0f = 0x800e950c;
data_800e951c_slot0f = 0x800e951c;
data_800e952c_slot0f = 0x800e952c;
data_800e9688_slot0f = 0x800e9688;
data_800e9698_slot0f = 0x800e9698;
data_800e96a8_slot0f = 0x800e96a8;
data_800e96b8_slot0f = 0x800e96b8;
data_800e96c8_slot0f = 0x800e96c8;
data_800e96d8_slot0f = 0x800e96d8;

/* Added by src.u_slot0f_2fcc. */
data_800df32c_slot0f = 0x800df32c;
data_800f8564_slot0f = 0x800f8564;
data_8016e68a = 0x8016e68a;
data_8016e68c = 0x8016e68c;
data_8016e68f = 0x8016e68f;
data_8016e690 = 0x8016e690;
data_800f855e_slot0f = 0x800f855e;
data_800e934c_slot0f = 0x800e934c;
data_800e96dc_slot0f = 0x800e96dc;
data_800e96e8_slot0f = 0x800e96e8;
data_800f855d_slot0f = 0x800f855d;
data_800e9074_slot0f = 0x800e9074;
data_800e8808_slot0f = 0x800e8808;
data_800e9008_slot0f = 0x800e9008;
data_800e9014_slot0f = 0x800e9014;
data_800e9024_slot0f = 0x800e9024;
data_800e9044_slot0f = 0x800e9044;

/* Added by src.u_slot0f_3478. */
data_800e945c_slot0f = 0x800e945c;
data_800e928c_slot0f = 0x800e928c;
data_800e953c_slot0f = 0x800e953c;
data_800e92a7_slot0f = 0x800e92a7;

/* Added by src.u_slot0f_3a14. */
data_800f020c_slot0f = 0x800f020c;
data_800f022c_slot0f = 0x800f022c;
data_800e99a0_slot0f = 0x800e99a0;
data_800e9783_slot0f = 0x800e9783;
data_800e981b_slot0f = 0x800e981b;
data_800f024c_slot0f = 0x800f024c;
data_800f0250_slot0f = 0x800f0250;
data_800f0254_slot0f = 0x800f0254;
data_800f0258_slot0f = 0x800f0258;
data_800f025c_slot0f = 0x800f025c;
data_800f0260_slot0f = 0x800f0260;
data_8016e674 = 0x8016e674;
data_8016e67c = 0x8016e67c;
data_800e98e3_slot0f = 0x800e98e3;
data_800e990f_slot0f = 0x800e990f;

/* Added by src.u_slot0f_3da0. */
data_800e9750_slot0f = 0x800e9750;
data_800e9938_slot0f = 0x800e9938;
data_800e9948_slot0f = 0x800e9948;
data_800e9958_slot0f = 0x800e9958;
data_800e9968_slot0f = 0x800e9968;
data_800e9978_slot0f = 0x800e9978;
data_800e9988_slot0f = 0x800e9988;
data_800e98e2_slot0f = 0x800e98e2;
data_800e990e_slot0f = 0x800e990e;
data_800e9998_slot0f = 0x800e9998;
data_800f856c_slot0f = 0x800f856c;

/* Added by src.u_slot0f_4800. */
data_800ebbf0_slot0f = 0x800ebbf0;
data_800ebbfc_slot0f = 0x800ebbfc;
data_800f0264_slot0f = 0x800f0264;
data_800f7264_slot0f = 0x800f7264;
data_800f7268_slot0f = 0x800f7268;
data_800e99b0_slot0f = 0x800e99b0;

/* Added by src.u_slot0f_4db4. */
data_800ea9b0_slot0f = 0x800ea9b0;
data_800eb970_slot0f = 0x800eb970;
data_800ebc08_slot0f = 0x800ebc08;
data_800f726c_slot0f = 0x800f726c;
data_800f728c_slot0f = 0x800f728c;
data_800f72ac_slot0f = 0x800f72ac;
data_800f752c_slot0f = 0x800f752c;
data_800f77ac_slot0f = 0x800f77ac;
data_800f77b0_slot0f = 0x800f77b0;

/* Added by src.u_slot0f_53ec. */
data_800ebc48_slot0f = 0x800ebc48;
data_800ebc68_slot0f = 0x800ebc68;
data_801903c8 = 0x801903c8;
data_801903ca = 0x801903ca;

/* Added by src.u_slot0f_5978. */
data_800ebc94_slot0f = 0x800ebc94;
data_800ebca0_slot0f = 0x800ebca0;

/* Added by src.u_slot0f_5ddc. */
data_800efa20_slot0f = 0x800efa20;
data_800efa30_slot0f = 0x800efa30;
data_800efad8_slot0f = 0x800efad8;
data_800ef978_slot0f = 0x800ef978;

/* Added by src.u_slot0f_6410. */
data_800ebcb8_slot0f = 0x800ebcb8;
data_800ef8d0_slot0f = 0x800ef8d0;
data_800efd8c_slot0f = 0x800efd8c;
data_800f7b94_slot0f = 0x800f7b94;
data_800f77b4_slot0f = 0x800f77b4;
data_800f78f4_slot0f = 0x800f78f4;
data_800f7a34_slot0f = 0x800f7a34;
data_800eff4c_slot0f = 0x800eff4c;
data_800efd30_slot0f = 0x800efd30;
data_800efd84_slot0f = 0x800efd84;
data_800efd88_slot0f = 0x800efd88;
data_800efef8_slot0f = 0x800efef8;
data_800efdc8_slot0f = 0x800efdc8;
data_800eff58_slot0f = 0x800eff58;

/* Added by src.u_slot0f_6a48. */
data_800f7b74_slot0f = 0x800f7b74;
data_800f7c94_slot0f = 0x800f7c94;
data_800f7cd4_slot0f = 0x800f7cd4;
data_800f7cf4_slot0f = 0x800f7cf4;
data_800eff60_slot0f = 0x800eff60;
data_800eff70_slot0f = 0x800eff70;
data_800eff78_slot0f = 0x800eff78;
data_800eff98_slot0f = 0x800eff98;
data_800eff9c_slot0f = 0x800eff9c;

/* Added by src.u_slot0f_7064. */
data_800f012c_slot0f = 0x800f012c;
data_800f013c_slot0f = 0x800f013c;
data_800f0164_slot0f = 0x800f0164;
data_800f018c_slot0f = 0x800f018c;
data_800f01a4_slot0f = 0x800f01a4;
data_800f01ac_slot0f = 0x800f01ac;
data_800f7d34_slot0f = 0x800f7d34;
data_800f7d38_slot0f = 0x800f7d38;
data_800f7d48_slot0f = 0x800f7d48;
data_800f7d4c_slot0f = 0x800f7d4c;
data_800f7d50_slot0f = 0x800f7d50;
data_800f7e90_slot0f = 0x800f7e90;

/* Added by src.u_slot0f_75ec. */
data_800f7d3c_slot0f = 0x800f7d3c;
data_800f7d3e_slot0f = 0x800f7d3e;
data_800f7d40_slot0f = 0x800f7d40;
data_800f7d41_slot0f = 0x800f7d41;
data_800f7d42_slot0f = 0x800f7d42;
data_800f7d43_slot0f = 0x800f7d43;
data_800f7d44_slot0f = 0x800f7d44;
data_800f7d80_slot0f = 0x800f7d80;
data_800f01bc_slot0f = 0x800f01bc;
data_800f01c4_slot0f = 0x800f01c4;
data_800f8020_slot0f = 0x800f8020;

/* Added by src.u_slot0f_7c20. */
data_800f7d70_slot0f = 0x800f7d70;
data_800f7fd0_slot0f = 0x800f7fd0;
data_800f7d60_slot0f = 0x800f7d60;
data_800f8070_slot0f = 0x800f8070;
data_800f00fc_slot0f = 0x800f00fc;
data_800f010c_slot0f = 0x800f010c;
data_800f011c_slot0f = 0x800f011c;
data_800f01cc_slot0f = 0x800f01cc;
data_800f01e0_slot0f = 0x800f01e0;

/* Added by src.u_slot0f_826c. */
data_800f01e8_slot0f = 0x800f01e8;

/* Added by src.u_slot0f_fc28x. */

/* Added by src.u_slot06_00_8000. */
func_801e8dc4_slot06_00 = 0x801e8dc4;
func_801e9080_slot06_00 = 0x801e9080;
data_801ec390_slot06_00 = 0x801ec390;
data_801e9f90_slot06_00 = 0x801e9f90;
data_801edc80_slot06_00 = 0x801edc80;
data_801f1790_slot06_00 = 0x801f1790;

/* Added by src.u_slot06_00_8544. */
data_801f3050_slot06_00 = 0x801f3050;
data_801eaf90_slot06_00 = 0x801eaf90;
data_801ebf90_slot06_00 = 0x801ebf90;

/* Added by src.u_slot06_00_8d84. */
data_801e9f50_slot06_00 = 0x801e9f50;
data_801f3010_slot06_00 = 0x801f3010;

/* Added by src.u_slot06_00_9080. */
data_801e9f80_slot06_00 = 0x801e9f80;
data_801e9f44_slot06_00 = 0x801e9f44;

/* Added by src.u_slot27_0000. */
func_8001188c_slot27 = 0x8001188c;
func_80015ca4_slot27 = 0x80015ca4;
func_8001791c_slot27 = 0x8001791c;
data_80017bc4_slot27 = 0x80017bc4;
data_80017bdc_slot27 = 0x80017bdc;
data_80029324_slot27 = 0x80029324;

/* Added by src.u_slot27_05ec. */
data_80017bf0_slot27 = 0x80017bf0;
data_80017c04_slot27 = 0x80017c04;
data_80017c10_slot27 = 0x80017c10;
data_8002f158_slot27 = 0x8002f158;

/* Added by src.u_slot27_1280. */
data_800267a4_slot27 = 0x800267a4;
data_800267b4_slot27 = 0x800267b4;
data_800267bc_slot27 = 0x800267bc;
data_80017c28_slot27 = 0x80017c28;
data_8001aa14_slot27 = 0x8001aa14;
data_800268b0_slot27 = 0x800268b0;

/* Added by src.u_slot27_188c. */
data_800267c4_slot27 = 0x800267c4;
data_80026908_slot27 = 0x80026908;
data_80026940_slot27 = 0x80026940;
data_80026950_slot27 = 0x80026950;
data_80026958_slot27 = 0x80026958;
data_80029338_slot27 = 0x80029338;

/* Added by src.u_slot27_1ea4. */
data_80026960_slot27 = 0x80026960;
data_80026968_slot27 = 0x80026968;
data_80026970_slot27 = 0x80026970;

/* Added by src.u_slot27_205c. */
data_8016e68e = 0x8016e68e;
data_801901ba = 0x801901ba;

/* Added by src.u_slot27_25f4. */
data_800269f4_slot27 = 0x800269f4;
data_80026988_slot27 = 0x80026988;
data_80026a08_slot27 = 0x80026a08;

/* Added by src.u_slot27_2ad0. */
data_80026a58_slot27 = 0x80026a58;

/* Added by src.u_slot27_3148. */
data_80026a7c_slot27 = 0x80026a7c;
data_80026a94_slot27 = 0x80026a94;
data_80026a9c_slot27 = 0x80026a9c;
data_80026a9e_slot27 = 0x80026a9e;

/* Added by src.u_slot27_369c. */
data_80026ec8_slot27 = 0x80026ec8;
data_80026aac_slot27 = 0x80026aac;
data_80026c30_slot27 = 0x80026c30;
data_80029328_slot27 = 0x80029328;
data_801a2b84 = 0x801a2b84;

/* Added by src.u_slot27_3cc4. */
data_80026ed0_slot27 = 0x80026ed0;
data_80026ee0_slot27 = 0x80026ee0;
data_80026ee8_slot27 = 0x80026ee8;
data_80027bc4_slot27 = 0x80027bc4;
data_80027c18_slot27 = 0x80027c18;

/* Added by src.u_slot27_4270. */
data_80026ef0_slot27 = 0x80026ef0;
data_80027c6c_slot27 = 0x80027c6c;
data_80027c7c_slot27 = 0x80027c7c;
data_80027cac_slot27 = 0x80027cac;
data_80027cbc_slot27 = 0x80027cbc;
data_80027ccc_slot27 = 0x80027ccc;
data_80027cdc_slot27 = 0x80027cdc;
data_80027cec_slot27 = 0x80027cec;

/* Added by src.u_slot27_48a4. */
data_80027cfc_slot27 = 0x80027cfc;
data_80027d0c_slot27 = 0x80027d0c;
data_800280bc_slot27 = 0x800280bc;
data_80027d14_slot27 = 0x80027d14;
data_80027fa8_slot27 = 0x80027fa8;
data_80028110_slot27 = 0x80028110;

/* Added by src.u_slot27_4f30. */
data_80028120_slot27 = 0x80028120;
data_80028128_slot27 = 0x80028128;
data_800282e8_slot27 = 0x800282e8;
data_800283c8_slot27 = 0x800283c8;
data_800284fc_slot27 = 0x800284fc;
data_80029348_slot27 = 0x80029348;

/* Added by src.u_slot27_5528. */
data_800284e0_slot27 = 0x800284e0;
data_800284f8_slot27 = 0x800284f8;
data_80028510_slot27 = 0x80028510;
data_8002851c_slot27 = 0x8002851c;
data_8002856c_slot27 = 0x8002856c;
data_80028aa0_slot27 = 0x80028aa0;
data_80028aac_slot27 = 0x80028aac;
data_80055f30 = 0x80055f30;
data_80055f34 = 0x80055f34;

/* Added by src.u_slot27_5ab4. */
table_80028a4c_slot27 = 0x80028a4c;
data_80028b38_slot27 = 0x80028b38;
data_80028abc_slot27 = 0x80028abc;
data_80028b0c_slot27 = 0x80028b0c;
data_800289b4_slot27 = 0x800289b4;
data_80028a74_slot27 = 0x80028a74;
data_8002b386_slot27 = 0x8002b386;
data_80028b88_slot27 = 0x80028b88;

/* Added by src.u_slot27_6634. */
data_80028b8c_slot27 = 0x80028b8c;
data_80028e68_slot27 = 0x80028e68;
data_80028e10_slot27 = 0x80028e10;
data_80028e74_slot27 = 0x80028e74;
data_80028e84_slot27 = 0x80028e84;
data_8002904c_slot27 = 0x8002904c;
data_80055eb2 = 0x80055eb2;
data_80055eb6 = 0x80055eb6;
data_8002b380_slot27 = 0x8002b380;

/* Added by src.u_slot27_6c38. */
data_80028e8c_slot27 = 0x80028e8c;
data_80028e94_slot27 = 0x80028e94;
data_80029064_slot27 = 0x80029064;
data_800292cc_slot27 = 0x800292cc;
data_8002b388_slot27 = 0x8002b388;
data_8002f0c8_slot27 = 0x8002f0c8;
data_800292d4_slot27 = 0x800292d4;
data_801a3fe4 = 0x801a3fe4;

/* Added by src.u_slot27_5e68x. */
data_801a2824 = 0x801a2824;
data_80028864_slot27 = 0x80028864;
data_801a3c24 = 0x801a3c24;

/* Added by src.u_slot06_01_8000. */
func_801e9f90_slot06_01 = 0x801e9f90;
data_80190542 = 0x80190542;
data_801eb9b4_slot06_01 = 0x801eb9b4;
data_801eedb4_slot06_01 = 0x801eedb4;
data_801ed9b4_slot06_01 = 0x801ed9b4;
data_801ee9b4_slot06_01 = 0x801ee9b4;
data_801f29dc_slot06_01 = 0x801f29dc;
data_801fa57c_slot06_01 = 0x801fa57c;
data_801f6b7c_slot06_01 = 0x801f6b7c;
data_801f94bc_slot06_01 = 0x801f94bc;

/* Added by src.u_slot06_01_9694. */
data_801ea930_slot06_01 = 0x801ea930;
data_801ea940_slot06_01 = 0x801ea940;
data_801ea94c_slot06_01 = 0x801ea94c;
data_801eb960_slot06_01 = 0x801eb960;
data_801eb98c_slot06_01 = 0x801eb98c;
data_801eb990_slot06_01 = 0x801eb990;
data_801eb994_slot06_01 = 0x801eb994;

/* Added by src.u_slot06_01_9ac0. */
data_801ea95c_slot06_01 = 0x801ea95c;
data_801ea964_slot06_01 = 0x801ea964;
data_801ea988_slot06_01 = 0x801ea988;
data_801ea998_slot06_01 = 0x801ea998;
data_801eb984_slot06_01 = 0x801eb984;
data_801eb9ac_slot06_01 = 0x801eb9ac;

/* Added by src.u_slot06_02_8000. */
func_801e99c8_slot06_02 = 0x801e99c8;
data_801e9f18_slot06_02 = 0x801e9f18;
data_801ef0e8_slot06_02 = 0x801ef0e8;
data_801f897c_slot06_02 = 0x801f897c;
data_801e9e74_slot06_02 = 0x801e9e74;
data_801eaf18_slot06_02 = 0x801eaf18;
data_801f1f48_slot06_02 = 0x801f1f48;

/* Added by src.u_slot06_02_93c4. */
data_801e9e7c_slot06_02 = 0x801e9e7c;
data_801e9e7e_slot06_02 = 0x801e9e7e;
data_801edf18_slot06_02 = 0x801edf18;
data_801ef0c8_slot06_02 = 0x801ef0c8;
data_801ef0d8_slot06_02 = 0x801ef0d8;
data_801f5cf8_slot06_02 = 0x801f5cf8;
data_801f8638_slot06_02 = 0x801f8638;
data_80190544 = 0x80190544;

/* Added by src.u_slot06_03_8000. */
func_801e9d04_slot06_03 = 0x801e9d04;
data_801e9fa4_slot06_03 = 0x801e9fa4;
data_801eb410_slot06_03 = 0x801eb410;
data_801ee810_slot06_03 = 0x801ee810;

/* Added by src.u_slot06_03_886c. */
data_801ec410_slot06_03 = 0x801ec410;
data_801ee410_slot06_03 = 0x801ee410;
data_801f0d40_slot06_03 = 0x801f0d40;
data_801f4c40_slot06_03 = 0x801f4c40;
data_801f62c0_slot06_03 = 0x801f62c0;
data_801f8004_slot06_03 = 0x801f8004;

/* Added by src.u_slot06_03_960c. */
data_801ea5f4_slot06_03 = 0x801ea5f4;
data_801ea604_slot06_03 = 0x801ea604;
data_801ea61c_slot06_03 = 0x801ea61c;
data_801ea67c_slot06_03 = 0x801ea67c;
data_801eb3e0_slot06_03 = 0x801eb3e0;
data_801ea680_slot06_03 = 0x801ea680;
data_801ea698_slot06_03 = 0x801ea698;
data_801ea2cc_slot06_03 = 0x801ea2cc;
data_801ea310_slot06_03 = 0x801ea310;
data_801ea324_slot06_03 = 0x801ea324;
data_801ea418_slot06_03 = 0x801ea418;
data_801ea53c_slot06_03 = 0x801ea53c;
data_801ea560_slot06_03 = 0x801ea560;
data_801ea294_slot06_03 = 0x801ea294;
data_801ea2a8_slot06_03 = 0x801ea2a8;
data_801ea6a8_slot06_03 = 0x801ea6a8;
data_801eb404_slot06_03 = 0x801eb404;
data_801ea6b8_slot06_03 = 0x801ea6b8;

/* Added by src.u_slot06_04_8000. */
func_801e99b4_slot06_04 = 0x801e99b4;
data_801eb348_slot06_04 = 0x801eb348;
data_801ec348_slot06_04 = 0x801ec348;
data_801eeb48_slot06_04 = 0x801eeb48;
data_801f03c8_slot06_04 = 0x801f03c8;
data_801f6ce0_slot06_04 = 0x801f6ce0;

/* Added by src.u_slot06_04_9194. */
data_801ed348_slot06_04 = 0x801ed348;
data_801f4ae0_slot06_04 = 0x801f4ae0;
data_801f6660_slot06_04 = 0x801f6660;
data_801e9ce0_slot06_04 = 0x801e9ce0;
data_801e9e6c_slot06_04 = 0x801e9e6c;
data_801e9e7c_slot06_04 = 0x801e9e7c;
data_801e9eac_slot06_04 = 0x801e9eac;
data_801eb300_slot06_04 = 0x801eb300;

/* Added by src.u_slot06_05_8000. */
func_801e8dc8_slot06_05 = 0x801e8dc8;
data_801e9938_slot06_05 = 0x801e9938;
data_801ea938_slot06_05 = 0x801ea938;
data_801eb938_slot06_05 = 0x801eb938;
data_801ec3b4_slot06_05 = 0x801ec3b4;
data_801edf0c_slot06_05 = 0x801edf0c;
data_801f30a4_slot06_05 = 0x801f30a4;
data_801f4aa4_slot06_05 = 0x801f4aa4;

/* Added by src.u_slot06_05_9110. */
data_801ec344_slot06_05 = 0x801ec344;
data_801ec378_slot06_05 = 0x801ec378;
data_801ec388_slot06_05 = 0x801ec388;
data_801ec398_slot06_05 = 0x801ec398;
data_801ec3a8_slot06_05 = 0x801ec3a8;

/* Added by src.u_slot06_06_8000. */
func_801e9798_slot06_06 = 0x801e9798;
data_801ea398_slot06_06 = 0x801ea398;
data_801ec398_slot06_06 = 0x801ec398;
data_801ee288_slot06_06 = 0x801ee288;
data_801f8458_slot06_06 = 0x801f8458;
data_801f25b0_slot06_06 = 0x801f25b0;

/* Added by src.u_slot06_06_92b0. */
data_801ed398_slot06_06 = 0x801ed398;
data_801f3618_slot06_06 = 0x801f3618;
data_801f6d18_slot06_06 = 0x801f6d18;
data_801ee190_slot06_06 = 0x801ee190;
data_801ee1c8_slot06_06 = 0x801ee1c8;
data_801ee278_slot06_06 = 0x801ee278;
data_801f83d8_slot06_06 = 0x801f83d8;

/* Added by src.u_slot06_07_8000. */
func_801e98dc_slot06_07 = 0x801e98dc;
func_801e9970_slot06_07 = 0x801e9970;
data_801eaed0_slot06_07 = 0x801eaed0;
data_801ee22c_slot06_07 = 0x801ee22c;
data_801f6384_slot06_07 = 0x801f6384;
data_801ebed0_slot06_07 = 0x801ebed0;
data_801ef09c_slot06_07 = 0x801ef09c;
data_801eced0_slot06_07 = 0x801eced0;
data_801f33c4_slot06_07 = 0x801f33c4;
data_801f4f44_slot06_07 = 0x801f4f44;

/* Added by src.u_slot06_07_96c0. */
data_801eaec0_slot06_07 = 0x801eaec0;
data_801ee1ac_slot06_07 = 0x801ee1ac;

/* Added by src.u_slot06_07_9d04. */
data_801ee188_slot06_07 = 0x801ee188;
data_801ee18c_slot06_07 = 0x801ee18c;
data_801ee190_slot06_07 = 0x801ee190;
data_801ee1a8_slot06_07 = 0x801ee1a8;
data_801ee1bc_slot06_07 = 0x801ee1bc;
data_801ee1cc_slot06_07 = 0x801ee1cc;
data_801ee1dc_slot06_07 = 0x801ee1dc;
data_801ee1e8_slot06_07 = 0x801ee1e8;
data_801ee1f8_slot06_07 = 0x801ee1f8;
data_801ee208_slot06_07 = 0x801ee208;
data_801ee210_slot06_07 = 0x801ee210;
data_801ee218_slot06_07 = 0x801ee218;
data_801ee21c_slot06_07 = 0x801ee21c;

/* Added by src.u_slot06_08_8000. */
func_801e8bd8_slot06_08 = 0x801e8bd8;
func_801e9b54_slot06_08 = 0x801e9b54;
func_801ea3b4_slot06_08 = 0x801ea3b4;
data_801ea6d8_slot06_08 = 0x801ea6d8;
data_801eb6d8_slot06_08 = 0x801eb6d8;
data_801ec6d8_slot06_08 = 0x801ec6d8;
data_801ed4f4_slot06_08 = 0x801ed4f4;
data_801f8344_slot06_08 = 0x801f8344;
data_801f085c_slot06_08 = 0x801f085c;
data_801f2504_slot06_08 = 0x801f2504;
data_801f7784_slot06_08 = 0x801f7784;

/* Added by src.u_slot06_08_98d8. */
data_801ea6b8_slot06_08 = 0x801ea6b8;
data_801ea6c0_slot06_08 = 0x801ea6c0;
data_801ea6d0_slot06_08 = 0x801ea6d0;
data_801ed480_slot06_08 = 0x801ed480;
data_801ed4c4_slot06_08 = 0x801ed4c4;

/* Added by src.u_slot06_08_9f10. */
data_801ed498_slot06_08 = 0x801ed498;
data_801ed4b4_slot06_08 = 0x801ed4b4;
data_801ed4d4_slot06_08 = 0x801ed4d4;
data_801ed4e4_slot06_08 = 0x801ed4e4;

/* Added by src.u_slot06_09_8000. */
func_801e9840_slot06_09 = 0x801e9840;
data_801e9e78_slot06_09 = 0x801e9e78;
data_801e9e7a_slot06_09 = 0x801e9e7a;
data_801ea124_slot06_09 = 0x801ea124;
data_801eb124_slot06_09 = 0x801eb124;
data_801eca24_slot06_09 = 0x801eca24;
data_801efaec_slot06_09 = 0x801efaec;
data_801f6724_slot06_09 = 0x801f6724;

/* Added by src.u_slot06_09_9350. */
data_801ec124_slot06_09 = 0x801ec124;
data_801f34e4_slot06_09 = 0x801f34e4;
data_801f5064_slot06_09 = 0x801f5064;
data_801ec92c_slot06_09 = 0x801ec92c;
data_801ec940_slot06_09 = 0x801ec940;
data_801ec944_slot06_09 = 0x801ec944;
data_801ec954_slot06_09 = 0x801ec954;
data_801ec956_slot06_09 = 0x801ec956;
data_801ec958_slot06_09 = 0x801ec958;
data_801eca14_slot06_09 = 0x801eca14;

/* Added by src.u_slot06_0a_8000. */
func_801e8df0_slot06_0a = 0x801e8df0;
data_801e991c_slot06_0a = 0x801e991c;
data_801ed498_slot06_0a = 0x801ed498;
data_801f4df0_slot06_0a = 0x801f4df0;
data_801ea11c_slot06_0a = 0x801ea11c;
data_801efed0_slot06_0a = 0x801efed0;
data_801ec11c_slot06_0a = 0x801ec11c;
data_801f3eb0_slot06_0a = 0x801f3eb0;

/* Added by src.u_slot06_0a_90f0. */
data_801ed400_slot06_0a = 0x801ed400;
data_801ed444_slot06_0a = 0x801ed444;
data_801ed454_slot06_0a = 0x801ed454;
data_801ed460_slot06_0a = 0x801ed460;
data_801ed470_slot06_0a = 0x801ed470;
data_801ed480_slot06_0a = 0x801ed480;

/* Added by src.u_slot06_0b_8000. */
func_801e96fc_slot06_0b = 0x801e96fc;
data_801e9b7c_slot06_0b = 0x801e9b7c;
data_801eab7c_slot06_0b = 0x801eab7c;
data_801ebb7c_slot06_0b = 0x801ebb7c;
data_801ec138_slot06_0b = 0x801ec138;
data_801ecc98_slot06_0b = 0x801ecc98;
data_801f04d0_slot06_0b = 0x801f04d0;
data_801f3bd0_slot06_0b = 0x801f3bd0;
data_801f5290_slot06_0b = 0x801f5290;

/* Added by src.u_slot06_0c_8000. */
func_801e98c8_slot06_0c = 0x801e98c8;
func_801ea640_slot06_0c = 0x801ea640;
data_801ebc70_slot06_0c = 0x801ebc70;
data_801ee070_slot06_0c = 0x801ee070;
data_801ecc70_slot06_0c = 0x801ecc70;
data_801eef88_slot06_0c = 0x801eef88;
data_801f5bc0_slot06_0c = 0x801f5bc0;
data_801f2980_slot06_0c = 0x801f2980;
data_801edc70_slot06_0c = 0x801edc70;
data_801f4500_slot06_0c = 0x801f4500;

/* Added by src.u_slot06_0c_96cc. */
data_801eab58_slot06_0c = 0x801eab58;
data_801eab68_slot06_0c = 0x801eab68;
data_801eab78_slot06_0c = 0x801eab78;
data_801eab84_slot06_0c = 0x801eab84;
data_801eab98_slot06_0c = 0x801eab98;
data_801ebc1c_slot06_0c = 0x801ebc1c;

/* Added by src.u_slot06_0c_9d74. */
data_801eaba4_slot06_0c = 0x801eaba4;
data_801eabb8_slot06_0c = 0x801eabb8;
data_801eabc4_slot06_0c = 0x801eabc4;
data_801eabd4_slot06_0c = 0x801eabd4;
data_801eabd6_slot06_0c = 0x801eabd6;
data_801eabdc_slot06_0c = 0x801eabdc;
data_801eabe0_slot06_0c = 0x801eabe0;

/* Added by src.u_slot06_0d_8000. */
func_801e998c_slot06_0d = 0x801e998c;
func_801e9f20_slot06_0d = 0x801e9f20;
data_801eb1e4_slot06_0d = 0x801eb1e4;
data_801ed5e4_slot06_0d = 0x801ed5e4;
data_801ec1e4_slot06_0d = 0x801ec1e4;
data_801eef44_slot06_0d = 0x801eef44;
data_801f685c_slot06_0d = 0x801f685c;
data_801f365c_slot06_0d = 0x801f365c;
data_801ed1e4_slot06_0d = 0x801ed1e4;
data_801f51dc_slot06_0d = 0x801f51dc;

/* Added by src.u_slot06_0d_96bc. */
data_801ea520_slot06_0d = 0x801ea520;
data_801ea530_slot06_0d = 0x801ea530;
data_801ea540_slot06_0d = 0x801ea540;
data_801ea550_slot06_0d = 0x801ea550;
data_801ea558_slot06_0d = 0x801ea558;
data_801eb1b0_slot06_0d = 0x801eb1b0;

/* Added by src.u_slot06_0e_8000. */
func_801e9738_slot06_0e = 0x801e9738;
func_801e9c80_slot06_0e = 0x801e9c80;
func_801e9d14_slot06_0e = 0x801e9d14;
data_801eb8ec_slot06_0e = 0x801eb8ec;
data_801ef554_slot06_0e = 0x801ef554;
data_801f8478_slot06_0e = 0x801f8478;
func_801e84cc_slot06_0e = 0x801e84cc;

/* Added by src.u_slot06_0e_8748. */
data_801ec8ec_slot06_0e = 0x801ec8ec;
data_801ed8ec_slot06_0e = 0x801ed8ec;
data_801f22d4_slot06_0e = 0x801f22d4;
data_801f3df4_slot06_0e = 0x801f3df4;
data_801f6734_slot06_0e = 0x801f6734;
data_801eac90_slot06_0e = 0x801eac90;

/* Added by src.u_slot06_0e_99ac. */
data_801eb8dc_slot06_0e = 0x801eb8dc;

/* Added by src.u_slot06_0e_a138. */
data_801ef40c_slot06_0e = 0x801ef40c;
data_801ef410_slot06_0e = 0x801ef410;
data_801ef414_slot06_0e = 0x801ef414;
data_801ef424_slot06_0e = 0x801ef424;
data_801ef42c_slot06_0e = 0x801ef42c;
data_801ef430_slot06_0e = 0x801ef430;
data_801ef434_slot06_0e = 0x801ef434;
data_801ef444_slot06_0e = 0x801ef444;
data_801ef44c_slot06_0e = 0x801ef44c;
data_801ef45c_slot06_0e = 0x801ef45c;
data_801ef464_slot06_0e = 0x801ef464;
data_801ef474_slot06_0e = 0x801ef474;
data_801ef484_slot06_0e = 0x801ef484;

/* Added by src.u_slot06_0e_a87c. */
data_801ef418_slot06_0e = 0x801ef418;
data_801ef4b4_slot06_0e = 0x801ef4b4;
data_801ef4bc_slot06_0e = 0x801ef4bc;
data_801ef4c4_slot06_0e = 0x801ef4c4;
data_801ef4d4_slot06_0e = 0x801ef4d4;
data_801f8474_slot06_0e = 0x801f8474;
data_801fb1f8_slot06_0e = 0x801fb1f8;

/* Added by src.u_slot06_0f_8000. */
func_801e9bb0_slot06_0f = 0x801e9bb0;
data_801ecbc4_slot06_0f = 0x801ecbc4;
data_801edbc4_slot06_0f = 0x801edbc4;
data_801ef004_slot06_0f = 0x801ef004;
data_801f1bfc_slot06_0f = 0x801f1bfc;
data_801f8364_slot06_0f = 0x801f8364;

/* Added by src.u_slot06_0f_91dc. */
data_801eebc4_slot06_0f = 0x801eebc4;
data_801f46a4_slot06_0f = 0x801f46a4;
data_801f6fe4_slot06_0f = 0x801f6fe4;
data_801ecba4_slot06_0f = 0x801ecba4;
data_801eb4a0_slot06_0f = 0x801eb4a0;
data_801eb4b0_slot06_0f = 0x801eb4b0;
data_801eb4b8_slot06_0f = 0x801eb4b8;
data_801eb4c0_slot06_0f = 0x801eb4c0;
data_801eb4c8_slot06_0f = 0x801eb4c8;
data_801eb4d8_slot06_0f = 0x801eb4d8;
data_801eb4e0_slot06_0f = 0x801eb4e0;
data_801eb4e4_slot06_0f = 0x801eb4e4;

/* Added by src.u_slot06_0f_9ed0. */
data_801eefc4_slot06_0f = 0x801eefc4;
data_801eefd4_slot06_0f = 0x801eefd4;
data_801eefe4_slot06_0f = 0x801eefe4;
data_801eefec_slot06_0f = 0x801eefec;
data_801eeff4_slot06_0f = 0x801eeff4;

/* Added by src.u_slot06_0f_a618. */
data_801eeffc_slot06_0f = 0x801eeffc;
data_1f8000b4 = 0x1f8000b4;
data_1f800164 = 0x1f800164;

/* Added by src.u_slot06_10_8000. */
func_801e90a8_slot06_10 = 0x801e90a8;
func_801e9ef4_slot06_10 = 0x801e9ef4;
data_801ebc10_slot06_10 = 0x801ebc10;
data_801eec10_slot06_10 = 0x801eec10;
data_801f0ed8_slot06_10 = 0x801f0ed8;
data_801f6800_slot06_10 = 0x801f6800;
data_801ecc10_slot06_10 = 0x801ecc10;
data_801edc10_slot06_10 = 0x801edc10;
data_801f50b0_slot06_10 = 0x801f50b0;

/* Added by src.u_slot06_10_8e18. */
data_801ea868_slot06_10 = 0x801ea868;
data_801ea8a0_slot06_10 = 0x801ea8a0;
data_801ea8d8_slot06_10 = 0x801ea8d8;
data_801ea944_slot06_10 = 0x801ea944;
data_801ea958_slot06_10 = 0x801ea958;
data_801ea968_slot06_10 = 0x801ea968;
data_801ea978_slot06_10 = 0x801ea978;
data_801ea988_slot06_10 = 0x801ea988;
data_801ebbc8_slot06_10 = 0x801ebbc8;

/* Added by src.u_slot06_10_9564. */
data_801ea998_slot06_10 = 0x801ea998;
data_801ea9a8_slot06_10 = 0x801ea9a8;
data_801ea9b4_slot06_10 = 0x801ea9b4;
data_801ea9c4_slot06_10 = 0x801ea9c4;
data_801ebbc4_slot06_10 = 0x801ebbc4;
data_801f67f0_slot06_10 = 0x801f67f0;
data_801f67f4_slot06_10 = 0x801f67f4;

/* Added by src.u_slot06_10_9c50. */
data_801ebbb8_slot06_10 = 0x801ebbb8;
data_801ea9d0_slot06_10 = 0x801ea9d0;

/* Added by src.u_slot06_11_8000. */
func_801e8fec_slot06_11 = 0x801e8fec;
data_801e9dc0_slot06_11 = 0x801e9dc0;
data_801eadc0_slot06_11 = 0x801eadc0;
data_801ecdc0_slot06_11 = 0x801ecdc0;
data_801ee328_slot06_11 = 0x801ee328;
data_801efe80_slot06_11 = 0x801efe80;
data_801f620c_slot06_11 = 0x801f620c;
data_801f5600_slot06_11 = 0x801f5600;

/* Added by src.u_slot06_11_8d24. */
data_801e9da0_slot06_11 = 0x801e9da0;
data_801e9db0_slot06_11 = 0x801e9db0;
data_801e9d98_slot06_11 = 0x801e9d98;

/* Added by src.u_slot06_11_92c4. */
data_801ee14c_slot06_11 = 0x801ee14c;
data_801ee160_slot06_11 = 0x801ee160;
data_801ee170_slot06_11 = 0x801ee170;
data_801ee180_slot06_11 = 0x801ee180;
data_801ee188_slot06_11 = 0x801ee188;
data_801ee288_slot06_11 = 0x801ee288;
data_801ee290_slot06_11 = 0x801ee290;
data_801ee2f0_slot06_11 = 0x801ee2f0;
data_801ee300_slot06_11 = 0x801ee300;
data_801ee314_slot06_11 = 0x801ee314;
data_801f6200_slot06_11 = 0x801f6200;
data_801f6204_slot06_11 = 0x801f6204;
data_801f6208_slot06_11 = 0x801f6208;

/* Added by src.u_slot06_12_8000. */
func_801e9114_slot06_12 = 0x801e9114;
data_801ef340_slot06_12 = 0x801ef340;
data_801f6070_slot06_12 = 0x801f6070;
data_801f11e0_slot06_12 = 0x801f11e0;
data_801ea52c_slot06_12 = 0x801ea52c;
data_801eb52c_slot06_12 = 0x801eb52c;
data_801e9ea8_slot06_12 = 0x801e9ea8;
data_801e9eaa_slot06_12 = 0x801e9eaa;

/* Added by src.u_slot06_12_8c44. */
data_801edd2c_slot06_12 = 0x801edd2c;
data_801f4cf0_slot06_12 = 0x801f4cf0;

/* Added by src.u_slot06_12_9450. */
data_801eee74_slot06_12 = 0x801eee74;
data_801eeec0_slot06_12 = 0x801eeec0;
data_801eeed0_slot06_12 = 0x801eeed0;
data_801eeed8_slot06_12 = 0x801eeed8;
data_801eeee0_slot06_12 = 0x801eeee0;
data_801eeef0_slot06_12 = 0x801eeef0;
data_801ef300_slot06_12 = 0x801ef300;

/* Added by src.u_slot06_12_9b1c. */
data_801904cc = 0x801904cc;
data_801ef310_slot06_12 = 0x801ef310;
data_801ef320_slot06_12 = 0x801ef320;
data_801ef330_slot06_12 = 0x801ef330;
data_801eef00_slot06_12 = 0x801eef00;
data_801ef2f4_slot06_12 = 0x801ef2f4;

/* Added by src.u_slot06_13_8000. */
data_801e8db8_slot06_13 = 0x801e8db8;
data_801e95b8_slot06_13 = 0x801e95b8;
data_801ea5b8_slot06_13 = 0x801ea5b8;
data_801eb5b8_slot06_13 = 0x801eb5b8;
data_801eec68_slot06_13 = 0x801eec68;
data_801f1710_slot06_13 = 0x801f1710;

/* Added by src.u_slot06_07_8128x. */
func_801e9d04_slot06_07 = 0x801e9d04;

/* Added by src.u_slot04_0c_0000. */
data_801bd828_slot04_0c = 0x801bd828;
data_801bd8c8_slot04_0c = 0x801bd8c8;
data_801bd8cc_slot04_0c = 0x801bd8cc;
data_801bd8d8_slot04_0c = 0x801bd8d8;
data_801bd8dc_slot04_0c = 0x801bd8dc;
data_801bd8e4_slot04_0c = 0x801bd8e4;
data_801bd8f0_slot04_0c = 0x801bd8f0;
data_801b4824_slot04_0c = 0x801b4824;
data_801b5bf4_slot04_0c = 0x801b5bf4;

/* Added by src.u_slot04_0c_0eb0. */
data_801a2944 = 0x801a2944;
data_801bd8f8_slot04_0c = 0x801bd8f8;
data_801bd92c_slot04_0c = 0x801bd92c;
data_801bd960_slot04_0c = 0x801bd960;
data_801bd974_slot04_0c = 0x801bd974;
data_801bd994_slot04_0c = 0x801bd994;
data_801bd9c4_slot04_0c = 0x801bd9c4;

/* Added by src.u_slot04_0c_161c. */
data_801bd9d0_slot04_0c = 0x801bd9d0;
data_801bd9e0_slot04_0c = 0x801bd9e0;
data_801bda10_slot04_0c = 0x801bda10;
data_801bda20_slot04_0c = 0x801bda20;

/* Added by src.u_slot04_0c_1e10. */
data_801bda28_slot04_0c = 0x801bda28;
data_801bda44_slot04_0c = 0x801bda44;
data_801bda50_slot04_0c = 0x801bda50;
data_801bda54_slot04_0c = 0x801bda54;
data_801bda5c_slot04_0c = 0x801bda5c;

/* Added by src.u_slot04_0c_2638. */
data_801bda6c_slot04_0c = 0x801bda6c;
data_801bda74_slot04_0c = 0x801bda74;
data_801bda84_slot04_0c = 0x801bda84;
data_801bdaa4_slot04_0c = 0x801bdaa4;
data_801bdaa8_slot04_0c = 0x801bdaa8;

/* Added by src.u_slot04_0c_2e34. */
data_801bd7fc_slot04_0c = 0x801bd7fc;
data_801bdab4_slot04_0c = 0x801bdab4;
data_801bdafc_slot04_0c = 0x801bdafc;
data_801bdb04_slot04_0c = 0x801bdb04;
data_801bdb0c_slot04_0c = 0x801bdb0c;
data_801bdb18_slot04_0c = 0x801bdb18;

/* Added by src.u_slot04_0c_3600. */
data_801bdb28_slot04_0c = 0x801bdb28;
data_801bdb48_slot04_0c = 0x801bdb48;
data_801bdb68_slot04_0c = 0x801bdb68;
data_801bdb88_slot04_0c = 0x801bdb88;
data_801be104_slot04_0c = 0x801be104;
data_801be19c_slot04_0c = 0x801be19c;
data_801be1ac_slot04_0c = 0x801be1ac;
data_801be1b4_slot04_0c = 0x801be1b4;
data_801be1bc_slot04_0c = 0x801be1bc;
data_801be1d0_slot04_0c = 0x801be1d0;
data_801be1d8_slot04_0c = 0x801be1d8;

/* Added by src.u_slot04_0c_3e10. */
data_801be124_slot04_0c = 0x801be124;
data_801be150_slot04_0c = 0x801be150;
data_801be188_slot04_0c = 0x801be188;
data_801be1e0_slot04_0c = 0x801be1e0;
data_801be1f0_slot04_0c = 0x801be1f0;
data_801be1f8_slot04_0c = 0x801be1f8;
data_801be204_slot04_0c = 0x801be204;
data_801be20c_slot04_0c = 0x801be20c;
data_801be218_slot04_0c = 0x801be218;

/* Added by src.u_slot04_0d_0000. */
data_801b6afc_slot04_0d = 0x801b6afc;
data_801b9910_slot04_0d = 0x801b9910;
data_801c2894_slot04_0d = 0x801c2894;
data_801c2934_slot04_0d = 0x801c2934;
data_801c2940_slot04_0d = 0x801c2940;
data_801c294c_slot04_0d = 0x801c294c;
data_801c2954_slot04_0d = 0x801c2954;
data_801c2960_slot04_0d = 0x801c2960;
data_801c296c_slot04_0d = 0x801c296c;

/* Added by src.u_slot04_0d_0820. */
data_801c2978_slot04_0d = 0x801c2978;

/* Added by src.u_slot04_0d_1048. */
data_801c2764_slot04_0d = 0x801c2764;
data_801c2768_slot04_0d = 0x801c2768;
data_801c2770_slot04_0d = 0x801c2770;
data_801c2980_slot04_0d = 0x801c2980;
data_801c29a8_slot04_0d = 0x801c29a8;
data_801c29d0_slot04_0d = 0x801c29d0;
data_801c29f8_slot04_0d = 0x801c29f8;
data_801c2a08_slot04_0d = 0x801c2a08;
data_801c2a18_slot04_0d = 0x801c2a18;

/* Added by src.u_slot04_0d_182c. */
data_801c2774_slot04_0d = 0x801c2774;
data_801c2a28_slot04_0d = 0x801c2a28;
data_801c2a38_slot04_0d = 0x801c2a38;
data_801c2a40_slot04_0d = 0x801c2a40;

/* Added by src.u_slot04_0d_2018. */
data_801c27d4_slot04_0d = 0x801c27d4;
data_801c2804_slot04_0d = 0x801c2804;
data_801c2a50_slot04_0d = 0x801c2a50;
data_801c2a60_slot04_0d = 0x801c2a60;
data_801c2a74_slot04_0d = 0x801c2a74;

/* Added by src.u_slot04_0d_2794. */
data_801c2808_slot04_0d = 0x801c2808;
data_801c2820_slot04_0d = 0x801c2820;
data_801c2824_slot04_0d = 0x801c2824;
data_801c2a90_slot04_0d = 0x801c2a90;
data_801c2aa4_slot04_0d = 0x801c2aa4;
data_801c2aac_slot04_0d = 0x801c2aac;

/* Added by src.u_slot04_0d_2f4c. */
data_801c2ac8_slot04_0d = 0x801c2ac8;
data_801c2adc_slot04_0d = 0x801c2adc;

/* Added by src.u_slot04_0d_3778. */
data_801c2828_slot04_0d = 0x801c2828;
data_801c2848_slot04_0d = 0x801c2848;
data_801c2aec_slot04_0d = 0x801c2aec;
data_801c2c00_slot04_0d = 0x801c2c00;
data_801c35a0_slot04_0d = 0x801c35a0;
data_801c35a8_slot04_0d = 0x801c35a8;
data_801c35c0_slot04_0d = 0x801c35c0;
data_801c35cc_slot04_0d = 0x801c35cc;
data_801c35dc_slot04_0d = 0x801c35dc;
data_801c35f0_slot04_0d = 0x801c35f0;

/* Added by src.u_slot04_0d_3f7c. */
data_801c35f8_slot04_0d = 0x801c35f8;
data_801c3600_slot04_0d = 0x801c3600;
data_801c3610_slot04_0d = 0x801c3610;
data_801c361c_slot04_0d = 0x801c361c;
data_801c3624_slot04_0d = 0x801c3624;
data_801c3630_slot04_0d = 0x801c3630;
data_801c36fc_slot04_0d = 0x801c36fc;
data_801c2e60_slot04_0d = 0x801c2e60;
data_801c3584_slot04_0d = 0x801c3584;
data_801c318c_slot04_0d = 0x801c318c;
data_801c2624_slot04_0d = 0x801c2624;
data_801c363c_slot04_0d = 0x801c363c;

/* Added by src.u_slot04_0d_4758. */
data_801c3648_slot04_0d = 0x801c3648;
data_801c3654_slot04_0d = 0x801c3654;
data_801c36dc_slot04_0d = 0x801c36dc;
data_801c36ec_slot04_0d = 0x801c36ec;
data_801c0a60_slot04_0d = 0x801c0a60;

/* Added by src.u_slot04_10_0118. */
data_801b6e08_slot04_10 = 0x801b6e08;
data_801ba218_slot04_10 = 0x801ba218;
data_801c68bc_slot04_10 = 0x801c68bc;
data_801c695c_slot04_10 = 0x801c695c;
data_801c6964_slot04_10 = 0x801c6964;
data_801c696c_slot04_10 = 0x801c696c;
data_801c6974_slot04_10 = 0x801c6974;
data_801c697c_slot04_10 = 0x801c697c;

/* Added by src.u_slot04_10_094c. */
data_801c6988_slot04_10 = 0x801c6988;
data_801c6998_slot04_10 = 0x801c6998;
data_801c69a0_slot04_10 = 0x801c69a0;

/* Added by src.u_slot04_10_1158. */
data_801c69ac_slot04_10 = 0x801c69ac;
data_801c69c0_slot04_10 = 0x801c69c0;

/* Added by src.u_slot04_10_217c. */
data_801c69d4_slot04_10 = 0x801c69d4;
data_801c6a04_slot04_10 = 0x801c6a04;
data_801c6a14_slot04_10 = 0x801c6a14;
data_801c6a1c_slot04_10 = 0x801c6a1c;

/* Added by src.u_slot04_10_29ac. */
data_801c6a2c_slot04_10 = 0x801c6a2c;
data_801c6a34_slot04_10 = 0x801c6a34;
data_801c6a44_slot04_10 = 0x801c6a44;
data_801c6a54_slot04_10 = 0x801c6a54;

/* Added by src.u_slot04_10_30b4. */
data_801c6a64_slot04_10 = 0x801c6a64;

/* Added by src.u_slot04_10_3878. */
data_801c6a84_slot04_10 = 0x801c6a84;
data_801c6aa8_slot04_10 = 0x801c6aa8;

/* Added by src.u_slot04_10_4048. */
data_801c6ab0_slot04_10 = 0x801c6ab0;
data_801c6abc_slot04_10 = 0x801c6abc;
data_801c6adc_slot04_10 = 0x801c6adc;
data_801c6ae4_slot04_10 = 0x801c6ae4;
data_801c6b14_slot04_10 = 0x801c6b14;
data_801c6b3c_slot04_10 = 0x801c6b3c;
data_801c6b6c_slot04_10 = 0x801c6b6c;
data_801c6b9c_slot04_10 = 0x801c6b9c;
data_801c6ba8_slot04_10 = 0x801c6ba8;

/* Added by src.u_slot04_10_4844. */
data_801c6bb8_slot04_10 = 0x801c6bb8;
data_801c6bc0_slot04_10 = 0x801c6bc0;
data_801c6bf0_slot04_10 = 0x801c6bf0;
data_801c6c38_slot04_10 = 0x801c6c38;
data_801c6c68_slot04_10 = 0x801c6c68;

/* Added by src.u_slot04_10_5768. */
data_801c6c6c_slot04_10 = 0x801c6c6c;
data_801c6c9c_slot04_10 = 0x801c6c9c;
data_801c6ca4_slot04_10 = 0x801c6ca4;
data_801c6cb0_slot04_10 = 0x801c6cb0;

/* Added by src.u_slot04_10_5f64. */
data_801c6ce0_slot04_10 = 0x801c6ce0;
data_801c6ce8_slot04_10 = 0x801c6ce8;
data_801c6d18_slot04_10 = 0x801c6d18;
data_801c6d80_slot04_10 = 0x801c6d80;
data_801c6dbc_slot04_10 = 0x801c6dbc;
data_801c6dc8_slot04_10 = 0x801c6dc8;
data_801c6dd8_slot04_10 = 0x801c6dd8;
data_801c6e9c_slot04_10 = 0x801c6e9c;

/* Added by src.u_slot04_10_6708. */
data_801c6e70_slot04_10 = 0x801c6e70;

data_801b6b0c_slot04_15 = 0x801b6b0c;
data_801b9b20_slot04_15 = 0x801b9b20;
data_801c2f18_slot04_15 = 0x801c2f18;
data_801c2fb8_slot04_15 = 0x801c2fb8;
data_801c2fc4_slot04_15 = 0x801c2fc4;
data_801c2fd0_slot04_15 = 0x801c2fd0;
data_801c2fd8_slot04_15 = 0x801c2fd8;
data_801c2fe4_slot04_15 = 0x801c2fe4;
data_801c2ff0_slot04_15 = 0x801c2ff0;
data_801c2ffc_slot04_15 = 0x801c2ffc;
data_801c2de8_slot04_15 = 0x801c2de8;
data_801c3004_slot04_15 = 0x801c3004;
data_801c302c_slot04_15 = 0x801c302c;
data_801c3054_slot04_15 = 0x801c3054;
data_801c307c_slot04_15 = 0x801c307c;
data_801c308c_slot04_15 = 0x801c308c;
data_801c2dec_slot04_15 = 0x801c2dec;
data_801c2df4_slot04_15 = 0x801c2df4;
data_801c309c_slot04_15 = 0x801c309c;
data_801c30ac_slot04_15 = 0x801c30ac;
data_801c30bc_slot04_15 = 0x801c30bc;
data_801c30c4_slot04_15 = 0x801c30c4;
data_801c2df8_slot04_15 = 0x801c2df8;
data_801c2e58_slot04_15 = 0x801c2e58;
data_801c2e88_slot04_15 = 0x801c2e88;
data_801c30d4_slot04_15 = 0x801c30d4;
data_801c30e4_slot04_15 = 0x801c30e4;
data_801c30f8_slot04_15 = 0x801c30f8;
data_801c2e8c_slot04_15 = 0x801c2e8c;
data_801c2ea4_slot04_15 = 0x801c2ea4;
data_801c2ea8_slot04_15 = 0x801c2ea8;
data_801c3114_slot04_15 = 0x801c3114;
data_801c3128_slot04_15 = 0x801c3128;
data_801c3130_slot04_15 = 0x801c3130;
data_801c2eac_slot04_15 = 0x801c2eac;
data_801c2ecc_slot04_15 = 0x801c2ecc;
data_801c314c_slot04_15 = 0x801c314c;
data_801c3160_slot04_15 = 0x801c3160;
data_801c3170_slot04_15 = 0x801c3170;
data_801c3284_slot04_15 = 0x801c3284;
data_801c3c24_slot04_15 = 0x801c3c24;
data_801c3c2c_slot04_15 = 0x801c3c2c;
data_801c3c44_slot04_15 = 0x801c3c44;
data_801c3c4c_slot04_15 = 0x801c3c4c;
data_801c3c5c_slot04_15 = 0x801c3c5c;
data_801c3c70_slot04_15 = 0x801c3c70;
data_801c3c78_slot04_15 = 0x801c3c78;
data_801c3c80_slot04_15 = 0x801c3c80;
data_801c3c90_slot04_15 = 0x801c3c90;
data_801c3ca0_slot04_15 = 0x801c3ca0;
data_801c3d7c_slot04_15 = 0x801c3d7c;
data_801c34e4_slot04_15 = 0x801c34e4;
data_801c3c08_slot04_15 = 0x801c3c08;
data_801c3810_slot04_15 = 0x801c3810;
data_801c2ca8_slot04_15 = 0x801c2ca8;
data_801c0fc4_slot04_15 = 0x801c0fc4;
data_801c3cb0_slot04_15 = 0x801c3cb0;
data_801c3cbc_slot04_15 = 0x801c3cbc;
data_801c3cc4_slot04_15 = 0x801c3cc4;
data_801c3cd0_slot04_15 = 0x801c3cd0;
data_801c3cdc_slot04_15 = 0x801c3cdc;
data_801c3ce8_slot04_15 = 0x801c3ce8;
data_801c3cf4_slot04_15 = 0x801c3cf4;

/* Added by src.u_slot04_16_0118. */
data_801b6e40_slot04_16 = 0x801b6e40;
data_801bb5bc_slot04_16 = 0x801bb5bc;
data_801ca050_slot04_16 = 0x801ca050;
data_801ca0f0_slot04_16 = 0x801ca0f0;
data_801ca0f8_slot04_16 = 0x801ca0f8;
data_801ca100_slot04_16 = 0x801ca100;
data_801ca108_slot04_16 = 0x801ca108;
data_801ca110_slot04_16 = 0x801ca110;
data_801ca11c_slot04_16 = 0x801ca11c;

/* Added by src.u_slot04_16_0ef4. */
data_801ca12c_slot04_16 = 0x801ca12c;
data_801ca134_slot04_16 = 0x801ca134;
data_801ca140_slot04_16 = 0x801ca140;
data_801ca154_slot04_16 = 0x801ca154;

/* Added by src.u_slot04_16_1d38. */
data_801ca168_slot04_16 = 0x801ca168;
data_801ca198_slot04_16 = 0x801ca198;
data_801ca1a8_slot04_16 = 0x801ca1a8;
data_801ca1b0_slot04_16 = 0x801ca1b0;
data_801ca1c0_slot04_16 = 0x801ca1c0;
data_801ca1c8_slot04_16 = 0x801ca1c8;

/* Added by src.u_slot04_16_2bac. */
data_801ca1d8_slot04_16 = 0x801ca1d8;
data_801ca1e8_slot04_16 = 0x801ca1e8;
data_801ca1f8_slot04_16 = 0x801ca1f8;

/* Added by src.u_slot04_16_3ba4. */
data_801ca218_slot04_16 = 0x801ca218;
data_801ca23c_slot04_16 = 0x801ca23c;
data_801ca244_slot04_16 = 0x801ca244;
data_801ca250_slot04_16 = 0x801ca250;
data_801ca270_slot04_16 = 0x801ca270;
data_801ca278_slot04_16 = 0x801ca278;
data_801ca2a8_slot04_16 = 0x801ca2a8;
data_801ca2d0_slot04_16 = 0x801ca2d0;
data_801ca300_slot04_16 = 0x801ca300;
data_801ca330_slot04_16 = 0x801ca330;
data_801ca33c_slot04_16 = 0x801ca33c;
data_801ca34c_slot04_16 = 0x801ca34c;
data_801ca354_slot04_16 = 0x801ca354;

/* Added by src.u_slot04_16_49e4. */
data_801ca384_slot04_16 = 0x801ca384;
data_801ca3cc_slot04_16 = 0x801ca3cc;
data_801ca3fc_slot04_16 = 0x801ca3fc;

/* Added by src.u_slot04_16_57a0. */
data_801ca400_slot04_16 = 0x801ca400;
data_801ca430_slot04_16 = 0x801ca430;
data_801ca438_slot04_16 = 0x801ca438;
data_801ca444_slot04_16 = 0x801ca444;
data_801ca474_slot04_16 = 0x801ca474;
data_801ca47c_slot04_16 = 0x801ca47c;
data_801ca4ac_slot04_16 = 0x801ca4ac;
data_801ca514_slot04_16 = 0x801ca514;
data_801ca550_slot04_16 = 0x801ca550;
data_801ca55c_slot04_16 = 0x801ca55c;
data_801ca56c_slot04_16 = 0x801ca56c;

/* Added by src.u_slot04_16_660c. */
data_801ca604_slot04_16 = 0x801ca604;
data_801ca630_slot04_16 = 0x801ca630;

func_801b21f8_slot04_0f = 0x801b21f8;
func_801b2270_slot04_0f = 0x801b2270;
func_801b248c_slot04_0f = 0x801b248c;
data_801b6e9c_slot04_0f = 0x801b6e9c;
data_801b9aa8_slot04_0f = 0x801b9aa8;
data_801c58c0_slot04_0f = 0x801c58c0;
data_801c5960_slot04_0f = 0x801c5960;
data_801c5968_slot04_0f = 0x801c5968;
data_801c5974_slot04_0f = 0x801c5974;
data_801c5980_slot04_0f = 0x801c5980;
data_801c5994_slot04_0f = 0x801c5994;
data_801c5998_slot04_0f = 0x801c5998;
data_801c599c_slot04_0f = 0x801c599c;
data_801c59a0_slot04_0f = 0x801c59a0;
data_801c59a4_slot04_0f = 0x801c59a4;
data_801c59b0_slot04_0f = 0x801c59b0;
data_801c59b4_slot04_0f = 0x801c59b4;
data_801c59b8_slot04_0f = 0x801c59b8;
data_801c59c4_slot04_0f = 0x801c59c4;
data_801c59c8_slot04_0f = 0x801c59c8;
data_801c59cc_slot04_0f = 0x801c59cc;
data_801c59d8_slot04_0f = 0x801c59d8;
data_801c59dc_slot04_0f = 0x801c59dc;
data_801c59e0_slot04_0f = 0x801c59e0;
data_801c59f8_slot04_0f = 0x801c59f8;
data_801c5a00_slot04_0f = 0x801c5a00;
data_801c5a0c_slot04_0f = 0x801c5a0c;
data_801c5a18_slot04_0f = 0x801c5a18;
data_801c5a30_slot04_0f = 0x801c5a30;
data_801c5a60_slot04_0f = 0x801c5a60;
data_801c5a90_slot04_0f = 0x801c5a90;
data_801c5ac0_slot04_0f = 0x801c5ac0;
data_801c5ad4_slot04_0f = 0x801c5ad4;
data_801c5aec_slot04_0f = 0x801c5aec;
data_801c5af8_slot04_0f = 0x801c5af8;
data_801c5b08_slot04_0f = 0x801c5b08;
data_801c5b18_slot04_0f = 0x801c5b18;
data_801c5b28_slot04_0f = 0x801c5b28;
data_801c5b34_slot04_0f = 0x801c5b34;
data_801c5b44_slot04_0f = 0x801c5b44;
data_801c5b5c_slot04_0f = 0x801c5b5c;
data_801c5b60_slot04_0f = 0x801c5b60;
data_801c5b94_slot04_0f = 0x801c5b94;
data_801c5b98_slot04_0f = 0x801c5b98;
data_801c5ba0_slot04_0f = 0x801c5ba0;
data_801c5bb4_slot04_0f = 0x801c5bb4;
data_801c5bbc_slot04_0f = 0x801c5bbc;
data_801c5bc4_slot04_0f = 0x801c5bc4;
data_801c5bd4_slot04_0f = 0x801c5bd4;
data_801c5be4_slot04_0f = 0x801c5be4;
data_801c5bf0_slot04_0f = 0x801c5bf0;
data_801c5c0c_slot04_0f = 0x801c5c0c;
data_801c5c1c_slot04_0f = 0x801c5c1c;
data_801c54dc_slot04_0f = 0x801c54dc;
data_801c5e30_slot04_0f = 0x801c5e30;
data_801c5e38_slot04_0f = 0x801c5e38;
data_801c5e48_slot04_0f = 0x801c5e48;
data_801c5e54_slot04_0f = 0x801c5e54;
data_801c5e58_slot04_0f = 0x801c5e58;
data_801716b4 = 0x801716b4;
data_801c5e6c_slot04_0f = 0x801c5e6c;
data_801c5e74_slot04_0f = 0x801c5e74;
data_801c5e7c_slot04_0f = 0x801c5e7c;
data_801c5e88_slot04_0f = 0x801c5e88;
data_801c5e94_slot04_0f = 0x801c5e94;
data_801c605c_slot04_0f = 0x801c605c;
data_801c6064_slot04_0f = 0x801c6064;
data_801c6074_slot04_0f = 0x801c6074;
data_801c6088_slot04_0f = 0x801c6088;
data_801c6294_slot04_0f = 0x801c6294;
data_801c6144_slot04_0f = 0x801c6144;
data_801c6150_slot04_0f = 0x801c6150;
data_801c618c_slot04_0f = 0x801c618c;
data_801c62a4_slot04_0f = 0x801c62a4;
data_801c62b8_slot04_0f = 0x801c62b8;
data_801c62c4_slot04_0f = 0x801c62c4;
data_801c62d0_slot04_0f = 0x801c62d0;
data_801c62d8_slot04_0f = 0x801c62d8;
data_801c62e4_slot04_0f = 0x801c62e4;
data_801c62ec_slot04_0f = 0x801c62ec;
data_801c62f8_slot04_0f = 0x801c62f8;
data_801c6300_slot04_0f = 0x801c6300;
data_801c630c_slot04_0f = 0x801c630c;
data_801c6324_slot04_0f = 0x801c6324;
data_801c63b0_slot04_0f = 0x801c63b0;
data_801c63c0_slot04_0f = 0x801c63c0;
data_801c63cc_slot04_0f = 0x801c63cc;
data_1f8000b0 = 0x1f8000b0;
data_1f800160 = 0x1f800160;

func_801b24f0_slot04_11 = 0x801b24f0;
func_801b27b0_slot04_11 = 0x801b27b0;
func_801b34e4_slot04_11 = 0x801b34e4;
func_801b43e8_slot04_11 = 0x801b43e8;
data_801b744c_slot04_11 = 0x801b744c;
data_801b9e04_slot04_11 = 0x801b9e04;
data_801c7244_slot04_11 = 0x801c7244;
data_801c72e4_slot04_11 = 0x801c72e4;
data_801c72f4_slot04_11 = 0x801c72f4;
data_801c7304_slot04_11 = 0x801c7304;
data_801c7314_slot04_11 = 0x801c7314;
data_801c731c_slot04_11 = 0x801c731c;
data_801c7324_slot04_11 = 0x801c7324;
func_801b00c8_slot04_11 = 0x801b00c8;
data_801c732c_slot04_11 = 0x801c732c;
data_801c7354_slot04_11 = 0x801c7354;
data_801c7388_slot04_11 = 0x801c7388;
data_801c7398_slot04_11 = 0x801c7398;
data_801c739c_slot04_11 = 0x801c739c;
data_801c73a4_slot04_11 = 0x801c73a4;
data_801c73c0_slot04_11 = 0x801c73c0;
data_801c73f0_slot04_11 = 0x801c73f0;
data_801c73f8_slot04_11 = 0x801c73f8;
data_801c701c_slot04_11 = 0x801c701c;
data_801c709c_slot04_11 = 0x801c709c;
data_801c73fc_slot04_11 = 0x801c73fc;
data_801c7404_slot04_11 = 0x801c7404;
data_801c7414_slot04_11 = 0x801c7414;
data_801c7420_slot04_11 = 0x801c7420;
data_801c7444_slot04_11 = 0x801c7444;
data_801c744c_slot04_11 = 0x801c744c;
data_801c74b8_slot04_11 = 0x801c74b8;
data_801c74c8_slot04_11 = 0x801c74c8;
data_801c74dc_slot04_11 = 0x801c74dc;
data_801c74d0_slot04_11 = 0x801c74d0;
data_801c74f4_slot04_11 = 0x801c74f4;
data_801c74f8_slot04_11 = 0x801c74f8;
data_801c7514_slot04_11 = 0x801c7514;
data_801c7568_slot04_11 = 0x801c7568;
data_801c7590_slot04_11 = 0x801c7590;
data_801c75c0_slot04_11 = 0x801c75c0;
data_801c75d0_slot04_11 = 0x801c75d0;
data_801c75fc_slot04_11 = 0x801c75fc;
data_801c75fd_slot04_11 = 0x801c75fd;
data_801c7604_slot04_11 = 0x801c7604;
data_801c7798_slot04_11 = 0x801c7798;
data_801c77a8_slot04_11 = 0x801c77a8;
data_801c77b8_slot04_11 = 0x801c77b8;
data_801c7644_slot04_11 = 0x801c7644;
data_801c77c0_slot04_11 = 0x801c77c0;
data_801c77c8_slot04_11 = 0x801c77c8;
data_801c77d4_slot04_11 = 0x801c77d4;
data_801c77f4_slot04_11 = 0x801c77f4;
data_801c7804_slot04_11 = 0x801c7804;
data_801c780c_slot04_11 = 0x801c780c;
data_801c7818_slot04_11 = 0x801c7818;
data_801c7838_slot04_11 = 0x801c7838;
data_801c7844_slot04_11 = 0x801c7844;
data_801c7854_slot04_11 = 0x801c7854;
data_801c785c_slot04_11 = 0x801c785c;
data_801c7a10_slot04_11 = 0x801c7a10;
data_801c7a1c_slot04_11 = 0x801c7a1c;
data_801c7a24_slot04_11 = 0x801c7a24;
data_801c7a2c_slot04_11 = 0x801c7a2c;
data_801c7a34_slot04_11 = 0x801c7a34;
data_801c7a40_slot04_11 = 0x801c7a40;
data_801c7a48_slot04_11 = 0x801c7a48;
data_801c7a50_slot04_11 = 0x801c7a50;
data_801c7be0_slot04_11 = 0x801c7be0;
data_801c7b58_slot04_11 = 0x801c7b58;
data_801c7bd0_slot04_11 = 0x801c7bd0;
data_801c7bf0_slot04_11 = 0x801c7bf0;
data_801c7bfc_slot04_11 = 0x801c7bfc;
data_801c7c5c_slot04_11 = 0x801c7c5c;
data_801c7cc0_slot04_11 = 0x801c7cc0;
data_801c7cd0_slot04_11 = 0x801c7cd0;
data_801c7cf0_slot04_11 = 0x801c7cf0;
data_801c7d10_slot04_11 = 0x801c7d10;
data_801c7ce0_slot04_11 = 0x801c7ce0;
data_801b4de8_slot04_12 = 0x801b4de8;
data_801b7604_slot04_12 = 0x801b7604;
data_801c4948_slot04_12 = 0x801c4948;
data_801c49ec_slot04_12 = 0x801c49ec;
data_801c49fc_slot04_12 = 0x801c49fc;
data_801c4a0c_slot04_12 = 0x801c4a0c;
data_801c4a18_slot04_12 = 0x801c4a18;
func_801b0904_slot04_12 = 0x801b0904;
data_801c4a20_slot04_12 = 0x801c4a20;
data_801c4a40_slot04_12 = 0x801c4a40;
data_801c4a6c_slot04_12 = 0x801c4a6c;
data_801c4a98_slot04_12 = 0x801c4a98;
data_801c4aa8_slot04_12 = 0x801c4aa8;
data_801c4ab8_slot04_12 = 0x801c4ab8;
data_801c4abc_slot04_12 = 0x801c4abc;
data_801c4b3c_slot04_12 = 0x801c4b3c;
data_801c4b50_slot04_12 = 0x801c4b50;
data_801c4b80_slot04_12 = 0x801c4b80;
data_801c4b8c_slot04_12 = 0x801c4b8c;
data_801c4b9c_slot04_12 = 0x801c4b9c;
data_801c4bcc_slot04_12 = 0x801c4bcc;
data_801c4bdc_slot04_12 = 0x801c4bdc;
data_801c4c0c_slot04_12 = 0x801c4c0c;
data_801c4c10_slot04_12 = 0x801c4c10;
data_801c4c28_slot04_12 = 0x801c4c28;
data_801c4c58_slot04_12 = 0x801c4c58;
data_801c4c5c_slot04_12 = 0x801c4c5c;
data_801c4c74_slot04_12 = 0x801c4c74;
data_801c4c78_slot04_12 = 0x801c4c78;
data_801c4c80_slot04_12 = 0x801c4c80;
data_801c4c90_slot04_12 = 0x801c4c90;
data_801c4c9c_slot04_12 = 0x801c4c9c;
data_801c4da0_slot04_12 = 0x801c4da0;
data_801c4da8_slot04_12 = 0x801c4da8;
data_801c4db8_slot04_12 = 0x801c4db8;
data_801c4dc4_slot04_12 = 0x801c4dc4;
data_801c4dc8_slot04_12 = 0x801c4dc8;
data_801c4de8_slot04_12 = 0x801c4de8;
data_801c4dec_slot04_12 = 0x801c4dec;
data_801c4e28_slot04_12 = 0x801c4e28;
data_801c4e30_slot04_12 = 0x801c4e30;
data_801c4e40_slot04_12 = 0x801c4e40;
data_801c4e54_slot04_12 = 0x801c4e54;
data_801c4f08_slot04_12 = 0x801c4f08;
data_1f8000f0 = 0x1f8000f0;
data_1f8001a0 = 0x1f8001a0;
data_801c4e64_slot04_12 = 0x801c4e64;
data_801c4e70_slot04_12 = 0x801c4e70;
data_801c4ef8_slot04_12 = 0x801c4ef8;
func_801b24e0_slot04_17 = 0x801b24e0;
func_801b27a0_slot04_17 = 0x801b27a0;
func_801b34b8_slot04_17 = 0x801b34b8;
func_801b4374_slot04_17 = 0x801b4374;
data_801b778c_slot04_17 = 0x801b778c;
data_801baddc_slot04_17 = 0x801baddc;
data_801ce178_slot04_17 = 0x801ce178;
data_801ce218_slot04_17 = 0x801ce218;
data_801ce228_slot04_17 = 0x801ce228;
data_801ce238_slot04_17 = 0x801ce238;
data_801ce248_slot04_17 = 0x801ce248;
data_801ce250_slot04_17 = 0x801ce250;
data_801ce258_slot04_17 = 0x801ce258;
data_801cdf50_slot04_17 = 0x801cdf50;
data_801ce260_slot04_17 = 0x801ce260;
data_801ce288_slot04_17 = 0x801ce288;
data_801ce2bc_slot04_17 = 0x801ce2bc;
data_801ce2cc_slot04_17 = 0x801ce2cc;
data_801ce2d0_slot04_17 = 0x801ce2d0;
data_801ce2d8_slot04_17 = 0x801ce2d8;
data_801ce2f4_slot04_17 = 0x801ce2f4;
data_801ce324_slot04_17 = 0x801ce324;
data_801cdfd0_slot04_17 = 0x801cdfd0;
data_801ce32c_slot04_17 = 0x801ce32c;
data_801ce330_slot04_17 = 0x801ce330;
data_801ce338_slot04_17 = 0x801ce338;
data_801ce348_slot04_17 = 0x801ce348;
data_801ce354_slot04_17 = 0x801ce354;
data_801ce378_slot04_17 = 0x801ce378;
data_801ce380_slot04_17 = 0x801ce380;
data_801ce3ec_slot04_17 = 0x801ce3ec;
data_801ce3fc_slot04_17 = 0x801ce3fc;
data_801ce404_slot04_17 = 0x801ce404;
data_801ce410_slot04_17 = 0x801ce410;
data_801ce428_slot04_17 = 0x801ce428;
data_801ce42c_slot04_17 = 0x801ce42c;
data_801ce448_slot04_17 = 0x801ce448;
data_801ce49c_slot04_17 = 0x801ce49c;
data_801ce4c4_slot04_17 = 0x801ce4c4;
data_801ce4f4_slot04_17 = 0x801ce4f4;
data_801ce504_slot04_17 = 0x801ce504;
data_801ce530_slot04_17 = 0x801ce530;
data_801ce531_slot04_17 = 0x801ce531;
data_801ce538_slot04_17 = 0x801ce538;
data_801ce578_slot04_17 = 0x801ce578;
data_801ce6cc_slot04_17 = 0x801ce6cc;
data_801ce6dc_slot04_17 = 0x801ce6dc;
data_801ce6ec_slot04_17 = 0x801ce6ec;
data_801ce6f4_slot04_17 = 0x801ce6f4;
data_801ce6fc_slot04_17 = 0x801ce6fc;
data_801ce708_slot04_17 = 0x801ce708;
data_801ce728_slot04_17 = 0x801ce728;
data_801ce738_slot04_17 = 0x801ce738;
data_801ce740_slot04_17 = 0x801ce740;
data_801ce74c_slot04_17 = 0x801ce74c;
data_801ce76c_slot04_17 = 0x801ce76c;
data_801ce778_slot04_17 = 0x801ce778;
data_801ce788_slot04_17 = 0x801ce788;
data_801ce790_slot04_17 = 0x801ce790;
data_801ce844_slot04_17 = 0x801ce844;
data_801ce850_slot04_17 = 0x801ce850;
data_801ce858_slot04_17 = 0x801ce858;
data_801ce860_slot04_17 = 0x801ce860;
data_801ce8f0_slot04_17 = 0x801ce8f0;
data_801ce900_slot04_17 = 0x801ce900;
data_801cec3c_slot04_17 = 0x801cec3c;
data_801ce868_slot04_17 = 0x801ce868;
data_801ce8e0_slot04_17 = 0x801ce8e0;
data_801ce90c_slot04_17 = 0x801ce90c;
data_801ce96c_slot04_17 = 0x801ce96c;
data_801ce9d0_slot04_17 = 0x801ce9d0;
data_801ce9e0_slot04_17 = 0x801ce9e0;
data_801ce9f0_slot04_17 = 0x801ce9f0;
data_801cea00_slot04_17 = 0x801cea00;
data_801cea08_slot04_17 = 0x801cea08;
data_801cea10_slot04_17 = 0x801cea10;
data_801cea18_slot04_17 = 0x801cea18;
data_801cea24_slot04_17 = 0x801cea24;
data_801cec5c_slot04_17 = 0x801cec5c;
data_801cea2c_slot04_17 = 0x801cea2c;
data_801cea34_slot04_17 = 0x801cea34;
func_801b1b40_slot04_0e = 0x801b1b40;
func_801b2e8c_slot04_0e = 0x801b2e8c;
func_801b339c_slot04_0e = 0x801b339c;
func_801b4100_slot04_0e = 0x801b4100;
func_801b420c_slot04_0e = 0x801b420c;
func_801b4a3c_slot04_0e = 0x801b4a3c;
func_801b5e34_slot04_0e = 0x801b5e34;
data_801b6f60_slot04_0e = 0x801b6f60;
data_80079a38_slot2b = 0x80079a38;
data_801c5dc4_slot04_0e = 0x801c5dc4;
data_801c5e64_slot04_0e = 0x801c5e64;
data_801c5e70_slot04_0e = 0x801c5e70;
data_801c5e7c_slot04_0e = 0x801c5e7c;
data_801c5e8c_slot04_0e = 0x801c5e8c;
data_801c5e98_slot04_0e = 0x801c5e98;
data_801c5eb0_slot04_0e = 0x801c5eb0;
data_801c5ee0_slot04_0e = 0x801c5ee0;
data_801c5da4_slot04_0e = 0x801c5da4;
data_801c5efc_slot04_0e = 0x801c5efc;
data_801c5f0c_slot04_0e = 0x801c5f0c;
data_801c5f28_slot04_0e = 0x801c5f28;
data_801c5f34_slot04_0e = 0x801c5f34;
data_801c5f40_slot04_0e = 0x801c5f40;
data_801c5f4c_slot04_0e = 0x801c5f4c;
data_801c5f58_slot04_0e = 0x801c5f58;
data_801c5f64_slot04_0e = 0x801c5f64;
data_801c5f6c_slot04_0e = 0x801c5f6c;
data_801c5f78_slot04_0e = 0x801c5f78;
data_801c5f80_slot04_0e = 0x801c5f80;
data_801c5f90_slot04_0e = 0x801c5f90;
data_801c5fc8_slot04_0e = 0x801c5fc8;
data_801c6000_slot04_0e = 0x801c6000;
data_801c601c_slot04_0e = 0x801c601c;
data_801c6028_slot04_0e = 0x801c6028;
data_801c6034_slot04_0e = 0x801c6034;
data_801c6050_slot04_0e = 0x801c6050;
data_801c6064_slot04_0e = 0x801c6064;
data_801c606c_slot04_0e = 0x801c606c;
data_801c6074_slot04_0e = 0x801c6074;
data_801c6084_slot04_0e = 0x801c6084;
data_801c6088_slot04_0e = 0x801c6088;
data_801c609c_slot04_0e = 0x801c609c;
data_801c60a8_slot04_0e = 0x801c60a8;
data_801c60ac_slot04_0e = 0x801c60ac;
data_801c60b4_slot04_0e = 0x801c60b4;
data_801c62b4_slot04_0e = 0x801c62b4;
data_801c0a00_slot04_0e = 0x801c0a00;
data_801c60c0_slot04_0e = 0x801c60c0;
data_801c60e0_slot04_0e = 0x801c60e0;
data_801c60e8_slot04_0e = 0x801c60e8;
data_801aa618 = 0x801aa618;
data_801c60fc_slot04_0e = 0x801c60fc;
data_801c6108_slot04_0e = 0x801c6108;
data_801c6110_slot04_0e = 0x801c6110;
data_801c611c_slot04_0e = 0x801c611c;
data_801c612c_slot04_0e = 0x801c612c;
data_801c6140_slot04_0e = 0x801c6140;
data_801c6170_slot04_0e = 0x801c6170;
data_801c61ac_slot04_0e = 0x801c61ac;
data_801c61b8_slot04_0e = 0x801c61b8;
data_801c5d84_slot04_0e = 0x801c5d84;
data_801a2804 = 0x801a2804;
data_801c61d4_slot04_0e = 0x801c61d4;
data_801c6214_slot04_0e = 0x801c6214;
data_801c6254_slot04_0e = 0x801c6254;
data_801c6294_slot04_0e = 0x801c6294;

/* Added by src.u_slot04_14_0000. */
func_801b1a00_slot04_14 = 0x801b1a00;
func_801b81f8_slot04_14 = 0x801b81f8;
data_801b8580_slot04_14 = 0x801b8580;
data_801ba928_slot04_14 = 0x801ba928;
data_801c60c8_slot04_14 = 0x801c60c8;
data_801c6168_slot04_14 = 0x801c6168;
data_801c6170_slot04_14 = 0x801c6170;
data_801c617c_slot04_14 = 0x801c617c;
data_801c6184_slot04_14 = 0x801c6184;
data_801c6190_slot04_14 = 0x801c6190;
data_801c61a0_slot04_14 = 0x801c61a0;
data_801c61a8_slot04_14 = 0x801c61a8;

/* Added by src.u_slot04_14_16b4. */
data_801c61b0_slot04_14 = 0x801c61b0;
data_801c61f0_slot04_14 = 0x801c61f0;
data_801c6230_slot04_14 = 0x801c6230;
data_801c6270_slot04_14 = 0x801c6270;
data_801c62b8_slot04_14 = 0x801c62b8;
data_801c62c4_slot04_14 = 0x801c62c4;

/* Added by src.u_slot04_14_20a8. */
data_801c62d4_slot04_14 = 0x801c62d4;
data_801c62e0_slot04_14 = 0x801c62e0;
data_801c62ec_slot04_14 = 0x801c62ec;
data_801c62f4_slot04_14 = 0x801c62f4;
data_801c6304_slot04_14 = 0x801c6304;
data_801c6310_slot04_14 = 0x801c6310;

/* Added by src.u_slot04_14_2a80. */
data_801c6324_slot04_14 = 0x801c6324;
data_801c6334_slot04_14 = 0x801c6334;
data_801c6350_slot04_14 = 0x801c6350;
data_801c6354_slot04_14 = 0x801c6354;
data_801c6368_slot04_14 = 0x801c6368;
data_801c636c_slot04_14 = 0x801c636c;

/* Added by src.u_slot04_14_340c. */
data_801c6378_slot04_14 = 0x801c6378;
data_801c6390_slot04_14 = 0x801c6390;
data_801c6398_slot04_14 = 0x801c6398;
data_801c63a8_slot04_14 = 0x801c63a8;
func_80157380 = 0x80157380;

/* Added by src.u_slot04_14_3db8. */
data_801c63c4_slot04_14 = 0x801c63c4;
data_801c63cc_slot04_14 = 0x801c63cc;
data_801c63d4_slot04_14 = 0x801c63d4;
data_801c6420_slot04_14 = 0x801c6420;
data_801c646c_slot04_14 = 0x801c646c;

/* Added by src.u_slot04_14_4700. */
data_801c649c_slot04_14 = 0x801c649c;
data_801c64a8_slot04_14 = 0x801c64a8;
data_801c64b4_slot04_14 = 0x801c64b4;
data_801c64bc_slot04_14 = 0x801c64bc;
data_801c64ac_slot04_14 = 0x801c64ac;

/* Added by src.u_slot04_14_509c. */
data_801c64c0_slot04_14 = 0x801c64c0;
data_801c64cc_slot04_14 = 0x801c64cc;
data_801c64d4_slot04_14 = 0x801c64d4;
data_801c64dc_slot04_14 = 0x801c64dc;
data_801c64ec_slot04_14 = 0x801c64ec;

/* Added by src.u_slot04_14_5a90. */
data_801c64f8_slot04_14 = 0x801c64f8;
data_801c650c_slot04_14 = 0x801c650c;
data_801c651c_slot04_14 = 0x801c651c;
data_801c652c_slot04_14 = 0x801c652c;
data_801c6538_slot04_14 = 0x801c6538;
data_801c6548_slot04_14 = 0x801c6548;
data_801c6554_slot04_14 = 0x801c6554;

/* Added by src.u_slot04_14_6464. */
data_801c6560_slot04_14 = 0x801c6560;
data_801c6570_slot04_14 = 0x801c6570;
data_801c6578_slot04_14 = 0x801c6578;
data_801c660c_slot04_14 = 0x801c660c;
data_801c6614_slot04_14 = 0x801c6614;
data_801c6630_slot04_14 = 0x801c6630;
data_801c663c_slot04_14 = 0x801c663c;

/* Added by src.u_slot04_14_6e50. */
data_801c6650_slot04_14 = 0x801c6650;
data_801c6658_slot04_14 = 0x801c6658;
data_801c665c_slot04_14 = 0x801c665c;
data_801c6664_slot04_14 = 0x801c6664;
data_801c6668_slot04_14 = 0x801c6668;
data_801c6674_slot04_14 = 0x801c6674;
data_801c667c_slot04_14 = 0x801c667c;
data_801c7868_slot04_14 = 0x801c7868;
data_801c7878_slot04_14 = 0x801c7878;
data_801c7884_slot04_14 = 0x801c7884;
data_801c7890_slot04_14 = 0x801c7890;
data_801c78cc_slot04_14 = 0x801c78cc;
data_801c7908_slot04_14 = 0x801c7908;
data_801c791c_slot04_14 = 0x801c791c;
data_801c7644_slot04_14 = 0x801c7644;

/* Added by src.u_slot04_14_781c. */
data_801c5af4_slot04_14 = 0x801c5af4;
data_801c766c_slot04_14 = 0x801c766c;
data_801c76ac_slot04_14 = 0x801c76ac;
data_801c7704_slot04_14 = 0x801c7704;
data_801c7744_slot04_14 = 0x801c7744;
data_801c7784_slot04_14 = 0x801c7784;
data_801c77dc_slot04_14 = 0x801c77dc;
data_801c7804_slot04_14 = 0x801c7804;
data_801c7818_slot04_14 = 0x801c7818;
data_801c7838_slot04_14 = 0x801c7838;
data_801c784c_slot04_14 = 0x801c784c;
data_801c7858_slot04_14 = 0x801c7858;
data_801c7864_slot04_14 = 0x801c7864;
data_801c7930_slot04_14 = 0x801c7930;
data_801c7938_slot04_14 = 0x801c7938;
data_801c7948_slot04_14 = 0x801c7948;
data_801c7950_slot04_14 = 0x801c7950;
data_801c7960_slot04_14 = 0x801c7960;
data_801c799c_slot04_14 = 0x801c799c;
data_801c79a4_slot04_14 = 0x801c79a4;
data_801c7f34_slot04_14 = 0x801c7f34;

/* Added by src.u_slot04_14_81f8. */
data_801c79b4_slot04_14 = 0x801c79b4;
data_801c7c74_slot04_14 = 0x801c7c74;
data_801a2bc4 = 0x801a2bc4;

data_801c6194_slot04_0f = 0x801c6194;
data_801ee1d8_slot06_06 = 0x801ee1d8;
data_801ed490_slot06_0a = 0x801ed490;
data_801ed494_slot06_0a = 0x801ed494;
data_801c719c_slot04_11 = 0x801c719c;
data_801c7708_slot04_11 = 0x801c7708;
data_801ce0d0_slot04_17 = 0x801ce0d0;
data_801ce63c_slot04_17 = 0x801ce63c;

/* Added by src.u_slot28_3834. */
func_800145f4_slot28 = 0x800145f4;
func_8001a69c_slot28 = 0x8001a69c;
func_80022944_slot28 = 0x80022944;
data_800289d4_slot28 = 0x800289d4;
data_800289f4_slot28 = 0x800289f4;
data_80028a4c_slot28 = 0x80028a4c;
data_8002ac7c_slot28 = 0x8002ac7c;
data_800285d4_slot28 = 0x800285d4;
data_80051844_slot28 = 0x80051844;
data_801a2be4 = 0x801a2be4;
func_8001389c_slot28 = 0x8001389c;

/* Added by src.u_slot28_3ea0. */
data_8002a87c_slot28 = 0x8002a87c;
data_8002a07c_slot28 = 0x8002a07c;
data_8002a47c_slot28 = 0x8002a47c;
data_80028aa0_slot28 = 0x80028aa0;
data_80028e0c_slot28 = 0x80028e0c;
data_8002ac84_slot28 = 0x8002ac84;
data_80051878_slot28 = 0x80051878;
data_8005187c_slot28 = 0x8005187c;
data_80029ed0_slot28 = 0x80029ed0;
data_80029ed8_slot28 = 0x80029ed8;
data_80029ee4_slot28 = 0x80029ee4;
data_8005188c_slot28 = 0x8005188c;
data_80029ecc_slot28 = 0x80029ecc;
data_80029ee0_slot28 = 0x80029ee0;
data_80029ef4_slot28 = 0x80029ef4;

/* Added by src.u_slot28_444c. */
data_8002acb8_slot28 = 0x8002acb8;

/* Added by src.u_slot28_49a0. */
data_800518c8_slot28 = 0x800518c8;
data_800518bc_slot28 = 0x800518bc;
data_80051890_slot28 = 0x80051890;
data_8002bddc_slot28 = 0x8002bddc;
data_800518b8_slot28 = 0x800518b8;
data_800518cc_slot28 = 0x800518cc;
data_80051894_slot28 = 0x80051894;
data_80051898_slot28 = 0x80051898;
data_8005189c_slot28 = 0x8005189c;
data_800518a0_slot28 = 0x800518a0;
data_8002bdd8_slot28 = 0x8002bdd8;
data_8002bdd4_slot28 = 0x8002bdd4;
data_8002bde4_slot28 = 0x8002bde4;
data_8002be28_slot28 = 0x8002be28;
data_8002acc4_slot28 = 0x8002acc4;
data_8002b024_slot28 = 0x8002b024;

/* Added by src.u_slot28_57cc. */
data_8002c328_slot28 = 0x8002c328;
data_8002bf28_slot28 = 0x8002bf28;
data_8002c77c_slot28 = 0x8002c77c;
data_8002c78c_slot28 = 0x8002c78c;
data_8002c79c_slot28 = 0x8002c79c;
data_8002c7a4_slot28 = 0x8002c7a4;
data_8002c7b0_slot28 = 0x8002c7b0;
data_8002c7b8_slot28 = 0x8002c7b8;
data_8002e190_slot28 = 0x8002e190;

/* Added by src.u_slot28_5f50. */
data_8002c838_slot28 = 0x8002c838;
data_8002fc8c_slot28 = 0x8002fc8c;
data_8002fc90_slot28 = 0x8002fc90;
data_8002fca0_slot28 = 0x8002fca0;
data_8002fca8_slot28 = 0x8002fca8;
data_8002fce8_slot28 = 0x8002fce8;
data_8002e1d4_slot28 = 0x8002e1d4;
data_8002e758_slot28 = 0x8002e758;
data_800518f8_slot28 = 0x800518f8;
data_800518fc_slot28 = 0x800518fc;
data_80051908_slot28 = 0x80051908;
data_8005190c_slot28 = 0x8005190c;
data_800518d0_slot28 = 0x800518d0;
data_800518d4_slot28 = 0x800518d4;
data_800518d8_slot28 = 0x800518d8;
data_800518dc_slot28 = 0x800518dc;

/* Added by src.u_slot28_6658. */

/* Added by src.u_slot28_6e58. */
data_800306c4_slot28 = 0x800306c4;
data_800306cc_slot28 = 0x800306cc;
data_80031c90_slot28 = 0x80031c90;
data_80031c8c_slot28 = 0x80031c8c;
data_80031c9c_slot28 = 0x80031c9c;
data_80031ca0_slot28 = 0x80031ca0;
data_80031cc0_slot28 = 0x80031cc0;
data_80051910_slot28 = 0x80051910;
data_80051950_slot28 = 0x80051950;
data_80032a1c_slot28 = 0x80032a1c;
data_80031c98_slot28 = 0x80031c98;

/* Added by src.u_slot28_7704. */
data_80032a5c_slot28 = 0x80032a5c;
data_80051918_slot28 = 0x80051918;
data_80032a4c_slot28 = 0x80032a4c;
data_80032a54_slot28 = 0x80032a54;
data_80051938_slot28 = 0x80051938;
data_8005193c_slot28 = 0x8005193c;
data_80051914_slot28 = 0x80051914;
data_800306d4_slot28 = 0x800306d4;
data_80030a4c_slot28 = 0x80030a4c;

/* Added by src.u_slot28_8050. */
data_8005198c_slot28 = 0x8005198c;
data_80051990_slot28 = 0x80051990;
data_8005197c_slot28 = 0x8005197c;
data_80051980_slot28 = 0x80051980;
data_80051984_slot28 = 0x80051984;
data_80051954_slot28 = 0x80051954;
data_80051958_slot28 = 0x80051958;
data_8005195c_slot28 = 0x8005195c;
data_80051960_slot28 = 0x80051960;
data_80051964_slot28 = 0x80051964;
data_80051968_slot28 = 0x80051968;
data_80034518_slot28 = 0x80034518;
data_80034514_slot28 = 0x80034514;
data_80034528_slot28 = 0x80034528;
data_8003455c_slot28 = 0x8003455c;
data_80034530_slot28 = 0x80034530;
data_80032a64_slot28 = 0x80032a64;
data_80032ec8_slot28 = 0x80032ec8;

/* Added by src.u_slot28_875c. */
data_80051994_slot28 = 0x80051994;
data_80034520_slot28 = 0x80034520;

/* Added by src.u_slot28_8e50. */
data_800353f8_slot28 = 0x800353f8;
data_800353a0_slot28 = 0x800353a0;

/* Added by src.u_slot28_99dc. */
data_80036690_slot28 = 0x80036690;
data_80051998_slot28 = 0x80051998;
data_8005199c_slot28 = 0x8005199c;
data_800519c0_slot28 = 0x800519c0;
data_800519c4_slot28 = 0x800519c4;
data_800519c8_slot28 = 0x800519c8;
data_800519d0_slot28 = 0x800519d0;
data_800519d4_slot28 = 0x800519d4;
data_80035430_slot28 = 0x80035430;
data_800357d8_slot28 = 0x800357d8;
data_800519a0_slot28 = 0x800519a0;
data_800519a4_slot28 = 0x800519a4;

/* Added by src.u_slot28_a0e4. */
data_80036694_slot28 = 0x80036694;
data_800366a8_slot28 = 0x800366a8;
data_8003669c_slot28 = 0x8003669c;
data_800519a8_slot28 = 0x800519a8;
data_800519ac_slot28 = 0x800519ac;

/* Added by src.u_slot28_a69c. */
data_8003743c_slot28 = 0x8003743c;

/* Added by src.u_slot28_b2b0. */
data_80051a10_slot28 = 0x80051a10;
data_80051eb8_slot28 = 0x80051eb8;
data_80051a08_slot28 = 0x80051a08;
data_80051a04_slot28 = 0x80051a04;
data_800519d8_slot28 = 0x800519d8;
data_80039290_slot28 = 0x80039290;
data_8003929c_slot28 = 0x8003929c;
data_80051a00_slot28 = 0x80051a00;
data_80051a14_slot28 = 0x80051a14;
data_80039280_slot28 = 0x80039280;
data_8003927c_slot28 = 0x8003927c;
data_80039298_slot28 = 0x80039298;
data_800392d0_slot28 = 0x800392d0;

/* Added by src.u_slot28_b9d0. */
data_8003a04c_slot28 = 0x8003a04c;
data_800374ac_slot28 = 0x800374ac;
data_80037a00_slot28 = 0x80037a00;

/* Added by src.u_slot28_c0d4. */
data_8003a068_slot28 = 0x8003a068;
data_8003a128_slot28 = 0x8003a128;
data_8003940c_slot28 = 0x8003940c;
data_8003a158_slot28 = 0x8003a158;

/* Added by src.u_slot28_cb7c. */
data_80051a44_slot28 = 0x80051a44;
data_80051a40_slot28 = 0x80051a40;
data_80051a50_slot28 = 0x80051a50;
data_80051a54_slot28 = 0x80051a54;
data_80051a18_slot28 = 0x80051a18;
data_8003b504_slot28 = 0x8003b504;
data_8003b50c_slot28 = 0x8003b50c;
data_8003b500_slot28 = 0x8003b500;
data_8003b530_slot28 = 0x8003b530;
data_8003b514_slot28 = 0x8003b514;

/* Added by src.u_slot28_d184. */
data_8003c2f4_slot28 = 0x8003c2f4;
data_8003c2f8_slot28 = 0x8003c2f8;
data_8003a1a0_slot28 = 0x8003a1a0;
data_8003a5d4_slot28 = 0x8003a5d4;
data_8003b510_slot28 = 0x8003b510;
data_80051a48_slot28 = 0x80051a48;

/* Added by src.u_slot28_db68. */
data_8003dc00_slot28 = 0x8003dc00;
data_80051a84_slot28 = 0x80051a84;
data_80051a80_slot28 = 0x80051a80;
data_80051a58_slot28 = 0x80051a58;
data_8003d2d0_slot28 = 0x8003d2d0;
data_8003d2d8_slot28 = 0x8003d2d8;
data_8003d2e0_slot28 = 0x8003d2e0;
data_80051a94_slot28 = 0x80051a94;
data_80051a90_slot28 = 0x80051a90;
data_8003d2cc_slot28 = 0x8003d2cc;
data_8003d2f4_slot28 = 0x8003d2f4;

/* Added by src.u_slot28_e4c0. */
data_80051a98_slot28 = 0x80051a98;
data_80051ac0_slot28 = 0x80051ac0;
data_80051ac4_slot28 = 0x80051ac4;
data_80051ac8_slot28 = 0x80051ac8;
data_80051ad0_slot28 = 0x80051ad0;
data_80051ad4_slot28 = 0x80051ad4;
data_8003f084_slot28 = 0x8003f084;
data_8003f080_slot28 = 0x8003f080;
data_8003f090_slot28 = 0x8003f090;
data_8003f0b8_slot28 = 0x8003f0b8;
data_8003f09c_slot28 = 0x8003f09c;
data_8003dea0_slot28 = 0x8003dea0;
data_8003e230_slot28 = 0x8003e230;

/* Added by src.u_slot28_ebb8. */
data_80051b00_slot28 = 0x80051b00;
data_80051b04_slot28 = 0x80051b04;
data_80051b08_slot28 = 0x80051b08;
data_80051b10_slot28 = 0x80051b10;
data_80051b14_slot28 = 0x80051b14;
data_80041498_slot28 = 0x80041498;
data_80041494_slot28 = 0x80041494;
data_800414a4_slot28 = 0x800414a4;
data_800414b0_slot28 = 0x800414b0;
data_800414c0_slot28 = 0x800414c0;
data_800414d4_slot28 = 0x800414d4;
data_80051ad8_slot28 = 0x80051ad8;
data_8003f9fc_slot28 = 0x8003f9fc;
data_8003feb8_slot28 = 0x8003feb8;

/* Added by src.u_slot28_f7f0. */
data_800414b8_slot28 = 0x800414b8;

/* Added by src.u_slot28_fec8. */
data_80042274_slot28 = 0x80042274;
data_80043848_slot28 = 0x80043848;
data_8004384c_slot28 = 0x8004384c;
data_80043858_slot28 = 0x80043858;
data_80043860_slot28 = 0x80043860;
data_80043870_slot28 = 0x80043870;
data_80051b40_slot28 = 0x80051b40;
data_80051b44_slot28 = 0x80051b44;
data_80051b48_slot28 = 0x80051b48;
data_80051b50_slot28 = 0x80051b50;
data_80051b54_slot28 = 0x80051b54;
data_80051b58_slot28 = 0x80051b58;
data_80051b5c_slot28 = 0x80051b5c;
data_80051b60_slot28 = 0x80051b60;

/* Added by src.u_slot28_109c8. */
data_8004231c_slot28 = 0x8004231c;
data_800426b8_slot28 = 0x800426b8;
data_80043864_slot28 = 0x80043864;
data_800445f0_slot28 = 0x800445f0;
data_80051b18_slot28 = 0x80051b18;

/* Added by src.u_slot28_114bc. */
data_80051b64_slot28 = 0x80051b64;
data_80051b8c_slot28 = 0x80051b8c;
data_80051b90_slot28 = 0x80051b90;
data_80051b94_slot28 = 0x80051b94;
data_80051b9c_slot28 = 0x80051b9c;
data_80045c40_slot28 = 0x80045c40;
data_80045c4c_slot28 = 0x80045c4c;
data_80045c58_slot28 = 0x80045c58;
data_80044620_slot28 = 0x80044620;
data_800449d0_slot28 = 0x800449d0;
data_80045c54_slot28 = 0x80045c54;
data_80051ba0_slot28 = 0x80051ba0;
data_80045c3c_slot28 = 0x80045c3c;
data_80045c6c_slot28 = 0x80045c6c;

/* Added by src.u_slot28_12138. */
data_80051ba4_slot28 = 0x80051ba4;
data_80051bcc_slot28 = 0x80051bcc;
data_80051bd0_slot28 = 0x80051bd0;
data_80051bd4_slot28 = 0x80051bd4;
data_80051bdc_slot28 = 0x80051bdc;
data_80051be0_slot28 = 0x80051be0;
data_80047bd4_slot28 = 0x80047bd4;
data_80047bd8_slot28 = 0x80047bd8;
data_80047bdc_slot28 = 0x80047bdc;
data_80047be0_slot28 = 0x80047be0;
data_80047be4_slot28 = 0x80047be4;
data_80047bf8_slot28 = 0x80047bf8;
data_800469ec_slot28 = 0x800469ec;
data_80046d34_slot28 = 0x80046d34;

/* Added by src.u_slot28_12944. */
data_80048ccc_slot28 = 0x80048ccc;
data_80048cf4_slot28 = 0x80048cf4;
data_80048d48_slot28 = 0x80048d48;
data_8004897c_slot28 = 0x8004897c;

/* Added by src.u_slot28_13110. */
data_80051c1c_slot28 = 0x80051c1c;
data_80051be4_slot28 = 0x80051be4;
data_80051c0c_slot28 = 0x80051c0c;
data_80051c10_slot28 = 0x80051c10;
data_8004a088_slot28 = 0x8004a088;
data_8004a09c_slot28 = 0x8004a09c;
data_80048d70_slot28 = 0x80048d70;
data_800490f4_slot28 = 0x800490f4;
data_8004aa38_slot28 = 0x8004aa38;
data_80051c20_slot28 = 0x80051c20;
data_8004a084_slot28 = 0x8004a084;
data_8004a094_slot28 = 0x8004a094;
data_8004a0d0_slot28 = 0x8004a0d0;

/* Added by src.u_slot28_138b0. */
data_8004aa48_slot28 = 0x8004aa48;
data_8004aa68_slot28 = 0x8004aa68;
data_8004aa88_slot28 = 0x8004aa88;
data_8004aab8_slot28 = 0x8004aab8;
data_8004aac0_slot28 = 0x8004aac0;
data_8004aac4_slot28 = 0x8004aac4;

/* Added by src.u_slot28_140d0. */
data_8004ab50_slot28 = 0x8004ab50;
data_8004ab80_slot28 = 0x8004ab80;

/* Added by src.u_slot28_147cc. */
data_8004bd70_slot28 = 0x8004bd70;
data_8004bd74_slot28 = 0x8004bd74;
data_8004bd7c_slot28 = 0x8004bd7c;
data_8004bda4_slot28 = 0x8004bda4;
data_80051c4c_slot28 = 0x80051c4c;
data_80051c50_slot28 = 0x80051c50;
data_80051c5c_slot28 = 0x80051c5c;
data_80051c60_slot28 = 0x80051c60;
data_8004bd84_slot28 = 0x8004bd84;
data_80051c24_slot28 = 0x80051c24;
data_80051c28_slot28 = 0x80051c28;
data_8004ab90_slot28 = 0x8004ab90;
data_8004aed8_slot28 = 0x8004aed8;
data_80051c2c_slot28 = 0x80051c2c;
data_80051c30_slot28 = 0x80051c30;
data_80051c34_slot28 = 0x80051c34;
data_80051c38_slot28 = 0x80051c38;
data_80051c3c_slot28 = 0x80051c3c;

/* Added by src.u_slot28_152b0. */
data_80051c8c_slot28 = 0x80051c8c;
data_80051c90_slot28 = 0x80051c90;
data_80051c94_slot28 = 0x80051c94;
data_80051c9c_slot28 = 0x80051c9c;
data_80051ca0_slot28 = 0x80051ca0;
data_80051c64_slot28 = 0x80051c64;
data_8004dcd4_slot28 = 0x8004dcd4;
data_8004dcd8_slot28 = 0x8004dcd8;
data_8004dce0_slot28 = 0x8004dce0;
data_8004dce8_slot28 = 0x8004dce8;
data_8004dd0c_slot28 = 0x8004dd0c;
data_8004dd1c_slot28 = 0x8004dd1c;

/* Added by src.u_slot28_15950. */
data_8004dce4_slot28 = 0x8004dce4;
data_8004c73c_slot28 = 0x8004c73c;
data_8004cbc4_slot28 = 0x8004cbc4;
data_8004ea70_slot28 = 0x8004ea70;
data_8004ea74_slot28 = 0x8004ea74;

/* Added by src.u_slot28_16480. */
data_80051cac_slot28 = 0x80051cac;
data_80051cdc_slot28 = 0x80051cdc;
data_80051ccc_slot28 = 0x80051ccc;
data_80051cc0_slot28 = 0x80051cc0;
data_800502c8_slot28 = 0x800502c8;
data_800502e8_slot28 = 0x800502e8;
data_80051cb0_slot28 = 0x80051cb0;
data_80051ca4_slot28 = 0x80051ca4;
data_80051ca8_slot28 = 0x80051ca8;
data_80051cbc_slot28 = 0x80051cbc;
data_80051cd0_slot28 = 0x80051cd0;
data_80051ce0_slot28 = 0x80051ce0;
data_800502c4_slot28 = 0x800502c4;
data_800502d4_slot28 = 0x800502d4;
data_80050304_slot28 = 0x80050304;

/* Added by src.u_slot28_16acc. */
data_8004ead4_slot28 = 0x8004ead4;
data_8004ef00_slot28 = 0x8004ef00;

/* Added by src.u_slot28_17134. */
data_80050ca0_slot28 = 0x80050ca0;
data_80050ca4_slot28 = 0x80050ca4;
data_8005174c_slot28 = 0x8005174c;
data_80051ce4_slot28 = 0x80051ce4;
data_80051df0_slot28 = 0x80051df0;

/* Added by src.u_slot28_179d4. */
data_80050ce4_slot28 = 0x80050ce4;
data_8005146c_slot28 = 0x8005146c;
data_80051528_slot28 = 0x80051528;
data_800516dc_slot28 = 0x800516dc;
data_800516ec_slot28 = 0x800516ec;
data_8005175c_slot28 = 0x8005175c;
data_800517b8_slot28 = 0x800517b8;
data_800517d8_slot28 = 0x800517d8;
data_80051e94_slot28 = 0x80051e94;

/* Added by src.u_slot28_18038. */
data_800517fc_slot28 = 0x800517fc;
data_80051820_slot28 = 0x80051820;
data_80051df4_slot28 = 0x80051df4;

/* Added by src.u_slot28_3ea0x. */

/* Added by src.u_slot28_3a00. */
data_8002c728_slot28 = 0x8002c728;
data_8002c730_slot28 = 0x8002c730;
data_8002c76c_slot28 = 0x8002c76c;
data_8002e1c8_slot28 = 0x8002e1c8;
data_8003065c_slot28 = 0x8003065c;
data_80030664_slot28 = 0x80030664;

/* Added by src.u_slot28_6ee8. */
data_8002fe5c_slot28 = 0x8002fe5c;
data_8003025c_slot28 = 0x8003025c;
data_80030694_slot28 = 0x80030694;
data_800306a4_slot28 = 0x800306a4;
data_80031e14_slot28 = 0x80031e14;
data_80032214_slot28 = 0x80032214;
data_80032614_slot28 = 0x80032614;
data_80032a14_slot28 = 0x80032a14;
data_80032a3c_slot28 = 0x80032a3c;
data_8003534c_slot28 = 0x8003534c;
data_80035354_slot28 = 0x80035354;
data_80051948_slot28 = 0x80051948;

/* Added by src.u_slot28_8fb0. */
data_8003474c_slot28 = 0x8003474c;
data_80034b4c_slot28 = 0x80034b4c;
data_80034f4c_slot28 = 0x80034f4c;
data_80035390_slot28 = 0x80035390;
data_800353c4_slot28 = 0x800353c4;
data_800353cc_slot28 = 0x800353cc;
data_800353d4_slot28 = 0x800353d4;
data_800353e8_slot28 = 0x800353e8;
data_80035400_slot28 = 0x80035400;
data_80035414_slot28 = 0x80035414;
data_800373f4_slot28 = 0x800373f4;
data_800373fc_slot28 = 0x800373fc;

/* Added by src.u_slot28_a878. */
data_800366e0_slot28 = 0x800366e0;
data_800367f4_slot28 = 0x800367f4;
data_80036bf4_slot28 = 0x80036bf4;
data_80036ff4_slot28 = 0x80036ff4;
data_8003742c_slot28 = 0x8003742c;
data_80037448_slot28 = 0x80037448;
data_8003746c_slot28 = 0x8003746c;
data_80037478_slot28 = 0x80037478;
data_80037480_slot28 = 0x80037480;
data_80037488_slot28 = 0x80037488;
data_800374a4_slot28 = 0x800374a4;
data_8003a00c_slot28 = 0x8003a00c;
data_8003a014_slot28 = 0x8003a014;

/* Added by src.u_slot28_bae0. */
data_8003980c_slot28 = 0x8003980c;
data_80039c0c_slot28 = 0x80039c0c;
data_8003a148_slot28 = 0x8003a148;
data_8003a160_slot28 = 0x8003a160;
data_8003a174_slot28 = 0x8003a174;
data_8003a184_slot28 = 0x8003a184;
data_8003a190_slot28 = 0x8003a190;
data_8003a198_slot28 = 0x8003a198;
data_8003c28c_slot28 = 0x8003c28c;

/* Added by src.u_slot28_cd04. */
data_8003b68c_slot28 = 0x8003b68c;
data_8003ba8c_slot28 = 0x8003ba8c;
data_8003be8c_slot28 = 0x8003be8c;
data_8003c294_slot28 = 0x8003c294;
data_8003c2c4_slot28 = 0x8003c2c4;
data_8003c2d4_slot28 = 0x8003c2d4;
data_8003c2e4_slot28 = 0x8003c2e4;
data_8003c2ec_slot28 = 0x8003c2ec;
data_8003dbd8_slot28 = 0x8003dbd8;
data_8003dbe0_slot28 = 0x8003dbe0;

/* Added by src.u_slot28_df84. */
data_8003c358_slot28 = 0x8003c358;
data_8003c5b8_slot28 = 0x8003c5b8;
data_8003d3d8_slot28 = 0x8003d3d8;
data_8003d7d8_slot28 = 0x8003d7d8;
data_8003f984_slot28 = 0x8003f984;
data_8003f98c_slot28 = 0x8003f98c;

/* Added by src.u_slot28_ef04. */
data_8003f184_slot28 = 0x8003f184;
data_8003f584_slot28 = 0x8003f584;
data_8003f9c0_slot28 = 0x8003f9c0;
data_8003f9d0_slot28 = 0x8003f9d0;
data_8003f9d4_slot28 = 0x8003f9d4;
data_8003f9e0_slot28 = 0x8003f9e0;
data_8003f9e8_slot28 = 0x8003f9e8;
data_8003f9f0_slot28 = 0x8003f9f0;
data_80042228_slot28 = 0x80042228;
data_80042230_slot28 = 0x80042230;

/* Added by src.u_slot28_100ec. */
data_80041628_slot28 = 0x80041628;
data_80041a28_slot28 = 0x80041a28;
data_80041e28_slot28 = 0x80041e28;
data_80042264_slot28 = 0x80042264;
data_800422e4_slot28 = 0x800422e4;
data_80042308_slot28 = 0x80042308;
data_80042314_slot28 = 0x80042314;
data_800445b4_slot28 = 0x800445b4;
data_800445bc_slot28 = 0x800445bc;

/* Added by src.u_slot28_1115c. */
data_800439b4_slot28 = 0x800439b4;
data_80043db4_slot28 = 0x80043db4;
data_800441b4_slot28 = 0x800441b4;
data_800445f8_slot28 = 0x800445f8;
data_80044608_slot28 = 0x80044608;
data_80044610_slot28 = 0x80044610;
data_80044618_slot28 = 0x80044618;
data_80046988_slot28 = 0x80046988;
data_80046990_slot28 = 0x80046990;

/* Added by src.u_slot28_11df4. */
data_80045d88_slot28 = 0x80045d88;
data_80046188_slot28 = 0x80046188;
data_80046588_slot28 = 0x80046588;
data_800469bc_slot28 = 0x800469bc;
data_800469cc_slot28 = 0x800469cc;
data_800469d0_slot28 = 0x800469d0;
data_800469d8_slot28 = 0x800469d8;
data_800469e4_slot28 = 0x800469e4;
data_80048940_slot28 = 0x80048940;
data_80048948_slot28 = 0x80048948;

/* Added by src.u_slot28_12d38. */
data_80047d40_slot28 = 0x80047d40;
data_80048140_slot28 = 0x80048140;
data_80048540_slot28 = 0x80048540;
data_80048cbc_slot28 = 0x80048cbc;
data_80048d3c_slot28 = 0x80048d3c;
data_8004a210_slot28 = 0x8004a210;
data_8004a610_slot28 = 0x8004a610;
data_8004aa10_slot28 = 0x8004aa10;
data_8004aa18_slot28 = 0x8004aa18;
data_8004aaa8_slot28 = 0x8004aaa8;
data_8004ab04_slot28 = 0x8004ab04;
data_8004ab14_slot28 = 0x8004ab14;
data_8004ab1c_slot28 = 0x8004ab1c;
data_8004ab28_slot28 = 0x8004ab28;

/* Added by src.u_slot28_14494. */
data_8004ab38_slot28 = 0x8004ab38;
data_8004ab40_slot28 = 0x8004ab40;
data_8004ab88_slot28 = 0x8004ab88;
data_8004bef8_slot28 = 0x8004bef8;
data_8004c2f8_slot28 = 0x8004c2f8;
data_8004c6f8_slot28 = 0x8004c6f8;
data_8004c700_slot28 = 0x8004c700;
data_8004c724_slot28 = 0x8004c724;
data_8004c734_slot28 = 0x8004c734;
data_8004ea10_slot28 = 0x8004ea10;

/* Added by src.u_slot28_157a4. */
data_8004de10_slot28 = 0x8004de10;
data_8004e210_slot28 = 0x8004e210;
data_8004e610_slot28 = 0x8004e610;
data_8004ea18_slot28 = 0x8004ea18;
data_8004ea40_slot28 = 0x8004ea40;
data_8004ea50_slot28 = 0x8004ea50;
data_8004ea60_slot28 = 0x8004ea60;
data_8004ea68_slot28 = 0x8004ea68;

/* Added by src.u_slot28_163f8. */
data_80050458_slot28 = 0x80050458;
data_80050858_slot28 = 0x80050858;
data_80050c58_slot28 = 0x80050c58;
data_80050c60_slot28 = 0x80050c60;
data_80050c90_slot28 = 0x80050c90;
data_80051788_slot28 = 0x80051788;
data_80051790_slot28 = 0x80051790;
data_800517f0_slot28 = 0x800517f0;
data_80051814_slot28 = 0x80051814;
data_80051828_slot28 = 0x80051828;

/* Added by src.u_slot01_00a4. */
func_800108d4_slot01 = 0x800108d4;
func_80012714_slot01 = 0x80012714;
func_80012990_slot01 = 0x80012990;
func_80012dec_slot01 = 0x80012dec;
data_8001514c_slot01 = 0x8001514c;
data_8002ceb0_slot01 = 0x8002ceb0;
data_80055f44_slot01 = 0x80055f44;
data_80055f3c_slot01 = 0x80055f3c;
data_80055f40_slot01 = 0x80055f40;
data_801a37e4 = 0x801a37e4;

/* Added by src.u_slot01_0724. */
data_800100a0_slot01 = 0x800100a0;

/* Added by src.u_slot01_0d0c. */
data_800151b4_slot01 = 0x800151b4;
data_80055ea4_slot01 = 0x80055ea4;

/* Added by src.u_slot01_1374. */
data_80015310_slot01 = 0x80015310;
data_80055f38_slot01 = 0x80055f38;

/* Added by src.u_slot01_1a14. */
data_800212ac_slot01 = 0x800212ac;
data_80015320_slot01 = 0x80015320;
data_800152e4_slot01 = 0x800152e4;
data_80055e94_slot01 = 0x80055e94;
data_80019330_slot01 = 0x80019330;
data_800195b8_slot01 = 0x800195b8;

/* Added by src.u_slot01_20ac. */
data_80015408_slot01 = 0x80015408;
data_80015418_slot01 = 0x80015418;
data_8002ceb8_slot01 = 0x8002ceb8;
data_80018a6c_slot01 = 0x80018a6c;
data_80023d9c_slot01 = 0x80023d9c;
data_80015420_slot01 = 0x80015420;
data_80015440_slot01 = 0x80015440;
data_80015680_slot01 = 0x80015680;
data_801a2964 = 0x801a2964;

/* Added by src.u_slot01_2714. */
data_800319b8_slot01 = 0x800319b8;

/* Added by src.u_slot01_2dec. */
data_80015870_slot01 = 0x80015870;
data_8001587c_slot01 = 0x8001587c;
data_80055eb2_slot01 = 0x80055eb2;
data_800242a8_slot01 = 0x800242a8;
data_80024344_slot01 = 0x80024344;
data_80024a7c_slot01 = 0x80024a7c;
data_800299a8_slot01 = 0x800299a8;

/* Added by src.u_slot01_3440. */
data_80015a4c_slot01 = 0x80015a4c;
data_80015a64_slot01 = 0x80015a64;
data_80019c68_slot01 = 0x80019c68;
data_80019dcc_slot01 = 0x80019dcc;
data_80024330_slot01 = 0x80024330;
data_80031b98_slot01 = 0x80031b98;

/* Added by src.u_slot01_3a2c. */
data_80015a70_slot01 = 0x80015a70;
data_80015ac0_slot01 = 0x80015ac0;
data_80015bf4_slot01 = 0x80015bf4;
data_80015c00_slot01 = 0x80015c00;
data_80015c8c_slot01 = 0x80015c8c;
data_80015c10_slot01 = 0x80015c10;
data_80015c60_slot01 = 0x80015c60;
data_80023ee0_slot01 = 0x80023ee0;
data_80023e48_slot01 = 0x80023e48;
data_80015bc8_slot01 = 0x80015bc8;
data_80055e9a_slot01 = 0x80055e9a;

/* Added by src.u_slot01_3f78. */
data_800240f8_slot01 = 0x800240f8;

/* Added by src.u_slot01_45b8. */
data_80015ce0_slot01 = 0x80015ce0;
data_80015dcc_slot01 = 0x80015dcc;
data_80015dd4_slot01 = 0x80015dd4;
data_80015ddc_slot01 = 0x80015ddc;
data_80033b98_slot01 = 0x80033b98;
data_80033b9c_slot01 = 0x80033b9c;
data_80033ba0_slot01 = 0x80033ba0;
data_80033ba4_slot01 = 0x80033ba4;
data_80033ba8_slot01 = 0x80033ba8;
data_80033bac_slot01 = 0x80033bac;
data_80033bb0_slot01 = 0x80033bb0;
data_8002b484_slot01 = 0x8002b484;
data_8002bb2c_slot01 = 0x8002bb2c;
data_80055ea0_slot01 = 0x80055ea0;

/* Added by src.u_slot01_4c8c. */
data_80033bb4_slot01 = 0x80033bb4;

/* Inside the image of slot01; the module of slot 0x27 uses the same two addresses, which lie outside its image. */
data_80055f30_slot01 = 0x80055f30;
data_80055f34_slot01 = 0x80055f34;

/* Added by src.u_slot01_07f8. */
data_80015770_slot01 = 0x80015770;
data_80015960_slot01 = 0x80015960;
data_80015a50_slot01 = 0x80015a50;
data_80015dc0_slot01 = 0x80015dc0;
data_80023d00_slot01 = 0x80023d00;
data_801f8978_slot06_02 = 0x801f8978;
data_801f8000_slot06_03 = 0x801f8000;

/* Added by src.u_slot04_00_017c. */
data_801bfd2c_slot04_00 = 0x801bfd2c;
data_801bfd70_slot04_00 = 0x801bfd70;
data_801bfdf0_slot04_00 = 0x801bfdf0;
data_801bfe20_slot04_00 = 0x801bfe20;
data_801bfe2c_slot04_00 = 0x801bfe2c;
data_801bfe6c_slot04_00 = 0x801bfe6c;
data_801bfe80_slot04_00 = 0x801bfe80;
data_801bfea0_slot04_00 = 0x801bfea0;
data_801bfea8_slot04_00 = 0x801bfea8;
data_801bfeb0_slot04_00 = 0x801bfeb0;
data_801bfeb8_slot04_00 = 0x801bfeb8;
data_801bfec8_slot04_00 = 0x801bfec8;
data_801c0540_slot04_00 = 0x801c0540;
data_801c0550_slot04_00 = 0x801c0550;
data_801c056c_slot04_00 = 0x801c056c;
data_801c05a0_slot04_00 = 0x801c05a0;

/* Added by src.u_slot04_01_015c. */
data_801beea0_slot04_01 = 0x801beea0;
data_801bef90_slot04_01 = 0x801bef90;
data_801befd0_slot04_01 = 0x801befd0;
data_801bf080_slot04_01 = 0x801bf080;
data_801bf0b8_slot04_01 = 0x801bf0b8;
data_801bf0c0_slot04_01 = 0x801bf0c0;
data_801bf0c8_slot04_01 = 0x801bf0c8;
data_801bf0d0_slot04_01 = 0x801bf0d0;
data_801bf0d8_slot04_01 = 0x801bf0d8;
data_801bf0e8_slot04_01 = 0x801bf0e8;
data_801bf0fc_slot04_01 = 0x801bf0fc;
data_801bf230_slot04_01 = 0x801bf230;
data_801bf254_slot04_01 = 0x801bf254;
data_801bf268_slot04_01 = 0x801bf268;
data_801bf280_slot04_01 = 0x801bf280;
data_801bf294_slot04_01 = 0x801bf294;
data_801bf31c_slot04_01 = 0x801bf31c;

/* Added by src.u_slot04_02_0230. */
data_801c61ac_slot04_02 = 0x801c61ac;
data_801c61b8_slot04_02 = 0x801c61b8;
data_801c61c8_slot04_02 = 0x801c61c8;
data_801c61d0_slot04_02 = 0x801c61d0;
data_801c6298_slot04_02 = 0x801c6298;
data_801c62e0_slot04_02 = 0x801c62e0;
data_801c62ec_slot04_02 = 0x801c62ec;
data_801c6320_slot04_02 = 0x801c6320;
data_801c6340_slot04_02 = 0x801c6340;
data_801c6354_slot04_02 = 0x801c6354;
data_801c6364_slot04_02 = 0x801c6364;
data_801c639c_slot04_02 = 0x801c639c;
data_801c63ac_slot04_02 = 0x801c63ac;
data_801c63bc_slot04_02 = 0x801c63bc;
data_801c63dc_slot04_02 = 0x801c63dc;
data_801c63ec_slot04_02 = 0x801c63ec;
data_801c6410_slot04_02 = 0x801c6410;
data_801c6418_slot04_02 = 0x801c6418;
data_801c6420_slot04_02 = 0x801c6420;
data_801c646c_slot04_02 = 0x801c646c;
data_801c6528_slot04_02 = 0x801c6528;
data_801c6538_slot04_02 = 0x801c6538;
data_801c6544_slot04_02 = 0x801c6544;
data_801c6558_slot04_02 = 0x801c6558;
data_801c6584_slot04_02 = 0x801c6584;
data_801c65a0_slot04_02 = 0x801c65a0;
data_801c65ac_slot04_02 = 0x801c65ac;
data_801c65bc_slot04_02 = 0x801c65bc;
data_801c65c4_slot04_02 = 0x801c65c4;
data_801c65cc_slot04_02 = 0x801c65cc;
data_801c7738_slot04_02 = 0x801c7738;
data_801c7800_slot04_02 = 0x801c7800;
data_801c7818_slot04_02 = 0x801c7818;
data_801c786c_slot04_02 = 0x801c786c;
data_801c7874_slot04_02 = 0x801c7874;
data_801c7e04_slot04_02 = 0x801c7e04;

/* Added by src.u_slot04_03_00fc. */
data_801c0500_slot04_03 = 0x801c0500;
data_801c0560_slot04_03 = 0x801c0560;
data_801c05d4_slot04_03 = 0x801c05d4;
data_801c05ec_slot04_03 = 0x801c05ec;
data_801c05f8_slot04_03 = 0x801c05f8;
data_801c0604_slot04_03 = 0x801c0604;
data_801c061c_slot04_03 = 0x801c061c;
data_801c0638_slot04_03 = 0x801c0638;
data_801c0658_slot04_03 = 0x801c0658;
data_801c06a8_slot04_03 = 0x801c06a8;
data_801c06d8_slot04_03 = 0x801c06d8;
data_801c070c_slot04_03 = 0x801c070c;
data_801c0718_slot04_03 = 0x801c0718;
data_801c0720_slot04_03 = 0x801c0720;
data_801c0728_slot04_03 = 0x801c0728;
data_801c0734_slot04_03 = 0x801c0734;
data_801c0750_slot04_03 = 0x801c0750;
data_801c0760_slot04_03 = 0x801c0760;
data_801c0774_slot04_03 = 0x801c0774;
data_801c0994_slot04_03 = 0x801c0994;
data_801c09bc_slot04_03 = 0x801c09bc;
data_801c09e8_slot04_03 = 0x801c09e8;
data_801c09f0_slot04_03 = 0x801c09f0;
data_801c0a20_slot04_03 = 0x801c0a20;

/* Added by src.u_slot04_04_0210. */
data_801c4154_slot04_04 = 0x801c4154;
data_801c4174_slot04_04 = 0x801c4174;
data_801c4200_slot04_04 = 0x801c4200;
data_801c42a4_slot04_04 = 0x801c42a4;
data_801c42e8_slot04_04 = 0x801c42e8;
data_801c42f4_slot04_04 = 0x801c42f4;
data_801c4334_slot04_04 = 0x801c4334;
data_801c4378_slot04_04 = 0x801c4378;
data_801c43c4_slot04_04 = 0x801c43c4;
data_801c43e0_slot04_04 = 0x801c43e0;
data_801c43e8_slot04_04 = 0x801c43e8;
data_801c43f8_slot04_04 = 0x801c43f8;
data_801c4404_slot04_04 = 0x801c4404;
data_801c4510_slot04_04 = 0x801c4510;
data_801c4530_slot04_04 = 0x801c4530;
data_801c4598_slot04_04 = 0x801c4598;
data_801c45a8_slot04_04 = 0x801c45a8;
data_801c45bc_slot04_04 = 0x801c45bc;
data_801c45cc_slot04_04 = 0x801c45cc;
data_801c4660_slot04_04 = 0x801c4660;

/* Added by src.u_slot04_05_0160. */
data_801c132c_slot04_05 = 0x801c132c;
data_801c1364_slot04_05 = 0x801c1364;
data_801c1374_slot04_05 = 0x801c1374;
data_801c1380_slot04_05 = 0x801c1380;
data_801c1388_slot04_05 = 0x801c1388;
data_801c1390_slot04_05 = 0x801c1390;
data_801c1414_slot04_05 = 0x801c1414;
data_801c1460_slot04_05 = 0x801c1460;
data_801c14a8_slot04_05 = 0x801c14a8;
data_801c14f0_slot04_05 = 0x801c14f0;
data_801c14fc_slot04_05 = 0x801c14fc;
data_801c1508_slot04_05 = 0x801c1508;
data_801c152c_slot04_05 = 0x801c152c;
data_801c154c_slot04_05 = 0x801c154c;
data_801c1574_slot04_05 = 0x801c1574;
data_801c1580_slot04_05 = 0x801c1580;
data_801c15b4_slot04_05 = 0x801c15b4;
data_801c15cc_slot04_05 = 0x801c15cc;
data_801c15e8_slot04_05 = 0x801c15e8;
data_801c15f0_slot04_05 = 0x801c15f0;
data_801c15f8_slot04_05 = 0x801c15f8;
data_801c1608_slot04_05 = 0x801c1608;
data_801c1618_slot04_05 = 0x801c1618;
data_801c1658_slot04_05 = 0x801c1658;
data_801c1664_slot04_05 = 0x801c1664;
data_801c166c_slot04_05 = 0x801c166c;
data_801c1674_slot04_05 = 0x801c1674;

/* Added by src.u_slot04_06_01f8. */
data_801c5228_slot04_06 = 0x801c5228;
data_801c5234_slot04_06 = 0x801c5234;
data_801c523c_slot04_06 = 0x801c523c;
data_801c5248_slot04_06 = 0x801c5248;
data_801c5250_slot04_06 = 0x801c5250;
data_801c525c_slot04_06 = 0x801c525c;
data_801c5268_slot04_06 = 0x801c5268;
data_801c5270_slot04_06 = 0x801c5270;
data_801c527c_slot04_06 = 0x801c527c;
data_801c5284_slot04_06 = 0x801c5284;
data_801c52e8_slot04_06 = 0x801c52e8;
data_801c536c_slot04_06 = 0x801c536c;
data_801c538c_slot04_06 = 0x801c538c;
data_801c53b0_slot04_06 = 0x801c53b0;
data_801c5418_slot04_06 = 0x801c5418;
data_801c542c_slot04_06 = 0x801c542c;
data_801c5488_slot04_06 = 0x801c5488;
data_801c549c_slot04_06 = 0x801c549c;
data_801c54a4_slot04_06 = 0x801c54a4;
data_801c54b0_slot04_06 = 0x801c54b0;
data_801c54c0_slot04_06 = 0x801c54c0;
data_801c54dc_slot04_06 = 0x801c54dc;
data_801c54f0_slot04_06 = 0x801c54f0;
data_801c54fc_slot04_06 = 0x801c54fc;
data_801c5540_slot04_06 = 0x801c5540;
data_801c5550_slot04_06 = 0x801c5550;
data_801c555c_slot04_06 = 0x801c555c;
data_801c5564_slot04_06 = 0x801c5564;
data_801c5570_slot04_06 = 0x801c5570;
data_801c5578_slot04_06 = 0x801c5578;
data_801c5584_slot04_06 = 0x801c5584;
data_801c5590_slot04_06 = 0x801c5590;
data_801c5598_slot04_06 = 0x801c5598;
data_801c55a4_slot04_06 = 0x801c55a4;
data_801c55ac_slot04_06 = 0x801c55ac;
data_801c55c4_slot04_06 = 0x801c55c4;
data_801c56c4_slot04_06 = 0x801c56c4;
data_801c56d0_slot04_06 = 0x801c56d0;

/* Added by src.u_slot04_07_0344. */
data_801c1e78_slot04_07 = 0x801c1e78;
data_801c1e84_slot04_07 = 0x801c1e84;
data_801c1ef0_slot04_07 = 0x801c1ef0;
data_801c1f58_slot04_07 = 0x801c1f58;
data_801c1f84_slot04_07 = 0x801c1f84;
data_801c200c_slot04_07 = 0x801c200c;
data_801c206c_slot04_07 = 0x801c206c;
data_801c2074_slot04_07 = 0x801c2074;
data_801c2084_slot04_07 = 0x801c2084;
data_801c20b0_slot04_07 = 0x801c20b0;
data_801c20bc_slot04_07 = 0x801c20bc;
data_801c20d4_slot04_07 = 0x801c20d4;
data_801c2104_slot04_07 = 0x801c2104;
data_801c2124_slot04_07 = 0x801c2124;

/* Added by src.u_slot04_08_02f0. */
data_801c3b58_slot04_08 = 0x801c3b58;
data_801c3b60_slot04_08 = 0x801c3b60;
data_801c3b68_slot04_08 = 0x801c3b68;
data_801c3be8_slot04_08 = 0x801c3be8;
data_801c3c10_slot04_08 = 0x801c3c10;
data_801c3c2c_slot04_08 = 0x801c3c2c;
data_801c3c68_slot04_08 = 0x801c3c68;
data_801c3cdc_slot04_08 = 0x801c3cdc;
data_801c3ce8_slot04_08 = 0x801c3ce8;
data_801c3d04_slot04_08 = 0x801c3d04;
data_801c3d20_slot04_08 = 0x801c3d20;
data_801c3d4c_slot04_08 = 0x801c3d4c;
data_801c3d54_slot04_08 = 0x801c3d54;
data_801c3d5c_slot04_08 = 0x801c3d5c;
data_801c3d64_slot04_08 = 0x801c3d64;
data_801c3d6c_slot04_08 = 0x801c3d6c;
data_801c3d7c_slot04_08 = 0x801c3d7c;
data_801c3dbc_slot04_08 = 0x801c3dbc;
data_801c3dc8_slot04_08 = 0x801c3dc8;
data_801c3de4_slot04_08 = 0x801c3de4;
data_801c3dec_slot04_08 = 0x801c3dec;
data_801c3df4_slot04_08 = 0x801c3df4;
data_801c3e0c_slot04_08 = 0x801c3e0c;

/* Added by src.u_slot04_09_015c. */
data_801c7b80_slot04_09 = 0x801c7b80;
data_801c7b88_slot04_09 = 0x801c7b88;
data_801c7bb4_slot04_09 = 0x801c7bb4;
data_801c7bbc_slot04_09 = 0x801c7bbc;
data_801c7bc4_slot04_09 = 0x801c7bc4;
data_801c7bcc_slot04_09 = 0x801c7bcc;
data_801c7c38_slot04_09 = 0x801c7c38;
data_801c7c48_slot04_09 = 0x801c7c48;
data_801c7c54_slot04_09 = 0x801c7c54;
data_801c7cc0_slot04_09 = 0x801c7cc0;
data_801c7ce4_slot04_09 = 0x801c7ce4;
data_801c7cf0_slot04_09 = 0x801c7cf0;
data_801c7d14_slot04_09 = 0x801c7d14;
data_801c7d38_slot04_09 = 0x801c7d38;
data_801c7d74_slot04_09 = 0x801c7d74;
data_801c7d80_slot04_09 = 0x801c7d80;
data_801c7d88_slot04_09 = 0x801c7d88;
data_801c7d9c_slot04_09 = 0x801c7d9c;
data_801c7dac_slot04_09 = 0x801c7dac;
data_801c7db4_slot04_09 = 0x801c7db4;
data_801c7dbc_slot04_09 = 0x801c7dbc;
data_801c7dc4_slot04_09 = 0x801c7dc4;
data_801c7dd4_slot04_09 = 0x801c7dd4;
data_801c7e00_slot04_09 = 0x801c7e00;

/* Added by src.u_slot04_0a_016c. */
data_801c04f0_slot04_0a = 0x801c04f0;
data_801c04fc_slot04_0a = 0x801c04fc;
data_801c0548_slot04_0a = 0x801c0548;
data_801c0550_slot04_0a = 0x801c0550;
data_801c0558_slot04_0a = 0x801c0558;
data_801c05fc_slot04_0a = 0x801c05fc;
data_801c0614_slot04_0a = 0x801c0614;
data_801c0634_slot04_0a = 0x801c0634;
data_801c0654_slot04_0a = 0x801c0654;
data_801c0660_slot04_0a = 0x801c0660;
data_801c0678_slot04_0a = 0x801c0678;
data_801c0694_slot04_0a = 0x801c0694;
data_801c06b0_slot04_0a = 0x801c06b0;
data_801c06c4_slot04_0a = 0x801c06c4;
data_801c06d8_slot04_0a = 0x801c06d8;
data_801c06e4_slot04_0a = 0x801c06e4;
data_801c06ec_slot04_0a = 0x801c06ec;
data_801c06f4_slot04_0a = 0x801c06f4;
data_801c0704_slot04_0a = 0x801c0704;
data_801c0714_slot04_0a = 0x801c0714;
data_801c0724_slot04_0a = 0x801c0724;
data_801c0830_slot04_0a = 0x801c0830;
data_801c0840_slot04_0a = 0x801c0840;
data_801c0874_slot04_0a = 0x801c0874;
data_801c087c_slot04_0a = 0x801c087c;
data_801c0884_slot04_0a = 0x801c0884;
data_801c0914_slot04_0a = 0x801c0914;
data_801c0924_slot04_0a = 0x801c0924;

/* Added by src.u_slot04_0b_02a8. */
data_801c2480_slot04_0b = 0x801c2480;
data_801c248c_slot04_0b = 0x801c248c;
data_801c24a0_slot04_0b = 0x801c24a0;
data_801c24a8_slot04_0b = 0x801c24a8;
data_801c24cc_slot04_0b = 0x801c24cc;
data_801c24d4_slot04_0b = 0x801c24d4;
data_801c24e0_slot04_0b = 0x801c24e0;
data_801c24e8_slot04_0b = 0x801c24e8;
data_801c24f4_slot04_0b = 0x801c24f4;
data_801c2554_slot04_0b = 0x801c2554;
data_801c2580_slot04_0b = 0x801c2580;
data_801c2590_slot04_0b = 0x801c2590;
data_801c25a0_slot04_0b = 0x801c25a0;
data_801c25c0_slot04_0b = 0x801c25c0;
data_801c25e0_slot04_0b = 0x801c25e0;
data_801c263c_slot04_0b = 0x801c263c;
data_801c2650_slot04_0b = 0x801c2650;
data_801c2664_slot04_0b = 0x801c2664;
data_801c266c_slot04_0b = 0x801c266c;
data_801c267c_slot04_0b = 0x801c267c;
data_801c2ea8_slot04_0b = 0x801c2ea8;
data_801c2ee8_slot04_0b = 0x801c2ee8;
data_801c2f24_slot04_0b = 0x801c2f24;
data_801c2f3c_slot04_0b = 0x801c2f3c;
data_801c2f44_slot04_0b = 0x801c2f44;
data_801c2f4c_slot04_0b = 0x801c2f4c;
data_801c2fd4_slot04_0b = 0x801c2fd4;
data_801c2fe4_slot04_0b = 0x801c2fe4;
data_801c2ff0_slot04_0b = 0x801c2ff0;
data_801c2ff8_slot04_0b = 0x801c2ff8;
data_801c3008_slot04_0b = 0x801c3008;

/* Added by src.u_slot04_00_0008. */
data_801bfd3c_slot04_00 = 0x801bfd3c;
data_801bfd4c_slot04_00 = 0x801bfd4c;
data_801b3df0_slot04_00 = 0x801b3df0;
data_801b5db8_slot04_00 = 0x801b5db8;
data_801bfc8c_slot04_00 = 0x801bfc8c;

/* Added by src.u_slot04_01_0000. */
data_801b4904_slot04_01 = 0x801b4904;
data_801b66bc_slot04_01 = 0x801b66bc;
data_801bee00_slot04_01 = 0x801bee00;
data_801beeb0_slot04_01 = 0x801beeb0;
data_801beec0_slot04_01 = 0x801beec0;

/* Added by src.u_slot04_02_0000. */
data_801b7f84_slot04_02 = 0x801b7f84;
data_801ba32c_slot04_02 = 0x801ba32c;
data_801c60f8_slot04_02 = 0x801c60f8;

/* Added by src.u_slot04_03_0000. */
data_801b487c_slot04_03 = 0x801b487c;
data_801b661c_slot04_03 = 0x801b661c;
data_801c0460_slot04_03 = 0x801c0460;
data_801c0510_slot04_03 = 0x801c0510;
data_801c0520_slot04_03 = 0x801c0520;
data_801c0548_slot04_03 = 0x801c0548;

/* Added by src.u_slot04_05_0000. */
data_801b4448_slot04_05 = 0x801b4448;
data_801b6ba4_slot04_05 = 0x801b6ba4;
data_801c128c_slot04_05 = 0x801c128c;
data_801c1340_slot04_05 = 0x801c1340;
data_801c1350_slot04_05 = 0x801c1350;

/* Added by src.u_slot04_06_0000. */
data_801b6794_slot04_06 = 0x801b6794;
data_801b9058_slot04_06 = 0x801b9058;
data_801c5188_slot04_06 = 0x801c5188;

/* Added by src.u_slot04_07_0000. */
data_801b52b4_slot04_07 = 0x801b52b4;
data_801b7630_slot04_07 = 0x801b7630;
data_801c1d98_slot04_07 = 0x801c1d98;
data_801c1e54_slot04_07 = 0x801c1e54;
data_801c1e64_slot04_07 = 0x801c1e64;

/* Added by src.u_slot04_08_0000. */
data_801c3a98_slot04_08 = 0x801c3a98;
data_801b4d18_slot04_08 = 0x801b4d18;
data_801b7a18_slot04_08 = 0x801b7a18;
data_801c3b48_slot04_08 = 0x801c3b48;

/* Added by src.u_slot04_04_0000. */
data_801b4e04_slot04_04 = 0x801b4e04;
data_801b7384_slot04_04 = 0x801b7384;
data_801c40b0_slot04_04 = 0x801c40b0;

/* Added by src.u_slot04_09_0000. */
data_801b6608_slot04_09 = 0x801b6608;
data_801ba1b0_slot04_09 = 0x801ba1b0;
data_801c7ae0_slot04_09 = 0x801c7ae0;
data_801c7b9c_slot04_09 = 0x801c7b9c;

/* Added by src.u_slot04_0a_0000. */
data_801b5174_slot04_0a = 0x801b5174;
data_801b6e48_slot04_0a = 0x801b6e48;
data_801c0450_slot04_0a = 0x801c0450;
data_801c0560_slot04_0a = 0x801c0560;
data_801c0568_slot04_0a = 0x801c0568;

/* Added by src.u_slot04_0b_0000. */
data_801b4e5c_slot04_0b = 0x801b4e5c;
data_801b7274_slot04_0b = 0x801b7274;
data_801c23e0_slot04_0b = 0x801c23e0;

/* Added by src.u_slot04_01_07fc. */
data_801bef6c_slot04_01 = 0x801bef6c;

/* Added by src.u_slot04_00_1330. */
data_801bfe08_slot04_00 = 0x801bfe08;
data_801bfe40_slot04_00 = 0x801bfe40;

/* Added by src.u_slot04_00_1b10. */
data_801bfe4c_slot04_00 = 0x801bfe4c;
data_801bfe7c_slot04_00 = 0x801bfe7c;

/* Added by src.u_slot04_00_2218. */
data_801bfe9c_slot04_00 = 0x801bfe9c;

/* Added by src.u_slot04_00_2cc4. */
data_801bfc70_slot04_00 = 0x801bfc70;
data_801c0564_slot04_00 = 0x801c0564;
data_801c0168_slot04_00 = 0x801c0168;
data_801c0314_slot04_00 = 0x801c0314;
data_801c052c_slot04_00 = 0x801c052c;
data_801c0148_slot04_00 = 0x801c0148;

/* Added by src.u_slot04_01_12e0. */
data_801befa4_slot04_01 = 0x801befa4;

/* Added by src.u_slot04_01_1b9c. */
data_801befb0_slot04_01 = 0x801befb0;
data_801bf000_slot04_01 = 0x801bf000;
data_801befe8_slot04_01 = 0x801befe8;

/* Added by src.u_slot04_01_24c8. */
data_801bf09c_slot04_01 = 0x801bf09c;
data_801bf0a0_slot04_01 = 0x801bf0a0;
data_801bf0b4_slot04_01 = 0x801bf0b4;

/* Added by src.u_slot04_01_2f40. */
data_801bf120_slot04_01 = 0x801bf120;

/* Added by src.u_slot04_01_3738. */
data_801bf228_slot04_01 = 0x801bf228;
data_801bf240_slot04_01 = 0x801bf240;
data_801bf250_slot04_01 = 0x801bf250;

/* Added by src.u_slot04_01_44f4. */

/* Added by src.u_slot04_02_1fdc. */
data_801c6300_slot04_02 = 0x801c6300;
data_801c630c_slot04_02 = 0x801c630c;
data_801c6318_slot04_02 = 0x801c6318;
data_801c6334_slot04_02 = 0x801c6334;

/* Added by src.u_slot04_02_2b2c. */
data_801c6380_slot04_02 = 0x801c6380;
data_801c6384_slot04_02 = 0x801c6384;
data_801c6398_slot04_02 = 0x801c6398;

/* Added by src.u_slot04_02_32a8. */
data_801c63d4_slot04_02 = 0x801c63d4;

/* Added by src.u_slot04_02_44b4. */
data_801c64e8_slot04_02 = 0x801c64e8;
data_801c6500_slot04_02 = 0x801c6500;
data_801c6508_slot04_02 = 0x801c6508;

/* Added by src.u_slot04_02_53ac. */
data_801c6518_slot04_02 = 0x801c6518;
data_801c6520_slot04_02 = 0x801c6520;

/* Added by src.u_slot04_02_6f48. */

/* Added by src.u_slot04_03_1304. */
data_801c05e0_slot04_03 = 0x801c05e0;

/* Added by src.u_slot04_03_1ab0. */
data_801c0674_slot04_03 = 0x801c0674;

/* Added by src.u_slot04_03_2148. */
data_801c0708_slot04_03 = 0x801c0708;

/* Added by src.u_slot04_03_28c8. */
data_801c0744_slot04_03 = 0x801c0744;

/* Added by src.u_slot04_03_39d8. */
data_801c098c_slot04_03 = 0x801c098c;
data_801c09a4_slot04_03 = 0x801c09a4;
data_801c09b0_slot04_03 = 0x801c09b0;
data_801c09d0_slot04_03 = 0x801c09d0;
data_801c0aa8_slot04_03 = 0x801c0aa8;
data_801c0aac_slot04_03 = 0x801c0aac;

/* Added by src.u_slot04_03_42fc. */
data_801c0a08_slot04_03 = 0x801c0a08;

/* Added by src.u_slot04_04_2e68. */
data_801c4508_slot04_04 = 0x801c4508;
data_801c4520_slot04_04 = 0x801c4520;
data_801c452c_slot04_04 = 0x801c452c;

/* Added by src.u_slot04_05_15ac. */
data_801c1424_slot04_05 = 0x801c1424;
data_801c1454_slot04_05 = 0x801c1454;
data_801c1478_slot04_05 = 0x801c1478;

/* Added by src.u_slot04_05_1c70. */
data_801aa5e4 = 0x801aa5e4;
data_801c14c0_slot04_05 = 0x801c14c0;
data_801c14d8_slot04_05 = 0x801c14d8;
data_801c1528_slot04_05 = 0x801c1528;

/* Added by src.u_slot04_05_246c. */
data_801c1590_slot04_05 = 0x801c1590;
data_801c1598_slot04_05 = 0x801c1598;

/* Added by src.u_slot04_05_3984. */
data_801c1630_slot04_05 = 0x801c1630;
data_801c1700_slot04_05 = 0x801c1700;

/* Added by src.u_slot04_06_113c. */
data_801c531c_slot04_06 = 0x801c531c;
data_801c5320_slot04_06 = 0x801c5320;
data_801c532c_slot04_06 = 0x801c532c;

/* Added by src.u_slot04_06_183c. */
data_801c535c_slot04_06 = 0x801c535c;
data_801c5368_slot04_06 = 0x801c5368;
data_801c537c_slot04_06 = 0x801c537c;

/* Added by src.u_slot04_06_1eb8. */
data_801c5380_slot04_06 = 0x801c5380;

/* Added by src.u_slot04_06_2568. */
data_801c5408_slot04_06 = 0x801c5408;
data_801c5410_slot04_06 = 0x801c5410;

/* Added by src.u_slot04_06_2bd8. */
data_801c5460_slot04_06 = 0x801c5460;

/* Added by src.u_slot04_06_32e4. */
data_801c5478_slot04_06 = 0x801c5478;

/* Added by src.u_slot04_06_4064. */
data_801c54cc_slot04_06 = 0x801c54cc;
data_801c54d4_slot04_06 = 0x801c54d4;
data_801c54e8_slot04_06 = 0x801c54e8;

/* Added by src.u_slot04_06_4960. */
data_801c5510_slot04_06 = 0x801c5510;
data_801c5520_slot04_06 = 0x801c5520;
data_801c552c_slot04_06 = 0x801c552c;

/* Added by src.u_slot04_06_50e8. */
data_801c5758_slot04_06 = 0x801c5758;

/* Added by src.u_slot04_06_5ed8. */
data_801c575c_slot04_06 = 0x801c575c;
data_801c55e4_slot04_06 = 0x801c55e4;
data_801c5604_slot04_06 = 0x801c5604;
data_801c5624_slot04_06 = 0x801c5624;
data_801c56a4_slot04_06 = 0x801c56a4;

/* Added by src.u_slot04_07_1068. */
data_801c1f10_slot04_07 = 0x801c1f10;

/* Added by src.u_slot04_07_2300. */
data_801c1f80_slot04_07 = 0x801c1f80;

/* Added by src.u_slot04_07_2a30. */
data_801c1f94_slot04_07 = 0x801c1f94;
data_801c1fc4_slot04_07 = 0x801c1fc4;
data_801c1fdc_slot04_07 = 0x801c1fdc;
data_801c1ff4_slot04_07 = 0x801c1ff4;
data_801c2034_slot04_07 = 0x801c2034;
data_801c2060_slot04_07 = 0x801c2060;

/* Added by src.u_slot04_07_315c. */
data_801c2090_slot04_07 = 0x801c2090;
data_801c209c_slot04_07 = 0x801c209c;

/* Added by src.u_slot04_07_3874. */
data_801c20a8_slot04_07 = 0x801c20a8;

/* Added by src.u_slot04_07_4048. */
data_801c1d70_slot04_07 = 0x801c1d70;

/* Added by src.u_slot04_08_0d18. */
data_801c3bfc_slot04_08 = 0x801c3bfc;
data_801c3c04_slot04_08 = 0x801c3c04;
data_801c3e94_slot04_08 = 0x801c3e94;
data_801c3e98_slot04_08 = 0x801c3e98;

/* Added by src.u_slot04_08_16b0. */
data_801c3c24_slot04_08 = 0x801c3c24;
data_801c3c4c_slot04_08 = 0x801c3c4c;

/* Added by src.u_slot04_08_1ecc. */
data_801c3c54_slot04_08 = 0x801c3c54;
data_801c3c60_slot04_08 = 0x801c3c60;
data_801c3c90_slot04_08 = 0x801c3c90;

/* Added by src.u_slot04_08_25a8. */
data_801c3cb8_slot04_08 = 0x801c3cb8;
data_801c3cc4_slot04_08 = 0x801c3cc4;
data_801c3d48_slot04_08 = 0x801c3d48;

/* Added by src.u_slot04_08_3510. */
data_801c3d8c_slot04_08 = 0x801c3d8c;
data_801c3d9c_slot04_08 = 0x801c3d9c;
data_801c3da8_slot04_08 = 0x801c3da8;

/* Added by src.u_slot04_08_3edc. */
data_801c3e9c_slot04_08 = 0x801c3e9c;
data_801c3ea0_slot04_08 = 0x801c3ea0;

/* Added by src.u_slot04_08_47d0. */
data_801c3ea4_slot04_08 = 0x801c3ea4;
data_801c3ea8_slot04_08 = 0x801c3ea8;

/* Added by src.u_slot04_09_1924. */
data_801c7c44_slot04_09 = 0x801c7c44;
data_801c7c98_slot04_09 = 0x801c7c98;

/* Added by src.u_slot04_09_2034. */
data_801c7cb0_slot04_09 = 0x801c7cb0;
data_801c7cb8_slot04_09 = 0x801c7cb8;

/* Added by src.u_slot04_09_352c. */
data_801c7d64_slot04_09 = 0x801c7d64;

/* Added by src.u_slot04_09_4548. */
data_801c7de4_slot04_09 = 0x801c7de4;
data_801c7de8_slot04_09 = 0x801c7de8;

/* Added by src.u_slot04_09_6068. */
data_801c7e24_slot04_09 = 0x801c7e24;

/* Added by src.u_slot04_0a_11bc. */
data_801c0608_slot04_0a = 0x801c0608;

/* Added by src.u_slot04_0a_1964. */
data_801c0424_slot04_0a = 0x801c0424;

/* Added by src.u_slot04_0a_2048. */
data_801c0688_slot04_0a = 0x801c0688;
data_801c068b_slot04_0a = 0x801c068b;

/* Added by src.u_slot04_0a_26c4. */
data_801c0690_slot04_0a = 0x801c0690;
data_801c06ac_slot04_0a = 0x801c06ac;

/* Added by src.u_slot04_0a_3e44. */
data_801c0828_slot04_0a = 0x801c0828;
data_801c0854_slot04_0a = 0x801c0854;

/* Added by src.u_slot04_0b_0bfc. */
data_801c2568_slot04_0b = 0x801c2568;

/* Added by src.u_slot04_0b_1ed8. */
data_801c25dc_slot04_0b = 0x801c25dc;

/* Added by src.u_slot04_0b_2594. */
data_801c2608_slot04_0b = 0x801c2608;
data_801c260c_slot04_0b = 0x801c260c;

/* Added by src.u_slot04_0b_3850. */
data_801c2f0c_slot04_0b = 0x801c2f0c;
data_801c2e14_slot04_0b = 0x801c2e14;
data_801c2e4c_slot04_0b = 0x801c2e4c;
data_801c2e90_slot04_0b = 0x801c2e90;
data_801c284c_slot04_0b = 0x801c284c;
data_801c2eb8_slot04_0b = 0x801c2eb8;
data_801c2ebc_slot04_0b = 0x801c2ebc;
data_801c2ee0_slot04_0b = 0x801c2ee0;

/* Added by src.u_slot04_01_0614. */
data_801bede4_slot04_01 = 0x801bede4;

/* Added by src.u_slot04_05_0d94. */
data_801c15b0_slot04_05 = 0x801c15b0;

/* Added by src.u_slot04_07_013c. */
data_801c1e38_slot04_07 = 0x801c1e38;
data_801c1e44_slot04_07 = 0x801c1e44;

/* Added by src.u_slot04_08_0100. */
data_801c3b38_slot04_08 = 0x801c3b38;

/* Added by src.u_slot04_0a_085c. */
data_801c0404_slot04_0a = 0x801c0404;

/* Added by src.u_slot04_0b_0690. */
data_801c24b8_slot04_0b = 0x801c24b8;
data_801c24c8_slot04_0b = 0x801c24c8;
data_801c2efc_slot04_0b = 0x801c2efc;
data_801c2f04_slot04_0b = 0x801c2f04;

/* A parked function that an exact unit calls. */
func_801b3a70_slot04_05 = 0x801b3a70;

/* Added by src.u_slot04_00_1d2cx. */
data_801c55b4_slot04_06 = 0x801c55b4;
data_801c55d4_slot04_06 = 0x801c55d4;
data_801bfe60_slot04_00 = 0x801bfe60;

/* Added by src.u_slot04_01_2108x. */
data_801befc4_slot04_01 = 0x801befc4;
data_801beff4_slot04_01 = 0x801beff4;
data_801beffc_slot04_01 = 0x801beffc;

/* Added by src.u_slot04_03_1eb4x. */
data_801c0678_slot04_03 = 0x801c0678;

/* Added by src.u_slot04_06_22e4x. */

/* Added by src.u_slot04_06_5b68x. */
data_801c7b94_slot04_09 = 0x801c7b94;
data_801c0508_slot04_0a = 0x801c0508;
data_801c0670_slot04_0a = 0x801c0670;

/* Added by src.u_slot04_07_51dcx. */
data_801c2134_slot04_07 = 0x801c2134;
data_8019054e = 0x8019054e;
data_801ef494_slot06_0e = 0x801ef494;
data_801ef4a4_slot06_0e = 0x801ef4a4;

/* Added by src.u_slot04_sel_0000. */
data_801b6d50_slot04_sel = 0x801b6d50;
data_801b6d60_slot04_sel = 0x801b6d60;
data_801b6d6c_slot04_sel = 0x801b6d6c;
data_801b6d78_slot04_sel = 0x801b6d78;
data_801b6e88_slot04_sel = 0x801b6e88;
data_801b7c34_slot04_sel = 0x801b7c34;
data_801b7c44_slot04_sel = 0x801b7c44;
data_801b7c50_slot04_sel = 0x801b7c50;
data_801b7c5c_slot04_sel = 0x801b7c5c;
data_801b7eb8_slot04_sel = 0x801b7eb8;
data_801b8c98_slot04_sel = 0x801b8c98;
data_801b8ca8_slot04_sel = 0x801b8ca8;
data_801b8cb0_slot04_sel = 0x801b8cb0;
data_801b8cbc_slot04_sel = 0x801b8cbc;
data_801b8df4_slot04_sel = 0x801b8df4;

/* Added by src.u_slot04_sel_0090. */
data_801b9cf0_slot04_sel = 0x801b9cf0;
data_801b9d24_slot04_sel = 0x801b9d24;
data_801b7570_slot04_sel = 0x801b7570;
data_801b7618_slot04_sel = 0x801b7618;
data_801b7654_slot04_sel = 0x801b7654;
data_801b7348_slot04_sel = 0x801b7348;

/* Added by src.u_slot04_sel_066c. */
data_801b6d88_slot04_sel = 0x801b6d88;
data_801b7034_slot04_sel = 0x801b7034;
data_801b7058_slot04_sel = 0x801b7058;
data_801b707c_slot04_sel = 0x801b707c;
data_801b70a0_slot04_sel = 0x801b70a0;
data_801b70c4_slot04_sel = 0x801b70c4;
data_801b70e8_slot04_sel = 0x801b70e8;
data_801b710c_slot04_sel = 0x801b710c;
data_801b7130_slot04_sel = 0x801b7130;
data_801b7154_slot04_sel = 0x801b7154;
data_801b7178_slot04_sel = 0x801b7178;
data_801b719c_slot04_sel = 0x801b719c;
data_801b71c0_slot04_sel = 0x801b71c0;
data_801b71e4_slot04_sel = 0x801b71e4;
data_801b7208_slot04_sel = 0x801b7208;
data_801b722c_slot04_sel = 0x801b722c;
data_801b7250_slot04_sel = 0x801b7250;
data_801b7274_slot04_sel = 0x801b7274;
data_801b7298_slot04_sel = 0x801b7298;
data_801b7364_slot04_sel = 0x801b7364;
data_801b7978_slot04_sel = 0x801b7978;
data_801b797c_slot04_sel = 0x801b797c;
data_801b797e_slot04_sel = 0x801b797e;

/* Added by src.u_slot04_sel_08b0. */
data_801b9d28_slot04_sel = 0x801b9d28;
data_801b9cec_slot04_sel = 0x801b9cec;
data_801b9ce8_slot04_sel = 0x801b9ce8;
data_801b6d8c_slot04_sel = 0x801b6d8c;
data_801b6de0_slot04_sel = 0x801b6de0;
data_801b6e4c_slot04_sel = 0x801b6e4c;
data_801b6e68_slot04_sel = 0x801b6e68;
data_801b6e70_slot04_sel = 0x801b6e70;
data_801b6f1c_slot04_sel = 0x801b6f1c;
data_801b6fcc_slot04_sel = 0x801b6fcc;
data_801b701c_slot04_sel = 0x801b701c;
data_801b72f4_slot04_sel = 0x801b72f4;
data_801b74cc_slot04_sel = 0x801b74cc;
data_801b791c_slot04_sel = 0x801b791c;

/* Added by src.u_slot04_sel_0ea0. */
data_801b9d2c_slot04_sel = 0x801b9d2c;
data_801b7648_slot04_sel = 0x801b7648;
data_8016e694 = 0x8016e694;
data_801b749c_slot04_sel = 0x801b749c;
data_801b7504_slot04_sel = 0x801b7504;
data_801b6f64_slot04_sel = 0x801b6f64;
data_801b6e7c_slot04_sel = 0x801b6e7c;
data_801b7350_slot04_sel = 0x801b7350;

/* Added by src.u_slot04_sel_13b8. */
data_801b7ba4_slot04_sel = 0x801b7ba4;
data_801b6e80_slot04_sel = 0x801b6e80;
data_801b734c_slot04_sel = 0x801b734c;
data_8016e696 = 0x8016e696;

/* Added by src.u_slot04_sel_1a8c. */
data_801b6e94_slot04_sel = 0x801b6e94;

/* Added by src.u_slot04_sel_2008. */
data_801b9d64_slot04_sel = 0x801b9d64;
data_8016e698 = 0x8016e698;
data_801b9d38_slot04_sel = 0x801b9d38;
data_801b6eb4_slot04_sel = 0x801b6eb4;
data_801b6ebc_slot04_sel = 0x801b6ebc;
data_801b6ec4_slot04_sel = 0x801b6ec4;

/* Added by src.u_slot04_sel_2580. */
data_8016e69a = 0x8016e69a;
data_801b8320_slot04_sel = 0x801b8320;
data_801b8540_slot04_sel = 0x801b8540;
data_801b85e8_slot04_sel = 0x801b85e8;
data_801b8618_slot04_sel = 0x801b8618;
data_801b9d3b_slot04_sel = 0x801b9d3b;
data_801b9d4d_slot04_sel = 0x801b9d4d;
data_801b7c6c_slot04_sel = 0x801b7c6c;
data_801b800c_slot04_sel = 0x801b800c;
data_801b833c_slot04_sel = 0x801b833c;
data_801b89d4_slot04_sel = 0x801b89d4;
data_801b9d68_slot04_sel = 0x801b9d68;
data_801b9d6c_slot04_sel = 0x801b9d6c;
data_801b8030_slot04_sel = 0x801b8030;
data_801b8054_slot04_sel = 0x801b8054;
data_801b8078_slot04_sel = 0x801b8078;
data_801b809c_slot04_sel = 0x801b809c;
data_801b80c0_slot04_sel = 0x801b80c0;
data_801b80e4_slot04_sel = 0x801b80e4;
data_801b8108_slot04_sel = 0x801b8108;
data_801b812c_slot04_sel = 0x801b812c;
data_801b8150_slot04_sel = 0x801b8150;
data_801b8174_slot04_sel = 0x801b8174;
data_801b8198_slot04_sel = 0x801b8198;
data_801b81bc_slot04_sel = 0x801b81bc;
data_801b81e0_slot04_sel = 0x801b81e0;
data_801b8204_slot04_sel = 0x801b8204;
data_801b8228_slot04_sel = 0x801b8228;
data_801b824c_slot04_sel = 0x801b824c;
data_801b8270_slot04_sel = 0x801b8270;
data_801b89d8_slot04_sel = 0x801b89d8;
data_801b89da_slot04_sel = 0x801b89da;

/* Added by src.u_slot04_sel_2b24. */
data_801b7f64_slot04_sel = 0x801b7f64;
data_801b7ff4_slot04_sel = 0x801b7ff4;
data_801b7fa4_slot04_sel = 0x801b7fa4;
data_801b84a4_slot04_sel = 0x801b84a4;
data_801b82cc_slot04_sel = 0x801b82cc;
data_801b8978_slot04_sel = 0x801b8978;
data_801b9d30_slot04_sel = 0x801b9d30;
data_801b9d34_slot04_sel = 0x801b9d34;
data_801b7cc4_slot04_sel = 0x801b7cc4;
data_801b7c70_slot04_sel = 0x801b7c70;
data_801b7d30_slot04_sel = 0x801b7d30;

/* Added by src.u_slot04_sel_310c. */
data_801b7d4c_slot04_sel = 0x801b7d4c;
data_801b7d54_slot04_sel = 0x801b7d54;
data_801b7d5c_slot04_sel = 0x801b7d5c;
data_801b7d64_slot04_sel = 0x801b7d64;

/* Added by src.u_slot04_sel_3380. */
data_8016e69c = 0x8016e69c;
data_8016e69d = 0x8016e69d;
data_801b7d7c_slot04_sel = 0x801b7d7c;
data_801b7d88_slot04_sel = 0x801b7d88;
data_801b7d90_slot04_sel = 0x801b7d90;
data_801b7ef4_slot04_sel = 0x801b7ef4;
data_801b84b4_slot04_sel = 0x801b84b4;
data_801b84cc_slot04_sel = 0x801b84cc;
data_801b84e8_slot04_sel = 0x801b84e8;

/* Added by src.u_slot04_sel_3828. */
data_801b7d98_slot04_sel = 0x801b7d98;
data_801b7d9c_slot04_sel = 0x801b7d9c;
data_801b7ddc_slot04_sel = 0x801b7ddc;
data_801b7de4_slot04_sel = 0x801b7de4;
data_801b7df0_slot04_sel = 0x801b7df0;
data_801b8328_slot04_sel = 0x801b8328;
data_801b8570_slot04_sel = 0x801b8570;
data_801b8474_slot04_sel = 0x801b8474;

/* Added by src.u_slot04_sel_3cec. */
data_801b7df4_slot04_sel = 0x801b7df4;
data_801b7e60_slot04_sel = 0x801b7e60;
data_801b7e8c_slot04_sel = 0x801b7e8c;
data_801b8324_slot04_sel = 0x801b8324;

/* Added by src.u_slot04_sel_4298. */
data_801b7ec4_slot04_sel = 0x801b7ec4;
data_801b7ecc_slot04_sel = 0x801b7ecc;
data_801b8c00_slot04_sel = 0x801b8c00;

/* Added by src.u_slot04_sel_48f4. */
data_801b7ed4_slot04_sel = 0x801b7ed4;
data_801b9c10_slot04_sel = 0x801b9c10;
data_801b9d78_slot04_sel = 0x801b9d78;
data_801b9da4_slot04_sel = 0x801b9da4;

/* Added by src.u_slot04_sel_4ec0. */
data_801b8cc8_slot04_sel = 0x801b8cc8;
data_801b8f74_slot04_sel = 0x801b8f74;
data_801b8fc4_slot04_sel = 0x801b8fc4;
data_801b8fe0_slot04_sel = 0x801b8fe0;
data_801b9da8_slot04_sel = 0x801b9da8;
data_801b9dac_slot04_sel = 0x801b9dac;
data_801b9004_slot04_sel = 0x801b9004;
data_801b9028_slot04_sel = 0x801b9028;
data_801b904c_slot04_sel = 0x801b904c;
data_801b9070_slot04_sel = 0x801b9070;
data_801b9094_slot04_sel = 0x801b9094;
data_801b90b8_slot04_sel = 0x801b90b8;
data_801b90dc_slot04_sel = 0x801b90dc;
data_801b9100_slot04_sel = 0x801b9100;
data_801b9124_slot04_sel = 0x801b9124;
data_801b9148_slot04_sel = 0x801b9148;
data_801b916c_slot04_sel = 0x801b916c;
data_801b9190_slot04_sel = 0x801b9190;
data_801b91b4_slot04_sel = 0x801b91b4;
data_801b91d8_slot04_sel = 0x801b91d8;
data_801b91fc_slot04_sel = 0x801b91fc;
data_801b9220_slot04_sel = 0x801b9220;
data_801b9244_slot04_sel = 0x801b9244;
data_801b9310_slot04_sel = 0x801b9310;
data_801b9330_slot04_sel = 0x801b9330;
data_801b9358_slot04_sel = 0x801b9358;

/* Added by src.u_slot04_sel_5224. */
data_801b8da8_slot04_sel = 0x801b8da8;
data_801b8fc8_slot04_sel = 0x801b8fc8;
data_801b8f78_slot04_sel = 0x801b8f78;
data_801b94c0_slot04_sel = 0x801b94c0;
data_801b9d70_slot04_sel = 0x801b9d70;
data_801b9d74_slot04_sel = 0x801b9d74;
data_801b8d20_slot04_sel = 0x801b8d20;
data_801b8ccc_slot04_sel = 0x801b8ccc;
data_801b8d8c_slot04_sel = 0x801b8d8c;
data_801b92a0_slot04_sel = 0x801b92a0;
data_801b9908_slot04_sel = 0x801b9908;

/* Added by src.u_slot04_sel_57c8. */
data_801b8db0_slot04_sel = 0x801b8db0;
data_801b8db8_slot04_sel = 0x801b8db8;
data_801b8dc0_slot04_sel = 0x801b8dc0;
data_801b8dd8_slot04_sel = 0x801b8dd8;
data_801b8eac_slot04_sel = 0x801b8eac;
data_801b8edc_slot04_sel = 0x801b8edc;
data_801b8f00_slot04_sel = 0x801b8f00;
data_801b8f30_slot04_sel = 0x801b8f30;
data_801b8c91_slot04_sel = 0x801b8c91;

/* Added by src.u_slot04_sel_5b0c. */
data_801b9964_slot04_sel = 0x801b9964;
data_801b9b90_slot04_sel = 0x801b9b90;
data_801b8c92_slot04_sel = 0x801b8c92;
data_801b8c93_slot04_sel = 0x801b8c93;
data_801b8c94_slot04_sel = 0x801b8c94;
data_801b8ddc_slot04_sel = 0x801b8ddc;
data_801b8de4_slot04_sel = 0x801b8de4;
data_801b8de8_slot04_sel = 0x801b8de8;
data_801b8dec_slot04_sel = 0x801b8dec;
data_801b8df0_slot04_sel = 0x801b8df0;

/* Added by src.u_slot04_sel_61c0. */
data_801b8e00_slot04_sel = 0x801b8e00;
data_801b8e08_slot04_sel = 0x801b8e08;
data_801b8e10_slot04_sel = 0x801b8e10;
data_801b9c20_slot04_sel = 0x801b9c20;
data_801b9c24_slot04_sel = 0x801b9c24;
data_801b9cd8_slot04_sel = 0x801b9cd8;
data_801b9db8_slot04_sel = 0x801b9db8;

/* Added by src.u_slot04_sel_6738. */
data_801b9c28_slot04_sel = 0x801b9c28;
data_801b9c30_slot04_sel = 0x801b9c30;
data_801b9c38_slot04_sel = 0x801b9c38;
data_801b9c40_slot04_sel = 0x801b9c40;
data_801b9db0_slot04_sel = 0x801b9db0;
data_801b9db4_slot04_sel = 0x801b9db4;

data_80051900_slot28 = 0x80051900;

data_801c7514_slot04_02 = 0x801c7514;
data_801c7748_slot04_02 = 0x801c7748;
data_801c7754_slot04_02 = 0x801c7754;
data_801c7760_slot04_02 = 0x801c7760;
data_801c779c_slot04_02 = 0x801c779c;
func_8013172c = 0x8013172c;
func_80010840_slot27 = 0x80010840;

data_801bfd54_slot04_00 = 0x801bfd54;
data_801bfd60_slot04_00 = 0x801bfd60;
data_801bfd78_slot04_00 = 0x801bfd78;
data_801bfda0_slot04_00 = 0x801bfda0;
data_801bfdc8_slot04_00 = 0x801bfdc8;
data_801bfed0_slot04_00 = 0x801bfed0;
data_801c0574_slot04_00 = 0x801c0574;
data_801c0584_slot04_00 = 0x801c0584;
data_801c0590_slot04_00 = 0x801c0590;
func_801b1008_slot04_01 = 0x801b1008;
data_801beec4_slot04_01 = 0x801beec4;
data_801beed0_slot04_01 = 0x801beed0;
data_801beefc_slot04_01 = 0x801beefc;
data_801bef28_slot04_01 = 0x801bef28;
data_801bef54_slot04_01 = 0x801bef54;
data_801bef84_slot04_01 = 0x801bef84;
data_801bf10c_slot04_01 = 0x801bf10c;
data_801bf128_slot04_01 = 0x801bf128;
data_801bf270_slot04_01 = 0x801bf270;
data_801bf288_slot04_01 = 0x801bf288;
data_801c59b0_slot04_02 = 0x801c59b0;
data_801c6198_slot04_02 = 0x801c6198;
data_801c61a4_slot04_02 = 0x801c61a4;
data_801c61d8_slot04_02 = 0x801c61d8;
data_801c6218_slot04_02 = 0x801c6218;
data_801c6258_slot04_02 = 0x801c6258;
data_801c64b8_slot04_02 = 0x801c64b8;
data_801c64f4_slot04_02 = 0x801c64f4;
data_801c64f8_slot04_02 = 0x801c64f8;
data_801c650c_slot04_02 = 0x801c650c;
data_801c6568_slot04_02 = 0x801c6568;
data_801c6578_slot04_02 = 0x801c6578;
data_801c6594_slot04_02 = 0x801c6594;
data_801c7684_slot04_02 = 0x801c7684;
data_801c7694_slot04_02 = 0x801c7694;
data_801c76ac_slot04_02 = 0x801c76ac;
data_801c76cc_slot04_02 = 0x801c76cc;
data_801c76dc_slot04_02 = 0x801c76dc;
data_801c76f4_slot04_02 = 0x801c76f4;
data_801c7714_slot04_02 = 0x801c7714;
data_801c7718_slot04_02 = 0x801c7718;
data_801c7720_slot04_02 = 0x801c7720;
data_801c772c_slot04_02 = 0x801c772c;
data_801c7730_slot04_02 = 0x801c7730;
data_801c7734_slot04_02 = 0x801c7734;
data_801c77d8_slot04_02 = 0x801c77d8;
data_801c77ec_slot04_02 = 0x801c77ec;
data_801c7808_slot04_02 = 0x801c7808;
data_801c7820_slot04_02 = 0x801c7820;
data_801c7830_slot04_02 = 0x801c7830;
data_801c7884_slot04_02 = 0x801c7884;
data_801c7b44_slot04_02 = 0x801c7b44;
data_801c0528_slot04_03 = 0x801c0528;
data_801c0530_slot04_03 = 0x801c0530;
data_801c0568_slot04_03 = 0x801c0568;
data_801c058c_slot04_03 = 0x801c058c;
data_801c05b0_slot04_03 = 0x801c05b0;
data_801c09d8_slot04_03 = 0x801c09d8;
func_801b0904_slot04_04 = 0x801b0904;
data_801c4164_slot04_04 = 0x801c4164;
data_801c4180_slot04_04 = 0x801c4180;
data_801c4188_slot04_04 = 0x801c4188;
data_801c41a8_slot04_04 = 0x801c41a8;
data_801c41d4_slot04_04 = 0x801c41d4;
data_801c4210_slot04_04 = 0x801c4210;
data_801c4220_slot04_04 = 0x801c4220;
data_801c4224_slot04_04 = 0x801c4224;
data_801c42b8_slot04_04 = 0x801c42b8;
data_801c4304_slot04_04 = 0x801c4304;
data_801c4344_slot04_04 = 0x801c4344;
data_801c4374_slot04_04 = 0x801c4374;
data_801c4390_slot04_04 = 0x801c4390;
data_801c43c0_slot04_04 = 0x801c43c0;
data_801c43dc_slot04_04 = 0x801c43dc;
data_801c4550_slot04_04 = 0x801c4550;
data_801c4554_slot04_04 = 0x801c4554;
data_801c4590_slot04_04 = 0x801c4590;
data_801c45d8_slot04_04 = 0x801c45d8;
data_801c4670_slot04_04 = 0x801c4670;
data_801c139c_slot04_05 = 0x801c139c;
data_801c13c4_slot04_05 = 0x801c13c4;
data_801c13ec_slot04_05 = 0x801c13ec;
data_801c1648_slot04_05 = 0x801c1648;
data_801c528c_slot04_06 = 0x801c528c;
data_801c52b8_slot04_06 = 0x801c52b8;
data_801c1e6c_slot04_07 = 0x801c1e6c;
data_801c1e8c_slot04_07 = 0x801c1e8c;
data_801c1ea0_slot04_07 = 0x801c1ea0;
data_801c1ec8_slot04_07 = 0x801c1ec8;
data_801c1f40_slot04_07 = 0x801c1f40;
data_801c1f64_slot04_07 = 0x801c1f64;
data_801c2014_slot04_07 = 0x801c2014;
data_801c20e8_slot04_07 = 0x801c20e8;
data_801c20f8_slot04_07 = 0x801c20f8;
data_801c2110_slot04_07 = 0x801c2110;
data_801c3b70_slot04_08 = 0x801c3b70;
data_801c3b98_slot04_08 = 0x801c3b98;
data_801c3bc0_slot04_08 = 0x801c3bc0;
data_801c3db4_slot04_08 = 0x801c3db4;
data_801c3dd4_slot04_08 = 0x801c3dd4;
data_801c3dfc_slot04_08 = 0x801c3dfc;
data_801c7be0_slot04_09 = 0x801c7be0;
data_801c7c0c_slot04_09 = 0x801c7c0c;
data_801c7cdc_slot04_09 = 0x801c7cdc;
data_801c7d6c_slot04_09 = 0x801c7d6c;
data_801c7e14_slot04_09 = 0x801c7e14;
data_801c0578_slot04_0a = 0x801c0578;
data_801c05a4_slot04_0a = 0x801c05a4;
data_801c05d0_slot04_0a = 0x801c05d0;
data_801c0864_slot04_0a = 0x801c0864;
data_801c24fc_slot04_0b = 0x801c24fc;
data_801c2528_slot04_0b = 0x801c2528;
data_801c2f2c_slot04_0b = 0x801c2f2c;

data_801ce790_slot05_06 = 0x801ce790;
data_801d1054_slot05_06 = 0x801d1054;
data_801dd184_slot05_06 = 0x801dd184;
data_801dd224_slot05_06 = 0x801dd224;
data_801dd230_slot05_06 = 0x801dd230;
data_801dd238_slot05_06 = 0x801dd238;
data_801dd244_slot05_06 = 0x801dd244;
data_801dd24c_slot05_06 = 0x801dd24c;
data_801dd258_slot05_06 = 0x801dd258;
data_801dd264_slot05_06 = 0x801dd264;
data_801dd26c_slot05_06 = 0x801dd26c;
data_801dd278_slot05_06 = 0x801dd278;
data_801dd280_slot05_06 = 0x801dd280;
data_801dd288_slot05_06 = 0x801dd288;
data_801dd2b4_slot05_06 = 0x801dd2b4;
data_801dd2e4_slot05_06 = 0x801dd2e4;
data_801dd318_slot05_06 = 0x801dd318;
data_801dd31c_slot05_06 = 0x801dd31c;
data_801dd328_slot05_06 = 0x801dd328;
data_801dd358_slot05_06 = 0x801dd358;
data_801dd364_slot05_06 = 0x801dd364;
data_801dd368_slot05_06 = 0x801dd368;
data_801dd378_slot05_06 = 0x801dd378;
data_801dd37c_slot05_06 = 0x801dd37c;
data_801dd388_slot05_06 = 0x801dd388;
data_801dd3ac_slot05_06 = 0x801dd3ac;
data_801dd404_slot05_06 = 0x801dd404;
data_801dd40c_slot05_06 = 0x801dd40c;
data_801dd414_slot05_06 = 0x801dd414;
data_801dd428_slot05_06 = 0x801dd428;
data_801dd45c_slot05_06 = 0x801dd45c;
data_801dd474_slot05_06 = 0x801dd474;
data_801dd484_slot05_06 = 0x801dd484;
data_801dd498_slot05_06 = 0x801dd498;
data_801dd4a0_slot05_06 = 0x801dd4a0;
data_801dd4ac_slot05_06 = 0x801dd4ac;
data_801dd4bc_slot05_06 = 0x801dd4bc;
data_801dd4c8_slot05_06 = 0x801dd4c8;
data_801dd4d0_slot05_06 = 0x801dd4d0;
data_801dd4d8_slot05_06 = 0x801dd4d8;
data_801dd4e4_slot05_06 = 0x801dd4e4;
data_801dd4ec_slot05_06 = 0x801dd4ec;
data_801dd4f8_slot05_06 = 0x801dd4f8;
data_801dd50c_slot05_06 = 0x801dd50c;
data_801dd51c_slot05_06 = 0x801dd51c;
data_801dd528_slot05_06 = 0x801dd528;
data_801dd53c_slot05_06 = 0x801dd53c;
data_801dd54c_slot05_06 = 0x801dd54c;
data_801dd558_slot05_06 = 0x801dd558;
data_801dd560_slot05_06 = 0x801dd560;
data_801dd56c_slot05_06 = 0x801dd56c;
data_801dd574_slot05_06 = 0x801dd574;
data_801dd580_slot05_06 = 0x801dd580;
data_801dd58c_slot05_06 = 0x801dd58c;
data_801dd594_slot05_06 = 0x801dd594;
data_801dd5a0_slot05_06 = 0x801dd5a0;
data_801dd5a8_slot05_06 = 0x801dd5a8;
data_801dd5b0_slot05_06 = 0x801dd5b0;
data_801dd5c0_slot05_06 = 0x801dd5c0;
data_801dd5d0_slot05_06 = 0x801dd5d0;
data_801dd5e0_slot05_06 = 0x801dd5e0;
data_801dd600_slot05_06 = 0x801dd600;
data_801dd620_slot05_06 = 0x801dd620;
data_801dd6a0_slot05_06 = 0x801dd6a0;
data_801dd6c0_slot05_06 = 0x801dd6c0;
data_801dd6cc_slot05_06 = 0x801dd6cc;
data_801dd754_slot05_06 = 0x801dd754;
data_801dd758_slot05_06 = 0x801dd758;

data_80051c54_slot28 = 0x80051c54;

data_801c25b4_slot04_0b = 0x801c25b4;
func_80158374 = 0x80158374;
SetDrawEnv = 0x80158a84;

/* Data that the nonmatching functions name. */
data_801e9f48_slot06_00 = 0x801e9f48;
data_801e9f60_slot06_00 = 0x801e9f60;
data_801c6190_slot04_0e = 0x801c6190;
data_801c6194_slot04_0e = 0x801c6194;
data_801c6198_slot04_0e = 0x801c6198;
data_801c5500_slot04_0f = 0x801c5500;
