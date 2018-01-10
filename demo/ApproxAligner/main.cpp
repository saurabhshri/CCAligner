#include "generate_approx_timestamp.h"
#include <iostream>
using namespace std;

int main()
{
cout<<"Enter path to the subtitle file : ";

string fileName;
    
	cin>>fileName;

    ApproxAligner * aligner = new ApproxAligner(fileName);	// that's it :) More customization to come!
    aligner->align();

    return 0;
}
