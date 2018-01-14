#include "debug.h"
#include "action.h"
#include "actionmap.h"
#include "hook.h"
#include "fc660c.h"

enum macro_id {
    HELLO,
};

enum function_id {
    LSHIFT_LPAREN,
    RSHIFT_RPAREN,
};

#define AC_L1 ACTION_LAYER_MOMENTARY(1)
#define AC_L2 ACTION_LAYER_TOGGLE(2)
#define AC_LSFTP ACTION_MODS_TAP_KEY(MOD_LSFT, MOD_LSFT | KC_9)
#define AC_RSFTP ACTION_MODS_TAP_KEY(MOD_RSFT, MOD_LSFT | KC_0)
#define AC_HELLO ACTION_MACRO(HELLO)
#define AC_LSLP ACTION_FUNCTION_TAP(LSHIFT_LPAREN)
#define AC_RSRP ACTION_FUNCTION_TAP(RSHIFT_RPAREN)
#define AC_SPCL1 ACTION_LAYER_TAP_KEY(1, KC_SPC)
#define AC_HYPER ACTION_MODS(MOD_LALT | MOD_LCTL | MOD_LSFT)
#define AC_HWREF ACTION_KEY(MOD_LGUI | KC_R)
#define AC_WBAK ACTION_KEY(MOD_LGUI | KC_LEFT)
#define AC_WFWD ACTION_KEY(MOD_LGUI | KC_RIGHT)
#define AC_WSTP ACTION_KEY(MOD_LGUI | KC_DOT)
#define AC_WDEV ACTION_KEY(KC_F12)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    [0] = \
    KMAP(
        GRV,    1,        2,      3,      4,      5,      6,      7,       8,       9,      0,      MINS,   EQL,   BSPC,   INS,
        TAB,    Q,        W,      E,      R,      T,      Y,      U,       I,       O,      P,      LBRC,   RBRC,  BSLS,   DEL,
        LCTL,   A,        S,      D,      F,      G,      H,      J,       K,       L,      SCLN,   QUOT,   ENT,
        LSLP,   Z,        X,      C,      V,      B,      N,      M,       COMM,    DOT,    SLSH,   RSRP,   UP,
        HYPER,  LALT,     LGUI,   SPCL1,  RALT,   LCTL,   L2,     LEFT,    DOWN,    RGHT
    ),
    [1] = \
    KMAP(
        ESC,    F1,       F2,     F3,     F4,     F5,     F6,     F7,      F8,      F9,       F10,    F11,   F12,    DEL,    PGUP,
        SPC,    TRNS,     TRNS,   HWREF,  TRNS,   TRNS,   TRNS,   TRNS,    TRNS,    TRNS,     TRNS,   TRNS,  TRNS,   TRNS,   PGDOWN,
        TRNS,   TRNS,     WBAK,   WSTP,   WFWD,   TRNS,   MPLY,   VOLD,    VOLU,    MNXT,     TRNS,   TRNS,  TRNS,
        TRNS,   TRNS,     TRNS,   WDEV,   TRNS,   TRNS,   TRNS,   MUTE,    TRNS,    TRNS,     TRNS,   TRNS,  TRNS,
        TRNS,   TRNS,     TRNS,   TRNS,   TRNS,   TRNS,   TRNS,   TRNS,    TRNS,    TRNS
    ),
    [2] = \
    KMAP(
        TRNS,   TRNS,     TRNS,   TRNS,   TRNS,   TRNS,   TRNS,   TRNS,    TRNS,    TRNS,     TRNS,   TRNS,  TRNS,   TRNS,   TRNS,
        TRNS,   TRNS,     TRNS,   TRNS,   TRNS,   TRNS,   TRNS,   TRNS,    TRNS,    TRNS,     TRNS,   TRNS,  TRNS,   TRNS,   TRNS,
        TRNS,   TRNS,     TRNS,   TRNS,   TRNS,   TRNS,   TRNS,   TRNS,    TRNS,    TRNS,     TRNS,   TRNS,  TRNS,
        TRNS,   TRNS,     TRNS,   TRNS,   TRNS,   TRNS,   TRNS,   TRNS,    TRNS,    TRNS,     TRNS,   TRNS,  TRNS,
        TRNS,   TRNS,     TRNS,   TRNS,   TRNS,   TRNS,   TRNS,   TRNS,    TRNS,    TRNS
    ),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case HELLO:
            return (record->event.pressed ?
                    MACRO( I(0), T(H), T(E), T(L), T(L), T(O), END ) :
                    MACRO_NONE );
    }
    return MACRO_NONE;
}

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    if (record->event.pressed) dprint("P"); else dprint("R");
    dprintf("%d", record->tap.count);
    if (record->tap.interrupted) dprint("i");
    dprint("\n");

    switch (id) {
      case LSHIFT_LPAREN:
        // Shift parentheses example: LShft + tap '('
        // http://stevelosh.com/blog/2012/10/a-modern-space-cadet/#shift-parentheses
        // http://geekhack.org/index.php?topic=41989.msg1304899#msg1304899
        if (record->event.pressed) {
            if (record->tap.count > 0 && !record->tap.interrupted) {
                if (record->tap.interrupted) {
                    dprint("tap interrupted\n");
                    register_mods(MOD_BIT(KC_LSHIFT));
                }
            } else {
                register_mods(MOD_BIT(KC_LSHIFT));
            }
        } else {
            if (record->tap.count > 0 && !(record->tap.interrupted)) {
                add_weak_mods(MOD_BIT(KC_LSHIFT));
                send_keyboard_report();
                register_code(KC_9);
                unregister_code(KC_9);
                del_weak_mods(MOD_BIT(KC_LSHIFT));
                send_keyboard_report();
                record->tap.count = 0;  // ad hoc: cancel tap
            } else {
                unregister_mods(MOD_BIT(KC_LSHIFT));
            }
        }
        break;
      case RSHIFT_RPAREN:
        // Shift parentheses example: RShft + tap ')'
        // http://stevelosh.com/blog/2012/10/a-modern-space-cadet/#shift-parentheses
        // http://geekhack.org/index.php?topic=41989.msg1304899#msg1304899
        if (record->event.pressed) {
            if (record->tap.count > 0 && !record->tap.interrupted) {
                if (record->tap.interrupted) {
                    dprint("tap interrupted\n");
                    register_mods(MOD_BIT(KC_RSHIFT));
                }
            } else {
                register_mods(MOD_BIT(KC_RSHIFT));
            }
        } else {
            if (record->tap.count > 0 && !(record->tap.interrupted)) {
                add_weak_mods(MOD_BIT(KC_RSHIFT));
                send_keyboard_report();
                register_code(KC_0);
                unregister_code(KC_0);
                del_weak_mods(MOD_BIT(KC_RSHIFT));
                send_keyboard_report();
                record->tap.count = 0;  // ad hoc: cancel tap
            } else {
                unregister_mods(MOD_BIT(KC_RSHIFT));
            }
        }
        break;
    }
}

void hook_layer_change(uint32_t layer_state)
{
    // lights LED on Insert when layer 1 is enabled
    if (layer_state & (1L<<1)) {
        PORTB &= ~(1<<5);
    } else {
        PORTB |=  (1<<5);
    }
}
