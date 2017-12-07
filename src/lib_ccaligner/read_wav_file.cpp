/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "read_wav_file.h"

int findIndex(std::vector<unsigned char>& fileData, const std::string& chunk)
{
    auto it = std::search(fileData.begin(), fileData.end(), chunk.begin(), chunk.end());
    return (int)(it-fileData.begin());  //returns beginning of the string passed through "chunk" ('fmt' / 'data')
}

WaveFileData::WaveFileData(std::string fileName, bool isRawFile) noexcept    //file is stored on disk
    : _fileName(std::move(fileName)),
      _openMode(readFile),
      _isRawFile(isRawFile)
{
    _samples.resize(0);
}

WaveFileData::WaveFileData(openMode mode, bool isRawFile) noexcept           //data being read from stream;
    : _openMode(mode),
      _isRawFile(isRawFile)
{
    _samples.resize(0);
}

bool WaveFileData::checkValidWave (const std::vector<unsigned char>& fileData)
{
    /*Offset  Size  Name             Description
     * 0         4   ChunkID          Contains the letters "RIFF" in ASCII form
     */

    DEBUG << "Checking chunkID, should be RIFF";
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
        DEBUG << "Invalid WAV file format";
        FATAL(InvalidFile) << "Invalid WAV file format : " << format;
    }

    DEBUG << "File format is identified as WAV";

    /*
     * Apparently, this is just not it. The `fmt ` and `data`  chunk may not necessarily be in continuation.
     * There may occur inclusion of metadata. So, we'll need to find the location of these chunks.
     */

    DEBUG << "Finding FMT and DATA subchunks";

    int fmtIndex = findIndex(_fileData, "fmt ");
    int dataIndex = findIndex(_fileData, "data");

    if(fmtIndex < 0)
    {
        DEBUG << "FMT subchunk not found!";
        FATAL(InvalidFile) << "FMT subchunk not found!";
    }

    if(dataIndex < 0)
    {
        DEBUG << "Data subchunk not found!";
        FATAL(InvalidFile) << "Data subchunk not found!";
    }

    DEBUG << "FMT index : "<< fmtIndex <<" , DATA index : " << dataIndex;

    std::string subChunk1ID(_fileData.begin() + fmtIndex, _fileData.begin() + fmtIndex + 4);

    if(subChunk1ID != "fmt ")
    {
        FATAL(InvalidFile) << "Invalid SubChunk1ID : " << subChunk1ID;
    }

    unsigned long subChunk1Size = fourBytesToInt(_fileData, fmtIndex + 4);

    if(subChunk1Size != 16)
    {
        FATAL(InvalidFile) << "Not PCM, SubChunk1Size : " << subChunk1Size;
    }

    int audioFormat = twoBytesToInt(_fileData, fmtIndex + 8);

    if(audioFormat != 1)
    {
        FATAL(InvalidFile) << "Not PCM, AudioFormat : " << audioFormat;
    }

    DEBUG << "PCM : True";

    int numChannels = twoBytesToInt(_fileData, fmtIndex + 10);

    if(numChannels != 1)
    {
        FATAL(InvalidFile) << "Not Mono, NumChannels : " << numChannels;
    }

    DEBUG << "MONO : True";

    unsigned long sampleRate = fourBytesToInt(_fileData, fmtIndex + 12);

    if(sampleRate != 16000)
    {
        FATAL(InvalidFile) << "Not 16000Hz SampleRate, SampleRate : " << sampleRate;
    }

    DEBUG << "Sample Rate 16KHz : True";

    unsigned long byteRate = fourBytesToInt(_fileData, fmtIndex + 16);

    int blockAlign = twoBytesToInt(_fileData, fmtIndex + 20);

    int bitRate = twoBytesToInt(_fileData, fmtIndex + 22); //BitsPerSample

    if(bitRate != 16)
    {
        FATAL(InvalidFile) << "Not 16 bits/sec, BitRate : " << bitRate;
    }

    DEBUG << "BitRate 16 bits/sec : True";

    if((byteRate != sampleRate * numChannels * bitRate/8) || (blockAlign != numChannels * bitRate/8))
    {
        FATAL(InvalidFile) << "Incorrect header, ByteRate and/or BlockAlign values do not match!";
    }

    std::string subChunk2ID (_fileData.begin() + dataIndex, _fileData.begin() + dataIndex + 4);

    if(subChunk2ID != "data")
    {
        FATAL(InvalidFile) << "Invalid SubChunk2ID : " << subChunk2ID;
    }

    unsigned long subChunk2Size = fourBytesToInt(_fileData, dataIndex + 4);

    unsigned long int numSamples = subChunk2Size * 8 / ( numChannels * bitRate);

    DEBUG << "Number of samples : " << numSamples;
    DEBUG << "Reading samples";

    for (unsigned long int i = 0; i < numSamples; i++)
    {
            unsigned long int sampleIndex = dataIndex + 8 + (blockAlign * i);
            // dataIndex = index of beginning of "data" subChunk
            // dataIndex + 8 is usually 44 as per the specs

            int16_t sample = twoBytesToInt(_fileData, sampleIndex);
            _samples.push_back(sample);
    }
    DEBUG << "Successfully decoded";
    return true;    //successfully decoded
}

