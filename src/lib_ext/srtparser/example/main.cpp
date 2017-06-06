#include "../srtparser.h"
using namespace std;

int main(int argc, char *argv[]) {

    // If filename not provided
    if(argc < 2) {
        cout << "Please pass the filename as an argument: ./a.out filename.srt\n";
        return 0;
    }

    SubtitleParserFactory *subParserFactory = new SubtitleParserFactory(argv[1]);
    SubtitleParser *parser = subParserFactory->getParser();

    std::vector<SubtitleItem*> sub = parser->getSubtitles();

    ofstream myfile;
    myfile.open ("out.srt");

    for(SubtitleItem * element : sub)
    {
        myfile<<"BEGIN"<<endl;
        myfile<<"startString : "<<element->getStartTimeString()<<endl;
        myfile<<"start : "<<element->getStartTime()<<endl;
        myfile<<"endString : "<<element->getEndTimeString()<<endl;
        myfile<<"end : "<<element->getEndTime()<<endl;
        myfile<<"text : "<<element->getText()<<endl;
        myfile<<"justDialogue : "<<element->getDialogue()<<endl;
        myfile<<"words count : "<<element->getWordCount()<<endl;
        myfile<<"words :";
        std::vector<std::string> word = element->getIndividualWords();
            for(std::string display : word)
                myfile<<display<<", ";
            myfile<<endl;

        myfile<<"speakerCount : "<<element->getSpeakerCount()<<endl;
        myfile<<"speakers : ";
        if(element->getSpeakerCount())
        {
            std::vector<std::string> name = element->getSpeakerNames();
            for(std::string display : name)
                myfile<<display<<", ";
            myfile<<endl;
        }

        myfile<<"ignore : "<<element->getIgnoreStatus()<<endl;
        myfile<<"END"<<endl<<endl;
    }

    return 0;
}
