// find Levenshtein distance and local alignment matrix with scoring matrix and affine gap penalties

#include "stepik_alignment.h"
#include <iostream>
#include <iomanip>

using namespace std;

const int N = 15000;
const int INF = 1000000;
const char this_ = 0;
const char up_ = 1;
const char down_ = 2;
const char zero_ = 3; // new local alignment


int score(string s1, string s2, int gapop, int gapex, map<char, map<char, int> > scoring_matrix, bool local) {
    /* Total gap penalty is gapop + gapex * (L - 1) where L is the length  of the gap */
    gapop -= gapex;
    int d[3][N + 2][2];
    // current is d[i][j][c], previous: d[i][j][1 - c]
    int c = 0;
    // 0 - upper matrix (gaps)
    // 1 - usual matrix
    // 2 - lower matrix (gaps)
    // base:
    if(local){
        for (size_t j = 0; j <= s2.size(); ++j) {
            d[0][j][c] = 0;
            d[1][j][c] = 0;
            d[2][j][c] = 0;
        }
    }else{
        for (size_t j = 0; j <= s2.size(); ++j) {
            d[0][j][c] = -gapop - j * gapex;
            d[1][j][c] = -gapop - j * gapex;
            d[2][j][c] = -INF;
        }
        d[0][0][c] = d[1][0][c] = d[2][0][c] = 0;
    }
    int maxscore = 0;
    for (size_t i = 1; i <= s1.size(); ++i) {
        c = 1 - c;
        if(local){
            d[0][0][c] = d[1][0][c] = 0;
        }else{
            d[2][0][c] = d[1][0][c] = -gapop - i * gapex;
            d[0][0][c] = -INF;
        }
        for (size_t j = 1; j <= s2.size(); ++j) {
            int up, down, ths;
            int zero = 0;
            ths  = d[0][j-1][c] - gapex;
            down = d[1][j-1][c] - gapop - gapex;
            if (zero > ths && zero > down && local) {
                d[0][j][c] = zero;
            }
            else if (ths > down) {
                d[0][j][c] = ths;
            }
            else {
                d[0][j][c] = down;
            }
            // lower
            ths = d[2][j][1-c] - gapex;
            up  = d[1][j][1-c] - gapop - gapex;
            if (zero > ths && zero > up && local) {
                d[2][j][c] = zero;
            }
            else if (ths > up) {
                d[2][j][c] = ths;
            }
            else {
                d[2][j][c] = up;
            }
            // middle
            ths  = d[1][j-1][1-c] + scoring_matrix[s1[i-1]][s2[j-1]];
            up   = d[0][j][c];
            down = d[2][j][c];
            if (zero > ths && zero > up && zero > down  && local) {
                d[1][j][c] = zero;
            }
            else if (ths > up && ths > down) {
                d[1][j][c] = ths;
            }
            else if (up > down) {
                d[1][j][c] = up;
            }
            else {
                d[1][j][c] = down;
            }
            for(size_t k = 0; k < 3; k++){
                if (d[k][j][c] > maxscore){
                    maxscore = d[k][j][c];
                }
            }
        }
    }

    if(local){
        return maxscore;
    }else{
        maxscore = d[1][s2.size()][c];
         for (int k = 0; k < 3; ++k) {
            if (d[k][s2.size()][c] > maxscore) {
                maxscore = d[k][s2.size()][c];
            }
        }
        return maxscore;
    }
}

//debug print
template <class T>
void print_matrix(vector<vector<vector<T> > > v);

