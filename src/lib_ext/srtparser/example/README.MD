# Example for srtparser.h : Simple, yet powerful C++ SRT Subtitle Parser Library.

## How to run?

CMakeLists.txt is provided. Simply run using cmake.

The compiled tool needs the subtitle file as input.

`./a.out input.srt`

## Output

A simple example resides in this directory (`main.cpp`). Demo subtitle file (`example.srt`) is also provided.
It's output is located in (`example-output.srt`).

Code Snippet:

```
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
```

Input : 

```

1
00:00:00,520 --> 00:00:03,536
Chris: Elon, hey, welcome back to TED.

2
00:00:03,560 --> 00:00:04,936
Chris : It's great to have you here.
Evan: Indeed it is.

3
00:00:04,960 --> 00:00:06,536
Elon: Thanks for having me.

4
00:00:06,560 --> 00:00:09,416
(Applause) Thanks

5
00:00:09,440 --> 00:00:11,256
<i>we're going to spend some time</i>
```

Parsed Output :

```

BEGIN
startString : 00:00:00,520
start : 520
endString : 00:00:03,536
end : 3536
text : Chris: Elon, hey, welcome back to TED.
justDialogue : Elon, hey, welcome back to TED.
words count : 6
words :Elon,, hey,, welcome, back, to, TED., 
speakerCount : 1
speakers : Chris, 
ignore : 0
END

BEGIN
startString : 00:00:03,560
start : 3560
endString : 00:00:04,936
end : 4936
text : Chris : It's great to have you here. Evan: Indeed it is.
justDialogue : It's great to have you here. Indeed it is.
words count : 9
words :It's, great, to, have, you, here., Indeed, it, is., 
speakerCount : 2
speakers : Chris ,  Evan, 
ignore : 0
END

BEGIN
startString : 00:00:04,960
start : 4960
endString : 00:00:06,536
end : 6536
text : Elon: Thanks for having me.
justDialogue : Thanks for having me.
words count : 4
words :Thanks, for, having, me., 
speakerCount : 1
speakers : Elon, 
ignore : 0
END

BEGIN
startString : 00:00:06,560
start : 6560
endString : 00:00:09,416
end : 9416
text : (Applause) Thanks
justDialogue : Thanks
words count : 1
words :Thanks, 
speakerCount : 0
speakers : ignore : 0
END

BEGIN
startString : 00:00:09,440
start : 9440
endString : 00:00:11,256
end : 11256
text : <i>we're going to spend some time</i>
justDialogue : we're going to spend some time
words count : 6
words :we're, going, to, spend, some, time, 
speakerCount : 0
speakers : ignore : 0
END




```
