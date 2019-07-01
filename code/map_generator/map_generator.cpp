#include <iostream>
#include <fstream>
using namespace std;

int map[88];
int top = 0; 

int main() {
    for (int i = 4; i < 12; ++i)
        for (int j = 3; j < 4; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    for (int i = 18; i < 26; ++i)
        for (int j = 3; j < 4; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    for (int i = 4; i < 12; ++i)
        for (int j = 16; j < 17; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    for (int i = 18; i < 26; ++i)
        for (int j = 16; j < 17; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    for (int i = 4; i < 5; ++i)
        for (int j = 4; j < 7; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    for (int i = 4; i < 5; ++i)
        for (int j = 13; j < 16; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    for (int i = 25; i < 26; ++i)
        for (int j = 4; j < 7; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    for (int i = 25; i < 26; ++i)
        for (int j = 13; j < 16; ++j) {
            map[top++] = i;
            map[top++] = j;
        }
    int x = 14, y = 8, dir = 1;
    ofstream fout;
    fout.open("double01.map", ios::binary);
    fout.write((char *) &x, sizeof(x))
        .write((char *) &y, sizeof(y))
        .write((char *) &dir, sizeof(dir));
    x = 15; y = 10; dir = 2;
    fout.write((char *) &x, sizeof(x))
        .write((char *) &y, sizeof(y))
        .write((char *) &dir, sizeof(dir));
    fout.write((char *) map, sizeof(map));
    fout.close();
    return 0;
}