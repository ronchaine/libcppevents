/*!
 *  \file       keyboard_codes.hpp
 *  \brief      keyboard codes used by libcppevents
 *  \author     Jari Ronkainen
 *  \version    0.9
 */
#ifndef LIBCPPEVENTS_KEYBOARD_CODES_HPP
#define LIBCPPEVENTS_KEYBOARD_CODES_HPP

#include <cstdint>

namespace cppevents::kb
{
    /*!
     *  \brief enumeration for keyboard scancodes
     *
     *  The scancodes follow USB HID Usage tables,
     *  0x10000+ are reserved for custom scancodes.
     *
     *  USB itself reserves 0xE8-0xFFFF, so those
     *  are left unused
     */
    enum class scancode : uint32_t
    {
        key_none                = 0x00,
        key_a                   = 0x04,
        key_b                   = 0x05,
        key_c                   = 0x06,
        key_d                   = 0x07,
        key_e                   = 0x08,
        key_f                   = 0x09,
        key_g                   = 0x0a,
        key_h                   = 0x0b,
        key_i                   = 0x0c,
        key_j                   = 0x0d,
        key_k                   = 0x0e,
        key_l                   = 0x0f,
        key_m                   = 0x10,
        key_n                   = 0x11,
        key_o                   = 0x12,
        key_p                   = 0x13,
        key_q                   = 0x14,
        key_r                   = 0x15,
        key_s                   = 0x16,
        key_t                   = 0x17,
        key_u                   = 0x18,
        key_v                   = 0x19,
        key_w                   = 0x1a,
        key_x                   = 0x1b,
        key_y                   = 0x1c,
        key_z                   = 0x1d,

        key_1                   = 0x1e,
        key_2                   = 0x1f,
        key_3                   = 0x20,
        key_4                   = 0x21,
        key_5                   = 0x22,
        key_6                   = 0x23,
        key_7                   = 0x24,
        key_8                   = 0x25,
        key_9                   = 0x26,
        key_0                   = 0x27,

        key_enter               = 0x28,
        key_esc                 = 0x29,
        key_backspace           = 0x2a,
        key_tab                 = 0x2b,
        key_space               = 0x2c,
        key_minus               = 0x2d,
        key_equal               = 0x2e,
        key_leftbrace           = 0x2f,
        key_rightbrace          = 0x30,
        key_backslash           = 0x31,
        key_hashtilde           = 0x32,
        key_semicolon           = 0x33,
        key_apostrophe          = 0x34,
        key_grave               = 0x35,
        key_comma               = 0x36,
        key_dot                 = 0x37,
        key_slash               = 0x38,
        key_capslock            = 0x39,

        key_f1                  = 0x3a,
        key_f2                  = 0x3b,
        key_f3                  = 0x3c,
        key_f4                  = 0x3d,
        key_f5                  = 0x3e,
        key_f6                  = 0x3f,
        key_f7                  = 0x40,
        key_f8                  = 0x41,
        key_f9                  = 0x42,
        key_f10                 = 0x43,
        key_f11                 = 0x44,
        key_f12                 = 0x45,

        key_sysrq               = 0x46,
        key_scrolllock          = 0x47,
        key_pause               = 0x48,
        key_insert              = 0x49,
        key_home                = 0x4a,
        key_pageup              = 0x4b,
        key_delete              = 0x4c,
        key_end                 = 0x4d,
        key_pagedown            = 0x4e,

        key_right               = 0x4f,
        key_left                = 0x50,
        key_down                = 0x51,
        key_up                  = 0x52,

        key_numlock             = 0x53,
        key_kp_slash            = 0x54,
        key_kp_asterisk         = 0x55,
        key_kp_minus            = 0x56,
        key_kp_plus             = 0x57,
        key_kp_enter            = 0x58,

        key_kp_1                = 0x59,
        key_kp_2                = 0x5a,
        key_kp_3                = 0x5b,
        key_kp_4                = 0x5c,
        key_kp_5                = 0x5d,
        key_kp_6                = 0x5e,
        key_kp_7                = 0x5f,
        key_kp_8                = 0x60,
        key_kp_9                = 0x61,
        key_kp_0                = 0x62,
        key_kp_dot              = 0x63,

        key_102nd               = 0x64,
        key_compose             = 0x65,
        key_power               = 0x66,
        key_kp_equal            = 0x67,

        key_f13                 = 0x68,
        key_f14                 = 0x69,
        key_f15                 = 0x6a,
        key_f16                 = 0x6b,
        key_f17                 = 0x6c,
        key_f18                 = 0x6d,
        key_f19                 = 0x6e,
        key_f20                 = 0x6f,
        key_f21                 = 0x70,
        key_f22                 = 0x71,
        key_f23                 = 0x72,
        key_f24                 = 0x73,

