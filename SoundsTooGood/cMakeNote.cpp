#include "stdafx.h"
#include "cMakeNote.h"


cMakeNote::cMakeNote(int _sampleRate, std::vector<double> _freq, double _seconds, double _volume, std::string _waveType) : sampleRate(_sampleRate), freqs(_freq), sec(_seconds), waveType(_waveType)
{
	int i;
	set_volume(_volume);

	init();
}



cMakeNote::cMakeNote(int _sampleRate, std::vector<std::string> _note_names, double _seconds, double _volume, std::string _waveType) : sampleRate(_sampleRate), sec(_seconds), waveType(_waveType)
{
	int i;
	set_volume(_volume);
	init();
}

void cMakeNote::init()
{
	if (waveType == "piano")
	{
		waveType = "saw_tooth";// always needs to be set prior to other modifications
		//waveType = "reverse_saw_tooth";
		//waveType = "square";
		//waveType = "sine";
		note = new cNote(freqs, volume, global_TopAmp, waveType);
		envelope_ptr = &cMakeNote::lnBellCurve;
		lnMean = 8;
		lnStandardDeviation = 1.2;
		lnPercentage = 1;
		set_repeats(8);
		set_div_k_mult(1);
		set_div_add(5); // just reset - sounds good.
		//set_mult_add(1);////////////////////////////////change to double - sounds wierd at 10
		dropOff = -5000;
		percentage = 0.5;
	}
	if (waveType == "bass")
	{
		waveType = "saw_tooth";// always needs to be set prior to other modifications
		//waveType = "reverse_saw_tooth";
		note = new cNote(freqs, volume, global_TopAmp, waveType);
		envelope_ptr = &cMakeNote::lnBellCurve;
		lnMean = 6;
		lnStandardDeviation = 1.5;
		lnPercentage = 1;
		set_repeats(8);
		//set_div_add(6); // just reset - sounds good.
		//set_mult_add(1);////////////////////////////////change to double - sounds wierd at 10
		dropOff = -5000;
		percentage = 0.8;
	}
	else if (waveType == "violin")
	{
		waveType = "saw_tooth";
		note = new cNote(freqs, volume, global_TopAmp, waveType);
		envelope_ptr = &cMakeNote::lnBellCurve;
		lnMean = 11;
		lnStandardDeviation = .5;
		lnPercentage = 1;
		set_repeats(8);
		set_div_k_mult(3);
		dropOff = -5000;
	}
	else if (waveType == "bell")
	{
		waveType = "triangle";
		note = new cNote(freqs, volume, global_TopAmp, waveType);
		envelope_ptr = &cMakeNote::expCurve;
		set_repeats(3);
		set_div_add(4);
		set_div_k_mult(8);
		dropOff = -5000;
		percentage = 0.6;
	}
	if (waveType == "voice")
	{
		waveType = "saw_tooth";// always needs to be set prior to other modifications///////////////////////////////
		//waveType = "reverse_saw_tooth";
		waveType = "square";
		//waveType = "sine";
		//waveType = "triangle";
		note = new cNote(freqs, volume, global_TopAmp, waveType);
		envelope_ptr = &cMakeNote::lnBellCurve;
		lnMean = 10;
		lnStandardDeviation = 1.2;
		lnPercentage = 1;
		set_repeats(10);
		//set_div_k_mult(5);
		set_div_add(-5);
		set_noise(1.0);
		//set_mult_add(-1);
		dropOff = -5000;
		percentage = 0.5;
	}
	else
	{
		note = new cNote(freqs, volume, global_TopAmp, waveType);
		mean = (int)(sampleRate * (double)sec / 6.0); // multiplied by the amount of seconds
		standardDeviation = (int)(sampleRate * 0.1); // again, number of seconds
		percentage = 1;
		//envelope_ptr = &cMakeNote::bellCurve;
		//envelope_ptr = &cMakeNote::tripleBellCurve;
		envelope_ptr = &cMakeNote::expCurve;
		//envelope_ptr = &cMakeNote::lnBellCurve;
		//envelope_ptr = &cMakeNote::noEffect;

		int mean2 = (int)(sampleRate * (double)sec / 6.0);
		int standardDeviation2 = (int)(sampleRate * 3);
		double percentage2 = .4;

		int mean3 = (int)(sampleRate * (double)sec / 2.0);
		int standardDeviation3 = (int)(sampleRate * 0.1);
		double percentage3 = 1;

		double percentOfRun = .05;
		lnMean = log(sampleRate * sec * percentOfRun);
		lnStandardDeviation = 1.0 - percentOfRun;
		lnPercentage = 1;
	}
}

