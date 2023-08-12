#ifndef __INCLUDE_DEMO_ACTION_DEFINE_H__
#define __INCLUDE_DEMO_ACTION_DEFINE_H__
//=======================================================================//
//= User definition.                                                    =//
//=======================================================================//
// Key value definition.
#define     KEY_VALUE_NONE                                  (0x0000)
#define     KEY_VALUE_BACKSPACE                             (0x0008)
#define     KEY_VALUE_TAB                                   (0x0009)
#define     KEY_VALUE_ENTER                                 (0x000D)
#define     KEY_VALUE_ESC                                   (0x001B)
#define     KEY_VALUE_SPACE                                 (0x0020)
#define     KEY_VALUE_DEL                                   (0x007F)
#define     KEY_VALUE_HOME                                  (0x0139)
#define     KEY_VALUE_END                                   (0x0138)
#define     KEY_VALUE_LEFT                                  (0x013A)
#define     KEY_VALUE_UP                                    (0x013B)
#define     KEY_VALUE_RIGHT                                 (0x013C)
#define     KEY_VALUE_DOWN                                  (0x013D)
#define     KEY_VALUE_INSERT                                (0x0142)
#define     KEY_VALUE_F1                                    (0x0154)
#define     KEY_VALUE_F2                                    (0x0155)
#define     KEY_VALUE_F3                                    (0x0156)
#define     KEY_VALUE_F4                                    (0x0157)
#define     KEY_VALUE_F5                                    (0x0158)
#define     KEY_VALUE_F6                                    (0x0159)
#define     KEY_VALUE_F7                                    (0x015A)
#define     KEY_VALUE_F8                                    (0x015B)
#define     KEY_VALUE_F9                                    (0x015C)
#define     KEY_VALUE_F10                                   (0x015D)
#define     KEY_VALUE_F11                                   (0x015E)
#define     KEY_VALUE_F12                                   (0x015F)
#define     KEY_VALUE_ENTER_PAD                             (0x0172)
#define     KEY_VALUE_PLUS_PAD                              (0x0184)
#define     KEY_VALUE_SUB_PAD                               (0x0186)
// User option flag value define
#define     KEY_OPTION_CTRL                                 (0x1000)
#define     KEY_OPTION_ALT                                  (0x2000)
#define     KEY_OPTION_SHIFT                                (0x4000)

#define     KEY_CODE_VALUE(CODE)                            (CODE & 0x0FFF)
#define     KEY_CODE_OPT(CODE)                              (CODE & 0xF000)
#endif // __INCLUDE_DEMO_ACTION_DEFINE_H__
