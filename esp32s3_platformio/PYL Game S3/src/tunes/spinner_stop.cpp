#include "tuneplayer.h"

Tune spinner_stop() {
    Tune tune;
    tune.loopable = false;
    tune.debug = false;
    tune.notes = {
        {640,250},
        {8, 250},
        {640, 250},
        {8, 0}
    };
    return tune;
}