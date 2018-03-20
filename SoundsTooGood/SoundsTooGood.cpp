/***********************************************************
* Author:				Kirt Stark
* Date Created:			04/27/15
* Last Modification Date:	6/20/16
* Filename:				SoundsTooGood.cpp
*
* Overview:
*   This program consists of a series of classes that use
*	data input from the main function to create sounds.
*	It is intended that musical instruments will be
*	mainly represented with standard defualt settings. However,
*	sound components will remain available by programmer to 
*	alter in order to create custom sounds.
*
* Input:
*	Sound data will be entered into the 
*
* Output:
*   The completed vector of tokens with the associated type name
*   will be output to the screen for each text file listed. The
*   output will be in the form:
*
*   type			token
*
*   Example:
*
*   identifier		foo
*   operator		++
*   str literal		"hello world!"
*   crg return		RETURN
*
*
*  After this, the parsed file is processed for the preprocessor
*  statements and the final result is printed to the screen
*  in the same format as above. The program is partially compiled
*  and the symbol table is printed to the screen.
************************************************************/

#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <MMSystem.h>
#include "cWav.h"
#include "cNote.h"
#include "cMakeNote.h"
#include "cMusic.h"
#include <math.h>
#include <time.h>
#include <string>
//#include <fstream>//////////////////////////////// used for debuging and testing purposes

// for instructions on setting up the project, see https://youtu.be/2Fqh-8XqK0M

using namespace std;

#ifndef PIT
#define PIT (3.141592654 * 2)
#endif

bool make_music(cMusic& cm, int selection);
bool mary_Lamb(cMusic& cm);
bool jazz_bass(cMusic& cm);
bool voice(cMusic& cm);
bool scales(cMusic& cm);

int main()
{
	//

	double noise = .98;
	for (int i = 0; i < 100; i++)
	{
		double theNoise = (noise * (((rand() % 100) / 100.0)) + 1.0 - noise);
		std::cout << theNoise << std::endl;
	}
		//std::cout << 1.0 * (rand() % 100) / 100 << std::endl;

	system("pause");

	



	////////////////
	int sampleRate = 44100; // Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz
	std::vector<std::string> freqStr;
	std::vector<double> freqFloat;
	cMusic cm(sampleRate);
	//double strange = .98;
	cm.set_strange_percent(.98);
	
	
	double sec = 2.5;
	double volume = .9; // range from 0 to 1

	//std::vector<int> vec;
	//for (int r = 0; r < (sec * sampleRate); r++)
	//{
	//	(int)(32767 * .9 * sin(55.0 * ((double)r * 3.141592654 * 2.0) / (double)sampleRate));
	//}

	//std::string instrument = "piano";
	//std::string instrument = "violin";
	//std::string instrument = "bass";
	//std::string instrument = "voice";



	std::cout << "processing data:" << std::endl;

	//make_music(cm, 4);

	freqFloat.push_back(254);
	
	cm.add_note(freqFloat, sec, .9, "voice", 0);

	std::cout << "putting it all together" << std::endl;
	
	cWav trial(*cm.buildMusic(), "test1");


	bool worked;
	std::cout << "playing sounds" << std::endl;
	worked = PlaySound(TEXT("test1"), NULL, SND_SYNC);
	std::cout << "done playing sounds" << std::endl;
	

	system("pause");
	return 0;
}

bool make_music(cMusic& cm, int selection)
{
	switch (selection)
	{
	case 1: // build Mary Had a Little Lamb with piano
		return mary_Lamb(cm);
		break;

	case 2: // jazzy bass notes
		return jazz_bass(cm);
		break;

	case 3: //voice type sounds
		return voice(cm);
		break;

	case 4: // plays piano scales
		return scales(cm);
		break;

	default: std::cout << "makeMusic selection out of range" << std::endl;
		return false;
	}
	return true;
}

