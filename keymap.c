
#include QMK_KEYBOARD_H


enum tap_dance_codes {
    SHIFT_ALT,
    CTRL_ALT,
    DTLT,
  DANCE_1,
  DANCE_2,
  DANCE_3,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_Q, KC_W, KC_E, KC_R, KC_T,    		                            KC_BSPC, KC_O, KC_I, KC_U, KC_Y,
        KC_A, KC_S, MT(OSM(MOD_LGUI),KC_D), MT(OSM(MOD_LALT),KC_F), KC_G,   KC_P, KC_L, MT(OSM(MOD_RGUI),KC_K), MT(OSM(MOD_RALT),KC_J), KC_H,
        KC_Z, KC_X, KC_C, KC_V, KC_B,     				                    TD(DANCE_3), TD(DANCE_1), TD(DANCE_2), KC_M, KC_N,
        MT(MOD_MEH,KC_SPACE), TD(SHIFT_ALT),                                TO(1), TD(CTRL_ALT)
    ),

    [1] = LAYOUT(
        KC_ESCAPE, KC_AT, KC_HASH, KC_DLR, KC_PERC,                                                                 KC_BSPC, KC_DELETE, KC_ASTR, KC_AMPR, KC_CIRC,
        KC_TAB, KC_LEFT_CURLY_BRACE, MT(OSM(MOD_LGUI),KC_LEFT_BRACKET), LSFT(KC_LEFT_PAREN), KC_LEFT_ANGLE_BRACKET, KC_ENTER, KC_RIGHT_CURLY_BRACE, MT(OSM(MOD_RGUI),KC_RIGHT_BRACKET), RSFT(KC_RIGHT_PAREN), KC_RIGHT_ANGLE_BRACKET,
        MT(LSFT(KC_GRV),KC_GRV), MT(LSFT(KC_QUOT),KC_QUOT), KC_PIPE, KC_COLON, KC_BACKSLASH,                        TO(2), KC_EXCLAIM, KC_QUESTION, KC_SEMICOLON, KC_SLASH,
        TD(DTLT), KC_TRANSPARENT,                                                                                   TO(3), KC_TRANSPARENT
    ),

    [2] = LAYOUT(
        KC_F1, KC_F2, KC_F3, KC_F4, KC_NO,      KC_TRANSPARENT, KC_TRANSPARENT, KC_NO, KC_NO, KC_NO,
        KC_F5, KC_F6, KC_F7, KC_F8, KC_NO,      KC_TRANSPARENT, KC_RIGHT, KC_UP, KC_DOWN, KC_LEFT,
        KC_F9, KC_F10, KC_F11, KC_F12, KC_NO,   KC_NO, KC_END, KC_PAGE_UP, KC_PAGE_DOWN, KC_HOME,
        KC_TRANSPARENT, KC_TRANSPARENT,         TO(1), KC_TRANSPARENT
    ),

    [3] = LAYOUT(
        KC_ASTR, KC_LPRN, KC_RPRN, KC_EQL, KC_NO,   KC_TRANSPARENT, KC_9, KC_8, KC_7, KC_PLUS,
        KC_NO, KC_DOT, KC_COMM, KC_SLASH, KC_NO,    KC_TRANSPARENT, KC_6, KC_5, KC_4, KC_MINS,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,          TO(2), KC_3, KC_2, KC_1, KC_0,
        KC_TRANSPARENT, KC_TRANSPARENT,             TO(1), KC_TRANSPARENT
    ),

    [4] = LAYOUT(
        KC_HASH, KC_HASH, KC_HASH, MT(MOD_MEH,KC_SPACE), TD(SHIFT_ALT), KC_TRANSPARENT, KC_DELETE, KC_HASH, TO(1), TD(CTRL_ALT),
        KC_CIRC, KC_T, KC_P, KC_H, KC_ASTR,                             KC_D,KC_T,KC_L,KC_P,KC_F,
        KC_S, KC_K, KC_W, KC_R, KC_ASTR,                                KC_Z, KC_S,KC_G, KC_B, KC_R,
        KC_A, KC_O,                                                     KC_E, KC_U 
    )

};





extern bool g_suspend_state;
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF
uint16_t last_keycode = KC_NO;
uint8_t last_modifier = 0;

typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

