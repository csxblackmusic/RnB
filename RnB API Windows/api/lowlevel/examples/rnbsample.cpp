#include "rnb.h"
using namespace std;                          //The Sample object is used to load play and manipulate digital audio (mp3,wav,ogg)
Sample samp1 = Sample("Verse1.wav", 0, true); //The first argument is the name of audio file you'd like to play
Sample samp2 = Sample("DooDooDo.wav", 1, true);//The second argument places the audio on an effects channel
                                               //The third argument is a flag that will loop the audio when true
                                               //by default the flag is false
                                               //There are 32 channels for effects (0-31)
                                               //One sound can play on a channel at a time
int main()
{
    samp1.play(); //Plays the audio and allows you to add effects
    samp2.play();
    samp2.set_speed(.90); //plays sample at 90% speed - negative values play backwards
    samp1.add_bass_mid_treble_ctrl();
    samp2.add_bass_mid_treble_ctrl();
    samp1.set_speed(.90);
    samp1.set_bass(2.0); //adds 2db to bass
    samp2.set_highs(-80.0);//subtracts 80db from highs
    samp1.set_mids(10); // adds 10db to mids
    samp2.set_mids(10);
    samp1.set_volume(.90); //sets sample to 95% volume
    samp1.add_flange();
    //samp1.add_echo();  //adds echo effect


    while (true)
    {
        samp1.upd(); //updates the audio engine so it can apply changes that you make to the sound - needs to be called frequently
        Sleep(50);   //so that the audio plays smoothly
    }

}
