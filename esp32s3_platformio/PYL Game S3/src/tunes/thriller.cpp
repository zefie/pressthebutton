#include "tuneplayer.h"

Tune thriller() {
    Tune tune;
    tune.loopable = true;
    tune.debug = false;
    tune.notes = {
        {415, 260}, {8, 4}, {494, 260}, {8, 8}, {415, 260}, {8, 8},
        {554, 132}, {8, 128}, {8, 128}, {8, 4}, {8, 4}, {494, 124}, {8, 4},
        {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 8}, {8, 128}, {8, 132},
        {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 128}, {8, 4}, {8, 124},
        {8, 4}, {8, 4}, {494, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 8},
        {466, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 8}, {415, 128},
        {8, 132}, {8, 132}, {8, 128}, {8, 4}, {8, 132}, {8, 128}, {8, 8},
        {415, 128}, {8, 60}, {8, 68}, {8, 8}, {415, 132}, {8, 128}, {8, 8},
        {370, 132}, {8, 128}, {8, 8}, {370, 128}, {8, 4}, {8, 0}, {277, 128},
        {8, 124}, {8, 8}, {8, 0}, {330, 128}, {8, 128}, {8, 132}, {8, 8},
        {277, 132}, {8, 128}, {8, 8}, {330, 132}, {8, 128}, {8, 8},
        {370, 128}, {8, 4}, {8, 0}, {415, 128}, {8, 132}, {8, 128}, {8, 8},
        {370, 132}, {8, 128}, {8, 4}, {370, 132}, {8, 128}, {8, 8},
        {330, 128}, {8, 4}, {8, 4}, {370, 124}, {8, 128}, {8, 132}, {8, 4},
        {8, 4}, {415, 132}, {8, 124}, {8, 4}, {8, 4}, {494, 132}, {8, 128},
        {8, 8}, {415, 132}, {8, 60}, {8, 64}, {8, 4}, {8, 4}, {554, 132},
        {8, 128}, {8, 128}, {8, 4}, {8, 4}, {494, 124}, {8, 4}, {8, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 8}, {8, 128}, {8, 132}, {8, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 8}, {494, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 8}, {466, 132}, {8, 128}, {8, 132},
        {8, 128}, {8, 8}, {415, 128}, {8, 132}, {8, 132}, {8, 128}, {8, 4},
        {8, 132}, {8, 128}, {8, 8}, {415, 132}, {8, 60}, {8, 20}, {8, 48},
        {8, 8}, {415, 132}, {8, 128}, {8, 8}, {370, 132}, {8, 128}, {8, 8},
        {370, 132}, {8, 128}, {8, 8}, {330, 128}, {8, 4}, {8, 0}, {330, 128},
        {8, 132}, {8, 128}, {8, 8}, {277, 132}, {8, 128}, {8, 4}, {330, 132},
        {8, 128}, {8, 8}, {370, 132}, {8, 60}, {8, 20}, {8, 36}, {8, 4},
        {8, 4}, {8, 36}, {8, 96}, {8, 128}, {8, 8}, {330, 128}, {8, 124},
        {8, 4}, {8, 4}, {277, 260}, {8, 4}, {8, 132}, {8, 128}, {8, 8},
        {392, 128}, {8, 124}, {8, 4}, {8, 4}, {330, 260}, {8, 4}, {8, 132},
        {8, 128}, {8, 8}, {494, 124}, {8, 4}, {8, 4}, {554, 132}, {8, 128},
        {8, 132}, {8, 128}, {8, 128}, {8, 4}, {8, 4}, {494, 124}, {8, 4},
        {8, 4}, {415, 132}, {8, 128}, {8, 128}, {8, 132}, {8, 8}, {370, 132},
        {8, 128}, {8, 8}, {330, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 4},
        {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 132},
        {8, 60}, {8, 68}, {8, 128}, {8, 132}, {8, 132}, {8, 128}, {8, 132},
        {8, 128}, {8, 128}, {8, 4}, {8, 128}, {8, 132}, {8, 128}, {8, 132},
        {247, 132}, {8, 128}, {8, 8}, {277, 132}, {8, 128}, {8, 8},
        {311, 128}, {8, 4}, {8, 0}, {330, 128}, {8, 132}, {8, 128}, {8, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 8}, {277, 128},
        {8, 4}, {247, 128}, {8, 8}, {208, 132}, {8, 128}, {8, 4}, {8, 132},
        {8, 128}, {8, 132}, {8, 4}, {208, 128}, {8, 4}, {247, 132}, {8, 128},
        {8, 8}, {277, 132}, {8, 128}, {8, 8}, {277, 132}, {8, 128}, {8, 8},
        {247, 132}, {8, 128}, {8, 8}, {277, 128}, {8, 132}, {8, 8},
        {247, 132}, {8, 124}, {8, 4}, {8, 4}, {277, 132}, {8, 128}, {8, 8},
        {330, 128}, {8, 4}, {8, 4}, {277, 128}, {8, 128}, {8, 4}, {8, 4},
        {247, 124}, {8, 4}, {8, 4}, {208, 132}, {8, 128}, {8, 132}, {8, 128},
        {8, 132}, {8, 128}, {8, 4}, {8, 128}, {8, 132}, {8, 132}, {8, 128},
        {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 128}, {8, 132},
        {8, 8}, {247, 132}, {8, 128}, {8, 8}, {277, 132}, {8, 128}, {8, 8},
        {311, 128}, {8, 4}, {8, 0}, {330, 128}, {8, 132}, {8, 128}, {8, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 4}, {8, 4},
        {277, 128}, {8, 8}, {247, 124}, {8, 4}, {8, 4}, {208, 132}, {8, 128},
        {8, 132}, {8, 128}, {8, 128}, {8, 8}, {208, 128}, {8, 4}, {247, 132},
        {8, 128}, {8, 8}, {277, 132}, {8, 128}, {8, 8}, {277, 132}, {8, 128},
        {8, 8}, {247, 132}, {8, 128}, {8, 8}, {277, 128}, {8, 132}, {8, 8},
        {247, 132}, {8, 124}, {8, 4}, {8, 4}, {277, 132}, {8, 128}, {8, 8},
        {330, 128}, {8, 4}, {8, 0}, {277, 128}, {8, 124}, {8, 8}, {8, 0},
        {247, 128}, {8, 8}, {208, 132}, {8, 128}, {8, 132}, {8, 128},
        {8, 132}, {8, 128}, {8, 4}, {8, 128}, {8, 132}, {8, 8}, {277, 132},
        {8, 128}, {8, 8}, {330, 132}, {8, 128}, {8, 8}, {415, 128}, {8, 4},
        {370, 128}, {8, 124}, {8, 8}, {8, 0}, {330, 128}, {8, 8}, {277, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 4}, {8, 128},
        {8, 132}, {8, 132}, {8, 128}, {8, 132}, {8, 4}, {370, 124}, {8, 132},
        {8, 128}, {8, 4}, {370, 128}, {8, 4}, {8, 4}, {330, 124}, {8, 128},
        {8, 4}, {8, 4}, {330, 124}, {8, 128}, {8, 4}, {8, 4}, {311, 124},
        {8, 128}, {8, 4}, {8, 4}, {311, 124}, {8, 128}, {8, 8}, {277, 124},
        {8, 128}, {8, 4}, {8, 4}, {277, 128}, {8, 128}, {8, 4}, {8, 4},
        {311, 124}, {8, 128}, {8, 8}, {330, 128}, {8, 128}, {8, 4}, {8, 4},
        {277, 124}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 132},
        {8, 128}, {8, 8}, {8, 128}, {8, 132}, {8, 132}, {8, 128}, {8, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128},
        {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 128}, {8, 132}, {8, 8},
        {277, 132}, {8, 128}, {8, 8}, {330, 132}, {8, 128}, {8, 8},
        {415, 128}, {8, 4}, {370, 128}, {8, 124}, {8, 8}, {8, 0}, {330, 128},
        {8, 8}, {277, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 132}, {8, 128},
        {8, 8}, {8, 128}, {8, 132}, {8, 132}, {8, 128}, {8, 132}, {8, 4},
        {370, 124}, {8, 132}, {8, 128}, {8, 4}, {370, 128}, {8, 4}, {8, 4},
        {330, 124}, {8, 128}, {8, 4}, {8, 4}, {330, 124}, {8, 128}, {8, 4},
        {8, 4}, {311, 124}, {8, 128}, {8, 4}, {8, 4}, {311, 124}, {8, 128},
        {8, 8}, {277, 124}, {8, 128}, {8, 4}, {8, 4}, {277, 128}, {8, 128},
        {8, 4}, {8, 4}, {311, 124}, {8, 128}, {8, 4}, {8, 0}, {330, 128},
        {8, 4}, {8, 132}, {8, 128}, {8, 132}, {8, 128}, {8, 4}, {8, 132},
        {8, 128}, {8, 132}, {8, 128}, {8, 8}, {330, 128}, {8, 132}, {8, 8},
        {370, 132}, {8, 128}, {8, 8}, {330, 132}, {8, 128}, {8, 8},
        {415, 128}, {8, 4}, {8, 0}, {330, 128}, {8, 8}, {370, 132}, {8, 128},
        {8, 132}, {8, 128}, {8, 4}, {8, 132}, {8, 128}, {8, 132}, {8, 128},
        {8, 128}, {8, 132}, {8, 4}, {8, 4}, {8, 0}
    };
    return tune;
}