static tap dance_state = {
    .is_press_action = true,
    .step = 0
};

void on_dance_2(qk_tap_dance_state_t *state, void *user_data);
uint8_t dance_2_dance_step(qk_tap_dance_state_t *state);
void dance_2_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_2_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_2(qk_tap_dance_state_t *state, void *user_data) {
	if(state->count == 3) {
		tap_code16(KC_COMM);
		tap_code16(KC_COMM);
		tap_code16(KC_COMM);
	}
	if(state->count > 3) {
		tap_code16(KC_COMM);
	}
}

uint8_t dance_2_dance_step(qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	return MORE_TAPS;
}
void dance_2_finished(qk_tap_dance_state_t *state, void *user_data) {
	dance_state.step = dance_2_dance_step(state);
	switch (dance_state.step) {
		case SINGLE_TAP: register_code16(KC_COMM); break;
		case SINGLE_HOLD: register_code16(KC_MINS); break;
		case DOUBLE_TAP: register_code16(KC_COMM); register_code16(KC_COMM); break;
		case DOUBLE_SINGLE_TAP: tap_code16(KC_COMM); register_code16(KC_COMM);
	}
}

void dance_2_reset(qk_tap_dance_state_t *state, void *user_data) {
	wait_ms(10);
	switch (dance_state.step) {
		case SINGLE_TAP: unregister_code16(KC_COMM); break;
		case SINGLE_HOLD: unregister_code16(KC_MINS); break;
		case DOUBLE_TAP: unregister_code16(KC_COMM); break;
		case DOUBLE_SINGLE_TAP: unregister_code16(KC_MINS); break;
	}
	dance_state.step = 0;
}
void on_dance_3(qk_tap_dance_state_t *state, void *user_data);
uint8_t dance_3_dance_step(qk_tap_dance_state_t *state);
void dance_3_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_3_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_3(qk_tap_dance_state_t *state, void *user_data) {
	if(state->count == 3) {
		tap_code16(KC_SLASH);
		tap_code16(KC_SLASH);
		tap_code16(KC_SLASH);
	}
	if(state->count > 3) {
		tap_code16(KC_SLASH);
	}
}

uint8_t dance_3_dance_step(qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	return MORE_TAPS;
}
void dance_3_finished(qk_tap_dance_state_t *state, void *user_data) {
	dance_state.step = dance_3_dance_step(state);
	switch (dance_state.step) {
		case SINGLE_TAP: register_code16(KC_SLASH); break;
		case SINGLE_HOLD: register_code16(S(KC_MINS)); break;
		case DOUBLE_TAP: register_code16(KC_SLASH); register_code16(KC_SLASH); break;
		case DOUBLE_SINGLE_TAP: tap_code16(KC_SLASH); register_code16(KC_SLASH);
	}
}

void dance_3_reset(qk_tap_dance_state_t *state, void *user_data) {
	wait_ms(10);
	switch (dance_state.step) {
		case SINGLE_TAP: unregister_code16(KC_SLASH); break;
		case SINGLE_HOLD: unregister_code16(S(KC_MINS)); break;
		case DOUBLE_TAP: unregister_code16(KC_SLASH); break;
		case DOUBLE_SINGLE_TAP: unregister_code16(KC_SLASH); break;
	}
	dance_state.step = 0;
}
void on_dance_1(qk_tap_dance_state_t *state, void *user_data);
uint8_t dance_1_dance_step(qk_tap_dance_state_t *state);
void dance_1_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_1_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_1(qk_tap_dance_state_t *state, void *user_data) {
	if(state->count == 3) {
		tap_code16(KC_DOT);
		tap_code16(KC_DOT);
		tap_code16(KC_DOT);
	}
	if(state->count > 3) {
		tap_code16(KC_DOT);
	}
}

uint8_t dance_1_dance_step(qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	return MORE_TAPS;
}
void dance_1_finished(qk_tap_dance_state_t *state, void *user_data) {
	dance_state.step = dance_1_dance_step(state);
	switch (dance_state.step) {
		case SINGLE_TAP: register_code16(KC_DOT); break;
		case SINGLE_HOLD: register_code16(KC_EXLM); break;
		case DOUBLE_TAP: register_code16(KC_DOT); register_code16(KC_DOT); break;
		case DOUBLE_SINGLE_TAP: tap_code16(KC_DOT); register_code16(KC_DOT);
	}
}

