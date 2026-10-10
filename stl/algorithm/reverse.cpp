#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void test()
{
    vector<vector<int>> vt;
    for(int i=0;i<9;i++)
    {
        if(i/3==0) vt.push_back(vector<int>());
	    vt[i/3].push_back(i+1);
    }    
    reverse(vt.begin(),vt.end());
    auto pt = [&](){
        for(int i=0;i<9;i++)
	        cout<<vt[i/3][i%3]<<" ";
    };
    pt();
    cout<<endl;
    for(int i=0;i<3;i++)
	    reverse(vt[i].begin(), vt[i].end());
    pt();
    cout<<endl;
}
int main()
{ 
    test();
    return 0;
}