bool mary_Lamb(cMusic& cm)
{
	std::string instrument = "piano";
	std::vector<std::string> freqStr;
	double sec = 2.5;

	freqStr.push_back("A1");
	cm.add_note(freqStr, sec, .8, instrument, 0);

	freqStr.clear();
	freqStr.push_back("G1");
	cm.add_note(freqStr, sec, .7, instrument, 1.0);


	freqStr.clear();
	freqStr.push_back("F1");
	cm.add_note(freqStr, sec, .7, instrument, 2.0);

	freqStr.clear();
	freqStr.push_back("G1");
	cm.add_note(freqStr, sec, .6, instrument, 3.0);

	freqStr.clear();
	freqStr.push_back("A1");
	cm.add_note(freqStr, 1.0, .9, instrument, 3.75);

	freqStr.clear();
	freqStr.push_back("A1");
	cm.add_note(freqStr, 1.0, .9, instrument, 4.25);

	freqStr.clear();
	freqStr.push_back("A1");
	cm.add_note(freqStr, sec, .9, instrument, 4.75);
	return true;
}

bool jazz_bass(cMusic& cm)
{
	std::vector<double> position_vector;
	std::string instrument = "bass";
	std::vector<std::string> freqStr;
	double sec = 2.5;

	position_vector.push_back(0.0);
	position_vector.push_back(4.0);
	freqStr.push_back("A0");
	cm.add_note(freqStr, sec, .8, instrument, position_vector);

	position_vector.clear();
	position_vector.push_back(0.5);
	position_vector.push_back(3.5);
	position_vector.push_back(4.5);
	position_vector.push_back(7.5);
	freqStr.clear();
	freqStr.push_back("B0");
	cm.add_note(freqStr, sec, .6, instrument, position_vector);

	position_vector.clear();
	position_vector.push_back(1.0);
	position_vector.push_back(3.0);
	position_vector.push_back(5.0);
	position_vector.push_back(7.0);
	freqStr.clear();
	freqStr.push_back("C1");
	cm.add_note(freqStr, sec, .7, instrument, position_vector);

	position_vector.clear();
	position_vector.push_back(1.5);
	position_vector.push_back(2.5);
	position_vector.push_back(5.5);
	position_vector.push_back(6.5);
	freqStr.clear();
	freqStr.push_back("D1");
	cm.add_note(freqStr, sec, .8, instrument, position_vector);

	position_vector.clear();
	position_vector.push_back(2.0);
	position_vector.push_back(6.0);
	freqStr.clear();
	freqStr.push_back("E1");
	cm.add_note(freqStr, sec, .9, instrument, position_vector);

	return true;
}

bool voice(cMusic& cm)
{
	std::vector<double> freqFloat;
	double sec = 2.5;
	std::string instrument = "voice";
	double pos_thing = 0;
	freqFloat.push_back(800);
	//freqFloat.push_back(800.2);
	cm.add_note(freqFloat, sec, 1.0, instrument, pos_thing);
	freqFloat.clear();
	freqFloat.push_back(1840);
	cm.add_note(freqFloat, sec, .05, instrument, pos_thing);

	float fra = 310;
	float vla = .36;
	float frb = 620;
	float vlb = .3;//  *32767.0;
	float frc = 930;
	float vlc = 1.0;// * 32767.0;
	float frd = 1240;
	float vld = .32;// *32767.0;
	float fre = 1550;
	float vle = 0.27;

	pos_thing += 3.0;

	freqFloat.clear();
	freqFloat.push_back(fra);
	freqFloat.push_back(110);
	cm.add_note(freqFloat, sec, vla, instrument, pos_thing);

	freqFloat.clear();
	freqFloat.push_back(frb);
	cm.add_note(freqFloat, sec, vlb, instrument, pos_thing);

	freqFloat.clear();
	freqFloat.push_back(frc);
	cm.add_note(freqFloat, sec, vlc, instrument, pos_thing);

	freqFloat.clear();
	freqFloat.push_back(frd);
	cm.add_note(freqFloat, sec, vld, instrument, pos_thing);

	freqFloat.clear();
	freqFloat.push_back(fre);
	cm.add_note(freqFloat, sec, vle, instrument, pos_thing);

	return true;
}

