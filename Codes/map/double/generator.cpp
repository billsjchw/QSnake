#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

typedef pair<int, int> Point;

const string name = "00";
const int M = 30;
const int N = 20;

int main() {
    ifstream fin((name + ".double.raw").c_str());
    ofstream fout((name + ".double.map").c_str());

    int lenRed, lenBlue;
    char wasdRed, wasdBlue;
    Point headRed, headBlue;
    vector<Point> bricks;
    fin >> lenRed >> wasdRed >> lenBlue >> wasdBlue;
    for (int j = 0; j < N; ++j)
        for (int i = 0; i < M; ++i) {
            char c;
            fin >> c;
            switch (c) {
            case '#':
                bricks.push_back(make_pair(i, j)); 
                break;
            case '+':
                headRed = make_pair(i, j);
                break;
            case '-':
                headBlue = make_pair(i, j);
                break;
            default:
                break;
            }
        }
    int num = bricks.size();
    fout << lenRed << " " << wasdRed << endl;
    fout << headRed.first << " " << headRed.second << endl;
    fout << lenBlue << " " << wasdBlue << endl;
    fout << headBlue.first << " " << headBlue.second << endl;
    fout << num << endl;
    for (int i = 0; i < num; ++i)
        fout << bricks[i].first << " " << bricks[i].second << endl;

    return 0;
}