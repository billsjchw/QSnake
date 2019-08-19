#include "configuration.h"

Configuration conf;

Configuration::Configuration():
    player(SINGLE),
    through(DISABLE),
    bricks(DISABLE),
    star(DISABLE),
    blockNum(0) {}
