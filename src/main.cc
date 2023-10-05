#include <iostream>
#include <string.h>
#include "../include/automaton.h"

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    std::cerr << "This program must be called with at least an argument, this being the file path to the automaton definition. Try -h for help" << std::endl;
    return 0;
  }
  if (strcmp(argv[1], "-h") == 0) {
    std::cout << "Automaton - Program that reads a file and generates a pushdown automaton with acceptance by final state accordingly.\nAuthor: Edwin Plasencia Hernández\n\nUsage: automaton <filepath> [-t]\n-t  |  Shows a trace of the automaton\n\nFormatting:\nComments can be used with \'#\' at any point" << std::endl;
    std::cout << "1st line - States\n2nd line - Tape symbols\n3rd line - Stack symbols\n4th line - Starting state\n5th line - Starting stack symbol\n6th line - Final states\n7th line onwards - Transitions" << std::endl;
    std::cout << "Examples can be found in the \"/test\" directory." << std::endl;
    return 0;
  }
  bool trace = false;
  if (strcmp(argv[2], "-t") == 0) {
    trace = true;
  }
  Automaton automaton(argv[1]);
  std::cout << "Automaton created correctly, input any string to check if the automaton recognizes it and use Ctrl+C to stop execution: ";
  for (;;) {
    std::string string;
    std::cin >> string;
    automaton.Solve(string, trace);
  }
  return 0;
}