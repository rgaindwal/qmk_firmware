#include "quantum.h"
#include "process_keycode/process_tap_dance.h"

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

//Tap dance enums
enum {
  X_BCSP = 0,
  X_CTRL = 1,
};


int cur_dance (qk_tap_dance_state_t *state);

// Backspace Tap Dance
void bscp_finished (qk_tap_dance_state_t *state, void *user_data);
void bscp_reset (qk_tap_dance_state_t *state, void *user_data);

// Left control Tap dance for DVORAK
void lctrl_finished(qk_tap_dance_state_t *state, void *user_data);
void lctrl_reset (qk_tap_dance_state_t *state, void *user_data);