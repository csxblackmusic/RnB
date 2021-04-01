//
//  Sample.cpp
//  effects
//

using namespace std;
#include "fmod.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

class Sample
{
private:
    static FMOD::System* soundsystem;
    static FMOD::Channel* chan [32];
    
    FMOD::Sound * snd;
    unsigned int version;
    const char * filename;
    void * extradriverdata=0;
    int chan_index;
    
public:
    Sample(const char * fname, int channel)
    {
        if(channel>=0 and channel<=32)
        {
            chan_index = channel;
        }
        else
        {
            chan_index = 0;
        }
        if(strncmp(fname, "", 1)!=0)
        {
            filename = fname;
        }
        else
        {
            cout<<"You have entered an invalid filename"<<endl;
        }
        if (soundsystem==nullptr)
        {
            FMOD::System_Create(&soundsystem);
        }
        soundsystem->getVersion(&version);
        soundsystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
        soundsystem->createSound(filename, FMOD_LOOP_OFF, 0, &snd);
        soundsystem->playSound(snd,0,true,&(chan[chan_index]));
    }
    void play()
    {
        if(snd!=nullptr && chan[chan_index]!=nullptr)
        {
            soundsystem->playSound(snd,0,false,&(chan[chan_index]));
        }
        else
        {
            cout<<"Either the sound or the channel hasnt been properly initialized"<<endl;
        }
    }
    static void update()
    {
        if(soundsystem!=nullptr)
            soundsystem->update();
    }
    ~Sample()
    {
        cout<<"destructor running"<<endl;
        snd->release();
        snd = nullptr;
        extradriverdata = nullptr;
    }
    
};

int main()
{
    Sample samp1 = Sample("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/Verse1.wav",0);
/*    samp1.play();
    while(true)
    {
        Sample::update();
        usleep(50);
    }
    return 0;*/
}

