/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "lib_ccaligner/ccaligner.h"
#include <typeinfo>
using namespace std;

void printUsage()
{
   cout<<"\n\nUsage << \n"
        "                 ccaligner -wav /path/to/wav/file -srt /path/to/srt/file\n"
        "                 ccaligner -wav /path/to/wav/file -srt /path/to/srt/file -out /path/to/output/file -oFormat <output_format>\n"
        "                                                                                                     (srt/xml/json/karaoke)\n"
        "                 e.g. ccaligner -wav tbbt.wav -srt tbbt.srt -out tbbt-karaoke.srt -oFormat karaoke\n"
        "\nFor a complete list of available parameters and documentation, refer to the README.\n";
}

void printHeader(const string& version)
{
    cout<<"\n";
    cout<<"  ____ ____    _    _ _                       \n";
    cout<<" / ___/ ___|  / \\  | (_) __ _ _ __   ___ _ __ \n";
    cout<<"| |  | |     / _ \\ | | |/ _` | '_ \\ / _ \\ '__|\n";
    cout<<"| |__| |___ / ___ \\| | | (_| | | | |  __/ |   \n";
    cout<<" \\____\\____/_/   \\_\\_|_|\\__, |_| |_|\\___|_|   \n";
    cout<<"                        |___/                 \n\n";

    cout<<"CCAligner "<<version<<"\n";
    cout<<"Word by Word Audio-Subtitle Synchronization\n";
    cout<<"Saurabh Shrivastava | saurabh.shrivastava54@gmail.com\n";
    cout<<"https://github.com/saurabhshri/CCAligner\n";
    cout<<"=====================================================\n\n";
}

void printFooter()
{
    cout<<"\n\n=====================================================\n";
    cout<<"Issues? Open a ticket here\n";
    cout<<"https://github.com/saurabhshri/CCAligner/issues\n";
}

int main(int argc, char *argv[])
{
    printHeader("0.03 Alpha [Shubham]");

    try {
        Params parameters;
        parameters.inputParams(argc, argv);

        CCAligner(&parameters).initAligner();
    }
    catch (exception& e) {
        cerr << "Program aborted because an exception has occurred." << endl;
        cerr << "Exception details:" << std::endl
            << "Type: " << typeid(e).name() << ". " << endl;
            << "Reason: " << e.what() << endl;
    }
    printFooter();

    return 0;
}