alignment_result alignment(vector<string> s1, vector<string> s2, int gapop, int gapex, map<string, map<string, int> > scoring_matrix, bool local) {
    /* Total gap penalty is gapop + gapex * (L - 1) where L is the length  of the gap */
    gapop -= gapex;
    vector<vector<vector<char> > > p(3, vector<vector<char> >(s1.size() + 1, vector<char>(s2.size() + 1))); // parent
    vector<vector<vector<int> > > d(3, vector<vector<int> >(s1.size() + 1, vector<int>(s2.size() + 1))); // distance

    // 0 - upper matrix (gaps)
    // 1 - usual matrix
    // 2 - lower matrix (gaps)

    // base:
    if(local){
        for (size_t i = 0; i <= s1.size(); ++i) {
            d[2][i][0] = 0;
            p[2][i][0] = zero_;
            d[1][i][0] = 0;
            p[1][i][0] = zero_;
        }
        for (size_t j = 0; j <= s2.size(); ++j) {
            d[0][0][j] = 0;
            p[0][0][j] = zero_;
            d[1][0][j] = 0;
            p[1][0][j] = zero_;
        }
    }else{
        for (size_t i = 0; i <= s1.size(); ++i) {
            d[2][i][0] = -gapop  - i * gapex;
            p[2][i][0] = this_;

            d[1][i][0] = -gapop  - i * gapex;
            p[1][i][0] = down_;

            d[0][i][0] = -INF;
            p[0][i][0] = zero_;
        }
        for (size_t j = 0; j <= s2.size(); ++j) {
            d[0][0][j] = -gapop  - j * gapex;
            p[0][0][j] = this_;

            d[1][0][j] = -gapop  - j * gapex;
            p[1][0][j] = up_;

            d[2][0][j] = -INF;
            p[2][0][j] = zero_;
        }
        d[0][0][0] = d[1][0][0] = d[2][0][0] = 0;
        p[0][0][0] = p[1][0][0] = p[2][0][0] = zero_;
    }

    // dynamic:
    for (size_t i = 1; i <= s1.size(); ++i) {
        for (size_t j = 1; j <= s2.size(); ++j) {
            int up, down, ths;
            int zero = 0;
            // upper
            ths  = d[0][i][j-1] - gapex;
            down = d[1][i][j-1] - gapop - gapex;
            if (zero >= ths && zero >= down && local) {
                d[0][i][j] = zero;
                p[0][i][j] = zero_;
            }
            else if (ths > down) {
                d[0][i][j] = ths;
                p[0][i][j] = this_;
            }
            else {
                d[0][i][j] = down;
                p[0][i][j] = down_;
            }
            // lower
            ths = d[2][i-1][j] - gapex;
            up  = d[1][i-1][j] - gapop - gapex;
            if (zero >= ths && zero >= up && local) {
                d[2][i][j] = zero;
                p[2][i][j] = zero_;
            }
            else if (ths > up) {
                d[2][i][j] = ths;
                p[2][i][j] = this_;
            }
            else {
                d[2][i][j] = up;
                p[2][i][j] = up_;
            }
            // middle
            ths  = d[1][i-1][j-1] + scoring_matrix.find(s1[i-1])->second.find(s2[j-1])->second;
            up   = d[0][i][j];
            down = d[2][i][j];
            if (zero >= ths && zero >= up && zero >= down && local) {
                d[1][i][j] = zero;
                p[1][i][j] = zero_;
            }
            else if (ths > up && ths > down ) {
                d[1][i][j] = ths;
                p[1][i][j] = this_;
            }
            else if (up > down) {
                d[1][i][j] = up;
                p[1][i][j] = up_;
            }
            else {
                d[1][i][j] = down;
                p[1][i][j] = down_;
            }
        }
	}
	// output distance
    int maxscore;
    int maxk;
    int maxi;
    int maxj;
    if(local) {
        maxscore = maxi = maxj = maxk = -1;
        for (int k = 0; k < 3; ++k) {
            for (size_t i = 0; i <= s1.size(); ++i) {
                for (size_t j = 0; j <= s2.size(); ++j) {
                    if (d[k][i][j] > maxscore) {
                        maxscore = d[k][i][j];
                        maxk = k;
                        maxi = i;
                        maxj = j;
                    }
                }
            }
        }
    }
    else{
        maxi = s1.size();
        maxj = s2.size();
        maxk = 1;
        maxscore = d[maxk][maxi][maxj];
        for (int k = 0; k < 3; ++k) {
            if (d[k][maxi][maxj] > maxscore) {
                maxscore = d[k][maxi][maxj];
                maxk = k;
            }
        }
    }
    vector<string> res1, res2;
    int k = maxk;
    int i = maxi;
    int j = maxj;
    while (i >= 0 && j >= 0 && k >= 0 && k <= 2) {
        if (p[k][i][j] == this_ && k == 1) { // diagonal
            i -= 1;
            j -= 1;
            res1.push_back(s1[i]);
            res2.push_back(s2[j]);
        }
        else if (p[k][i][j] == this_ && k == 0) { // horisontal gap
            j -= 1;
            res1.push_back("$");
            res2.push_back(s2[j]);
        }
        else if (p[k][i][j] == this_ && k == 2) { // vertical gap
            i -= 1;
            res1.push_back(s1[i]);
            res2.push_back("$");
        }
        else if (p[k][i][j] == down_ && k == 0) { // gap opening/closing, 1->0
            k = 1;
            j -= 1;
            res1.push_back("$");
            res2.push_back(s2[j]);
        }
        else if (p[k][i][j] == down_ && k == 1) { // gap opening/closing, 2->1
            k = 2;
        }
        else if (p[k][i][j] == up_ && k == 2) { // gap opening/closing, 1->2
            k = 1;
            i -= 1;
            res1.push_back(s1[i]);
            res2.push_back("$");
        }
        else if (p[k][i][j] == up_ && k == 1) { // gap opening/closing, 0->1
            k = 0;
        }
        else {
            break;
        }
	}

	reverse(res1.begin(), res1.end());
	reverse(res2.begin(), res2.end());
	alignment_result ans;
	ans.score = maxscore;
	ans.s1 = res1;
	ans.s2 = res2;
    return ans;
}

template <class T>
void print_matrix(vector<vector<vector<T> > > v){
    cout <<"\n";
    for(size_t i = 0; i < v.size(); i++){
        for(size_t k = 0; k < 3; k++){
            for(size_t j = 0; j < v[0].size(); j++){
                cout << int(v[k][i][j]) << " ";
            }
            cout << "   ";
        }
        cout <<"\n";
    }
}

