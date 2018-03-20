#pragma once
#include <math.h>
#include <time.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <exception>
#include <time.h>
#include <random>

#ifndef PIT
#define PIT (3.141592654 * 2)
#endif

class cNote
{
public:
	cNote();
	cNote(std::vector<double> freq, double volume, int _globalTopAmp, std::string wave_type);
	~cNote();
	int slice(int position);
	int strangeSlice(int position, double percnt, int slices);
	//int sliceProduct(int position); // Experimental method for later use

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
	void set_noise(double value);

	void display();	

private:
	int global_TopAmp = 32767; // depends on the bit if it's 8 or 16
	double global_amplitude = 0.0; // between 0  and 1 only
	std::vector<double> frequency;
	double global_numeratorExp = 0.0; // must be int at 0 or more
	int global_denomonatorExp = 0; // must be int at 1 or more
	int global_oscillator = 1; // int -1 or 1
	int mult_k_mult = 1; // + int
	int mult_add = 0; // int
	int div_k_mult = 1; // int 1 or greater - divide by zero error possible otherwise
	int div_add = 0; // int 0 or bigger
	int div_exp = 1; // int 0 or bigger
	double rev_negator = 1.0;  // 1 or -1 only - it's positive or negative
	int repeats = 10; // int 0 or bigger
	double noise = 0.0;
	double seed = time(NULL);

	int sampleRate = 44100;
	std::vector<double>::iterator freqIt;
	
};
