Creating & Adding New Unit Tests for CCAligner
----------------------------------------------

[WAV Test]
----------
> Prerequisites :
1. 16 bit PCM mono wav file sampled at 16 kHz.
2. .srt file for the .wav file.
3. CCAligner installed along with all dependencies.

> Steps :

1. Generate a .json file for your .wav file using CCAligner: ./ccalinger -wav <wav_file>.wav -srt <srt_file>.srt -oFormat json
NOTE: Make sure that the names of all the files are same.
2. Add .expected at the end of the name of generated .json file (for example, if the .json file was "a.json", the file should be renamed to "a.json.expected").
3. Copy the unit test files (.wav, .srt & .json.expected) and paste them in CCAligner/test/data.
4. Add the name of your unit test to test/test_info.json.

//more tests here





