#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>

// Defines what a transition is
struct Transition {
  std::string tapeSymbol;
  std::string stackSymbol;
  std::string endingState;
  std::vector<std::string> toStack;
};

// Defines what a state is
class State {
  public:
    State();
    State(std::string);

    std::string GetName();
    std::vector<Transition> GetTransitions();
    // Sets the state as an ending state
    void SetAsEnd();
    // Sets the state as a starting state
    void SetAsStart();
    bool GetEnd();

    void AddTransition(Transition);
  private:
    std::string name;
    std::vector<Transition> transitions;
    bool startingState = false;
    bool endingState = false;
};

#endif