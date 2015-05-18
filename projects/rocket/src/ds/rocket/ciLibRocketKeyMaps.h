/*
NOTE: this is an internal header. do not use directly!
*/

#ifndef CI_LIBROCKET_RENDERER_KEY_MAP_H_
#define CI_LIBROCKET_RENDERER_KEY_MAP_H_

#include <map>

#include <Rocket/Core/Input.h>
#include <cinder/app/KeyEvent.h>

namespace {

static std::map<int, Rocket::Core::Input::KeyIdentifier> CINDER_KEY_TO_ROCKET_MAP;

void initKeyMap()
{
    using namespace ci::app;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_BACKSPACE] = Rocket::Core::Input::KI_BACK;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_TAB] = Rocket::Core::Input::KI_TAB;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_CLEAR] = Rocket::Core::Input::KI_CLEAR;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_RETURN] = Rocket::Core::Input::KI_RETURN;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_PAUSE] = Rocket::Core::Input::KI_PAUSE;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_ESCAPE] = Rocket::Core::Input::KI_ESCAPE;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_SPACE] = Rocket::Core::Input::KI_SPACE;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_QUOTE] = Rocket::Core::Input::KI_OEM_7;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_PLUS] = Rocket::Core::Input::KI_OEM_PLUS;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_COMMA] = Rocket::Core::Input::KI_OEM_COMMA;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_PERIOD] = Rocket::Core::Input::KI_OEM_PERIOD;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_SLASH] = Rocket::Core::Input::KI_OEM_2;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_0] = Rocket::Core::Input::KI_0;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_1] = Rocket::Core::Input::KI_1;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_2] = Rocket::Core::Input::KI_2;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_3] = Rocket::Core::Input::KI_3;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_4] = Rocket::Core::Input::KI_4;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_5] = Rocket::Core::Input::KI_5;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_6] = Rocket::Core::Input::KI_6;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_7] = Rocket::Core::Input::KI_7;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_8] = Rocket::Core::Input::KI_8;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_9] = Rocket::Core::Input::KI_9;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_SEMICOLON] = Rocket::Core::Input::KI_OEM_1;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_EQUALS] = Rocket::Core::Input::KI_OEM_PLUS;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_LEFTBRACKET] = Rocket::Core::Input::KI_OEM_4;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_BACKSLASH] = Rocket::Core::Input::KI_OEM_5;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_RIGHTBRACKET] = Rocket::Core::Input::KI_OEM_6;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_BACKQUOTE] = Rocket::Core::Input::KI_OEM_3;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_a] = Rocket::Core::Input::KI_A;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_b] = Rocket::Core::Input::KI_B;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_c] = Rocket::Core::Input::KI_C;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_d] = Rocket::Core::Input::KI_D;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_e] = Rocket::Core::Input::KI_E;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_f] = Rocket::Core::Input::KI_F;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_g] = Rocket::Core::Input::KI_G;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_h] = Rocket::Core::Input::KI_H;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_i] = Rocket::Core::Input::KI_I;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_j] = Rocket::Core::Input::KI_J;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_k] = Rocket::Core::Input::KI_K;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_l] = Rocket::Core::Input::KI_L;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_m] = Rocket::Core::Input::KI_M;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_n] = Rocket::Core::Input::KI_N;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_o] = Rocket::Core::Input::KI_O;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_p] = Rocket::Core::Input::KI_P;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_q] = Rocket::Core::Input::KI_Q;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_r] = Rocket::Core::Input::KI_R;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_s] = Rocket::Core::Input::KI_S;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_t] = Rocket::Core::Input::KI_T;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_u] = Rocket::Core::Input::KI_U;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_v] = Rocket::Core::Input::KI_V;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_w] = Rocket::Core::Input::KI_W;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_x] = Rocket::Core::Input::KI_X;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_y] = Rocket::Core::Input::KI_Y;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_z] = Rocket::Core::Input::KI_Z;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_DELETE] = Rocket::Core::Input::KI_DELETE;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP0] = Rocket::Core::Input::KI_NUMPAD0;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP1] = Rocket::Core::Input::KI_NUMPAD1;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP2] = Rocket::Core::Input::KI_NUMPAD2;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP3] = Rocket::Core::Input::KI_NUMPAD3;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP4] = Rocket::Core::Input::KI_NUMPAD4;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP5] = Rocket::Core::Input::KI_NUMPAD5;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP6] = Rocket::Core::Input::KI_NUMPAD6;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP7] = Rocket::Core::Input::KI_NUMPAD7;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP8] = Rocket::Core::Input::KI_NUMPAD8;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP9] = Rocket::Core::Input::KI_NUMPAD9;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP_PERIOD] = Rocket::Core::Input::KI_DECIMAL;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP_DIVIDE] = Rocket::Core::Input::KI_DIVIDE;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP_MULTIPLY] = Rocket::Core::Input::KI_MULTIPLY;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP_PLUS] = Rocket::Core::Input::KI_ADD;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP_ENTER] = Rocket::Core::Input::KI_NUMPADENTER;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_KP_EQUALS] = Rocket::Core::Input::KI_OEM_NEC_EQUAL;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_UP] = Rocket::Core::Input::KI_UP;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_DOWN] = Rocket::Core::Input::KI_DOWN;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_RIGHT] = Rocket::Core::Input::KI_RIGHT;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_LEFT] = Rocket::Core::Input::KI_LEFT;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_INSERT] = Rocket::Core::Input::KI_INSERT;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_HOME] = Rocket::Core::Input::KI_HOME;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_END] = Rocket::Core::Input::KI_END;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_PAGEUP] = Rocket::Core::Input::KI_PRIOR;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_PAGEDOWN] = Rocket::Core::Input::KI_NEXT;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F1] = Rocket::Core::Input::KI_F1;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F2] = Rocket::Core::Input::KI_F2;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F3] = Rocket::Core::Input::KI_F3;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F4] = Rocket::Core::Input::KI_F4;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F5] = Rocket::Core::Input::KI_F5;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F6] = Rocket::Core::Input::KI_F6;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F7] = Rocket::Core::Input::KI_F7;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F8] = Rocket::Core::Input::KI_F8;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F9] = Rocket::Core::Input::KI_F9;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F10] = Rocket::Core::Input::KI_F10;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F11] = Rocket::Core::Input::KI_F11;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F12] = Rocket::Core::Input::KI_F12;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F13] = Rocket::Core::Input::KI_F13;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F14] = Rocket::Core::Input::KI_F14;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_F15] = Rocket::Core::Input::KI_F15;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_NUMLOCK] = Rocket::Core::Input::KI_NUMLOCK;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_CAPSLOCK] = Rocket::Core::Input::KI_CAPITAL;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_SCROLLOCK] = Rocket::Core::Input::KI_SCROLL;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_RSHIFT] = Rocket::Core::Input::KI_RSHIFT;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_LSHIFT] = Rocket::Core::Input::KI_LSHIFT;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_RCTRL] = Rocket::Core::Input::KI_RCONTROL;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_LCTRL] = Rocket::Core::Input::KI_LCONTROL;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_RMETA] = Rocket::Core::Input::KI_RMETA;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_LMETA] = Rocket::Core::Input::KI_LMETA;

    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_HELP] = Rocket::Core::Input::KI_HELP;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_PRINT] = Rocket::Core::Input::KI_SNAPSHOT;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_MENU] = Rocket::Core::Input::KI_LMENU;
    CINDER_KEY_TO_ROCKET_MAP[KeyEvent::KEY_POWER] = Rocket::Core::Input::KI_POWER;
}