cMakeNote::~cMakeNote()
{
	if (note)
		delete note;
}

std::vector<int> cMakeNote::buildSong()
{
	int i;
	int count = 0;
	int temp = 0;
	double theNote = 0.0;
	double temperedDown = 0.0;
	double holdValue = 0.0;
	double maxVolumeLevel =  volume * global_TopAmp;
	double volumeAdjust = 0.0;
	int highest = 0;

	std::vector<int> song;
	std::vector<int> result;

	if (reverb > 0.0)
	{
		for (i = 0; i < (sec * sampleRate); i++)
		{
			theNote = (this->*envelope_ptr)(i)* note->slice(i)  * (global_TopAmp * reverbAmp * sin(PIT * i / (sampleRate * reverb)) + 1);

			if (theNote > highest)
				highest = theNote;

			song.push_back(theNote);
		}
	}
	else
	{
		for (i = 0; i < (sec * sampleRate); i++)
		{
			theNote = (this->*envelope_ptr)(i)* note->slice(i);

			if (theNote > highest)
				highest = theNote;

			song.push_back(theNote);
		}
	}

	temperedDown = holdValue = (this->*envelope_ptr)(i + 1);

	while (temperedDown > .01)
	{
		temperedDown = holdValue * exp(++count / (dropOff)); /////////////////////check for divide by 0!!!!!!!!!!!!!!!!!!!!!
		if (reverb > 0.0)
		{
			theNote = temperedDown * note->slice(i + count) * ( global_TopAmp * reverbAmp * .5 * sin(PIT * (i + count) / (sampleRate * reverb)) + 1);
		}
		else
		{
			theNote = temperedDown * note->slice(i + count);
		}

		song.push_back(theNote);
	}

	volumeAdjust = maxVolumeLevel / (double)highest;

	for (i = 0; i < song.size(); i++)
	{
		song[i] = (int)(volumeAdjust * (double)song[i]);

		//double mult = ((double)i * (3.141592654 * 2)) / 44100.0;// (double)sampleRate;
		//song[i] = (int)(global_TopAmp * sin(110 * mult));

		//song[i] = (int)(volumeAdjust * (double)song[i] * global_TopAmp * sin(220 * mult));
	}

	if (makeGraph)
	{
		build_graph(song);
	}

	return song;
}

std::vector<int> cMakeNote::buildStrangeSong(double percnt)
{
	std::vector<int> song;
	double theNote;
	int i;
	int count = 0;
	double temperedDown = 0.0;
	double holdValue = 0.0;
	double maxVolumeLevel = volume * global_TopAmp;
	double volumeAdjust = 0.0;
	int highest = 0;

	if (reverb > 0.0)
	{
		for (i = 1; i < (sec * sampleRate); i++)
		{
			theNote = (this->*envelope_ptr)(i)* note->strangeSlice(i, percnt, (sampleRate * sec)) + global_TopAmp * reverbAmp * sin(PIT * i / (sampleRate * reverb)) / 2;

			if (theNote > highest)
				highest = theNote;

			song.push_back(theNote);
		}
	}
	else
	{
		for (i = 1; i < (sec * sampleRate); i++)
		{
			theNote = (this->*envelope_ptr)(i)* note->strangeSlice(i, percnt, (sampleRate * sec));

			if (theNote > highest)
				highest = theNote;

			song.push_back(theNote);
		}
	}

	while (theNote > 100 || theNote < -100)
	{
		temperedDown = holdValue * exp(++count / (dropOff));
		if (reverb > 0.0)
		{
			theNote = temperedDown * note->strangeSlice(i + count, percnt, (sampleRate * sec)) + global_TopAmp * reverbAmp * sin(PIT * (i + count) / (sampleRate * reverb)) / 2;
		}
		else
		{
			theNote = temperedDown * note->strangeSlice(i + count, percnt, (sampleRate * sec));
		}

		song.push_back(theNote);
	}

	volumeAdjust = maxVolumeLevel / (double)highest;

	for (i = 0; i < song.size(); i++)
	{
		song[i] = (int)(volumeAdjust * (double)song[i]);
	}

	if (makeGraph)
	{
		build_graph(song);
	}

	return song;
}

