#include "generate_approx_timestamp.h"

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		std::cout<<"Usage : ccaligner input.srt\n";
		return 1;
	}

    std::string filename(argv[1]);

    ApproxAligner * aligner = new ApproxAligner(filename);
    aligner->align();

    return 0;
}
