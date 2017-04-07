%module stepik_alignment

%{
#define SWIG_FILE_WITH_INIT
#include "stepik_alignment.h"
%}

%include "typemaps.i"
%include "std_map.i"
%include "std_string.i"
%include "std_wstring.i"
%include "std_vector.i"
%include "std_pair.i"

namespace std{
%template(mapString)    map<string, int>;
%template(mapStringString)  map<string, map<string, int> >;
%template(intPair)      pair<int, int>;
%template(vecIntPair)   vector<pair<int, int> >;
%template(StringVector) vector<string>;
%template(ConstCharVector) vector<const char*>;
};

%include "stepik_alignment.h"