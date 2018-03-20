#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>


class cWav
{
public:
	cWav();
	cWav(int inFileSize, std::string inFileName);
	cWav(std::vector<int> &inSong, std::string outFileName, int inSampleRate = 44100);
	cWav(std::vector < std::vector<int>> &inSong, std::string outFileName, int inSampleRate = 44100);
	~cWav();

	bool setFileSize(int inFileSize);
	int getFileSize();
	bool setFileName(std::string inFileName); 
	std::string getFileName();

	


private:
	bool buildHeader(int inFileSize = 0);
	void flipChars(char* inChar, const unsigned int inSize);
	std::string parseFileName(std::string inFileName);
	std::string fileName;
	int fileSize;

	char header[0xc];
	char format[0x1a];
};

