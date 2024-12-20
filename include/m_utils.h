#ifndef DEV_UTILS_H
#define DEV_UTILS_H

#include <fstream>
#include <string>

bool ReadFile(const char* fileName, std::string& outFile);
char* ReadBinaryFile(const char* fileName, int& size);

#endif