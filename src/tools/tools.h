
#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED
#include<bits/stdc++.h>
#include <vector>
using namespace std;
namespace TOOLS
{

    static bool FPs[10]; 
    class FP
    {
    private:
           /* data */
    public:
        bool Value;
        FP(bool value,int i);
        ~FP();
    };

    class convert
    {
        private:

        public:
       static vector<u_int8_t> ToUInt8(string s);
    };
    



}


#endif
