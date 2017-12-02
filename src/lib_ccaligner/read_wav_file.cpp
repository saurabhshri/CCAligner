/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "read_wav_file.h"

int findIndex(std::vector<unsigned char>& fileData, std::string chunk)
{
    auto it = std::search(fileData.begin(), fileData.end(), chunk.begin(), chunk.end());
    return (int)(it-fileData.begin());  //returns beginning of the string passed through "chunk" ('fmt' / 'data')
}

WaveFileData::WaveFileData(std::string fileName, bool isRawFile)    //file is stored on disk
{
    _fileName = fileName;
    _samples.resize(0);
    _openMode = readFile;
    _isRawFile = isRawFile;
}

WaveFileData::WaveFileData(openMode mode, bool isRawFile)           //data being read from stream;
{
    _samples.resize(0);
    _openMode = mode;
    _isRawFile = isRawFile;
}

bool WaveFileData::checkValidWave (std::vector<unsigned char>& fileData)
{
    /*Offset  Size  Name             Description
     * 0         4   ChunkID          Contains the letters "RIFF" in ASCII form
     */

    LOG("Checking chunkID, should be RIFF");
    std::string chunkID (fileData.begin(), fileData.begin() + 4);
    return chunkID == "RIFF";

}

bool WaveFileData::decode()     //decodes the wave file
{
    /* Wave file format :

        Offset  Size  Name             Description

        The canonical WAVE format starts with the RIFF header:

        0         4   ChunkID          Contains the letters "RIFF" in ASCII form
                                       (0x52494646 big-endian form).
        4         4   ChunkSize        36 + SubChunk2Size, or more precisely:
                                       4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
                                       This is the size of the rest of the chunk
                                       following this number.  This is the size of the
                                       entire file in bytes minus 8 bytes for the
                                       two fields not included in this count:
                                       ChunkID and ChunkSize.
        8         4   Format           Contains the letters "WAVE"
                                       (0x57415645 big-endian form).

        The "WAVE" format consists of two subchunks: "fmt " and "data":
        The "fmt " subchunk describes the sound data's format:

        12        4   Subchunk1ID      Contains the letters "fmt "
                                       (0x666d7420 big-endian form).
        16        4   Subchunk1Size    16 for PCM.  This is the size of the
                                       rest of the Subchunk which follows this number.
        20        2   AudioFormat      PCM = 1 (i.e. Linear quantization)
                                       Values other than 1 indicate some
                                       form of compression.
        22        2   NumChannels      Mono = 1, Stereo = 2, etc.
        24        4   SampleRate       8000, 44100, etc.
        28        4   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
        32        2   BlockAlign       == NumChannels * BitsPerSample/8
                                       The number of bytes for one sample including
                                       all channels. I wonder what happens when
                                       this number isn't an integer?
        34        2   BitsPerSample    8 bits = 8, 16 bits = 16, etc.
                  2   ExtraParamSize   if PCM, then doesn't exist
                  X   ExtraParams      space for extra parameters

        The "data" subchunk contains the size of the data and the actual sound:

        36        4   Subchunk2ID      Contains the letters "data"
                                       (0x64617461 big-endian form).
        40        4   Subchunk2Size    == NumSamples * NumChannels * BitsPerSample/8
                                       This is the number of bytes in the data.
                                       You can also think of this as the size
                                       of the read of the subchunk following this
                                       number.
        44        *   Data             The actual sound data.

        SOURCE : http://soundfile.sapp.org/doc/WaveFormat/

        NOTE : I later found out that the subChunks may be located at different offsets
               in presence of metadata like LIST INFO etc..

               See : L#108

     */

    std::string format(_fileData.begin() + 8, _fileData.begin() + 12);

    if(format != "WAVE")
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file format : %s ", format.c_str());
    }

    LOG("File format is identified as WAV");

    /*
     * Apparently, this is just not it. The `fmt ` and `data`  chunk may not necessarily be in continuation.
     * There may occur inclusion of metadata. So, we'll need to find the location of these chunks.
     */

    LOG("Finding FMT and DATA subchunks");

    int fmtIndex = findIndex(_fileData, "fmt ");
    int dataIndex = findIndex(_fileData, "data");

    if(fmtIndex < 0)
    {
        FATAL(EXIT_INVALID_FILE, "FMT subchunk not found!");
    }

    if(dataIndex < 0)
    {
        FATAL(EXIT_INVALID_FILE, "Data subchunk not found!");
    }

    LOG("FMT index : %d , DATA index :%d", fmtIndex, dataIndex);

    std::string subChunk1ID(_fileData.begin() + fmtIndex, _fileData.begin() + fmtIndex + 4);

    if(subChunk1ID != "fmt ")
    {
        FATAL(EXIT_INVALID_FILE, "Invalid SubChunk1ID : %s", subChunk1ID.c_str());
    }

    unsigned long subChunk1Size = fourBytesToInt(_fileData, fmtIndex + 4);

    if(subChunk1Size != 16)
    {
        FATAL(EXIT_INVALID_FILE, "Not PCM, SubChunk1Size : %lu", subChunk1Size);
    }

    int audioFormat = twoBytesToInt(_fileData, fmtIndex + 8);

    if(audioFormat != 1)
    {
        FATAL(EXIT_INVALID_FILE, "Not PCM, AudioFormat : %d", audioFormat);
    }

    LOG("PCM : True");

    int numChannels = twoBytesToInt(_fileData, fmtIndex + 10);

    if(numChannels != 1)
    {
        FATAL(EXIT_INVALID_FILE, "Not Mono, NumChannels : %d", numChannels);
    }

    LOG("MONO : True");

    unsigned long sampleRate = fourBytesToInt(_fileData, fmtIndex + 12);

    if(sampleRate != 16000)
    {
        FATAL(EXIT_INVALID_FILE, "Not 16000Hz SampleRate, SampleRate : %lu", sampleRate);
    }

    LOG("Sample Rate 16KHz : True");

    unsigned long byteRate = fourBytesToInt(_fileData, fmtIndex + 16);

    int blockAlign = twoBytesToInt(_fileData, fmtIndex + 20);

    int bitRate = twoBytesToInt(_fileData, fmtIndex + 22); //BitsPerSample

    if(bitRate != 16)
    {
        FATAL(EXIT_INVALID_FILE, "Not 16 bits/sec, BitRate : %d", bitRate);
    }

    LOG("BitRate 16 bits/sec : True");

    if((byteRate != sampleRate * numChannels * bitRate/8) || (blockAlign != numChannels * bitRate/8))
    {
        FATAL(EXIT_INVALID_FILE, "Incorrect header, ByteRate and/or BlockAlign values do not match!");
    }

    std::string subChunk2ID (_fileData.begin() + dataIndex, _fileData.begin() + dataIndex + 4);

    if(subChunk2ID != "data")
    {
        FATAL(EXIT_INVALID_FILE, "Invalid SubChunk2ID : %s", subChunk2ID.c_str());
    }

    unsigned long subChunk2Size = fourBytesToInt(_fileData, dataIndex + 4);

    unsigned long int numSamples = subChunk2Size * 8 / ( numChannels * bitRate);

    LOG("Number of samples : %lu", numSamples);
    LOG("Reading samples");

    for (unsigned long int i = 0; i < numSamples; i++)
    {
            unsigned long int sampleIndex = dataIndex + 8 + (blockAlign * i);
            // dataIndex = index of beginning of "data" subChunk
            // dataIndex + 8 is usually 44 as per the specs

            int16_t sample = twoBytesToInt(_fileData, sampleIndex);
            _samples.push_back(sample);
    }

    return true;    //successfully decoded
}