/*
// experimental method for future use
std::vector<int> cMakeNote::buildSound()
{
	int echo = .1 * sampleRate;

	std::vector<int> song;
	for (int i = 1; i < (sec * sampleRate); i++)
	{
		if (i >= echo)
		{
			song.push_back(.8 * song[i - echo] + (note->sliceProduct(i)));
		}
		else
			song.push_back(note->sliceProduct(i));
	}

	return song;
}
*/

// at 2 standard deviations values are about 10% of max, at 3 standard deviations they are at about 1 %
double cMakeNote::bellCurve(int position)
{
	return (exp((-1 * pow((position - mean), 2)) / (2.0 * pow(standardDeviation, 2)))) * percentage;
}

// at 2 standard deviations values are about 10% of max, at 3 standard deviations they are at about 1 %
double cMakeNote::tripleBellCurve(int position)
{
	if (position <= mean)
		return (exp((-1 * pow((position - mean), 2)) / (2.0 * pow(standardDeviation, 2)))) * percentage;
	else if ((exp((-1 * pow((position - mean), 2)) / (2.0 * pow(standardDeviation, 2)))) * percentage > (exp((-1 * pow((position - mean2), 2)) / (2.0 * pow(standardDeviation2, 2)))) * percentage2)
		return (exp((-1 * pow((position - mean), 2)) / (2.0 * pow(standardDeviation, 2)))) * percentage;
	else if (((exp((-1 * pow((position - mean2), 2)) / (2.0 * pow(standardDeviation2, 2)))) * percentage2) < ((exp((-1 * pow((position - mean3), 2)) / (2.0 * pow(standardDeviation3, 2)))) * percentage3))
		return ((exp((-1 * pow((position - mean2), 2)) / (2.0 * pow(standardDeviation2, 2)))) * percentage2);
	else
		return ((exp((-1 * pow((position - mean3), 2)) / (2.0 * pow(standardDeviation3, 2)))) * percentage3);
}

double cMakeNote::lnBellCurve(int position)
{
	return (exp((-1 * pow((log(position) - lnMean), 2)) / (2.0 * pow(lnStandardDeviation, 2)))) * lnPercentage;
}

double cMakeNote::expCurve(int position)
{
	return (exp(-.5 * (double)sec * (double)(position - 1) / (double)sampleRate)) * percentage;
}

double cMakeNote::noEffect(int position)
{
	return 1.0;
}