bool scales(cMusic& cm)
{
	int sampleRate = 44100; // Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz
	std::vector<std::string> freqStr;
	std::vector<double> freqFloat;
	double position = 0;

	std::cout << "running note scales test - this may take several minutes" << std::endl;
	std::cout << "\n\tcreating vector of note names and filling that vector" << std::endl;
	std::vector<std::string> note_names;
	note_names.push_back("C");
	note_names.push_back("C#");
	note_names.push_back("Db");
	note_names.push_back("D");
	note_names.push_back("D#");
	note_names.push_back("Eb");
	note_names.push_back("E");
	note_names.push_back("F");
	note_names.push_back("F#");
	note_names.push_back("Gb");
	note_names.push_back("G");
	note_names.push_back("G#");
	note_names.push_back("Ab");
	note_names.push_back("A");
	note_names.push_back("A#");
	note_names.push_back("Bb");
	note_names.push_back("B");

	std::cout << "\n\tnote names are complete" << std::endl;
	std::cout << "\n\building the notes" << std::endl;
	freqStr.clear();
	std::vector<cMakeNote*> vNotes;

	std::string n;
	cMakeNote* tempNote;

	for (int i = 0; i <= 8; i++)
	{
		std::cout << "\n\t\tworking on i as " << i << std::endl;
		for (int j = 0; j < note_names.size(); j++)
		{
			std::cout << "\n\t\t\tworking on j as " << j << std::endl;
			n = note_names[j] + std::to_string(i);
			freqStr.clear();
			freqStr.push_back(n);
			cm.add_note(freqStr, .8, .7, "piano", position);
			position += 1.0;		
		}
	}

	freqStr.clear();

	std::cout << "adding loopy stuff" << std::endl;

	for (int i = 1; i < 6; i++)
	{
		freqFloat.clear();
		freqFloat.push_back(110 * i);
		cm.add_note(freqFloat, 2.5, .7, "piano", (4.0 + i));
	}

	return true;
}


