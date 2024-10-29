#ifndef TUNE_H
#define TUNE_H

struct Note {
    int frequency;  // Frequency in Hz
    int duration;   // Duration in milliseconds
};

struct Tune {
    float multiplier = 1.0f;
    bool loopable = false;
    std::function<void()> callback = nullptr;
    int callback_interval = 0;
    bool callback_at_start = false;
    bool debug = false;
    std::vector<Note> notes;
};

#endif // #TUNE_H