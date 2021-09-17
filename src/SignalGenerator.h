/* date = September 14th 2021 7:17 pm */
#define _USE_MATH_DEFINES

#ifndef _SIGNAL_GENERATOR_H
#define _SIGNAL_GENERATOR_H

#include <cmath>

class SignalGenerator
{
    public:
    SignalGenerator(){
        localSampleRate = 1;
        waveform = 1;
    }
    
	SignalGenerator(double sampleRate, int wave) {
		localSampleRate = sampleRate;
		waveform = wave;
	}
    
	void freqToAngle(double frequency) {
		auto cyclesPerSecond = frequency;
		auto cyclesPerSample = cyclesPerSecond / localSampleRate;
        
		angleDelta = cyclesPerSample * 2.0 * pi;
        
		currentAngle += angleDelta;
	}
    
	float oscCycleWithFreq( double frequency, double level) {
        
		freqToAngle(frequency);
        
		switch (waveform) {
            // Sin
            case 1:
            {
                value = level * sin(currentAngle);
                break;
            }
            // Triangle
            case 2:
			sineValue = sin(currentAngle);
            
			value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
			break;
            // Square
            case 3:
			sineValue = sin(currentAngle);
            
			value = (sineValue >= 0) ? 1 : -1;
			break;
		}
        
		return value;
	}
    
	float oscCycleWithAngle(double currentAngle, double level) {
		
		switch (waveform) {
            // Sin
            case 1:
            {
                value = level * sin(currentAngle);
                break;
            }
            // Triangle
            case 2:
			sineValue = sin(currentAngle);
            
			value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
			value = level * value;
			break;
            // Square
            case 3:
			sineValue = sin(currentAngle);
            
			value = (sineValue >= 0) ? 1 : -1;
			value *= level;
			break;
		}
        
		return value;
	}
    
	void setWaveform(int waveshape) {
		waveform = waveshape;
	}
    
    void setSampleRate(double samplerate){
        localSampleRate = samplerate;
    }
    
    private:
	float currentAngle = 1;
	float angleDelta;
	double localSampleRate;
	int waveform;
	float value = 1;
    
	float sineValue = 0;
	float triangleDelta = 0.1;
    
    const double pi = 2 * acos(0.0);
};

#endif //_SIGNAL_GENERATOR_H