bool WaveFileData::openFile ()
{
    std::ifstream infile (_fileName, std::ios::binary);

    DEBUG << "Trying to read from file : " << _fileName;

    if (!infile)
    {
        FATAL(FileNotFound) << "Unable to open file : " << _fileName;
    }

    DEBUG << "Reading file data";

    if (_isRawFile) { // handle raw audio files
        infile.seekg(0, std::ios::end);
        std::streamsize size = infile.tellg();
        infile.seekg(0, std::ios::beg);

        _samples = std::vector<int16_t>(size/2); // size is in unit of byte, while one int_16 uses 2 bytes
        if (infile.read(reinterpret_cast<char*>(_samples.data()), size)) {
            DEBUG << "File data read";
            DEBUG << "Decoding is skipped since it is raw audio file";
            return true;
        }
        else {
            FATAL(InvalidFile) << "Unable to read from file : " << _fileName;
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

    DEBUG << "File data read and stored in buffer";

    DEBUG << "Processing data and extracting samples";

    if(checkValidWave(fileData))
    {
        DEBUG << "Wave File chunkID verification successful";

        _fileData = fileData;   //wave is valid, store and proceed

        DEBUG << "Begin decoding wave file";

        decode();

        DEBUG << "File decoded successfully";

        return true;
    }

    else
    {
        DEBUG << "Invalid WAV file";
        FATAL(InvalidFile) << "Invalid WAV file!";
    }

    return true;
}

int WaveFileData::processStreamHeader()
{
    DEBUG << "Processing Stream Header";
    DEBUG << "Checking chunkID, should be RIFF";
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
                FATAL(InvalidFile) << "Invalid WAV file : Incorrect subChunk1ID!";
            }

            if (currentByteCount == 3)
            {
                riffRead = true;
                DEBUG << "chunkID = RIFF confirmed!";
                DEBUG << "Checking WAV file header, should be WAVE";

            }
        }

        else
        {
            if (currentByteCount >= 8)
            {
                if (wave[currentByteCount - 8] != byteData) //checking WAVE format
                {
                    DEBUG << "Error: Incorrect header";
                    FATAL(InvalidFile) << "Invalid WAV file : Incorrect Header!";
                }

                if (currentByteCount == 11)
                {
                    return chunkSize - 8;
                    DEBUG << "wav header = WAVE confirmed!";
                }
            }

            else
            {
                chunkSize = chunkSize | byteData << (8 * (currentByteCount - 4));   //calculating chunksize (filesize - 8)
            }
        }
    }
    FATAL(UnknownError) << "Error occurred while processing stream header!";
}

int WaveFileData::seekToEndOfSubChunk1ID(int remainingBytes)
{
    DEBUG << "Checking SubChunk1ID, should be fmt";

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
                DEBUG << "SubChunk1ID = fmt confirmed!";

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
            DEBUG << "SubChunk1 ('fmt') not found";
            FATAL(InvalidFile) << "Invalid WAV file: SubChunk1 ('fmt') not found!";
        }
    }
    FATAL(UnknownError) << "Error occurred while checking SubChunk1ID";
    return -1;
}