void dance_1_reset(qk_tap_dance_state_t *state, void *user_data) {
	wait_ms(10);
	switch (dance_state.step) {
		case SINGLE_TAP: unregister_code16(KC_DOT); break;
		case SINGLE_HOLD: unregister_code16(KC_EXLM); break;
		case DOUBLE_TAP: unregister_code16(KC_DOT); break;
		case DOUBLE_SINGLE_TAP: unregister_code16(KC_DOT); break;
	}
	dance_state.step = 0;
}

void on_shift_alt(qk_tap_dance_state_t *state, void *user_data);
uint8_t shift_alt_dance_step(qk_tap_dance_state_t *state);
void shift_alt_finished(qk_tap_dance_state_t *state, void *user_data);
void shift_alt_reset(qk_tap_dance_state_t *state, void *user_data);

void on_shift_alt(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
    }
    if(state->count > 3) {
        tap_code16(KC_RIGHT);
    }
}

uint8_t shift_alt_dance_step(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted && !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted && !state->pressed) return DOUBLE_TAP;
        else return DOUBLE_HOLD;
    }
    return MORE_TAPS;
}
void shift_alt_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state.step = shift_alt_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP: set_oneshot_mods(MOD_BIT(KC_LSFT)); break;
        case SINGLE_HOLD: register_mods(MOD_BIT(KC_LSFT)); break;
        case DOUBLE_TAP: set_oneshot_mods(MOD_BIT(KC_LSFT)|MOD_BIT(KC_LALT)); break;
        case DOUBLE_HOLD: register_mods(MOD_BIT(KC_LSFT)|MOD_BIT(KC_LALT)); break;
    }
}

void shift_alt_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_HOLD: unregister_mods(MOD_BIT(KC_LSFT)); break;
        case DOUBLE_HOLD: unregister_mods(MOD_BIT(KC_LSFT)|MOD_BIT(KC_LALT)); break;
    }
    dance_state.step = 0;
}

void on_ctrl_alt(qk_tap_dance_state_t *state, void *user_data);
uint8_t ctrl_alt_dance_step(qk_tap_dance_state_t *state);
void ctrl_alt_finished(qk_tap_dance_state_t *state, void *user_data);
void ctrl_alt_reset(qk_tap_dance_state_t *state, void *user_data);

void on_ctrl_alt(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
    }
    if(state->count > 3) {
        tap_code16(KC_RIGHT);
    }
}

uint8_t ctrl_alt_dance_step(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted && !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted && !state->pressed) return DOUBLE_TAP;
        else return DOUBLE_HOLD;
    }
    return MORE_TAPS;
}
void ctrl_alt_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state.step = shift_alt_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP: set_oneshot_mods(MOD_BIT(KC_RCTL)); break;
        case SINGLE_HOLD: register_mods(MOD_BIT(KC_RCTL)); break;
        case DOUBLE_TAP: set_oneshot_mods(MOD_BIT(KC_RCTL)|MOD_BIT(KC_RALT)); break;
        case DOUBLE_HOLD: register_mods(MOD_BIT(KC_RCTL)|MOD_BIT(KC_RALT)); break;
    }
}

void ctrl_alt_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_HOLD: unregister_mods(MOD_BIT(KC_RCTL)); break;
        case DOUBLE_HOLD: unregister_mods(MOD_BIT(KC_RCTL)|MOD_BIT(KC_RALT)); break;
    }
    dance_state.step = 0;
}

void dtlt(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_move(0);
        reset_tap_dance(state);
    } else if (state->count == 2) {
        layer_move(4);
        reset_tap_dance(state);
    } else if (state->count >= 3) {
        reset_tap_dance(state);
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [DANCE_1] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_1, dance_1_finished, dance_1_reset),
    [DANCE_2] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_2, dance_2_finished, dance_2_reset),
    [DANCE_3] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_3, dance_3_finished, dance_3_reset),
    [SHIFT_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(on_shift_alt, shift_alt_finished, shift_alt_reset),
    [CTRL_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(on_ctrl_alt, ctrl_alt_finished, ctrl_alt_reset),
    [DTLT] = ACTION_TAP_DANCE_FN(dtlt),
};