%module stepik_alignment

%{
#define SWIG_FILE_WITH_INIT
#include "stepik_alignment.h"
%}

%include "typemaps.i"
%include "std_map.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"

namespace std{
%template(mapChar)      map<char, int>;
%template(mapCharChar)  map<char, map<char, int> >;
%template(intPair)      pair<int, int>;
%template(vecIntPair)   vector<pair<int, int> >;
};

%include "stepik_alignment.h"