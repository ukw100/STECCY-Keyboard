/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd.h - PS/2 keyboard scancodes and functions
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * See also:
 *    https://www.marjorie.de/ps2/scancode-set2.htm
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * MIT License
 *
 * Copyright (c) 2021 Frank Meyer - frank(at)fli4l.de
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define PS2KBD_SCANCODE_MASK    0x01FF                                                          // 512 possible key scancodes
#define PS2KBD_EXTENDED_FLAG    0x0100
#define PS2KBD_RELEASED_FLAG    0x0200

// PS/2 scancodes SET 2
#define PS2KBD_SCANCODE_A              0x1c
#define PS2KBD_SCANCODE_B              0x32
#define PS2KBD_SCANCODE_C              0x21
#define PS2KBD_SCANCODE_D              0x23
#define PS2KBD_SCANCODE_E              0x24
#define PS2KBD_SCANCODE_F              0x2b
#define PS2KBD_SCANCODE_G              0x34
#define PS2KBD_SCANCODE_H              0x33
#define PS2KBD_SCANCODE_I              0x43
#define PS2KBD_SCANCODE_J              0x3b
#define PS2KBD_SCANCODE_K              0x42
#define PS2KBD_SCANCODE_L              0x4b
#define PS2KBD_SCANCODE_M              0x3a
#define PS2KBD_SCANCODE_N              0x31
#define PS2KBD_SCANCODE_O              0x44
#define PS2KBD_SCANCODE_P              0x4d
#define PS2KBD_SCANCODE_Q              0x15
#define PS2KBD_SCANCODE_R              0x2d
#define PS2KBD_SCANCODE_S              0x1b
#define PS2KBD_SCANCODE_T              0x2c
#define PS2KBD_SCANCODE_U              0x3c
#define PS2KBD_SCANCODE_V              0x2a
#define PS2KBD_SCANCODE_W              0x1d
#define PS2KBD_SCANCODE_X              0x22
#define PS2KBD_SCANCODE_Y              0x35
#define PS2KBD_SCANCODE_Z              0x1a
#define PS2KBD_SCANCODE_0              0x45
#define PS2KBD_SCANCODE_1              0x16
#define PS2KBD_SCANCODE_2              0x1e
#define PS2KBD_SCANCODE_3              0x26
#define PS2KBD_SCANCODE_4              0x25
#define PS2KBD_SCANCODE_5              0x2e
#define PS2KBD_SCANCODE_6              0x36
#define PS2KBD_SCANCODE_7              0x3d
#define PS2KBD_SCANCODE_8              0x3e
#define PS2KBD_SCANCODE_9              0x46
#define PS2KBD_SCANCODE_BSP            0x66
#define PS2KBD_SCANCODE_SPACE          0x29
#define PS2KBD_SCANCODE_TAB            0x0D
#define PS2KBD_SCANCODE_CAPS           0x58
#define PS2KBD_SCANCODE_LSHFT          0x12
#define PS2KBD_SCANCODE_RSHFT          0x59
#define PS2KBD_SCANCODE_LCTRL          0x14
#define PS2KBD_SCANCODE_RCTRL          (0x14 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_LALT           0x11
#define PS2KBD_SCANCODE_RALT           (0x11 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_LWIN           (0x1F | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_RWIN           (0x27 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_MENU           (0x2F | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_ENTER          0x5A
#define PS2KBD_SCANCODE_ESC            0x76
#define PS2KBD_SCANCODE_F1             0x05
#define PS2KBD_SCANCODE_F2             0x06
#define PS2KBD_SCANCODE_F3             0x04
#define PS2KBD_SCANCODE_F4             0x0C
#define PS2KBD_SCANCODE_F5             0x03
#define PS2KBD_SCANCODE_F6             0x0B
#define PS2KBD_SCANCODE_F7             0x83
#define PS2KBD_SCANCODE_F8             0x0A
#define PS2KBD_SCANCODE_F9             0x01
#define PS2KBD_SCANCODE_F10            0x09
#define PS2KBD_SCANCODE_F11            0x78
#define PS2KBD_SCANCODE_F12            0x07
#define PS2KBD_SCANCODE_SCROLL         0x7E
#define PS2KBD_SCANCODE_INSERT         (0x70 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_HOME           (0x6C | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_PG_UP          (0x7D | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_DELETE         (0x71 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_END            (0x69 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_PG_DN          (0x7A | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_U_ARROW        (0x75 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_L_ARROW        (0x6B | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_D_ARROW        (0x72 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_R_ARROW        (0x74 | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_NUM            0x77
#define PS2KBD_SCANCODE_KP_SLASH       (0x4A | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_KP_ASTERISK    0x7C
#define PS2KBD_SCANCODE_KP_MINUS       0x7B
#define PS2KBD_SCANCODE_KP_PLUS        0x79
#define PS2KBD_SCANCODE_KP_ENTER       (0x5A | PS2KEY_EXTENDED_FLAG)
#define PS2KBD_SCANCODE_KP_COMMA       0x71
#define PS2KBD_SCANCODE_KP_0           0x70
#define PS2KBD_SCANCODE_KP_1           0x69
#define PS2KBD_SCANCODE_KP_2           0x72
#define PS2KBD_SCANCODE_KP_3           0x7A
#define PS2KBD_SCANCODE_KP_4           0x6B
#define PS2KBD_SCANCODE_KP_5           0x73
#define PS2KBD_SCANCODE_KP_6           0x74
#define PS2KBD_SCANCODE_KP_7           0x6C
#define PS2KBD_SCANCODE_KP_8           0x75
#define PS2KBD_SCANCODE_KP_9           0x7D

#define PS2KBD_SCANCODE_SHARP_S        0x4E
#define PS2KBD_SCANCODE_U_UMLAUT       0x54
#define PS2KBD_SCANCODE_O_UMLAUT       0x4C
#define PS2KBD_SCANCODE_A_UMLAUT       0x52
#define PS2KBD_SCANCODE_PLUS           0x5B
#define PS2KBD_SCANCODE_HASH           0x5D
#define PS2KBD_SCANCODE_MINUS          0x4A
#define PS2KBD_SCANCODE_DOT            0x49
#define PS2KBD_SCANCODE_COMMA          0x41
#define PS2KBD_SCANCODE_LESS           0x61

extern void     ps2kbd_send_code (uint_fast8_t ch);
extern void     ps2kbd_init (void);
