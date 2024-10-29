#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>

void debug(const char* format, ...);
void mkscore(int score, std::string& score_str);
int rng(int x);

#endif // FUNCTIONS_H