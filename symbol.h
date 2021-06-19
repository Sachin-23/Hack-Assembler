#include <map>

using namespace std;

class SymbolTable {
  private:
    map<string, int> symbol_table = {
      {"SP",  0x0}, {"LCL", 0x1}, {"ARG",  0x2}, {"THIS",  0x3}, {"THAT",  0x4}, 
      {"R0",  0x0}, {"R1",  0x1}, {"R2",  0x2}, {"R3",  0x3}, 
      {"R4",  0x4}, {"R5",  0x5}, {"R6",  0x6}, {"R7",  0x7}, 
      {"R8",  0x8}, {"R9",  0x9}, {"R10",  0xa}, {"R11",  0xb}, 
      {"R12",  0xc}, {"R13",  0xd}, {"R14",  0xe}, {"R15",  0xf}, 
      {"SCREEN",  0x4000}, {"KBD",  0x6000}, 
    };         

  public:
    int used = 15;
    void print() {
      map<string, int>::iterator itr;
      printf("---------------\n");
      for (itr = symbol_table.begin(); itr != symbol_table.end(); ++itr) {
        printf("%s \t| %d\n", (itr -> first).c_str(), itr -> second);
      }
      printf("---------------\n");
    }

    int getAddress(string key) {
      for (map<string, int>::iterator itr = symbol_table.find(key); itr != symbol_table.end(); itr++)  {
        return itr -> second;
      }
      return -1;
    }

    bool contains(string symbol) {
      if (symbol_table.count(symbol)) {
        return true;
      }
      return false;
    }

    int addEntry(string symbol, int address) {
      if (address < 0) {
        return -1;
      }
      symbol_table.insert({ symbol, address });
      return 0;
    }
};
