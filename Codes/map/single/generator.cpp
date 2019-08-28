#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

typedef pair<int, int> Point;

const string name = "01";
const int M = 20;
const int N = 20;

int main() {
    ifstream fin((name + ".single.raw").c_str());
    ofstream fout((name + ".single.map").c_str());

    int len;
    char wasd;
    Point head;
    vector<Point> bricks;
    fin >> len >> wasd;
    for (int j = 0; j < N; ++j)
        for (int i = 0; i < M; ++i) {
            char c;
            fin >> c;
            switch (c) {
            case '#':
                bricks.push_back(make_pair(i, j)); 
                break;
            case '*':
                head = make_pair(i, j);
                break;
            default:
                break;
            }
        }
    int num = bricks.size();
    fout << len << " " << wasd << endl;
    fout << head.first << " " << head.second << endl;
    fout << num << endl;
    for (int i = 0; i < num; ++i)
        fout << bricks[i].first << " " << bricks[i].second << endl;

    return 0;
}