int WaveFileData::validateSubChunk1(int remainingBytes)
{
    DEBUG << "Validating SubChunk1";

    unsigned char byteData;
    std::vector<unsigned char> fmtBlock;
    int currentByteCount = -1;


    while(std::cin>>std::noskipws>>byteData)
    {
        _fileData.push_back(byteData);
        currentByteCount++;
        fmtBlock.push_back(byteData);               //storing 'fmt' data in a buffer, processing live is dangerous
        DEBUG << "Storing 'fmt' data in a buffer";

        if(currentByteCount == 19)
        {
            break;
        }

    }

    unsigned long subChunk1Size = fourBytesToInt(fmtBlock, 0);

    if(subChunk1Size != 16)
    {
        FATAL(InvalidFile) << "Invalid WAV file: Not PCM, SubChunk1Size: " << subChunk1Size;
    }

    int audioFormat = twoBytesToInt(fmtBlock, 4);

    if(audioFormat != 1)
    {
        FATAL(InvalidFile) << "Invalid WAV file: Not PCM, AudioFormat: " << audioFormat;
    }

    int numChannels = twoBytesToInt(fmtBlock, 6);

    if(numChannels != 1)
    {
        FATAL(InvalidFile) << "Invalid WAV file: Not Mono, NumChannels: " << numChannels;
    }

    unsigned long sampleRate = fourBytesToInt(fmtBlock, 8);

    if(sampleRate != 16000)
    {
        FATAL(InvalidFile) << "Invalid WAV file: Not 16000Hz SampleRate, SampleRate: " << sampleRate;
    }

    unsigned long byteRate = fourBytesToInt(fmtBlock, 12);

    int blockAlign = twoBytesToInt(fmtBlock, 16);

    int bitRate = twoBytesToInt(fmtBlock, 18); //BitsPerSample

    if(bitRate != 16)
    {
        FATAL(InvalidFile) << "Invalid WAV file: Not 16 bits/sec, BitRate: " << bitRate;
    }

    if((byteRate != sampleRate * numChannels * bitRate/8) || (blockAlign != numChannels * bitRate/8))
    {
        FATAL(InvalidFile) << "Incorrect header, ByteRate and/or BlockAlign values do not match!";
    }

    return currentByteCount + 1;

}

int WaveFileData::seekToEndOfSubChunk2ID(int remainingBytes)
{
    DEBUG << "Reading SubChunk2";

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
            DEBUG << "SubChunk2 ('data') not found";
            FATAL(InvalidFile) << "SubChunk2 ('data') not found!";
        }
    }
    FATAL(UnknownError) << "Error occurred while reading SubChunk2!";
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
    DEBUG << "Reading and decoding samples from stream...";

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
            DEBUG << "Storing sample";
            twoBytes.clear();
            two = 0;
        }

        if(bytesRead > numberOfSamples * 2)
        {
            DEBUG << "\nLooks like number of bytes exceeds the expected amount! Still processing.";
        }
    }

    if(bytesRead < numberOfSamples * 2)
    {
		DEBUG << "\nReceived less number of samples than the expected amount! Still processing.";
    }

    DEBUG << "Samples read and decoded!";


    return -1;

}

bool WaveFileData::readStream()
{
    DEBUG << "Reading WAV file from stream";
    if (_isRawFile) {
        DEBUG << "Raw audio mode is enabled. Turn to read stream using buffer...";
        return readStreamUsingBuffer();
    }

    int remainingBytes = processStreamHeader();                 //processing wave header
    remainingBytes -= seekToEndOfSubChunk1ID(remainingBytes);   //searching 'fmt' subchunk
    remainingBytes -= validateSubChunk1(remainingBytes);        //processing 'fmt' subchunk
    remainingBytes -= seekToEndOfSubChunk2ID(remainingBytes);   //searching 'data' subchunk

    int numberOfSamples = getNumberOfSamples();                 //getting number of samples

    if(remainingBytes != 2 * numberOfSamples)
    {
        DEBUG << "\nLooks like there is some error in reading samples from the file. Still proceeding.";
        DEBUG << "Potential error(s) in reading samples, still proceeding";
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
        FATAL(InvalidFile) << "Invalid WAV file: SubChunk2 ('data') not found!";
    }

    return true;

}

bool WaveFileData::read()   //decided the function based on set mode
{
    DEBUG << "Begin reading WAV file";

    switch (_openMode)
    {
        case readFile             : DEBUG << "Opening mode chosen: readFile, proceeding";
                                    openFile();             //file on disk
                                    break;
        case readStreamDirectly   : DEBUG << "Opening mode chosen: readStreamDirectly, proceeding";
                                    readStream();           //from stream/pipe
                                    break;
        case readStreamIntoBuffer : DEBUG << "Opening mode chosen: readStreamIntoBuffer, proceeding";
                                    readStreamUsingBuffer();//from stream/pipe into buffer and then processing
                                    break;
        default                   : FATAL(InvalidFile)<<"Error choosing opening mode, please report!";

    }
    return true;
}

/* Convert 4 bytes to int
 * Reference :
 * https://stackoverflow.com/a/2386134/6487831
 */

unsigned long WaveFileData::fourBytesToInt (const std::vector<unsigned char>& fileData, int index)
{
    return ((fileData[index + 3] << 24) | (fileData[index + 2] << 16) | (fileData[index + 1] << 8) | fileData[index]);
}

int WaveFileData::twoBytesToInt (const std::vector<unsigned char>& fileData, int index)
{
    return ((fileData[index + 1] << 8) | fileData[index]);
}

const std::vector<int16_t>& WaveFileData::getSamples() const noexcept
{
    return _samples;    //returning sample vector
}
