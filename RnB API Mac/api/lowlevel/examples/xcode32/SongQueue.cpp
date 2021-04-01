#include "rnb.h"
#include <iostream>
#include <deque>
#include <stack>

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
        Sample current_song = playlist.front();
        history.push(current_song);
        playlist.erase(playlist.begin());
        cout<<"You havent played any songs yet"<<endl;
    }
    
    void add_song(Sample new_song)
    {
        playlist.push_back(new_song);
        cout<<"Added a new song"<<endl;
    }
};

int main ()
{
    Sample song1 = Sample("media/Verse1.wav",0);
    song1.play();
    
}
