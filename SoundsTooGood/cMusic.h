#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "cMakeNote.h"

class cMusic
{
public:
	cMusic(int _sampleRate, int _topAmp, double _targetVolume = .8);
	cMusic(int _sampleRate, double _targetVolume = .8);
	~cMusic();

	void set_sampleRate(int value);
	void set_topAmp(int value);
	void add_note(double position, std::vector<int>note);
	void add_note(std::vector<double> _freq, double _seconds, double _volume, std::string _waveType, double _position, bool _strange = false);
	void add_note(std::vector<std::string> _note_names, double _seconds, double _volume, std::string _waveType, double _position, bool _strange = false);
	void add_note(std::vector<double> _freq, double _seconds, double _volume, std::string _waveType, std::vector<double> _positions, bool _strange = false);
	void add_note(std::vector<std::string> _note_names, double _seconds, double _volume, std::string _waveType, std::vector<double> _positions, bool _strange = false);
	void set_strange_percent(double value);
	std::vector<int>* buildMusic();

private:
	int sampleRate;
	int topAmp = 32767;
	std::multimap<double, std::vector<int>>* sounds;
	std::multimap<double, std::vector<int>>::iterator soundsIt;
	std::vector<int>* final_result;
	std::vector<int>::iterator vectorIt;
	std::vector<std::vector<int>> note_vector;
	double lowest = 0;
	double highest = 0;
	int loudest = 0;
	double targetVolume = 0;
	cMakeNote* note;
	bool strange = false;
	double strange_percent = 0.95;

	double get_frequency(std::string _note_name);
};

