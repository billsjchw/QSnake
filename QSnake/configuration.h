#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>

struct Configuration: public QObject {
    enum {SINGLE, DOUBLE};
    int player;
    enum {DISABLE, ENABLE};
    int through;
    int bricks;
    int star;
    static const int MAX_BLOCK_NUM_SINGLE = 3;
    static const int MAX_BLOCK_NUM_DOUBLE = 5;
    int blockNum;
    Configuration();
};

extern Configuration conf;

#endif // CONFIGURATION_H
