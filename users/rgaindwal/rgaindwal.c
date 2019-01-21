#include "rgaindwal.h"
#include "quantum.h"
#include "action.h"
#include "process_keycode/process_tap_dance.h"

#define _BL 0
#define _DVORAK 1
#define _FL 2
#define _SL 3

//Tap Dance 
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = 0
};



// Backspace Tap dance

void bscp_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP: register_code(KC_BSLS); break;
    case DOUBLE_TAP: register_code(KC_CAPS); break;
  }
}

void bscp_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(KC_BSLS); break;
    case DOUBLE_TAP: unregister_code(KC_CAPS); break;
  }
  xtap_state.state = 0;
}


// Left ctrl Tap Dance

void lctrl_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
        case SINGLE_TAP:
    case SINGLE_HOLD: register_code(KC_LCTL); 
                        layer_on(_SL);
                        break;
  }
}


void lctrl_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
        case SINGLE_TAP:
    case SINGLE_HOLD: layer_off(_SL);
                        unregister_code(KC_LCTL); break;
  }
  xtap_state.state = 0;
}



qk_tap_dance_action_t tap_dance_actions[] = {
  [X_BCSP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,bscp_finished, bscp_reset),
  [X_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,lctrl_finished, lctrl_reset),
};
