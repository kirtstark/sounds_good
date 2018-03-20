#include "stdafx.h"
#include "cNote.h"


cNote::cNote()
{
}

cNote::cNote(std::vector<double> freq, double volume, int _globalTopAmp, std::string wave_type)
{
	global_TopAmp = _globalTopAmp;
	set_frequency(freq);
	set_global_amplitude(volume);

	if (wave_type == "square")
	{
		set_global_numeratorExp(2);
		set_global_denomonatorExp(1);
		set_global_oscillator(1);
		set_mult_k_mult(2);
		set_mult_add(-1);
		set_div_k_mult(2);
		set_div_add(-1);
		set_div_exp(1);
		set_rev_negator(true);
	}

	else if (wave_type == "saw_tooth")
	{
		set_global_numeratorExp(1);
		set_global_denomonatorExp(1);
		set_global_oscillator(-1);
		set_mult_k_mult(1);
		set_mult_add(0);
		set_div_k_mult(1);
		set_div_add(0);
		set_div_exp(1);
		set_rev_negator(false);
	}

	else if (wave_type == "reverse_saw_tooth")
	{
		set_global_numeratorExp(1);
		set_global_denomonatorExp(1);
		set_global_oscillator(-1);
		set_mult_k_mult(1);
		set_mult_add(0);
		set_div_k_mult(1);
		set_div_add(0);
		set_div_exp(1);
		set_rev_negator(true);
	}

	else if (wave_type == "triangle")
	{
		set_global_numeratorExp(5.5);
		set_global_denomonatorExp(2);
		set_global_oscillator(-1);
		set_mult_k_mult(2);
		set_mult_add(-1);
		set_div_k_mult(2);
		set_div_add(1);
		set_div_exp(2);
		set_rev_negator(true);
	}

	else if (wave_type == "sine")
	{		
		set_global_numeratorExp(0.0);
		set_global_denomonatorExp(0);
		set_global_oscillator(1);
		set_mult_k_mult(1);
		set_mult_add(0);
		set_div_k_mult(1);
		set_div_add(0);
		set_div_exp(0);
		set_rev_negator(true);
		set_repeats(1);		
	}

	else
		std::cout << "\nUsing default values, string input not recognized" << std::endl;
}

cNote::~cNote()
{
}

int cNote::slice(int position)
{
	double result = 0;
	unsigned j = 0;

	try{
		if (sampleRate < 1)
			throw 1;

		//noise = .9;

		srand(seed += 1.3);
		double mult = (double)position * PIT / (double)sampleRate;

		double theNoise = (noise * (((rand() % 100) / 100.0)) + 1.0 - noise);

		//std::cout << theNoise << std::endl;

		double volume = (double)global_TopAmp * global_amplitude * theNoise;

		for (freqIt = frequency.begin(); freqIt < frequency.end(); freqIt++)
		{

			for (j = 1; j <= repeats; j++)
			{
				result += (pow((double)global_oscillator, (double)j)) * sin(*freqIt * mult * ((double)(mult_k_mult * j + mult_add))) / pow((double)(div_k_mult * j + div_add), (double)div_exp);
			}

		}

		result = (result * volume * rev_negator * (pow(2.0, global_numeratorExp) / (pow(3.141592654, global_denomonatorExp))));
		
	}
	catch (std::exception& e)
	{
		std::cout << "ERROR - exception thrown in cNote.cpp method slice, error: " << e.what() << std::endl;
		return 0;
	}
	catch (int e)
	{
		std::cout << "ERROR - exception thrown in cNote.cpp method slice, error number: " << e << std::endl;
		return 0;
	}
	catch (...)
	{
		std::cout << "ERROR - exception thrown in cNote.cpp method slice, error unknown" << std::endl;
		return 0;
	}
	return (int) result;
}

int cNote::strangeSlice(int position, double percnt, int slices)
{
	srand(seed += 1.7);
	double result = 0;
	double denomonator = 1.0;
	unsigned j = 0;

	try{
		if (slices < 1 || sampleRate < 1)
			throw 2;
		
		double volume = (double)global_TopAmp * global_amplitude + noise * (rand() % 100) / 100;

		for (freqIt = frequency.begin(); freqIt < frequency.end(); freqIt++)
		{
			if (slices == 0)
				throw 1;

			double freqNow = *freqIt * (1.0 - (1.0 - percnt) * ((double)(position - 1) / (double)slices));
			double mult = freqNow * (double)position * PIT / (double)sampleRate;

			for (j = 1; j <= repeats; j++)
			{
				denomonator = pow((double)(div_k_mult * j + div_add), (double)div_exp);

				if (denomonator < .000001 && denomonator > -.000001)
					throw 1;

				result += (pow((double)global_oscillator, (double)j)) * sin(mult * ((double)(mult_k_mult * j + mult_add))) / denomonator;
			}
		}

		result = (result * volume * rev_negator * (pow(2.0, global_numeratorExp) / (pow(3.141592654, global_denomonatorExp))));
	}
	catch (std::exception& e)
	{
		std::cout << "ERROR - exception thrown in cNote.cpp method strangeSlice, error: " << e.what() << std::endl;
		return 0;
	}
	catch (int e)
	{
		std::cout << "ERROR - exception thrown in cNote.cpp method slice, error number: " << e << std::endl;
		return 0;
	}
	catch (...)
	{
		std::cout << "ERROR - exception thrown in cNote.cpp method strangeSlice, error unknown" << std::endl;
		return 0;
	}
	return (int)result;
}

