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
            Sample current_song = playlist.front();
            current_song.play();
            usleep(current_song.get_length() *1000);
            history.push(current_song);
            playlist.erase(playlist.begin());
        }
    }
    
    void play_playlist()
    {
        cout<<"now playing the entire playlist"<<endl;
        while(playlist.size()>0)
        {
            play_current_song();
        }
    }
    
    void back()
    {
        if(history.size()>0)
        {
            
            Sample current_song = history.top();
            playlist.push_front(current_song);
            history.pop();
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
            Sample current_song = playlist.front();
            history.push(current_song);
            playlist.erase(playlist.begin());
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

/*
int main ()
{
    Sample song1 = Sample("class_songs/slowdown_short.wav",2);
    Sample song2 = Sample("class_songs/footstepsshort.wav",1);
    Sample song3 = Sample("class_songs/honeywheat_short.wav",2);
    Sample song4 = Sample("Verse1.wav",0);
    
    SpotifyCover playlist;
    playlist.add_song(song1);
    playlist.add_song(song2);
    playlist.add_song(song3);
    playlist.add_song(song4);
    playlist.forward();
    playlist.play_playlist();
}
*/
