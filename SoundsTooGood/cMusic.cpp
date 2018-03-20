#include "stdafx.h"
#include "cMusic.h"


cMusic::cMusic(int _sampleRate, int _topAmp, double _targetVolume)
{
	targetVolume = _targetVolume;
	sounds = new std::multimap<double, std::vector<int>>;
	final_result = new std::vector<int>;
	set_sampleRate(_sampleRate);
	set_topAmp(_topAmp);
	highest = 0;
}

cMusic::cMusic(int _sampleRate, double _targetVolume)
{
	targetVolume = _targetVolume;
	sounds = new std::multimap<double, std::vector<int>>;
	final_result = new std::vector<int>;
	set_sampleRate(_sampleRate);
}

cMusic::~cMusic()
{
	if (sounds)
		delete sounds;
	if (final_result)
		delete final_result;
}

void cMusic::set_sampleRate(int value)
{
	if (value < 1)
		sampleRate = 1;
	else if (value > 45000)
		sampleRate = 44100;
	else
		sampleRate = value;
}

void cMusic::set_topAmp(int value)
{
	if (value < 0)
		topAmp = 0;
	else
		topAmp = value;
}

void cMusic::add_note(double position, std::vector<int>note)
{
	if (position < 0)
		return;
	else
	{
		sounds->insert(std::pair<double, std::vector<int>>(position, note));
		if (position < lowest)
			lowest = position;
		else if (position > highest)
			highest = position;
	}
}

void cMusic::add_note(std::vector<double> _freq, double _seconds, double _volume, std::string _waveType, double _position, bool _strange)
{
	strange = _strange;
	std::vector<int> temp_notes_vector;
	note = new cMakeNote(sampleRate, _freq, _seconds, _volume, _waveType);
	note_vector.clear();
	
	if (strange)
		temp_notes_vector = note->buildStrangeSong(strange_percent);
	else
		temp_notes_vector = note->buildSong();

	add_note(_position, temp_notes_vector);
	note_vector.push_back(temp_notes_vector);

	delete note;

	if (_waveType == "piano")
	{
		std::vector<double> _freq1;
		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 1.5);
			_freq1.push_back(_freq[i] - 1.5);
		}
		/*
		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .5), _waveType);

		//if (strange)
		//	add_note(_position, note->buildStrangeSong(strange_percent));
		//else
		//	add_note(_position, note->buildSong());
		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;
		
		_freq1.clear();
		*/
		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 5.0);
			_freq1.push_back(_freq[i] - 5.0);
		}

		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .25), _waveType);

		//if (strange)
		//	add_note(_position, note->buildStrangeSong(strange_percent));
		//else
		//	add_note(_position, note->buildSong());

		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;
	}
	else if (_waveType == "bass")
	{
		/*
		std::vector<double> _freq1;
		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 3.5);
			_freq1.push_back(_freq[i] - 3.5);
		}

		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .8), _waveType);

		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;
		
		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 5.5);
			_freq1.push_back(_freq[i] - 5.5);
		}
		
		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .85), _waveType);

		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;
		*/
	}
	else if (_waveType == "violin")
	{
		std::vector<double> _freq1;
		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 1.5);
			_freq1.push_back(_freq[i] - 1.5);
		}

		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .15), _waveType);

		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;

		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 5.0);
			_freq1.push_back(_freq[i] - 5.0);
		}

		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .25), _waveType);

		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;
	}
	else if (_waveType == "bell")
	{
		std::vector<double> _freq1;
		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 3.5);
			_freq1.push_back(_freq[i] - 3.5);
		}

		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .55), _waveType);

		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;

		for (int i = 0; i < _freq.size(); i++)
		{
			_freq1.push_back(_freq[i] + 4.5);
			_freq1.push_back(_freq[i] - 4.5);
		}

		note = new cMakeNote(sampleRate, _freq1, _seconds, (_volume * .35), _waveType);

		if (strange)
			temp_notes_vector = note->buildStrangeSong(strange_percent);
		else
			temp_notes_vector = note->buildSong();

		add_note(_position, temp_notes_vector);
		note_vector.push_back(temp_notes_vector);

		delete note;
	}

	/*
	else
	{
		note = new cMakeNote(sampleRate, _freq, _seconds, (_volume * .5), _waveType);

		if (strange)
			add_note(_position, note->buildStrangeSong(strange_percent));
		else
			add_note(_position, note->buildSong());

		delete note;
	}
	*/
}

void cMusic::add_note(std::vector<double> _freq, double _seconds, double _volume, std::string _waveType, std::vector<double> _positions, bool _strange)
{
	int i = 0;
	int j = 0;
	if (_positions.size() < 1)
		return;

	add_note(_freq, _seconds, _volume, _waveType, _positions[0], _strange);

	for (i = 1; i < _positions.size(); i++)
	{
		for (j = 0; j < note_vector.size(); j++)
		{
			add_note(_positions[i], note_vector[j]);
		}		
	}
}

void cMusic::add_note(std::vector<std::string> _note_names, double _seconds, double _volume, std::string _waveType, double _position, bool _strange)
{
	std::vector<double> freqs;
	std::vector<std::string>::iterator _note_names_it;

	for (_note_names_it = _note_names.begin(); _note_names_it < _note_names.end(); _note_names_it++)
		freqs.push_back(get_frequency(*_note_names_it));

	add_note(freqs, _seconds, _volume, _waveType, _position, _strange);
}

void cMusic::add_note(std::vector<std::string> _note_names, double _seconds, double _volume, std::string _waveType, std::vector<double> _positions, bool _strange)
{
	std::vector<double> freqs;
	std::vector<std::string>::iterator _note_names_it;

	for (_note_names_it = _note_names.begin(); _note_names_it < _note_names.end(); _note_names_it++)
		freqs.push_back(get_frequency(*_note_names_it));

	add_note(freqs, _seconds, _volume, _waveType, _positions, _strange);
}



std::vector<int>* cMusic::buildMusic()
{
	int j = 0;
	double calc;

	for (soundsIt = sounds->lower_bound(lowest); soundsIt != sounds->upper_bound(highest); soundsIt++)
	{
		int place = (int)((*soundsIt).first * sampleRate);

		for (vectorIt = (*soundsIt).second.begin(); vectorIt < (*soundsIt).second.end(); vectorIt++)
		{
			if (place < final_result->size())
			{
				(*final_result)[place] += *vectorIt;
				if ((*final_result)[place] > loudest)
				{
					loudest = (*final_result)[place];
				}

				place++;
			}
			else
			{
				while (place > final_result->size())
				{
					final_result->push_back(0);
				}
				final_result->push_back(*vectorIt);
				if (*vectorIt > loudest)
					loudest = *vectorIt;
				place++;
			}
		}
		(*soundsIt).second.clear(); // drop data in element to save on space
	}

	calc = ((double)topAmp * targetVolume) / (double)loudest;

	for (j = 0; j < final_result->size(); j++)
	{
		(*final_result)[j] *= calc;
	}

	return final_result;
}

double cMusic::get_frequency(std::string _note_name)
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

void cMusic::set_strange_percent(double value)
{
	strange_percent = value;
}