char ascii_map[4][51] =
{
    // shift off and capslock off
    {
        0,
        ' ',
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x',
        'y',
        'z',
        ';',
        '=',
        ',',
        '-',
        '.',
        '/',
        '`',
        '[',
        '\\',
        ']',
        '\'',
        0,
        0
    },

    // shift on and capslock off
    {
        0,
        ' ',
        ')',
        '!',
        '@',
        '#',
        '$',
        '%',
        '^',
        '&',
        '*',
        '(',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P',
        'Q',
        'R',
        'S',
        'T',
        'U',
        'V',
        'W',
        'X',
        'Y',
        'Z',
        ':',
        '+',
        '<',
        '_',
        '>',
        '?',
        '~',
        '{',
        '|',
        '}',
        '"',
        0,
        0
    },

    // shift on and capslock on
    {
        0,
        ' ',
        ')',
        '!',
        '@',
        '#',
        '$',
        '%',
        '^',
        '&',
        '*',
        '(',
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x',
        'y',
        'z',
        ':',
        '+',
        '<',
        '_',
        '>',
        '?',
        '~',
        '{',
        '|',
        '}',
        '"',
        0,
        0
    },

    // shift off and capslock on
    {
        0,
        ' ',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P',
        'Q',
        'R',
        'S',
        'T',
        'U',
        'V',
        'W',
        'X',
        'Y',
        'Z',
        ';',
        '=',
        ',',
        '-',
        '.',
        '/',
        '`',
        '[',
        '\\',
        ']',
        '\'',
        0,
        0
    }
};

char keypad_map[2][18] =
{
    {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '\n',
        '*',
        '+',
        0,
        '-',
        '.',
        '/',
        '='
    },

    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        '\n',
        '*',
        '+',
        0,
        '-',
        0,
        '/',
        '='
    }
};

} //!anonymous namespace

#endif //!CI_LIBROCKET_RENDERER_KEY_MAP_H_