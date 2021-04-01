
#include "rnb.h"
using namespace std;
Sample samp1 = Sample("Verse1.wav", 0, true); //true will make it repeat ad infinitum
Sample samp2 = Sample("DooDooDo.wav", 1, true);//Sample plays on chan 1

int main()      //There are 32 channels 0-31 - only 32 sounds can play at once
{
    samp1.play();
    samp2.play();
    samp2.set_speed(.90); //plays sample at 90% speed - negative values play backwards
    samp1.add_bass_mid_treble_ctrl();
    samp2.add_bass_mid_treble_ctrl();
    samp1.set_speed(.90);
    samp1.set_bass(2.0); //adds 2db to bass
    samp2.set_highs(-80.0);
    samp1.set_mids(10); // adds 10db to mids
    samp2.set_mids(10);
    samp1.set_volume(.9); //sets sample to 90% volume
    samp1.add_flange();
	//samp1.add_echo();  //adds echo effect
	
    
	while (true)
	{
		samp1.upd(); //updates the audio engine so it can apply changes that you make to the sound - needs to be called frequently
        sleep(50);   //so that the audio plays smoothly
	}

}