bool WaveFileData::openFile ()
{
    std::ifstream infile (_fileName, std::ios::binary);

    LOG("Trying to read from file : %s", _fileName.c_str());

    if (!infile)
    {
        FATAL(EXIT_FILE_NOT_FOUND, "Unable to open file : %s", _fileName.c_str());
    }

    LOG("Reading file data");

    if (_isRawFile) { // handle raw audio files
        infile.seekg(0, std::ios::end);
        std::streamsize size = infile.tellg();
        infile.seekg(0, std::ios::beg);

        _samples = std::vector<int16_t>(size/2); // size is in unit of byte, while one int_16 uses 2 bytes
        if (infile.read(reinterpret_cast<char*>(_samples.data()), size)) {
            LOG("File data read");
            LOG("Decoding is skipped since it is raw audio file");
            return true;
        }
        else {
            FATAL(EXIT_INVALID_FILE, "Unable to read from file : %s", _fileName.c_str());
            return false;
        }
    }


    /*
    * When reading characters, std::istream_iterator skips whitespace by default
    * (unless disabled with std::noskipws or equivalent)..
    * http://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/iterator/istream_iterator.html#Notes
    */

    std::noskipws(infile);
    std::istream_iterator<unsigned char> begin(infile), end;
    std::vector<unsigned char> fileData (begin, end);   //read complete file content

    LOG("File data read and stored in buffer");

    LOG("Processing data and extracting samples");

    if(checkValidWave(fileData))
    {
        LOG("Wave File chunkID verification successful");

        _fileData = fileData;   //wave is valid, store and proceed

        LOG("Begin decoding wave file");

        decode();

        LOG("File decoded successfully");

        return true;
    }

    else
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file!");
    }

    return true;
}

