#include "generate_approx_timestamp.h"

int main()
{
	std::cout<<"Enter path to the subtitle file : ";

    std::string filename;
    std::cin>>filename;

    ApproxAligner * aligner = new ApproxAligner(filename);	// that's it :) More customization to come!
    aligner->align();

    return 0;
}