double cMakeNote::get_frequency(std::string _note_name)
{
	if (_note_name == "C0")
		return 16.35;
	else if (_note_name == "C#0" || _note_name == "Db0")
		return 17.32;
	else if (_note_name == "D0")
		return 18.35;
	else if (_note_name == "D#0" || _note_name == "Eb0")
		return 19.45;
	else if (_note_name == "E0")
		return 20.6;
	else if (_note_name == "F0")
		return 21.83;
	else if (_note_name == "F#0" || _note_name == "Gb0")
		return 23.12;
	else if (_note_name == "G0")
		return 24.5;
	else if (_note_name == "G#0" || _note_name == "Ab0")
		return 25.96;
	else if (_note_name == "A0")
		return 27.5;
	else if (_note_name == "A#0" || _note_name == "Bb0")
	return 29.14;
	else if (_note_name == "B0")
		return 30.87;

	else if (_note_name == "C1")
		return 32.7;
	else if (_note_name == "C#1" || _note_name == "Db1")
		return 34.65;
	else if (_note_name == "D1")
		return 36.71;
	else if (_note_name == "D#1" || _note_name == "Eb1")
		return 38.89;
	else if (_note_name == "E1")
		return 41.2;
	else if (_note_name == "F1")
		return 43.65;
	else if (_note_name == "F#1" || _note_name == "Gb1")
		return 46.25;
	else if (_note_name == "G1")
		return 49.0;
	else if (_note_name == "G#1" || _note_name == "Ab1")
		return 51.91;
	else if (_note_name == "A1")
		return 55.0;
	else if (_note_name == "A#1" || _note_name == "Bb1")
		return 58.27;
	else if (_note_name == "B1")
		return 61.74;

	else if (_note_name == "C2")
		return 65.41;
	else if (_note_name == "C#2" || _note_name == "Db2")
		return 69.3;
	else if (_note_name == "D2")
		return 73.42;
	else if (_note_name == "D#2" || _note_name == "Eb2")
		return 77.78;
	else if (_note_name == "E2")
		return 82.41;
	else if (_note_name == "F2")
		return 87.31;
	else if (_note_name == "F#2" || _note_name == "Gb2")
		return 92.5;
	else if (_note_name == "G2")
		return 98.0;
	else if (_note_name == "G#2" || _note_name == "Ab2")
		return 103.83;
	else if (_note_name == "A2")
		return 110.0;
	else if (_note_name == "A#2" || _note_name == "Bb2")
		return 116.54;
	else if (_note_name == "B2")
		return 123.47;

	else if (_note_name == "C3")
		return 130.81;
	else if (_note_name == "C#3" || _note_name == "Db3")
		return 138.59;
	else if (_note_name == "D3")
		return 146.83;
	else if (_note_name == "D#3" || _note_name == "Eb3")
		return 155.56;
	else if (_note_name == "E3")
		return 164.81;
	else if (_note_name == "F3")
		return 174.61;
	else if (_note_name == "F#3" || _note_name == "Gb3")
		return 185.00;
	else if (_note_name == "G3")
		return 196.0;
	else if (_note_name == "G#3" || _note_name == "Ab3")
		return 207.65;
	else if (_note_name == "A3")
		return 220.0;
	else if (_note_name == "A#3" || _note_name == "Bb3")
		return 233.08;
	else if (_note_name == "B3")
		return 246.94;

	else if (_note_name == "C4")
		return 261.63;
	else if (_note_name == "C#4" || _note_name == "Db4")
		return 277.18;
	else if (_note_name == "D4")
		return 293.66;
	else if (_note_name == "D#4" || _note_name == "Eb4")
		return 311.13;
	else if (_note_name == "E4")
		return 329.63;
	else if (_note_name == "F4")
		return 349.23;
	else if (_note_name == "F#4" || _note_name == "Gb4")
		return 369.99;
	else if (_note_name == "G4")
		return 392.0;
	else if (_note_name == "G#4" || _note_name == "Ab4")
		return 415.3;
	else if (_note_name == "A4")
		return 440.0;
	else if (_note_name == "A#4" || _note_name == "Bb4")
		return 466.16;
	else if (_note_name == "B4")
		return 493.88;

	else if (_note_name == "C5")
		return 523.25;
	else if (_note_name == "C#5" || _note_name == "Db5")
		return 554.37;
	else if (_note_name == "D5")
		return 587.33;
	else if (_note_name == "D#5" || _note_name == "Eb5")
		return 622.25;
	else if (_note_name == "E5")
		return 659.25;
	else if (_note_name == "F5")
		return 698.46;
	else if (_note_name == "F#5" || _note_name == "Gb5")
		return 739.99;
	else if (_note_name == "G5")
		return 783.99;
	else if (_note_name == "G#5" || _note_name == "Ab5")
		return 830.61;
	else if (_note_name == "A5")
		return 880.0;
	else if (_note_name == "A#5" || _note_name == "Bb5")
		return 932.33;
	else if (_note_name == "B5")
		return 987.77;

	else if (_note_name == "C6")
		return 1046.5;
	else if (_note_name == "C#6" || _note_name == "Db6")
		return 1108.73;
	else if (_note_name == "D6")
		return 1174.66;
	else if (_note_name == "D#6" || _note_name == "Eb6")
		return 1244.51;
	else if (_note_name == "E6")
		return 1318.51;
	else if (_note_name == "F6")
		return 1396.91;
	else if (_note_name == "F#6" || _note_name == "Gb6")
		return 1479.98;
	else if (_note_name == "G6")
		return 1567.98;
	else if (_note_name == "G#6" || _note_name == "Ab6")
		return 1661.22;
	else if (_note_name == "A6")
		return 1760.0;
	else if (_note_name == "A#6" || _note_name == "Bb6")
		return 1864.66;
	else if (_note_name == "B6")
		return 1975.53;

	else if (_note_name == "C7")
		return 2093.0;
	else if (_note_name == "C#7" || _note_name == "Db7")
		return 2217.46;
	else if (_note_name == "D7")
		return 2349.32;
	else if (_note_name == "D#7" || _note_name == "Eb7")
		return 2489.02;
	else if (_note_name == "E7")
		return 2637.02;
	else if (_note_name == "F7")
		return 2793.83;
	else if (_note_name == "F#7" || _note_name == "Gb7")
		return 2959.96;
	else if (_note_name == "G7")
		return 3135.96;
	else if (_note_name == "G#7" || _note_name == "Ab7")
		return 3322.44;
	else if (_note_name == "A7")
		return 3520.0;
	else if (_note_name == "A#7" || _note_name == "Bb7")
		return 3729.31;
	else if (_note_name == "B7")
		return 3951.07;

	else if (_note_name == "C8")
		return 4186.01;
	else if (_note_name == "C#8" || _note_name == "Db8")
		return 4434.92;
	else if (_note_name == "D8")
		return 4698.63;
	else if (_note_name == "D#8" || _note_name == "Eb8")
		return 4978.03;
	else if (_note_name == "E8")
		return 5274.04;
	else if (_note_name == "F8")
		return 5587.65;
	else if (_note_name == "F#8" || _note_name == "Gb8")
		return 5919.91;
	else if (_note_name == "G8")
		return 6271.93;
	else if (_note_name == "G#8" || _note_name == "Ab8")
		return 6644.88;
	else if (_note_name == "A8")
		return 7040.0;
	else if (_note_name == "A#8" || _note_name == "Bb8")
		return 7458.62;
	else if (_note_name == "B8")
		return 7902.13;

	else
	{
		std::cout << "ERROR - Note " << _note_name << "not recognized" << std::endl;
	}
}