int WaveFileData::processStreamHeader()
{
    LOG("Processing Stream Header");
    LOG("Checking chunkID, should be RIFF");
    unsigned char byteData;
    std::string riff ("RIFF"), wave ("WAVE");
    bool riffRead = false;                      //are 'RIFF' bytes read?
    int currentByteCount = -1, chunkSize = 0;

    while (std::cin >> std::noskipws >> byteData)
    {
        _fileData.push_back(byteData);
        currentByteCount++;

        if (!riffRead)
        {
            if (riff[currentByteCount] != byteData)     //checking RIFF header
            {
                FATAL(EXIT_INVALID_FILE, "Invalid WAV file : Incorrect subChunk1ID!");
            }

            if (currentByteCount == 3)
            {
                riffRead = true;
                LOG("chunkID = RIFF confirmed!");
                LOG("Checking WAV file header, should be WAVE");

            }
        }

        else
        {
            if (currentByteCount >= 8)
            {
                if (wave[currentByteCount - 8] != byteData) //checking WAVE format
                {
                    FATAL(EXIT_INVALID_FILE, "Invalid WAV file : Incorrect Header!");
                }

                if (currentByteCount == 11)
                {
                    return chunkSize - 8;
                    LOG("wav header = WAVE confirmed!");
                }
            }

            else
            {
                chunkSize = chunkSize | byteData << (8 * (currentByteCount - 4));   //calculating chunksize (filesize - 8)
            }
        }
    }
    FATAL(EXIT_UNKNOWN, "Error occured while processing stream header!");
    return -1;  //some error; more robust exit errors coming soon
}

int WaveFileData::seekToEndOfSubChunk1ID(int remainingBytes)
{
    LOG("Checking SubChunk1ID, should be fmt");

    unsigned char byteData;
    std::string fmt ("fmt");
    int fmtCount = 0, readBytes = 0;

    while(std::cin>>std::noskipws>>byteData)
    {
        _fileData.push_back(byteData);

        readBytes++;

        if(byteData == fmt[fmtCount])   //probably 'fmt'
        {
            fmtCount++;

            if(fmtCount == 3)           //definitely 'fmt'
            {
                LOG("SubChunk1ID = fmt confirmed!");

                std::cin>>std::noskipws>>byteData;
                _fileData.push_back(byteData);
                return readBytes + 1;   // +1 because the block is 4 bytes 'fmt' + ' ' an empty byte.
            }

        }

        else
        {
            fmtCount = 0;
        }

        if(readBytes > remainingBytes)
        {
            FATAL(EXIT_INVALID_FILE, "Invalid WAV file: SubChunk1 ('fmt') not found!");
        }
    }
    FATAL(EXIT_UNKNOWN, "Error occured while checking SubChunk1ID");
    return -1;
}

