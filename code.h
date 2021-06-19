#include <map>

using namespace std;

class Code {
  private:
    map<string, string> dest_map = { 
      {"", "000"}, {"M", "001"}, 
      {"D", "010"}, {"MD", "011"}, 
      {"A", "100"}, {"AM", "101"},
      {"AD", "110"}, { "AMD", "111"}
    };

    map<string, string> jump_map = { 
      {"", "000"}, {"JGT", "001"}, 
      {"JEQ", "010"}, {"JGE", "011"}, 
      {"JLT", "100"}, {"JNE", "101"},
      {"JLE", "110"}, { "JMP", "111"}
    };

    map<string, string> comp_map = { 
      { "0", "101010" }, { "1", "111111" }, { "-1", "111010" },
      { "D", "001100" }, { "A", "110000" }, { "!D", "001101" },
      { "!A", "110001" }, { "-D", "001111" }, { "-A", "110011" },
      { "D+1", "011111" }, { "A+1", "110111" }, { "D-1", "001110" }, 
      { "A-1", "110010"}, { "D+A", "000010"}, { "D-A" , "010011" }, 
      { "A-D", "000111" }, { "D&A", "000000" }, {"D|A", "010101" }
    };         

  public:   
    string dest(string mnemonic) {
      for (map<string, string>::iterator itr = dest_map.find(mnemonic); itr != dest_map.end(); itr++)  {
        return itr -> second;
      }
      return "Error from code.h 35";
    }

    string comp(string mnemonic) {
      int pos;
      string b = "0";
      pos = mnemonic.find('M');
      if (pos != -1) {
        b = "1";
        mnemonic.replace(pos, 1, "A"); 
      }
      for (map<string, string>::iterator itr = comp_map.find(mnemonic); itr != comp_map.end(); itr++)  {
        return b.append(itr -> second);
      }
      return "Error from code.h 50";
    }

    string jump(string mnemonic) {
      for (map<string, string>::iterator itr = jump_map.find(mnemonic); itr != jump_map.end(); itr++)  {
        return itr -> second;
      }
      return "Error from code.h 57";
    }
};