// cNote controls
void cMakeNote::set_global_TopAmp(int value)
{
	note->set_global_TopAmp(value);
}

void cMakeNote::set_global_amplitude(double value)
{
	note->set_global_amplitude(value);
}

void cMakeNote::set_frequency(std::vector<double> value)
{
	note->set_frequency(value);
}

void cMakeNote::set_global_numeratorExp(int value)
{
	note->set_global_numeratorExp(value);
}

void cMakeNote::set_global_denomonatorExp(int value)
{
	note->set_global_denomonatorExp(value);
}

void cMakeNote::set_global_oscillator(int value)
{
	note->set_global_oscillator(value);
}

void cMakeNote::set_mult_k_mult(int value)
{
	note->set_mult_k_mult(value);
}

void cMakeNote::set_mult_add(int value)
{
	note->set_mult_add(value);
}

void cMakeNote::set_div_k_mult(int value)
{
	note->set_div_k_mult(value);
}

void cMakeNote::set_div_add(int value)
{
	note->set_div_add(value);
}

void cMakeNote::set_div_exp(int value)
{
	note->set_div_exp(value);
}

void cMakeNote::set_rev_negator(bool value)
{
	note->set_rev_negator(value);
}

void cMakeNote::set_repeats(int value)
{
	note->set_repeats(value);
}

