#include <string>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

struct alignment_result{
    int score;
    vector<string> s1;
    vector<string> s2;
};

alignment_result alignment(vector<string> s1, vector<string> s2, int gapop, int gapex, map<string, map<string, int> > scoring_matrix, bool local);
