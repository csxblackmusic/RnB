#include "rnb.h"
#include <vector>
#include <iterator>
#include <deque>
#include <stack>
using namespace std;

/**
 *
 *
 *
 PASTE YOUR SPOTIFY COVER CLASS AND HERE
 Make sure its uncommented
 *
 *
 */

SpotifyCover rnb;//no need to change this line

void phone_interface(const string address,vector<float>data) //no need to change this function
{
    if (address=="/1/push4" and data[0]==1) //If we push the top green button play playlist
    {
        rnb.play_playlist();
    }
    else if (address=="/1/push1" and data[0]==1) //If we push the bottom red button go back
    {
        rnb.back();
    }
    else if (address=="/1/push2" and data[0]==1) //If we push the top red button go forward
    {
        rnb.forward();
    }
    
    else if(address=="/1/push3" and data[0]==1)//If we push the bottom green button play current song
    {
        rnb.play_current_song();
    }
}

int main()
{
    
    Sample song1 = Sample("class_songs/footstepsshort.wav", 0); // Replace the filename
    Sample song2 = Sample("class_songs/honeywheat_short.wav", 1);// Replace the filename
    Sample song3 = Sample("class_songs/slowdown_short.wav",2);// Replace the filename
    Sample song4 = Sample("Verse1.wav",3); //replace the filename
    rnb.add_song(song1);
    rnb.add_song(song2);
    rnb.add_song(song3);
    rnb.add_song(song4);//no need to change the following 8 lines
    osc_rec buttons(8000,"",phone_interface);
    buttons.start();
    while(true)
    {
        song1.upd();
        usleep(50);
    }
}
