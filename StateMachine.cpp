
#include "StateMachine.h"



State::State() {
}


void State::init(transition tf, updateContext uf) {
  _transitionFunction = tf;
  _updateFunction = uf;
}

int State::handle(int evt) {
  int newValue = _transitionFunction(evt);
  _updateFunction(newValue);
  return newValue;
}



StateMachine::StateMachine() {
}
  
void StateMachine::init(int initialState) {
  _currentSate = initialState;
}

void StateMachine::addState(int id, transition tf, updateContext uf) {
  _states[id].init(tf, uf);
}

int StateMachine::update(int newValue) {
  int newState = _states[_currentSate].handle(newValue);
  _currentSate = newState;
  return newState;
}

