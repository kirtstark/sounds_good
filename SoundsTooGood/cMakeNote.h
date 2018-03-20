#pragma once
#include "cNote.h"
#include <math.h>
#include <fstream>
#include "lwrite.h"

class cMakeNote
{
public:
	//static cMakeNote init(int _sampleRate, std::vector<double> _freq, int _seconds, double _volume, std::string _waveType);
	//static cMakeNote init(int _sampleRate, std::vector<std::string> _note_names, int _seconds, double _volume, std::string _waveType);
	cMakeNote(int _sampleRate, std::vector<double> _freq, double _seconds, double _volume, std::string _waveType);
	cMakeNote(int _sampleRate, std::vector<std::string> _note_names, double _seconds, double _volume, std::string _waveType);
	cMakeNote(){}
	~cMakeNote();
	std::vector<int> buildSong();
	std::vector<int> buildStrangeSong(double percnt);
	std::vector<int> buildSound();
	bool makeGraph = true;
	

	// control over cNote values
	void set_global_TopAmp(int value);
	void set_global_amplitude(double value);
	void set_frequency(std::vector<double> value);
	void set_global_numeratorExp(int value);
	void set_global_denomonatorExp(int value);
	void set_global_oscillator(int value);
	void set_mult_k_mult(int value);
	void set_mult_add(int value);
	void set_div_k_mult(int value);
	void set_div_add(int value);
	void set_div_exp(int value);
	void set_rev_negator(bool value);
	void set_repeats(int value);
	void set_sampleRate(int value);
	void set_reverb(double value);
	void set_reverbAmp(double value);
	void set_volume(double value);
	void set_noise(double value);
	void set_freq_add(double value);
	void display();
	

private:
	//cMakeNote(int _sampleRate, std::vector<double> _freq, int _seconds, double _volume, std::string _waveType);
	void init();
	int global_TopAmp = 32767; // depends on the bit if it's 8 or 16
	int sampleRate; // Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz
	std::vector<double> freqs;
	std::vector<double> freq_add;
	double sec;
	double volume; // range from 0 to 1
	std::string waveType;
	
	double bellCurve(int position);
	double tripleBellCurve(int position);
	double expCurve(int position);
	double lnBellCurve(int position);
	double noEffect(int position);
	double(cMakeNote::*envelope_ptr)(int);
	void build_graph(std::vector<int> line);

	cNote *note;

	// envelope structures
	int mean;
	int standardDeviation;
	double percentage = .5;

	int mean2;
	int standardDeviation2;
	double percentage2;

	int mean3;
	int standardDeviation3;
	double percentage3;

	double lnMean; // set value to ln of desired position, or ln(desired seconds * sampleRate)
	double lnStandardDeviation; // works well at 1 - percentage of lnMean position. If lnMean is set to be 30% into the total run, make lnsd .7
	double lnPercentage;
	double reverb = 0.0; // measured in length in seconds of each reverberation, 2 is slow reverb, .1 is fast
	double reverbAmp = .1;
	double get_frequency(std::string _note_name);

	double dropOff = -1000; // larger numbers means slower dropoff of notes - CANNOT BE 0 OR NOTHING EVER COMPLETES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
};

