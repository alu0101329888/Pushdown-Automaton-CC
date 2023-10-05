#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <vector>
#include <stack>
#include "./state.h"

class Automaton {
  public:
    Automaton();
    Automaton(std::string);
    ~Automaton();

    // Given a string, returns if it's recognized by the automaton
    bool Solve(std::string, bool = false);
    bool Step(std::stack<std::string>, std::string, std::string, int, bool, bool = false);
    void BuildAutomaton(std::string);
    void toString();
    
    void ReadStates(std::string);
    void ReadTapeSymbols(std::string);
    void ReadStackSymbols(std::string);
    void ReadStartingState(std::string);
    void ReadEndingStates(std::string);
    void ReadStackStart(std::string);
    void ReadTransitions(std::vector<std::string>);
  private:
    std::vector<State> states;
    std::vector<std::string> tapeSymbols;
    std::vector<std::string> stackSymbols;
    std::stack<std::string> stack;
    std::string startingStack;
    std::string startingState;
};

#endif