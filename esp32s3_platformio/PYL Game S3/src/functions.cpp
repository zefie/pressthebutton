#include <random>
#include <stdarg.h>

void debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

void mkscore(int score, std::string& score_str) {
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%08d", score);
    score_str = buffer;
}

int rng(int x) {
    // Seed with a real random value, if available
    static std::random_device rd;

    // Choose a random number generator
    static std::mt19937 gen(rd());

    // Define the distribution range [0, x]
    std::uniform_int_distribution<> distrib(0, x-1);

    return distrib(gen);
}