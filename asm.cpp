#include "parser.h"
#include "code.h"
#include "symbol.h"

#include <algorithm>

#define IN_EXT "asm"
#define OUT_EXT ".hack"

using namespace std;

bool isNumber(string str) {
  for (size_t i = 0; i < str.size(); i++) {
    if (isdigit(str[i]) == 0) return false;
  }
  return true;
}

string decToBin(int dec) {
  string bin;
  while (dec) {
    bin.push_back(dec % 2 ? '1' : '0');
    dec /= 2;
  }
  int8_t size = 15 - bin.size();
  for (int8_t i = 0; i < size; i++) {
    bin.push_back('0');
  }
  reverse(bin.begin(), bin.end());
  return bin;
}

int main(int argc, char **argv) {
  string inFilename, outFilename;
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "usage: asm input_filename.asm (optional)output_filename.hack\n");
    exit(1);
  }

  inFilename = argv[1];
  outFilename = argv[1];

  if (argc == 3) {
    outFilename = argv[2];
  }
  else {
    size_t found;
    if ((found = outFilename.find(".asm")) == string::npos) {
      fprintf(stderr, "File not recognized\n");
      exit(1);
    }
    outFilename.replace(found + 1, 4, "hack");
  }

  uint32_t pc = 0;
  try {
    Parser parser(inFilename);
    SymbolTable symbolTable;
    // First pass. 
    while (parser.hasMoreCommands()) {
      parser.advance();       
      if (parser.commandType() == L_COMMAND) {
        symbolTable.addEntry(parser.symbol(), pc); 
      }
      else {
        pc++;
      }
    }

    // Second pass.
    parser.seek();
    Code code; 

    ofstream outFile(outFilename, ios::out | ios::trunc ); 
    if (!outFile.is_open()) 
      throw runtime_error("Error opening outFile");

    int8_t type = 0, address;
    string symbol;
    string instruction;
    while (parser.hasMoreCommands()) {
      string binary_address;
      parser.advance();       
      type = parser.commandType();
      if (type == A_COMMAND) {
        instruction = "0";
        symbol = parser.symbol();
        if (isNumber(symbol)) {
          binary_address = decToBin(stoi(symbol));
        }
        else if ((symbolTable.contains(symbol))) {
          binary_address = decToBin(symbolTable.getAddress(symbol));
        }
        else {
          address = ++symbolTable.used;
          symbolTable.addEntry(symbol, address);
          binary_address = decToBin(address);
        }
        instruction.append(binary_address);
      }
      else if (type == C_COMMAND) {
        instruction = "111";
        instruction.append(code.comp(parser.comp()));
        instruction.append(code.dest(parser.dest()));
        instruction.append(code.jump(parser.jump()));
      }
      instruction.push_back('\n');
      outFile.write(instruction.c_str(), instruction.size());
    }
    outFile.close();
  }
  catch (const std::runtime_error& e){
    printf("%s\n", e.what());
  }
  return 0;
}
