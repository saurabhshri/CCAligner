/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "params.h"
#include "recognize_using_pocketsphinx.h"

static void printUsage()
{
    std::cout<<"\n\nUsage : \n"
        "                 ccaligner -wav /path/to/wav/file -srt /path/to/srt/file\n"
        "                 ccaligner -wav /path/to/wav/file -srt /path/to/srt/file -out /path/to/output/file -oFormat <output_format>\n"
        "                                                                                                     (srt/xml/json/karaoke)\n"
        "                 e.g. ccaligner -wav tbbt.wav -srt tbbt.srt -out tbbt-karaoke.srt -oFormat karaoke\n"
        "\nFor a complete list of available parameters and documentation, refer to the README.\n";
}

static void printHeader(std::string version)
{
    std::cout<<"\n";
    std::cout<<"  ____ ____    _    _ _                       \n";
    std::cout<<" / ___/ ___|  / \\  | (_) __ _ _ __   ___ _ __ \n";
    std::cout<<"| |  | |     / _ \\ | | |/ _` | '_ \\ / _ \\ '__|\n";
    std::cout<<"| |__| |___ / ___ \\| | | (_| | | | |  __/ |   \n";
    std::cout<<" \\____\\____/_/   \\_\\_|_|\\__, |_| |_|\\___|_|   \n";
    std::cout<<"                        |___/                 \n\n";

    std::cout<<"CCAligner "<<version<<"\n";
    std::cout<<"Word by Word Audio-Subtitle Synchronization\n";
    std::cout<<"Saurabh Shrivastava | saurabh.shrivastava54@gmail.com\n";
    std::cout<<"https://github.com/saurabhshri/CCAligner\n";
    std::cout<<"=====================================================\n\n";
}

static void printFooter()
{
    std::cout<<"\n\n=====================================================\n";
    std::cout<<"Issues? Open a ticket here\n";
    std::cout<<"https://github.com/saurabhshri/CCAligner/issues\n";
}

int main(int argc, char *argv[])
{
    printHeader("0.03 Alpha [Shubham]");

    Params parameters;
    parameters.inputParams(argc,argv);

    if(parameters.chosenAlignerType == approxAligner)
    {
        ApproxAligner(parameters.subtitleFileName, srt).align();
    }

    else if(parameters.chosenAlignerType == asrAligner)
    {
        PocketsphinxAligner(&parameters).align();
    }

    else
    {
        FATAL(EXIT_INVALID_PARAMETERS, "Unsupported Aligner Type!");
        return -1;
    }

    printFooter();

    return 0;
}
