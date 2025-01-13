#ifndef MUSIC_H
#define MUSIC_H

#include "../timer/timer.h"

#define TIMER_FREQUENCY 25000000  // 25 MHz (timer frequency)
#define NOTE_DIVISOR 45  					// Divisor for note frequency calculation

#define TIMERSCALER 1							// Timer scaler for frequency adjustments

#define SECOND (TIM_MS_TO_TICKS_SIMPLE(1000) * TIMERSCALER)  // Constant representing one second

#ifdef SIMULATOR
    // In simulation mode, these values are adjusted to produce a "familiar" sound 
    // for the human ear when played through the buzzer. This ensures the sound 
    // resembles what it would sound like on the real hardware.
    #define SPEEDUP        1.4f    // Increase the speed to adjust timing for simulation
    #define AMPLIFIER      2       // Amplify the sound for better simulation
#else
    // Default values for real hardware playback
    #define SPEEDUP        1.0f    // Normal speed
    #define AMPLIFIER      1       // No amplification
#endif

// Macro to calculate the frequency of a note /* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */
#define NOTE_ENTRY(note) (TIMER_FREQUENCY / ((note) * NOTE_DIVISOR))

// Enumeration of note durations (lengths of time for each note)
typedef enum note_durations
{
	time_128th 	  = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5),	    // 1/128 (duration for a 1/128 note)
	time_64th  	  = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5),     // 1/64 (duration for a 1/64 note)
	time_32nd  	  = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5),     // 1/32 (duration for a 1/32 note)
	time_16th  	  = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5),      // 1/16 (duration for a 1/16 note)
	time_8th   	  = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5),     	// 1/4 (duration for a 1/4 note)
	time_4th   	  = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5),      // 1/2 (duration for a 1/2 note)
	time_whole 	  = (unsigned int)(SECOND * SPEEDUP + 0.5),             // 1 (duration for a whole note)
	time_double_whole = (unsigned int)(SECOND * SPEEDUP * 2.0f + 0.5),	// 2 beats (duration for a double whole note)

	// Dotted notes (duration increased by a quarter)
	time_double_whole_dotted = (unsigned int)(SECOND * SPEEDUP * 2.25f + 0.5),	// 2 beats + 1/4
	time_whole_dotted 	 = (unsigned int)(SECOND * SPEEDUP * 1.125f + 0.5),   	// 1 beat + 1/8
	time_8th_dotted 	 = (unsigned int)(SECOND * SPEEDUP * 0.625f + 0.5),     // 1/2 beat + 1/16
	time_16th_dotted 	 = (unsigned int)(SECOND * SPEEDUP * 0.375f + 0.5),     // 1/4 beat + 1/32
	time_32nd_dotted	 = (unsigned int)(SECOND * SPEEDUP * 0.25f + 0.5),  	// 1/8 beat + 1/64
	time_64th_dotted 	 = (unsigned int)(SECOND * SPEEDUP * 0.1875f + 0.5),    // 1/16 beat + 1/128
} NOTE_DURATION;


