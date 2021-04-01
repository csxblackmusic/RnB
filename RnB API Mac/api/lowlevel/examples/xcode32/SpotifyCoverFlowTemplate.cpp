#include "rnb.h"
#include <vector>
#include <iterator>
#include <deque>
#include <stack>
using namespace std;

class SpotifyCover
{
private:
    deque<Sample> playlist;
    stack<Sample> history;
public:
    void play_current_song()
    {
        if (playlist.size()>0)
        {
            //save the song at the front of the playlist in to Sample variable
            //play the Sample
            //pause for thr length of the sample
            //push the song onto your history stack
            //eliminate the song from the front of your playlist
        }
    }
    
    void play_playlist()
    {
        cout<<"now playing the playlist"<<endl;
        while(playlist.size()>0)
        {
            play_current_song();
        }
    }
    
    void back()
    {
        if(history.size()>0)
        {
            //Save the song at the top of the stack into a sample variable
            //push that song to the front of the playlist
            //delete the song on the top of the stack
            cout<<"Moved back one song"<<endl;
        }
        else
        {
            cout<<"You havent played any songs yet"<<endl;
        }
    }
    
    void forward()
    {
        if (playlist.size()>=1)
        {
            //save the song at the front of the playlist in to Sample variable
            //push that song onto your history stack
            //delete the song at the front of the playlist
        }
        else
        {
            cout<<"You havent played any songs yet"<<endl;
        }
    }
    
    void add_song(Sample new_song)
    {
        playlist.push_back(new_song);
        cout<<"Added a new song"<<endl;
    }
};


int main ()
{
    Sample song1 = Sample("class_songs/slowdown_short.wav",0);//add songs that are on your computer
    Sample song2 = Sample("class_songs/footstepsshort.wav",1);
    Sample song3 = Sample("class_songs/honeywheat_short.wav",2);
    Sample song4 = Sample("Verse1.wav",3);
    
    SpotifyCover neosoul;
    neosoul.add_song(song1);
    neosoul.add_song(song2);
    neosoul.add_song(song3);
    neosoul.add_song(song4);
    neosoul.forward();
    neosoul.play_current_song();
    neosoul.back();
    neosoul.back();
    neosoul.play_playlist();
}

