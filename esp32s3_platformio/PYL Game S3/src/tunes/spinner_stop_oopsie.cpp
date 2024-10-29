#include "tuneplayer.h"

Tune spinner_stop_oopsie() {
    Tune tune;
    tune.loopable = false;
    tune.debug = false;
    tune.notes = {
        {164, 500},
        {69, 750},
        {8, 0}
    };
    return tune;
}