void cMakeNote::set_sampleRate(int value)
{
	note->set_sampleRate(value);
}

void cMakeNote::set_reverb(double value)
{
	if (value > 0.0 && value < 5.0)
	{
		reverb = value;
	}
}

void cMakeNote::set_reverbAmp(double value)
{
	if (value >= 0.0 && value <= 1.0)
	{
		reverbAmp = value;
	}
}

void cMakeNote::set_volume(double value)
{
	if (value > 1)
		value = 1;
	else if (value < 0)
		value = 0;
	volume = value;
}

void cMakeNote::set_noise(double value)
{
	note->set_noise(value);
}

void cMakeNote::set_freq_add(double value)
{
	if (abs(value) < 6)
	{
		freq_add.push_back(value);
	}
	else
	{
		std::cout << "ERROR: frequency addition is too large" << std::endl;
	}
}

void cMakeNote::display()
{
	note->display();
}

// end section

void cMakeNote::build_graph(std::vector<int> line)
{
	int count = 0;
	std::string fileName = "graph.bmp";
	std::cout << "file size is " << line.size() << std::endl;

	std::cout << "Creating a bitmap file" << std::endl;
	int COLUMN = line.size() / 100 + (4 - (line.size() / 100) % 4);
	int ROW = 656; // top amount is 65534 with half over 0, so add the half back and then divide by 100;
	double RESOLUTION = 200;

	std::vector<int>lineModified;
	// prepare new line for use in making graph
	for (int i = 0; i < line.size(); i += 100)
		lineModified.push_back(line[i]/100 + ROW/2);


	if (COLUMN > 20000)
	{
		std::cout << "line graph too large. Sorry." << std::endl;
		std::cout << "line is " << COLUMN << std::endl;
		return;
	}
	std::cout << "line is " << COLUMN << std::endl;

	int fileSize = 54 + 3 * ROW * COLUMN;
	int imageSize = ROW * COLUMN;
	double ppm = RESOLUTION * 39.37008; // resolution in pixels per meter
	unsigned char write;
	std::vector <short> data;

	std::ofstream outImage(fileName, std::ios::out | std::ios::trunc | std::ios::binary);

	outImage.put('B').put('M');

	outImage << lwrite(54 + ROW * COLUMN * 3, 4);  // bfSize
	outImage << lwrite(0, 2);  // bfReserved1
	outImage << lwrite(0, 2);  // bfReserved2
	outImage << lwrite(54, 4);  // bfOffBits

	// Write the BITMAPINFOHEADER
	outImage << lwrite(40, 4);  // biSize
	outImage << lwrite(COLUMN, 4);  // biWidth
	outImage << lwrite(ROW, 4);  // biHeight
	outImage << lwrite(1, 2);  // biPlanes
	outImage << lwrite(24, 2);  // biBitCount
	outImage << lwrite(0, 4);  // biCompression=BI_RGB
	outImage << lwrite(ROW * COLUMN * 3, 4);  // biSizeImage
	outImage << lwrite(ppm, 4);  // biXPelsPerMeter
	outImage << lwrite(ppm, 4);  // biYPelsPerMeter
	outImage << lwrite(0, 4);  // biClrUsed
	outImage << lwrite(0, 4);  // biClrImportant

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (i == .5 * ROW || ((lineModified.size() > j) && (lineModified[j] == i)))
			{
				outImage.put(215);
				outImage.put(21);
				outImage.put(2);
			}
			else
			{				
				outImage.put(210);
				outImage.put(215);
				outImage.put(250);
			}
		}
	}

	std::cout << "Done making a bitmap file" << std::endl;
	outImage.close();
}

