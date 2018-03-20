#include "stdafx.h"
#include "cWav.h"


cWav::cWav()
{
	fileSize = 0;
	buildHeader();
}

cWav::cWav(int inFileSize, std::string inFileName)
{
	if (inFileSize % 2 != 0)
	{
		std::cout << "Error in file size in cWave constructor, will not work" << std::endl; // fix to work regardless
	}
	buildHeader(inFileSize);
	fileName = parseFileName(inFileName);
}

cWav::cWav(std::vector<int> &inSong, std::string outFileName, int inSampleRate)
{
	int numChannels = 1;
	int sampleRate = inSampleRate; // Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz
	int sigBitsPerSample = 16; // significant bits per sample, usually 8, 16, 24, or 32. number of channels don't matter
	int blockAligh = (sigBitsPerSample * numChannels) / 8;
	int averBytesPerSec = sampleRate * blockAligh;
	std::ofstream outFile;

	outFileName = parseFileName(outFileName);

	if (inSong.size() % 2 != 0)
		inSong.push_back(0);

	int totalSize = (inSong.size() * (sigBitsPerSample / 8) + 44);
	int totalSize2 = totalSize - 8;
	char* data;
	data = new char[totalSize];
	int dataChunkSize = totalSize - 44;

	// need to check for endianess and swap if necessary
	char *chars = reinterpret_cast<char*>(&totalSize2);
	char *channels = reinterpret_cast<char*>(&numChannels);
	char *sampleRateArray = reinterpret_cast<char*>(&sampleRate);
	char *sigBitsPerSampleArray = reinterpret_cast<char*>(&sigBitsPerSample);
	char *blockAlighArray = reinterpret_cast<char*>(&blockAligh);
	char *averBytesPerSecArray = reinterpret_cast<char*>(&averBytesPerSec);
	char *totalSizeArray = reinterpret_cast<char*>(&dataChunkSize);


	// main header
	data[0] = (char)0x52; // R
	data[1] = (char)0x49; // I
	data[2] = (char)0x46;  // F
	data[3] = (char)0x46; // F
	data[4] = chars[0];  // 4 bytes of total file size minus the first 4 bytes
	data[5] = chars[1];
	data[6] = chars[2];
	data[7] = chars[3];
	data[8] = (char)0x57; // W
	data[9] = (char)0x41; // A
	data[10] = (char)0x56; // V
	data[11] = (char)0x45; // E

	// format header
	data[12] = (char)0x66; // f
	data[13] = (char)0x6D; // m
	data[14] = (char)0x74; // t
	data[15] = (char)0x20; // space
	data[16] = (char)0x10;  // 4 bytes to equal 16 as long as we have no extra format bytes
	data[17] = (char)0x0;
	data[18] = (char)0x0;
	data[19] = (char)0x0;
	data[20] = (char)0x1; // 2 bytes for compression code - PCM/uncompressed
	data[21] = (char)0x0;
	data[22] = channels[0]; // 2 bytes for number of channels
	data[23] = channels[1];
	data[24] = sampleRateArray[0]; // 4 bytes for sample rate
	data[25] = sampleRateArray[1];
	data[26] = sampleRateArray[2];
	data[27] = sampleRateArray[3];
	data[28] = averBytesPerSecArray[0]; // 4 bytes for average bytes per second
	data[29] = averBytesPerSecArray[1];
	data[30] = averBytesPerSecArray[2];
	data[31] = averBytesPerSecArray[3];
	data[32] = blockAlighArray[0]; // 2 bytes for block align
	data[33] = blockAlighArray[1];
	data[34] = sigBitsPerSampleArray[0]; // 2 bytes for signficant bits per sample
	data[35] = sigBitsPerSampleArray[1];
//	data[36] = (char)0x0; // 2 bytes for extra format bytes, set to 0
//	data[37] = (char)0x0;

	// data header
	data[36] = (char)0x64;
	data[37] = (char)0x61;
	data[38] = (char)0x74;
	data[39] = (char)0x61;
	data[40] = totalSizeArray[0];
	data[41] = totalSizeArray[1];
	data[42] = totalSizeArray[2];
	data[43] = totalSizeArray[3];

	int currentPosition = 43;

	// add data to array
	for each(int i in inSong)
	{
		//std::cout << "the int is " << i << std::endl;
		char *iArray = reinterpret_cast<char*>(&i); // need to check for endianess and swap if necessary

		for (int j = 0; j < (sigBitsPerSample / 8); j++)
		{
			if (currentPosition + 2 > totalSize)
				std::cout << "oversize error" << std::endl;
			data[++currentPosition] = iArray[j];
			//std::cout << "\tcurrent position " << currentPosition << " is " << iArray[j] << std::endl;
		}
	}

	// write to file
	outFile.open(outFileName, std::ios::binary | std::ios::out);
	if (!outFile)
	{
		std::cout << "error opening file" << std::endl;
		return;
	}

	outFile.write(data, totalSize);
	outFile.close();
}


cWav::~cWav()
{
}

bool cWav::buildHeader(int inFileSize)
{
	if (inFileSize > 0)
		inFileSize += 4;

	char *chars = reinterpret_cast<char*>(&inFileSize);
	int charSize = sizeof(chars);

	if (charSize < 4)
	{
		std::cout << "Error in size of integer in buildHeader method, cWav class" << std::endl;
		return false;
	}

	header[0] = 0x52;
	header[1] = 0x49;
	header[2] = 0x46;
	header[3] = 0x46;
	header[4] = chars[0];
	header[5] = chars[1];
	header[6] = chars[2];
	header[7] = chars[3];
	header[8] = 0x57;
	header[9] = 0x41;
	header[10] = 0x56;
	header[11] = 0x45;

	return true;
}

// Short, simple program to flip a char array around. Used to account
// for little endian to big endian differences between
// different computers when changing int to char[] and visa versa
void cWav::flipChars(char* inChar, const unsigned int inSize)
{
	int low = 0; int high = inSize - 1;

	while (low < high)
	{
		inChar[low] = inChar[low] ^ inChar[high];
		inChar[high] = inChar[low] ^ inChar[high];
		inChar[low] = inChar[low] ^ inChar[high];
		low++;
		high--;
	}
}

std::string cWav::parseFileName(std::string inFileName)
{
	int pos = 0;

	for (int i = 0; i < (inFileName.size() - 1); i++)
	{
		if (inFileName[i] == '.')
		{
			pos = i;
			break;
		}
	}

	if (pos == 0)
		return inFileName + ".wav";
	else
		return inFileName.substr(0, pos) + ".wav";
}

bool cWav::setFileSize(int inFileSize)
{
	if (inFileSize > 0)
		inFileSize += 4;

	char *chars = reinterpret_cast<char*>(&inFileSize);
	int charSize = sizeof(chars);

	if (charSize < 4)
	{
		std::cout << "Error in size of integer in buildHeader method, cWav class" << std::endl;
		return false;
	}

	header[4] = chars[0];
	header[5] = chars[1];
	header[6] = chars[2];
	header[7] = chars[3];

	return true;
}

int cWav::getFileSize()
{
	return fileSize;
}

bool cWav::setFileName(std::string inFileName)
{
	fileName = parseFileName(inFileName);
	return true;
}

std::string cWav::getFileName()
{
	return fileName;
}

