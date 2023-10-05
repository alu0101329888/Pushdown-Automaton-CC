#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "../include/automaton.h"

Automaton::Automaton() {}

Automaton::Automaton(std::string file) {
  BuildAutomaton(file);
}

Automaton::~Automaton() {}

void Automaton::toString() {
  std::cout << "States: ";
  for (int i = 0; i < states.size(); i++) {
    std::cout << states[i].GetName() << " ";
  }
  std::cout << std::endl;
  std::cout << "Tape symbols: ";
  for (int i = 0; i < tapeSymbols.size(); i++) {
    std::cout << tapeSymbols[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "Stack symbols: ";
  for (int i = 0; i < stackSymbols.size(); i++) {
    std::cout << stackSymbols[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "Starting stack: " << startingStack << std::endl;
  std::cout << "Starting state: " << startingState << std::endl;
  std::cout << "End states: ";
  for (int i = 0; i < states.size(); i++) {
    if (states[i].GetEnd()) {
      std::cout << states[i].GetName() << " ";
    }
  }
  std::cout << std::endl << std::endl;
  std::cout << "Transitions:" << std::endl;
  for (int i = 0; i < states.size(); i++) {
    for (int j = 0; j < states[i].GetTransitions().size(); j++) {
      std::cout << "State: " << states[i].GetName() << " - Reads: " << states[i].GetTransitions()[j].tapeSymbol << ", Stack has: " << states[i].GetTransitions()[j].stackSymbol << ", Moves to: " << states[i].GetTransitions()[j].endingState << ", Writes: ";
      for (int k = 0; k < states[i].GetTransitions()[j].toStack.size(); k++) {
        std::cout << states[i].GetTransitions()[j].toStack[k] << " ";
      }
      std::cout << std::endl;
    }
  }
}

void Automaton::BuildAutomaton(std::string file) {
  std::ifstream input(file);
  if (!input.is_open()) {
    std::cerr << "There has been a problem trying to read the file, please make sure the path to the file is correct" << std::endl;
    throw;
  }

  // Reads every line and gets rid of comments
  std::string line;
  std::vector<std::string> formattedFile;
  while (std::getline(input, line)) {
    size_t comment = line.find('#');
    if (comment != -1) {
      line = line.substr(0, comment);
    }
    if (line.size() > 0) {
      formattedFile.push_back(line);
    }
  }

  input.close();

  ReadStates(formattedFile[0]);
  ReadTapeSymbols(formattedFile[1]);
  ReadStackSymbols(formattedFile[2]);
  ReadStartingState(formattedFile[3]);
  ReadStackStart(formattedFile[4]);
  ReadEndingStates(formattedFile[5]);
  ReadTransitions(formattedFile);
}

void Automaton::ReadStates(std::string line) {
  std::istringstream formattedLine(line);
  std::string formattedWord;
  while (formattedLine >> formattedWord) {
    bool contained = false;
    for (int i = 0; i < states.size(); i++) {
      if (states[i].GetName() == formattedWord) {
        contained = true;
      }
    }
    if (!contained) {
      State newState(formattedWord);
      states.push_back(newState);
    }
  }
}

void Automaton::ReadTapeSymbols(std::string line) {
  std::istringstream formattedLine(line);
  std::string formattedWord;
  while (formattedLine >> formattedWord && std::find(tapeSymbols.begin(), tapeSymbols.end(), formattedWord) == tapeSymbols.end()) {
    tapeSymbols.push_back(formattedWord);
  }
  tapeSymbols.push_back(".");
}

void Automaton::ReadStackSymbols(std::string line) {
  std::istringstream formattedLine(line);
  std::string formattedWord;
  while (formattedLine >> formattedWord && std::find(stackSymbols.begin(), stackSymbols.end(), formattedWord) == stackSymbols.end()) {
    stackSymbols.push_back(formattedWord);
  }
  stackSymbols.push_back(".");
}

void Automaton::ReadStartingState(std::string line) {
  std::istringstream formattedLine(line);
  std::string formattedWord;
  int startingStateCount = 0;
  while (formattedLine >> formattedWord) {
    startingStateCount++;
    bool exists = false;
    for (int i = 0; i < states.size(); i++) {
      if (states[i].GetName() == formattedWord) {
        exists = true;
        states[i].SetAsStart();
        startingState = formattedWord;
      }
    }
    if (!exists) {
      std::cerr << "Initial state doesn't exist, have you configured the automaton correctly?" << std::endl;
      throw;
    }
    if (startingStateCount > 1) {
      std::cerr << "Cannot have more than one starting state for the automaton!" << std::endl;
      throw;
    }
  }
}

void Automaton::ReadEndingStates(std::string line) {
  std::istringstream formattedLine(line);
  std::string formattedWord;
  while (formattedLine >> formattedWord) {
    bool exists = false;
    for (int i = 0; i < states.size(); i++) {
      if (states[i].GetName() == formattedWord) {
        exists = true;
        states[i].SetAsEnd();
      }
    }
    if (!exists) {
      std::cerr << "Ending state doesn't exist, have you configured the automaton correctly?" << std::endl;
      throw;
    }
  }
}

void Automaton::ReadStackStart(std::string line) {
  std::istringstream formattedLine(line);
  std::string formattedWord;
  int startingStackSymbols = 0;
  while (formattedLine >> formattedWord) {
    startingStackSymbols++;
    bool exists = false;
    for (int i = 0; i < stackSymbols.size(); i++) {
      if (stackSymbols[i] == formattedWord) {
        exists = true;
        startingStack = formattedWord;
      }
    }
    if (!exists) {
      std::cerr << "Initial stack symbol doesn't exist, have you configured the automaton correctly?" << std::endl;
      throw;
    }
    if (startingStackSymbols > 1) {
      std::cerr << "Cannot have more than one starting stack symbol for the automaton!" << std::endl;
      throw;
    }
  }
}

void Automaton::ReadTransitions(std::vector<std::string> transitions) {
  for (int i = 6; i < transitions.size(); i++) {
    std::istringstream formattedLine(transitions[i]);

    std::string startingState;
    formattedLine >> startingState;
    std::string tapeSymbol;
    formattedLine >> tapeSymbol;
    std::string stackSymbol;
    formattedLine >> stackSymbol;
    std::string endingState;
    formattedLine >> endingState;
    if (formattedLine.fail()) {
      std::cerr << "There has been an error extracting the transitions, are they correctly defined?" << std::endl;
      throw;
    }
    std::string element;
    std::vector<std::string> stackWrite;
    while (formattedLine >> element) {
      stackWrite.push_back(element);
    }
    
    Transition newTransition;
    bool tapeExist = false;
    for (int i = 0; i < tapeSymbols.size(); i++) {
      if (tapeSymbols[i] == tapeSymbol) {
        tapeExist = true;
        newTransition.tapeSymbol = tapeSymbol;
      }
    }

    bool stackExist = false;
    for (int i = 0; i < stackSymbols.size(); i++) {
      if (stackSymbols[i] == stackSymbol) {
        stackExist = true;
        newTransition.stackSymbol = stackSymbol;
      }
    }

    bool endExist = false;
    for (int i = 0; i < states.size(); i++) {
      if (states[i].GetName() == endingState) {
        endExist = true;
        newTransition.endingState = endingState;
      }
    }

    for (int j = 0; j < stackWrite.size(); j++) {
      bool symbolExists = false;
      for (int i = 0; i < stackSymbols.size(); i++) {
        if (stackSymbols[i] == stackWrite[j]) {
          symbolExists = true;
          newTransition.toStack.push_back(stackWrite[j]);
        }
      }
      if (!symbolExists) {
        std::cerr << "Symbol in transition is not defined in the automaton" << std::endl;
        throw;
      }
    }

    bool startExist = false;
    for (int i = 0; i < states.size(); i++) {
      if (states[i].GetName() == startingState) {
        startExist = true;
        states[i].AddTransition(newTransition);
      }
    }

    if (!startExist || !endExist || !stackExist || !tapeExist) {
      std::cerr << "There are one or more symbols and/or states in the transitions that aren't defined in the automaton" << std::endl;
      throw;
    }
  }
}

bool Automaton::Solve(std::string input, bool trace) {
  // Initializes the stack and indexPointer
  std::string currentState = startingState;
  int currentIndex = 0;
  stack.push(startingStack);

  // Calls step, a recursive function that calculates the result
  bool result = Step(stack, currentState, input, currentIndex, trace);

  // Clearing up the stack for any other calls to Solve
  while (!stack.empty()) {
    stack.pop();
  }

  // Print and return result
  if (result) {
    std::cout << input << " is recognized by the automaton" << std::endl;
  } else {
    std::cout << input << " is not recognized by the automaton" << std::endl;
  }
  return result;
}

bool Automaton::Step(std::stack<std::string> givenStack, std::string currentState, std::string input, int currentIndex, bool trace, bool epsilon) {
  std::vector<Transition> possibleTransitions;
  // We read the tape
  std::string currentTapeSymbol(1, input[currentIndex]);
  // Given that an automaton can choose not to consume an element from the tape, we have to account for this possibility
  if (epsilon) {
    currentTapeSymbol = ".";
    currentIndex--;
  }
  // If we are at the end of the string, the consumed token will be epsilon ("."")
  if (currentIndex >= input.size()) {
    currentTapeSymbol = ".";
  }
  // We pop the stack
  std::string currentStackSymbol = givenStack.top();
  if (currentStackSymbol.size() == 0) {
    currentStackSymbol = ".";
  }
  givenStack.pop();

  // If we are in an ending state and there are no more tokens to consume, we return true,
  // we find all possible transitions for the current state we are in otherwise
  for (int i = 0; i < states.size(); i++) {
    if (states[i].GetName() == currentState) {
      if (currentIndex >= input.size() && states[i].GetEnd()) {
        return true;
      }
      possibleTransitions = states[i].GetTransitions();
    }
  }
  // We filter those that can't be used
  for (int i = 0; i < possibleTransitions.size(); i++) {
    if (possibleTransitions[i].tapeSymbol != currentTapeSymbol || possibleTransitions[i].stackSymbol != currentStackSymbol) {
      possibleTransitions.erase(possibleTransitions.begin() + i);
      i--;
    }
  }

  // If trace is specified, show information about the step
  if (trace) {
    std::cout << "Step: " << currentIndex << ", State: " << currentState << ", Read: " << currentTapeSymbol << ", Popped: " << currentStackSymbol << ", Possible transitions: | ";
    if (possibleTransitions.size() == 0) {
      std::cout << "NONE |";
    }
    for (int i = 0; i < possibleTransitions.size(); i++) {
      std::cout << possibleTransitions[i].tapeSymbol << " / " << possibleTransitions[i].stackSymbol << " / " << possibleTransitions[i].endingState << " - ";
      for (int j = 0; j < possibleTransitions[i].toStack.size(); j++) {
        std::cout << possibleTransitions[i].toStack[j] << " ";
      }
      std::cout << "| ";
    }
  std::cout << std::endl;
  }

  // If there aren't any possible transitions, return false
  if (possibleTransitions.size() == 0) {
    return false;
  }

  // If we aren't at the end of the input, we move up the pointer
  if (currentIndex < input.size()) {
    currentIndex++;
  }
  // We try each possible transition until one of them brings us to an ending state having emptied the tape or none of them do
  for (int i = 0; i < possibleTransitions.size(); i++) {
    std::stack<std::string> transitionStack = givenStack;
    for (int j = possibleTransitions[i].toStack.size() - 1; j >= 0; j--) {
      if (possibleTransitions[i].toStack[j] != ".") {
        transitionStack.push(possibleTransitions[i].toStack[j]);
      }
    }
    if (Step(transitionStack, possibleTransitions[i].endingState, input, currentIndex, trace)) {
      return true;
    }
    // If we chose to not consume a token this step, nothing happens, we try to not consume it this time otherwise
    if (!epsilon) {
      if (Step(transitionStack, possibleTransitions[i].endingState, input, currentIndex, trace, true)) {
        return true;
      }
    }
  }
  return false;
}