#include <string>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

struct alignment_result{
    int score;
    string s1;
    string s2;
};

int score(string s1, string s2, int gapop, int gapex, map<char, map<char, int> > scoring_matrix, bool local);

alignment_result alignment(string s1, string s2, int gapop, int gapex, map<char, map<char, int> > scoring_matrix, bool local);

int fitting_score(string s1, string s2, int gap, map<char, map<char, int> > scoring_matrix);

int overlap_score(string s1, string s2, int gap, map<char, map<char, int> > scoring_matrix);

int semiglobal_score(string s1, string s2, int gap, map<char, map<char, int> > scoring_matrix);

int no_deletion_score(string s, string t, int gap, map<char, map<char, int> > scoring_matrix);

vector<pair<int, int> > k_edit_matches(int k, string text, string pattern);