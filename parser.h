#include <string>
#include <fstream>
#include <stdexcept>
#include <stdint.h>

#define L_COMMAND 0
#define A_COMMAND 1
#define C_COMMAND 2

using namespace std;

class Parser {

  private: 
    ifstream inFile;
    string line, cmd;

  public:
    Parser(string filename) {
      inFile.open(filename, ios::in);
      
      if (!inFile.is_open()) 
        throw runtime_error("Error opening file");
    }

    ~Parser() {
      inFile.close();
    }

    bool hasMoreCommands() {
      bool valid = true;
      bool instruction = false;
      while (valid) {
        line.clear();
        if (inFile.eof()) {
          return false;
        }
        getline(inFile, line);
        if (line.size() == 0) {
          continue;
        };
        for (size_t i = 0; i < line.size() - 1; i++) {
          if (line[i] == '/' && line[i + 1] == '/') {
             if (instruction) {
                printf("Error at %s\n", line.c_str());
                throw runtime_error("Error in input file");
             }
             break;
          }
          if (instruction && (line[i] == ' ')) {
            valid = false;
            instruction = false;
            break;
          }
          if (!instruction && (line[i] == '(' || line[i] == '@' || line[i] == '0' || line[i] == 'M' || line[i] == 'D' || line[i] == 'A')) {
            instruction = true;
            valid = false;
          }
        }
      }
      return true;
    }

    void seek() {
      inFile.clear();
      inFile.seekg(0);
    }

    void advance() {
      bool instruction;
      int8_t pos_s = 0, pos_e = line.size();
      instruction = false;
      for (uint8_t i = 0; i < line.size(); i++) {
        if (instruction && (line[i] == ' ')) {
          pos_e = i;
          break;
        }
        if (!instruction && (line[i] == '(' || line[i] == '@' || line[i] == '0' || line[i] == 'M' || line[i] == 'D' || line[i] == 'A')) {
          pos_s = i;
          instruction = true;
          continue;
        }
      }
      cmd = line.substr(pos_s, pos_e - pos_s);
    }
    
    int8_t commandType() {
      if (cmd[0] == '@') {
        return A_COMMAND;
      }
      else if (cmd[0] == '(') {
        return L_COMMAND;
      }
      else if (cmd[0] == '0' || cmd[0] == 'M' || cmd[0] == 'D' || cmd[0] == 'A') {
        return C_COMMAND;
      }
      else {
        return -1;
      }
    }

    string symbol() {
      if (cmd[0] == '@') {
        return cmd.substr(1);
      }
      return cmd.substr(1, cmd.size() - 2);
    }

    string dest() {
      int8_t pos = cmd.find('=');
      if (pos == -1) {
        return "";
      }
      return cmd.substr(0, pos); 
    }
    
    string comp() {
      int8_t pos0 = cmd.find("=") + 1;
      int8_t pos1 = cmd.find(";");
      if (pos1 == -1) {
        pos1 = cmd.size();
      }
      if (pos1 == -1) {
        return "";
      }
      return cmd.substr(pos0, pos1 - pos0);
    }

    string jump() {
      int8_t pos1 = cmd.find(";");
      if (pos1 == -1) {
        return ""; 
      }
      return cmd.substr(pos1 + 1);
    }
};
