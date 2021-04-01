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

SpotifyCover rnb;

void process_interface(const string address,vector<float>data)
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
    
    else if(address=="/1/push3" and data[0]==1)//If we push the bottom green button play song
    {
        rnb.play_current_song();
    }
}

int main()
{
    Sample footsteps = Sample("/Users/djames18/Desktop/RnB API Mac/api/lowlevel/examples/xcode32/class_songs/footstepsshort.wav", 0);
    Sample honeywheat = Sample("class_songs/honeywheat_short.wav", 1);//Sample plays on chan 1
    Sample slowdown=Sample("class_songs/slowdown_short.wav",2);
    Sample location = Sample("Verse1.wav",3);
    rnb.add_song(footsteps);
    rnb.add_song(honeywheat);
    rnb.add_song(slowdown);
    rnb.add_song(location);
    osc_rec buttons(8000,"",process_interface);
    //osc_rec buttons(8000);
    buttons.start();
    while(true)
    {
        footsteps.upd();
        usleep(50);
    }
}