int WaveFileData::validateSubChunk1(int remainingBytes)
{
    LOG("Validating SubChunk1");

    unsigned char byteData;
    std::vector<unsigned char> fmtBlock;
    int currentByteCount = -1;


    while(std::cin>>std::noskipws>>byteData)
    {
        _fileData.push_back(byteData);
        currentByteCount++;
        fmtBlock.push_back(byteData);       //storing 'fmt' data in a buffer, processing live is dangerous

        if(currentByteCount == 19)
        {
            break;
        }

    }

    unsigned long subChunk1Size = fourBytesToInt(fmtBlock, 0);

    if(subChunk1Size != 16)
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file: Not PCM, SubChunk1Size: %lu", subChunk1Size);
    }

    int audioFormat = twoBytesToInt(fmtBlock, 4);

    if(audioFormat != 1)
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file: Not PCM, AudioFormat: %d", audioFormat);
    }

    int numChannels = twoBytesToInt(fmtBlock, 6);

    if(numChannels != 1)
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file: Not Mono, NumChannels: %d", numChannels);
    }

    unsigned long sampleRate = fourBytesToInt(fmtBlock, 8);

    if(sampleRate != 16000)
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file: Not 16000Hz SampleRate, SampleRate: %lu", sampleRate);
    }

    unsigned long byteRate = fourBytesToInt(fmtBlock, 12);

    int blockAlign = twoBytesToInt(fmtBlock, 16);

    int bitRate = twoBytesToInt(fmtBlock, 18); //BitsPerSample

    if(bitRate != 16)
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file: Not 16 bits/sec, BitRate: %d", bitRate);
    }

    if((byteRate != sampleRate * numChannels * bitRate/8) || (blockAlign != numChannels * bitRate/8))
    {
        FATAL(EXIT_INVALID_FILE, "Incorrect header, ByteRate and/or BlockAlign values do not match!");
    }

    return currentByteCount + 1;

}

int WaveFileData::seekToEndOfSubChunk2ID(int remainingBytes)
{
    LOG("Reading SubChunk2");

    unsigned char byteData;
    //char *data = "data";
    std::string data ("data");
    int dataCount = 0, readBytes = 0;

    while(std::cin>>std::noskipws>>byteData)
    {
        _fileData.push_back(byteData);

        readBytes++;

        if(byteData == data[dataCount]) //probably 'data' subchunk
        {
            dataCount++;

            if(dataCount == 4)  //definitely 'data' subchunk
                return readBytes; //return end of 'data' subchunk ID
        }

        else
        {
            dataCount = 0;
        }

        if(readBytes > remainingBytes)
        {
            FATAL(EXIT_INVALID_FILE, "SubChunk2 ('data') not found!");
        }
    }
    FATAL(EXIT_UNKNOWN, "Error occured while Reading SubChunk2!");
    return -1; //some error
}

int WaveFileData::getNumberOfSamples()
{
    unsigned char byteData;
    int subChunk2Size = 0;
    for(int i = 0; i < 4; i++)
    {
        std::cin>>std::noskipws>>byteData;
        _fileData.push_back(byteData);

        subChunk2Size = subChunk2Size | byteData << (8 * i);
    }

    return subChunk2Size / 2; // in our case, wave file are 16 bit. So, 2 bytes = 1 sample. No. of Sample = size/2
}