/*
int makeNote2(int pos, int vowel)
{
	// constants to read into the method
	double freq = 440.0;
	double volume = 0.8 * 32767.0;
	int sampleRate = 44100;
	double timbreMod = 3.0;

	// preparations
	//double mult = (double)pos * PIT * freq / (double)sampleRate; ///////////////////////////////////////restore later
	double mult = (double)pos * PIT / (double)sampleRate; /////////////////////////delete later
	double result = 0.0;

	// creating sound
	
	for (int j = 1; j <= 4; j++)
	{
		result += sin(mult * j);
		//result += sin(mult);
	}
	
	//result += sin(mult) * sin(mult * 2) * sin(mult * 4) * sin(mult * 8);
	//////////////////////////////////////////////////////////////////////////////////////////

	double fra = 0;
	double vla = 0;
	double frb = 0;
	double vlb = 0;
	double frc = 0;
	double vlc = 0;
	double frd = 0;
	double vld = 0;
	double fre = 0;
	double vle = 0;

	// mat vowel sound
	if (vowel == 1)
	{
		fra = 800;
		vla = 1 * 32767.0;
		frb = 1840;
		vlb = .05  * 32767.0;
		frc = 440;
		vlc = 0 * 32767.0;
		frd = 440;
		vld = 0 * 32767.0;
		fre = 0;
		vle = 0;
	}

	// mat vowel sound
	else if(vowel == 2)
	{
		fra = 800;
		vla = 1 * 32767.0;
		frb = 1840;
		vlb = .05  * 32767.0;
		frc = 440;
		vlc = 0 * 32767.0;
		frd = 440;
		vld = 0 * 32767.0;
		fre = 0;
		vle = 0;
	}

	// meet vowel sound
	else if(vowel == 3)
	{
		fra = 310;
		vla = 1 * 32767.0;
		frb = 3100;
		vlb = .05  * 32767.0;
		frc = 440;
		vlc = 0 * 32767.0;
		frd = 440;
		vld = 0 * 32767.0;
		fre = 0;
		vle = 0;
	}

	// met vowel sound
	else if(vowel == 4)
	{
		fra = 690;
		vla = 1 * 32767.0;
		frb = 1950;
		vlb = .05  * 32767.0;
		frc = 440;
		vlc = 0 * 32767.0;
		frd = 440;
		vld = 0 * 32767.0;
		fre = 0;
		vle = 0;
	}

	// mat vowel sound
	else if(vowel == 5)
	{
		fra = 490;
		vla = 1 * 32767.0;
		frb = 2460;
		vlb = .07  * 32767.0;
		frc = 440;
		vlc = 0 * 32767.0;
		frd = 440;
		vld = 0 * 32767.0;
		fre = 0;
		vle = 0;
	}

	// ah saprano vowel sound
	else  if (vowel == 6)
	{
		fra = 310;
		vla = .36 * 32767.0;
		frb = 620;
		vlb = .3  * 32767.0;
		frc = 930;
		vlc = 1 * 32767.0;
		frd = 1240;
		vld = .32 * 32767.0;
		fre = 1550;
		vle = 0.27;
	}

	// ah bass vowel sound
	else
	{
		fra = 310;
		vla = .21 * 32767.0;
		frb = 775;
		vlb = .43  * 32767.0;
		frc = 930;
		vlc = 1 * 32767.0;
		frd = 1240;
		vld = .32 * 32767.0;
		fre = 1085;
		vle = 0.32;
	}

	return (int)(vla * sin(fra * mult) + vlb * sin(frb * mult) + vlc * sin(frc * mult) + vld * sin(frd * mult) + vle * sin(fre * mult));



	/////////////////////////////////////////////////////////////////////////////////

	// adding envelope and returning
	//return (int)(volume * result *exp(timbreMod * (-(double)pos + 1.0) / (double)sampleRate)); /////////////////////////restore later
}

int makeNote(int pos, double freq, double volume, int sigBiytes, int sampleRate, int harmony, double oddDiv, double evenDiv)
{
	double div = 1;

	if (sigBiytes > 2 || sigBiytes < 0 || volume > 1 || volume < 0)
	{
		std::cout << "Error - significant bytes and volume must be between 0 and 1" << std::endl;
		return 0;
	}

	if ((oddDiv < 0.1 && oddDiv > -0.1) || (evenDiv < 0.1 && evenDiv > -0.1))
	{
		std::cout << "Error - the divisors must be greater than .1 or less than -.1" << std::endl;
		return 0;
	}

	if (sigBiytes == 2)
		volume = volume * 32767.0;
	else // assume 1 byte sigBits
		volume = volume * 127.0;

	double timbreMod = 3.0;
	double mult = (double)pos * PIT * freq / (double) sampleRate;
	double result = 0.0;
	//std::cout << "result is " << result << " and volume is " << volume << " and together they are " << volume * result << std::endl;

	result = sin(mult) + sin(2 * mult)/2 +sin(4 * mult)/4 + sin(8 * mult)/8;

	for (int j = 1; j <= harmony; j++)
	{
		if (j == 1)
			div = 1.0;
		else if (j % 2 == 0)
		{
			if (oddDiv < 0)
				div = pow((j - 1), (-1) * oddDiv);
			else
				div = oddDiv * (j - 1);
		}
		else
		{
			if (evenDiv < 0)
				div = pow((j - 1), (-1) * evenDiv);
			else
				div = evenDiv * (j - 1);
		}
		
		//result += sin((double)(pow(2.0, (long)j) * mult) / div);
		//result += sin((double)(pow(2.0, (long)j) * mult) / ((double)(j * j)));
		//result += sin((double)j * mult) / div;
		result += sin(mult * j * 2);
	}
	//return volume * sin(pos * freq * 2 * PI / sampleRate);
	//std::cout << (int)(volume * result) << std::endl;
	
		return (int)((volume * result) * exp(timbreMod * (-(double)pos + 1.0) / (double)sampleRate));
	//return (int)((volume * result) * sin(PIT / 4 + mult / 1720.0));
}
*/
/*


void readFile(std::string fileName)
{
	std::vector<char> entireFile;
	std::fstream rFile;
	long outSize;
	char* buffer = NULL;

	if ((outSize = getOutFileSize(fileName)) > -1)
	{		
		buffer = new char[outSize];
		getData(rFile, 0l, outSize, buffer, fileName);

		for (int i = 0; i < outSize; i++)
		{
			entireFile.push_back(buffer[i]);
		}

		
	}

	std::cout << "Total file size came in at " << outSize << std::endl << std::endl;

	if (entireFile.size() > 49)
	{
		std::cout << "chunk ID -> " << entireFile[0] << entireFile[1] << entireFile[2] << entireFile[3] << std::endl;
		std::cout << "chunk size -> " << littleEndInt(buffer, 4) << std::endl;
		std::cout << "RIFF type -> " << entireFile[8] << entireFile[9] << entireFile[10] << entireFile[11] << std::endl;
		std::cout << "chunk ID -> " << entireFile[12] << entireFile[13] << entireFile[14] << entireFile[15] << std::endl;
		std::cout << "chunk data size -> " << littleEndInt(buffer, 16) << std::endl; ////////////////////////////////////////
		std::cout << "Compression Code -> " << littleEndInt(buffer, 20, 1) << std::endl;
		std::cout << "Number of Channels -> " << littleEndInt(buffer, 22, 1) << std::endl;
		std::cout << "Sample Rate -> " << littleEndInt(buffer, 24) << std::endl;
		std::cout << "Average Bytes per Second -> " << littleEndInt(buffer, 28) << std::endl;
		std::cout << "Block Align -> " << littleEndInt(buffer, 32, 1) << std::endl;
		std::cout << "Significant bits per sample -> " << littleEndInt(buffer, 34, 1) << std::endl;
		std::cout << "chunk ID -> " << entireFile[36] << entireFile[37] << entireFile[38] << entireFile[39] << std::endl;
		std::cout << "chunk size -> " << littleEndInt(buffer, 40) << std::endl; /////////////////////////////////////////////////

		for (int i = 44; i < 60; i++)
		{
			std::cout << entireFile[i] << std::endl;
		}
	}
	else
		std::cout << ">>>>>>>>>>>>>>>> error in file - less than 50 thingys" << std::endl;

	delete buffer;
}

long getOutFileSize(std::string fileName)
{
	std::ofstream outFile;
	outFile.open(fileName, std::ios::app);
	if (!outFile)
	{
		std::cout << "We have an error opening file" << std::endl;
		return -1;
	}
	outFile.seekp(0, std::ios::end);
	long outSize = outFile.tellp();
	outFile.close();
	return outSize;
}

bool getData(std::fstream& stream, long position, int length, char buffer[], std::string fileName)
{
	if (getOutFileSize(fileName) < (position + long(length)))
	{
		std::cout << "reading from file not allowed due to file size overrun or file does not exist";
		return false;
	}
	else
	{
		stream.open(fileName, std::ios::in | std::ios::ate);
		if (!stream)
		{
			std::cout << "error opening file" << std::endl;
			return false;
		}
		stream.seekg(position, std::ios::beg);
		stream.read(buffer, length);
		stream.close();

		return true;
	}
}

int littleEndInt(char *b, int start, int code) {

	if (code != 0)
		return (((0x0 & 0xff) << 24) | ((0x0 & 0xff) << 16) | ((b[start + 1] & 0xff) << 8) | ((b[start + 0] & 0xff)));
	return (((b[start + 3] & 0xff) << 24) | ((b[start + 2] & 0xff) << 16) | ((b[start + 1] & 0xff) << 8) | ((b[start + 0] & 0xff)));
}

*/