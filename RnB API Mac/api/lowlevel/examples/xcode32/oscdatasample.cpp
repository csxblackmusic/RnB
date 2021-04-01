#include "rnb.h"
#include <vector>
#include <iterator>
using namespace std;
vector<Sample> playlist;
int sample_index=0;

void process_interface(const string address,vector<float>data)
{
    for(int i=0;i<data.size();i++)
    {
        cout<<"Parameter"<<i+1<<":"<<" "<<data[i]<<" is from address: "<<address<<endl;
    }
    if(data[0]==1)
    {
        playlist.at(sample_index).play();
        sample_index++;
    }
    
    if(sample_index>=playlist.size())
    {
        sample_index=0;
    }
}

int main()
{
    Sample footsteps = Sample("class_songs/footstepsshort.wav", 0);
    Sample honeywheat = Sample("class_songs/honeywheat_short.wav", 1);//Sample plays on chan 1
    Sample slowdown=Sample("class_songs/slowdown_short.wav",2);
    playlist={footsteps,honeywheat,slowdown};
    osc_rec buttons(8000,"/1/push*",process_interface);
    //osc_rec buttons(8000);
    buttons.start();
    while(true)
    {
        footsteps.upd();
        usleep(50);
    }
}
