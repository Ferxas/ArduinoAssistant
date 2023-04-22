#include "tools/tools.h"

using namespace TOOLS;
using namespace std;

FP::FP(bool value,int i)
{
    Value= value && !TOOLS::FPs[i];
    TOOLS::FPs[i]= value;
}
FP::~FP(){}








vector<u_int8_t> convert::ToUInt8(string s)
    {
vector<u_int8_t> result;
const int length = s.length();
char* char_array = new char[length + 1]; //+1 /0
for (char number : s)
{
   result.push_back(number);
}
    

  return result;
    }

