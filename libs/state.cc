#include "../include/state.h"

State::State() {}

State::State(std::string _name) {
  name = _name;
}

std::string State::GetName() {
  return name;
}

std::vector<Transition> State::GetTransitions() {
  return transitions;
}

void State::AddTransition(Transition newTransition) {
  transitions.push_back(newTransition);
}

void State::SetAsEnd() {
  endingState = true;
}

void State::SetAsStart() {
  startingState = true;
}

bool State::GetEnd() {
  return endingState;
}