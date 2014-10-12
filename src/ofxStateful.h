#include "ofxRemoteUIServer.h"

class ofxStateful {
public:
  void setup_sm() {
    ofAddListener(ofEvents().update, this, &ofxStateful::update_sm); 
    RUI_SETUP();
    RUI_NEW_GROUP("STATE");
    RUI_SHARE_ENUM_PARAM(public_state, START, NUM_STATES-1, vector<string>(state_names, state_names+NUM_STATES));
    first_update = true;
  }

  void update_sm(ofEventArgs &eventArgs) {
    if (first_update) { 
      // this is to prevent the app from automatically entering
      // last used state
      public_state = state;
      first_update = false;
    } else if (public_state != state) {
      transition(public_state);
    }
  }

  void transition(state_t _state) {
    onExitState();
    ofLog() << "transition from " << state_names[state] << " to " << state_names[_state];
    state_start_at = ofGetElapsedTimeMillis();
    public_state = state = _state;
    cur_state_name = state_names[state];
    //OFX_REMOTEUI_SERVER_PUSH_TO_CLIENT();
    onEnterState();
  }

  long timeInState() {
    return ofGetElapsedTimeMillis() - state_start_at;
  }

  virtual void onEnterState() = 0;
  virtual void onExitState() = 0;

protected:
  state_t public_state;
  state_t state;
  long state_start_at;
  string cur_state_name;
  bool first_update;
};