int max3(int a, int b, int c){
    return std::max(std::max(a, b), c);
}

int fitting_score(string text, string pattern, int gap, map<char, map<char, int> > scoring_matrix){
    int diag, diagn, se;
    vector<int> t;
    size_t n = pattern.size();
    size_t m = text.size();
    for (size_t i = 0; i <= n; ++i){
        t.push_back(-i * gap);
    }
    se = t[n];
    for (size_t j = 0; j < m; ++j)  {
        diag = t[0];
        for (size_t i = 0; i < n; ++i) {
            diagn = t[i+1];
            t[i+1] = max3(t[i] - gap, t[i+1] - gap, diag + scoring_matrix[pattern[i]][text[j]]);
            diag = diagn;
        }
        se = std::max(se, t[n]);
    }
    return se;
}

int overlap_score(string s1, string s2, int gap, map<char, map<char, int> > scoring_matrix){
    int diag, diagn, se;
    vector<int> t;
    size_t n = s1.size();
    size_t m = s2.size();
    for (size_t i = 0; i <= n; ++i){
        t.push_back(0);
    }
    se = t[n];
    for (size_t j = 0; j < m; ++j)  {
        diag = t[0];
        t[0] = -(j+1) * gap;
        for (size_t i = 0; i < n; ++i) {
            diagn = t[i+1];
            t[i+1] = max3(t[i] - gap, t[i+1] - gap, diag + scoring_matrix[s1[i]][s2[j]]);
            diag = diagn;
        }
        se = std::max(se, t[n]);
    }
    return se;
}

int semiglobal_score(string s1, string s2, int gap, map<char, map<char, int> > scoring_matrix){
    int diag, diagn, se;
    vector<int> t;
    size_t n = s1.size();
    size_t m = s2.size();
    for (size_t i = 0; i <= n; ++i){
        t.push_back(0);
    }
    se = t[n];
    for (size_t j = 0; j < m; ++j)  {
        diag = t[0];
        t[0] = 0;
        for (size_t i = 0; i < n; ++i) {
            diagn = t[i+1];
            t[i+1] = max3(t[i] - gap, t[i+1] - gap, diag + scoring_matrix[s1[i]][s2[j]]);
            diag = diagn;
        }
        se = std::max(se, t[n]);
    }
    for(size_t i = 0; i <= n; i++){
        se = std::max(se, t[i]);
    }
    return se;
}

void pt(vector<int> v){
    for(size_t i = 0; i < v.size(); ++i){
        std::cout << v[i] << ' ';
    }
    std::cout << '\n';
}

int no_deletion_score(string s, string t, int gap, map<char, map<char, int> > scoring_matrix){
    vector<int> r;
    size_t n = s.size();
    size_t m = t.size();
    size_t k = m - n;

    for(size_t j = 0; j <= k; ++j){
        r.push_back(-j * gap);
    }
    for(size_t i = 0; i < n; ++i){
        r[0] = r[0] + scoring_matrix[s[i]][t[i]];
        for(size_t j = 0; j < k; ++j){
            r[j+1] = std::max(r[j] - gap, r[j+1] + scoring_matrix[s[i]][t[i+j+1]]);
        }
    }
    return r[k];
}

vector<pair<int, int> > k_edit_matches(int k, string pattern, string text) {

    vector<pair<int, int> > ans;

    int n = pattern.size() - 1;
    int m = text.size() - 1;

	int j = 0;
	vector<vector<int> > L(3 * k + 3);
	for(unsigned int i = 0; i < L.size(); ++i){
	    L[i].resize(2 * k + 3);
	}

    for (int i = 0; i <= m - n + k; ++i) {
		//Initialization
		for (int d = -(k+1); d<=k+1; ++d) {
			L[d + (k+1)][std::abs(d) - 2 + 2] = -INF;
			if (d<0){
				L[d + (k+1)][std::abs(d) - 1 + 2] = std::abs(d) - 1;
		    } else {
		        L[d + (k+1)][std::abs(d) - 1 + 2] = -1;
		    }
		}


		//Main
		for (int e = 0; e<=k; ++e) {
			for (int d = -e; d<=e; ++d) {

				int cd = L[d + (k+1)][e + 2 - 1];
				int ld = L[d + (k+1) - 1][e + 2  - 1];
				int ud = L[d + (k+1) + 1][e + 2 - 1];
				int row = max3(cd + 1, ld, ud + 1);


				while (row < n && pattern[row + 1] == text[i + row + 1 + d]){
					++row;
				}

				L[d + (k+1)][e + 2] = std::min(row, (int)n);
				if (L[d + (k+1)][e + 2]  == n) {
					j = L[d + (k+1)][e + 2]  + d + i;
					if (j<=m) {
					    pair<int, int> p;
					    p.first = i;
					    p.second = max(i+1, j);
					    ans.push_back(p);
					}
				}
			}
		}

	}

	return ans;
}
