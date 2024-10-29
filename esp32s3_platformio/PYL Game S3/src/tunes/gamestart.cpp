#include "tuneplayer.h"

Tune gamestart() {
    Tune tune;
    tune.loopable = false;
    tune.debug = false;
    tune.notes = {
        {262, 109}, {8, 36}, {349, 109}, {8, 39}, {392, 109}, {8, 39},
        {262, 109}, {8, 39}, {330, 109}, {8, 39}, {294, 109}, {8, 39},
        {392, 121}, {8, 0}
    };
    return tune;
}