bool WaveFileData::readSamplesFromStream(int numberOfSamples)
{
    LOG("Reading and decoding samples from stream...");

    unsigned char byteData;
    std::vector<unsigned char> twoBytes;
    int two = 0, bytesRead = 0;

    while(std::cin>>std::noskipws>>byteData)
    {
        _fileData.push_back(byteData);

        two++; bytesRead++;
        twoBytes.push_back(byteData);   //processing two bytes at a time

        if(two == 2)
        {
            int16_t sample = twoBytesToInt(twoBytes, 0);    //16 bit PCM, 2 bytes = 1 sample
            _samples.push_back(sample); //storing sample
            twoBytes.clear();
            two = 0;
        }

        if(bytesRead > numberOfSamples * 2)
        {
            std::cout<<"\nLooks like number of bytes exceeds the expected amount! Still processing.";
        }
    }

    if(bytesRead < numberOfSamples * 2)
    {
        std::cout<<"\nReceived less number of samples than the expected amount! Still processing.";
    }

    LOG("Samples read and decoded!");


    return -1;

}

bool WaveFileData::readStream()
{
    LOG("Reading WAV file from stream");
    if (_isRawFile) {
        LOG("Raw audio mode is enabled. Turn to read stream using buffer...");
        return readStreamUsingBuffer();
    }

    int remainingBytes = processStreamHeader();                 //processing wave header
    remainingBytes -= seekToEndOfSubChunk1ID(remainingBytes);   //searching 'fmt' subchunk
    remainingBytes -= validateSubChunk1(remainingBytes);        //processing 'fmt' subchunk
    remainingBytes -= seekToEndOfSubChunk2ID(remainingBytes);   //searching 'data' subchunk

    int numberOfSamples = getNumberOfSamples();                 //getting number of samples

    if(remainingBytes != 2 * numberOfSamples)
    {
        std::cout<<"\nLooks like there is some error in reading samples from the file. Still proceeding.";
    }

    return readSamplesFromStream(numberOfSamples);              //reading samples

}

bool WaveFileData::readStreamUsingBuffer()
{
    if (_isRawFile) {
        unsigned char byteData, byteData2;
        while (std::cin >> std::noskipws >> byteData) {
            std::cin >> std::noskipws >> byteData2;
            _samples.push_back(((byteData2 << 8) | byteData));  //storing the stream into sample directly
        }
        return true;
    }

    unsigned char byteData;

    while(std::cin >> std::noskipws >> byteData)
    {
        _fileData.push_back(byteData);  //storing the stream into buffer
    }

    if(checkValidWave(_fileData))   //checking if buffer has valid WAVE file data
    {
        decode();   //decode the buffer
        return true;
    }

    else
    {
        FATAL(EXIT_INVALID_FILE, "Invalid WAV file: SubChunk2 ('data') not found!");
    }

    return true;

}

bool WaveFileData::read()   //decided the function based on set mode
{
    LOG("Begin reading WAV file");

    switch (_openMode)
    {
        case readFile             : openFile();             //file on disk
                                    break;
        case readStreamDirectly   : readStream();           //from stream/pipe
                                    break;
        case readStreamIntoBuffer : readStreamUsingBuffer();//from stream/pipe into buffer and then processing
                                    break;
        default                   : FATAL(EXIT_INVALID_FILE, "Error choosing opening mode, please report!");

    }
    return true;
}

/* Convert 4 bytes to int
 * Reference :
 * https://stackoverflow.com/a/2386134/6487831
 */

unsigned long WaveFileData::fourBytesToInt (std::vector<unsigned char>& fileData, int index)
{
    return ((fileData[index + 3] << 24) | (fileData[index + 2] << 16) | (fileData[index + 1] << 8) | fileData[index]);
}

int WaveFileData::twoBytesToInt (std::vector<unsigned char>& fileData, int index)
{
    return ((fileData[index + 1] << 8) | fileData[index]);
}

std::vector<int16_t> WaveFileData::getSamples()
{
    return _samples;    //returning sample vector
}

WaveFileData::~WaveFileData()
{

}
