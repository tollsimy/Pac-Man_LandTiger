#include "pacman.h"

static int currentNote = -1;

void play_melody_note(NOTE melody[], int lenght){
	
	if(currentNote >= 0){
		if(!isNotePlaying())
		{
			playNote(melody[currentNote++]);
		}
		// disable melody when finished
		if(currentNote == lenght)
		{
			currentNote = -1;
		}
	}
}

// enable melody (again)
void enable_melody(){
	currentNote = 0;
}