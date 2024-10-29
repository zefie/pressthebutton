#include "tuneplayer.h"

Tune spinner_tune() {
    Tune tune;
    tune.loopable = true;
    tune.debug = false;
    tune.callback_interval = 4;
    tune.callback_at_start = true;
    tune.notes = {
        {293,100}, {8, 50},
        {311,100}, {8, 50},
        {391,100}, {8, 50},
        {440,100}, {8, 50},
        {554,100}, {8, 50},
        {369,100}, {8, 50},
        {277,100}, {8, 50},
        {493,100}, {8, 50},
        {329,100}, {8, 50},
        {554,100}, {8, 50},
        {466,100}, {8, 50},
        {415,100}, {8, 50},
        {523,100}, {8, 50},
        {311,100}, {8, 50},
        {349,100}, {8, 50},
        {415,100}, {8, 50}
    };
    return tune;
}