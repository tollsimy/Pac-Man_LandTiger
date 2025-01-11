#include "../common.h"
#include "../music/music.h"
#include "../RIT/RIT.h"

//TODO
NOTE pacman_song[] = {
	{g4, time_croma},
};

int currentNote = 0;

void play_melody_note(){
	if(!isNotePlaying())
	{
		playNote(pacman_song[currentNote++]);
	}
	if(currentNote == (sizeof(pacman_song) / sizeof(pacman_song[0])))
	{
		currentNote = 0;
	}
}