#include "m_utils.h"

bool ReadFile(const char* fileName, std::string& outFile){
    std::ifstream f(fileName);

    if(f.is_open()){
        std::string line;

        while(getline(f, line)){
            outFile.append(line);
            outFile.append("\n");
        }
        f.close();

        return true;
    }

    return false;
}