/*
// Experimental method for future playing around
int cNote::sliceProduct(int position)
{
	srand(time(NULL));
	double noise = .03;
	double mess = noise * (rand() % 100) / 100;

	double result = 1;
	unsigned j = 0;
	double mult = (double)position * PIT * frequency / (double)sampleRate;
	double volume = (double)global_TopAmp * global_amplitude;

	for (j = 1; j <= repeats; j++)
	{
		result *= (pow((double)global_oscillator, (double)j)) * sin(mult * ((double)(mult_k_mult * j + mult_add))) / pow((double)(div_k_mult * j + div_add), (double)div_exp);
	}
	std::cout << "result is " << result << std::endl;
	result = (int)(result * volume * rev_negator * (pow(2.0, global_numeratorExp) / (pow(3.141592654, global_denomonatorExp))));
	
	if (result > global_TopAmp)
		return global_TopAmp;
	else
		return (int)result;
}
*/

void cNote::set_global_TopAmp(int value)
{
	if (value < 127)
		global_TopAmp = 127;
	else if (value > 32767)
		global_TopAmp = 32767;
	else
		global_TopAmp = value;
}

void cNote::set_global_amplitude(double value)
{
	if (value > 1.0)
		global_amplitude = 1.0;
	else if (value < 0.0)
		global_amplitude = 0.0;
	else
		global_amplitude = value;
}

void cNote::set_frequency(std::vector<double> value)
{
	for (freqIt = value.begin(); freqIt < value.end(); freqIt++)
	{
		if (*freqIt > 0)
			frequency.push_back(*freqIt);
	}
}

void cNote::set_global_numeratorExp(int value)
{
	if (value < 0)
		global_numeratorExp = 0;
	else
		global_numeratorExp = value;
}

void cNote::set_global_denomonatorExp(int value)
{
	if (value < 0)
		global_denomonatorExp = 0;
	else
		global_denomonatorExp = value;
}

void cNote::set_global_oscillator(int value)
{
	if (value != 1 && value != -1)
		global_oscillator = 1;
	else
		global_oscillator = value;
}

void cNote::set_mult_k_mult(int value)
{
	if (value < 0)
		mult_k_mult = 0;
	else
		mult_k_mult = value;
}

void cNote::set_mult_add(int value)
{
	mult_add = value;
}

void cNote::set_div_k_mult(int value)
{
	if (value < 1)
		div_k_mult = 1;
	else
		div_k_mult = value;
}

void cNote::set_div_add(int value)
{
	if (value < 0)
		div_add = 0;
	else
		div_add = value;
}

void cNote::set_div_exp(int value)
{
	if (value < 0)
		div_exp = 0;
	else
		div_exp = value;
}

void cNote::set_rev_negator(bool value)
{
	if (!value)
		rev_negator = -1.0;
	else
		rev_negator = 1.0;
}

void cNote::set_repeats(int value)
{
	if (value < 1)
		repeats = 1;
	else if (value > 10)
		repeats = 10;
	else
		repeats = value;
}

void cNote::set_sampleRate(int value)
{
	if (value < 1)
		sampleRate = 1;
	else if (value > 45000)
		sampleRate = 44100;
	else
		sampleRate = value;
}

void cNote::set_noise(double value)
{
	if (value > 0 && value <= 1.0)
		noise = value;
	else
		noise = 0;
}

void cNote::display()
{
	std::cout << "\n\n****************************************************************" << std::endl;
	std::cout << "List of class values:\n" << std::endl;

	std::cout << "global_TopAmp:   " << global_TopAmp << std::endl;
	std::cout << "global_amplitude:   " << global_amplitude << std::endl;
	std::cout << "\nFrequencies:" << std::endl;
	for (freqIt = frequency.begin(); freqIt < frequency.end(); freqIt++)
		std::cout << "frequency:   " << *freqIt << std::endl;

	std::cout << "\nglobal_numeratorExp:   " << global_numeratorExp << std::endl;
	std::cout << "global_denomonatorExp:   " << global_denomonatorExp << std::endl;
	std::cout << "global_oscillator:   " << global_oscillator << std::endl;
	std::cout << "mult_k_mult:   " << mult_k_mult << std::endl;
	std::cout << "mult_add:   " << mult_add << std::endl;
	std::cout << "div_k_mult:   " << div_k_mult << std::endl;
	std::cout << "div_add:   " << div_add << std::endl;
	std::cout << "div_exp:   " << div_exp << std::endl;
	std::cout << "rev_negator:   " << rev_negator << std::endl;
	std::cout << "repeats:   " << repeats << std::endl;
	std::cout << "sampleRate:   " << sampleRate << std::endl;

	std::cout << "\n****************************************************************" << std::endl;
}