        key_open                = 0x74,
        key_help                = 0x75,
        key_props               = 0x76,
        key_front               = 0x77,
        key_stop                = 0x78,
        key_again               = 0x79,
        key_undo                = 0x7a,
        key_cut                 = 0x7b,
        key_copy                = 0x7c,
        key_paste               = 0x7d,
        key_find                = 0x7e,

        key_mute                = 0x7f,
        key_volumeup            = 0x80,
        key_volumedown          = 0x81,

        key_kb_locking_caps     = 0x82,
        key_kb_locking_num      = 0x83,
        key_kb_locking_scroll   = 0x84,
        key_kp_comma            = 0x85,

        key_ro                  = 0x87,
        key_katakanahiragana    = 0x88,
        key_yen                 = 0x89,
        key_henkan              = 0x8a,
        key_muhenkan            = 0x8b,
        key_kp_jpcomma          = 0x8c,

        key_intl1               = 0x87,
        key_intl2               = 0x88,
        key_intl3               = 0x89,
        key_intl4               = 0x8a,
        key_intl5               = 0x8b,
        key_intl6               = 0x8c,
        key_intl7               = 0x8d,
        key_intl8               = 0x8e,
        key_intl9               = 0x8f,

        key_hangul              = 0x90,
        key_hanja               = 0x91,
        key_katakana            = 0x92,
        key_hiragana            = 0x93,
        key_zenkakuhankaku      = 0x94,

        key_lang1               = 0x90,
        key_lang2               = 0x91,
        key_lang3               = 0x92,
        key_lang4               = 0x93,
        key_lang5               = 0x94,
        key_lang6               = 0x95,
        key_lang7               = 0x96,
        key_lang8               = 0x97,
        key_lang9               = 0x98,

        key_alternate_erase     = 0x99,
        key_sysreq_attention    = 0x9a,
        key_cancel              = 0x9b,
        key_clear               = 0x9c,
        key_prior               = 0x9d,
        key_return              = 0x9e,
        key_separator           = 0x9f,
        key_out                 = 0xa0,
        key_oper                = 0xa1,
        key_clear_again         = 0xa2,
        key_crsel_props         = 0xa3,
        key_exsel               = 0xa4,

        key_keypad_00           = 0xb0,
        key_keypad_000          = 0xb1,
        key_thousands_separator = 0xb2,
        key_decimal_separator   = 0xb3,
        key_currency_unit       = 0xb4,
        key_currency_subunit    = 0xb5,

        key_kp_left_parenthesis = 0xb6,
        key_kp_right_parenthesis= 0xb7,
        key_kp_left_brace       = 0xb8,
        key_kp_right_brace      = 0xb9,

        key_kp_tab              = 0xba,
        key_kp_backspace        = 0xbb,
        key_kp_a                = 0xbc,
        key_kp_b                = 0xbd,
        key_kp_c                = 0xbe,
        key_kp_d                = 0xbf,
        key_kp_e                = 0xc0,
        key_kp_f                = 0xc1,
        key_kp_xor              = 0xc2,
        key_kp_exp              = 0xc3,
        key_kp_percent          = 0xc4,
        key_kp_lessthan         = 0xc5,
        key_kp_greaterthan      = 0xc6,
        key_kp_bitwise_and      = 0xc7,
        key_kp_and              = 0xc8,
        key_kp_bitwise_or       = 0xc9,
        key_kp_or               = 0xca,
        key_kp_colon            = 0xcb,
        key_kp_number           = 0xcc,
        key_kp_space            = 0xcd,
        key_kp_at               = 0xce,
        key_kp_exclamation      = 0xcf,

        key_kp_memory_store     = 0xd0,
        key_kp_memory_recall    = 0xd1,
        key_kp_memory_clear     = 0xd2,
        key_kp_memory_add       = 0xd3,
        key_kp_memory_substract = 0xd4,
        key_kp_memory_multiply  = 0xd5,
        key_kp_memory_divide    = 0xd6,
        key_kp_plusminus        = 0xd7,
        key_kp_clear            = 0xd8,
        key_kp_clear_entry      = 0xd9,
        key_kp_binary           = 0xda,
        key_kp_octal            = 0xdb,
        key_kp_decimal          = 0xdc,
        key_kp_hexadecimal      = 0xdd,

        key_left_ctrl           = 0xe0,
        key_left_shift          = 0xe1,
        key_left_alt            = 0xe2,
        key_left_meta           = 0xe3,
        key_right_ctrl          = 0xe4,
        key_right_shift         = 0xe5,
        key_right_alt           = 0xe6,
        key_right_meta          = 0xe7,
    };
}

#endif
/*
    Copyright (c) 2020 Jari Ronkainen

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose, including
    commercial applications, and to alter it and redistribute it freely.
*/