// Enumeration of frequencies for various musical notes (B0 to C8)
typedef enum frequencies
{
	NOTE_B0  = NOTE_ENTRY(31),
	NOTE_C1  = NOTE_ENTRY(33),
	NOTE_CS1 = NOTE_ENTRY(35),
	NOTE_D1  = NOTE_ENTRY(37),
	NOTE_DS1 = NOTE_ENTRY(39),
	NOTE_E1  = NOTE_ENTRY(41),
	NOTE_F1  = NOTE_ENTRY(44),
	NOTE_FS1 = NOTE_ENTRY(46),
	NOTE_G1  = NOTE_ENTRY(49),
	NOTE_GS1 = NOTE_ENTRY(52),
	NOTE_A1  = NOTE_ENTRY(55),
	NOTE_AS1 = NOTE_ENTRY(58),
	NOTE_B1  = NOTE_ENTRY(62),
	NOTE_C2  = NOTE_ENTRY(65),
	NOTE_CS2 = NOTE_ENTRY(69),
	NOTE_D2  = NOTE_ENTRY(73),
	NOTE_DS2 = NOTE_ENTRY(78),
	NOTE_E2  = NOTE_ENTRY(82),
	NOTE_F2  = NOTE_ENTRY(87),
	NOTE_FS2 = NOTE_ENTRY(93),
	NOTE_G2  = NOTE_ENTRY(98),
	NOTE_GS2 = NOTE_ENTRY(104),
	NOTE_A2  = NOTE_ENTRY(110),
	NOTE_AS2 = NOTE_ENTRY(117),
	NOTE_B2  = NOTE_ENTRY(123),
	NOTE_C3B = NOTE_ENTRY(127),
	NOTE_C3  = NOTE_ENTRY(131),
	NOTE_CS3 = NOTE_ENTRY(139),
	NOTE_D3  = NOTE_ENTRY(147),
	NOTE_DS3 = NOTE_ENTRY(156),
	NOTE_E3  = NOTE_ENTRY(165),
	NOTE_F3  = NOTE_ENTRY(175),
	NOTE_FS3 = NOTE_ENTRY(185),
	NOTE_G3  = NOTE_ENTRY(196),
	NOTE_GS3 = NOTE_ENTRY(208),
	NOTE_A3  = NOTE_ENTRY(220),
	NOTE_AS3 = NOTE_ENTRY(233),
	NOTE_B3  = NOTE_ENTRY(247),
	NOTE_C4  = NOTE_ENTRY(262),
	NOTE_CS4 = NOTE_ENTRY(277),
	NOTE_D4  = NOTE_ENTRY(294),
	NOTE_DS4 = NOTE_ENTRY(311),
	NOTE_E4  = NOTE_ENTRY(330),
	NOTE_F4  = NOTE_ENTRY(349),
	NOTE_FS4 = NOTE_ENTRY(370),
	NOTE_G4  = NOTE_ENTRY(392),
	NOTE_GS4 = NOTE_ENTRY(415),
	NOTE_A4  = NOTE_ENTRY(440),
	NOTE_AS4 = NOTE_ENTRY(466),
	NOTE_B4  = NOTE_ENTRY(494),
	NOTE_C5  = NOTE_ENTRY(523),
	NOTE_CS5 = NOTE_ENTRY(554),
	NOTE_D5  = NOTE_ENTRY(587),
	NOTE_DS5 = NOTE_ENTRY(622),
	NOTE_E5  = NOTE_ENTRY(659),
	NOTE_F5  = NOTE_ENTRY(698),
	NOTE_FS5 = NOTE_ENTRY(740),
	NOTE_G5  = NOTE_ENTRY(784),
	NOTE_GS5 = NOTE_ENTRY(831),
	NOTE_A5  = NOTE_ENTRY(880),
	NOTE_AS5 = NOTE_ENTRY(932),
	NOTE_B5  = NOTE_ENTRY(988),
	NOTE_C6  = NOTE_ENTRY(1047),
	NOTE_CS6 = NOTE_ENTRY(1109),
	NOTE_D6  = NOTE_ENTRY(1175),
	NOTE_DS6 = NOTE_ENTRY(1245),
	NOTE_E6  = NOTE_ENTRY(1319),
	NOTE_F6  = NOTE_ENTRY(1397),
	NOTE_FS6 = NOTE_ENTRY(1480),
	NOTE_G6  = NOTE_ENTRY(1568),
	NOTE_GS6 = NOTE_ENTRY(1661),
	NOTE_A6  = NOTE_ENTRY(1760),
	NOTE_AS6 = NOTE_ENTRY(1865),
	NOTE_B6  = NOTE_ENTRY(1976),
	NOTE_C7  = NOTE_ENTRY(2093),
	NOTE_CS7 = NOTE_ENTRY(2217),
	NOTE_D7  = NOTE_ENTRY(2349),
	NOTE_DS7 = NOTE_ENTRY(2489),
	NOTE_E7  = NOTE_ENTRY(2637),
	NOTE_F7  = NOTE_ENTRY(2794),
	NOTE_FS7 = NOTE_ENTRY(2960),
	NOTE_G7  = NOTE_ENTRY(3136),
	NOTE_GS7 = NOTE_ENTRY(3322),
	NOTE_A7  = NOTE_ENTRY(3520),
	NOTE_AS7 = NOTE_ENTRY(3729),
	NOTE_B7  = NOTE_ENTRY(3951),
	NOTE_C8	 = NOTE_ENTRY(4186),
	NOTE_CS8 = NOTE_ENTRY(4435),
	NOTE_D8	 = NOTE_ENTRY(4699),
	NOTE_DS8 = NOTE_ENTRY(4978),

	REST = 0	// DO NOT SOUND
} FREQUENCY;

// Structure to represent a note with frequency and duration
typedef struct 
{
	FREQUENCY freq;						// The frequency of the note
	NOTE_DURATION duration;		// The duration of the note
} NOTE;

// Function to play a note (with frequency and duration)
void playNote(NOTE note);

// Function to check if a note is currently being played
char isNotePlaying(void);

#endif