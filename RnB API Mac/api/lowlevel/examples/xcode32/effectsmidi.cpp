/*==============================================================================
Effects Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2018.

This example shows how to apply some of the built in software effects to sounds
by applying them to the master channel group. All software sounds played here
would be filtered in the same way. To filter per channel, and not have other
channels affected, simply apply the same functions to the FMOD::Channel instead
of the FMOD::ChannelGroup.
==============================================================================*/
#include "fmod.hpp"
//#include "common.h"
#include "RtMidi.h"
#include "udp.hh"
#include "oscpkt.hh"
#include <unistd.h>
#include <cstdlib>
#include <thread>
#define OSCPKT_OSTREAM_OUTPUT
#include "oscpkt.hh"
#include "udp.hh"
#include "math.h"
#include "VinylTrackFramework.h"


using namespace std;


bool quit = false;
bool once = true;

bool dsplowpassOne_bypass=false;
bool dsphighpassOne_bypass=false;
bool dspechoOne_bypass=false;
bool dspflangeOne_bypass=false;
bool dspflangeTwo_bypass=false;
bool dsplowpassTwo_bypass=false;
bool dsphighpassTwo_bypass=false;
bool dspechoTwo_bypass=false;

FMOD::System       *soundsystem   = 0;
FMOD::Sound        *sound         = 0;
FMOD::Sound        *sound2      = 0;
FMOD::Sound        *sound3      = 0;
FMOD::Sound        *sound4      = 0;
FMOD::Sound        *sound5      = 0;
FMOD::Sound        *sound6      = 0;
FMOD::Sound        *sound7      = 0;
FMOD::Sound        *sound8      = 0;
FMOD::Channel      *channel       = 0;
FMOD::Channel      *channelTwo    = 0;
FMOD::DSP          *dsplowpassOne    = 0;
FMOD::DSP          *dsphighpassOne   = 0;
FMOD::DSP          *dspechoOne       = 0;
FMOD::DSP          *dspflangeOne     = 0;
FMOD::DSP          *dsplowpassTwo    = 0;
FMOD::DSP          *dsphighpassTwo   = 0;
FMOD::DSP          *dspechoTwo       = 0;
FMOD::DSP          *dspflangeTwo     = 0;
FMOD::DSP          *dspthreebandOne     = 0;
FMOD::DSP          *dspthreebandTwo     = 0;
FMOD_RESULT        result;
FMOD::Sound        *turntableAudio =0;

unsigned int       version;
unsigned char prev_message=0;
void               *extradriverdata = 0;
float currFreq=0.0;
float jogFreq=0.0;
float rjogFreq=0.0;
float RcurrFreq=0.0;
float lastFaderFreq= 44100.0;
float lastRFaderFreq= 44100.0;
float lastLChannelVol =1;
float lastRChannelVol=1;
float lastxfadervalue=0.5;
bool wheel_touched = false;
bool Rwheel_touched = false;
bool scratch = false;
bool Rscratch = false;
bool wheel = true;
bool Rwheel = true;
float scratch_scaling_factor = 1;
bool fwd = false;
bool Rfwd=false;
bool backward = true;
bool Rbackward=false;
float increment=0;
float Rincrement=0;
int LeffectSelector=0;
int ReffectSelector=0;
RtMidiIn *midiin = 0;
const int PORT_NUM = 8000;
using namespace oscpkt;
using namespace std;
void osc_server()
{
    UdpSocket sock;
    bool showalldata = false;
    sock.bindTo(8000);
    if (!sock.isOk()) {
        cerr << "Error opening port " << PORT_NUM << ": " << sock.errorMessage() << "\n";
    } else {
        cout << "Server started, will listen to packets on port " << PORT_NUM << std::endl;
        PacketReader pr;
        PacketWriter pw;
        while (sock.isOk())
        {
            if (sock.receiveNextPacket(30 /* timeout, in ms */))
            {
                pr.init(sock.packetData(), sock.packetSize());
                oscpkt::Message *msg;
                
                while (pr.isOk() && (msg = pr.popMessage()) != 0)
                {
                    Message::ArgReader arg(msg->arg());
                    string addr = msg->addressPattern();
                    float velocity;
                    if(addr=="/mspinky/velocity"||showalldata==true)
                    {
                        while(arg.nbArgRemaining())
                        {
                            if(arg.isInt32())
                            {
                                int a;
                                arg.popInt32(a);
                                cout<<a<<" received from "<<addr<<endl;
                            }
                            else if (arg.isInt64())
                            {
                                int64_t b;
                                arg.popInt64(b);
                                cout<<b<<" received from "<<addr<<endl;
                            }
                            else if(arg.isFloat())
                            {
                                arg.popFloat(velocity);
                                channelTwo->setFrequency(velocity*44100);
                            }
                            else if(arg.isBool())
                            {
                                bool tf;
                                arg.popBool(tf);
                                cout<<tf<<" received from "<<addr<<endl;
                            }
                            else if(arg.isStr())
                            {
                                string s;
                                arg.popStr(s);
                                cout<<s<<" received from "<<addr<<endl;
                            }
                        }
                    }
                }
            }
        }
    }
}
void osc_server2()
{
    UdpSocket sock;
 //   bool showalldata = false;
    bool showalldata = true;
    sock.bindTo(8000);
    if (!sock.isOk()) {
        cerr << "Error opening port " << PORT_NUM << ": " << sock.errorMessage() << "\n";
    } else {
        cout << "Server started, will listen to packets on port " << PORT_NUM << std::endl;
        PacketReader pr;
        PacketWriter pw;
        while (sock.isOk())
        {
            if (sock.receiveNextPacket(30 /* timeout, in ms */))
            {
                pr.init(sock.packetData(), sock.packetSize());
                oscpkt::Message *msg;
                
                while (pr.isOk() && (msg = pr.popMessage()) != 0)
                {
                    Message::ArgReader arg(msg->arg());
                    string addr = msg->addressPattern();
                    float velocity;
                    int argument = 0;
                    if(addr=="/mspinky/velocity"||showalldata==true)
                    {
                        while(arg.nbArgRemaining())
                        {
                            if(arg.isInt32())
                            {
                                int a;
                                arg.popInt32(a);
                                cout<<a<<" received from "<<addr<<endl;
                            }
                            else if (arg.isInt64())
                            {
                                int64_t b;
                                arg.popInt64(b);
                                cout<<b<<" received from "<<addr<<endl;
                            }
                            else if(arg.isFloat())
                            {
                                float f;
                                arg.popFloat(f);
                                if(addr=="/muse/gyro")
                                {
                                    cout<<"received argument "<< argument <<" "<<f<<"from "<<addr<<endl;
                                    if (argument==2)
                                    {
                                        f = (f*21990.0) + 10.0;
                                        dsplowpassOne->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, f);
                                    }
                                }
                                
                            }
                            else if(arg.isBool())
                            {
                                bool tf;
                                arg.popBool(tf);
                                cout<<tf<<" received from "<<addr<<endl;
                            }
                            else if(arg.isStr())
                            {
                                string s;
                                arg.popStr(s);
                                cout<<s<<" received from "<<addr<<endl;
                            }
                            else if(arg.isDouble())
                            {
                                double d;
                                arg.popDouble(d);
                                cout<<d<<" received from "<<addr<<endl;
                            }
                            ++argument;
                        }
                        argument =0;
                    }
                }
            }
        }
    }
}

void usage( void ) {
    // Error function in case of incorrect command-line
    // argument specifications.
    cout << "\nuseage: cmidiin <port>\n";
    cout << "    where port = the device to use (default = 0).\n\n";
    exit( 0 );
}

void numark(std::vector< unsigned char > *message, double delta)
{
    
    int LnumDSP,RnumDSP=0;
    channel->getNumDSPs(&LnumDSP);
    channelTwo->getNumDSPs(&RnumDSP);
    if(LnumDSP<2)
    {
        cout<<"added DSP to channels one again"<<endl;
        result = channel->removeDSP(dsplowpassOne);
        result = channel->removeDSP(dsphighpassOne);
        result = channel->removeDSP(dspechoOne);
        result = channel->removeDSP(dspflangeOne);
        result = channel->removeDSP(dspthreebandOne);
        
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpassOne);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpassOne);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspechoOne);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspflangeOne);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_THREE_EQ, &dspthreebandOne);
        
        //Adds the effects to the left channel
        result = channel->addDSP(0, dsplowpassOne);
        result = channel->addDSP(1,dsphighpassOne);
        result = channel->addDSP(2, dspechoOne);
        result = channel->addDSP(3, dspflangeOne);
        result = channel->addDSP(4,dspthreebandOne);
        
        dsplowpassOne->setBypass(true);
        dspthreebandOne->setBypass(false);
        dsphighpassOne->setBypass(true);
        dspflangeOne->setBypass(true);
        dspechoOne->setBypass(true);
    }
    if(RnumDSP<2)
    {
        cout<<"added DSP to channel two again"<<endl;
        
        //removes the effects from the channel
        
        result = channelTwo->removeDSP(dsplowpassTwo);
        result = channelTwo->removeDSP(dsphighpassTwo);
        result = channelTwo->removeDSP(dspechoTwo);
        result = channelTwo->removeDSP(dspflangeTwo);
        result = channelTwo->removeDSP(dspthreebandTwo);
        
        //Create the built in effects
        
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpassTwo);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpassTwo);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspechoTwo);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspflangeTwo);
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_THREE_EQ, &dspthreebandTwo);
        
        //Add the effects to right channel again
   

        result = channelTwo->addDSP(0, dsplowpassTwo);
        result = channelTwo->addDSP(1, dsphighpassTwo);
        result = channelTwo->addDSP(2, dspechoTwo);
        result = channelTwo->addDSP(3, dspflangeTwo);
        result = channelTwo->addDSP(4,dspthreebandTwo);
        
        dsplowpassTwo->setBypass(true);
        dspthreebandTwo->setBypass(false);
        dsphighpassTwo->setBypass(true);
        dspflangeTwo->setBypass(true);
        dspechoTwo->setBypass(true);
    }
    if(message->at(0)==144)
    {
        if(message->at(1)==18) //if you press down the big knob you quit
        {quit = true;}
    }
    else if (message->at(0)==146)
    {
        if(message->at(1)==27 && message->at(2)==127) //Channel two Pad 1
        {
            channelTwo->stop();
            soundsystem->playSound(sound5,0,false,&channelTwo);
            channelTwo->setVolume((lastRChannelVol*lastxfadervalue));
        }
        else if(message->at(1)==28 && message->at(2)==127)  //Channel two Pad 2
        {
            channelTwo->stop();
            soundsystem->playSound(sound6,0,false,&channelTwo);
            channelTwo->setVolume((lastRChannelVol*lastxfadervalue));
        }
        else if(message->at(1)==29 && message->at(2)==127) //Channel two Pad 3
        {
            channelTwo->stop();
            soundsystem->playSound(sound7,0,false,&channelTwo);
            channelTwo->setVolume((lastRChannelVol*lastxfadervalue));
        }
        else if(message->at(1)==30 && message->at(2)==127) // //Channel two Pad 4
        {
            channelTwo->stop();
            soundsystem->playSound(sound8,0,false,&channelTwo);
            channelTwo->setVolume((lastRChannelVol*lastxfadervalue));
        }
        else if(message->at(1)==(12) &&message->at(2)==127) //Once the right jogwheel is touched the song stops
        {
            channelTwo->setFrequency(0);
            Rwheel_touched = true;
        }
        
        else if(message->at(1)==(12) &&message->at(2)==0) //When you let go of the wheel it should resume playing at the previous speed
        {
            channelTwo->setFrequency(lastFaderFreq);
            Rwheel_touched = false;
        }
        else if (message->at(1)==6 and message->at(2)==127)//When the wheel button is touched it turns scratching on and off
        {
            Rwheel =!Rwheel;
        }
        else if(message->at(1)==7)//Turns on right  channel lowpass filter effect
        {
            if(message->at(2)==127)
            {
                dsplowpassTwo->getBypass(&dsplowpassTwo_bypass);
                dsplowpassTwo->setBypass(!dsplowpassTwo_bypass);
                cout<<"pushed on/off button for lowpass"<<endl;
                cout<<"bypass is"<<dsplowpassTwo_bypass<<endl;
            }
        }
        else if(message->at(1)==8) //Turns on right channel flanger effect
        {
            if(message->at(2)==127)
            {
                dspflangeTwo->getBypass(&dspflangeTwo_bypass);
                dspflangeTwo->setBypass(!dspflangeTwo_bypass);
                cout<<"pushed on/off button for flange"<<endl;
                cout<<"bypass is"<<dspflangeTwo_bypass<<endl;
            }
        }
        else if(message->at(1)==9) //Turns on right channel highpass filter
        {
            if(message->at(2)==127)
            {
                dsphighpassTwo->getBypass(&dsphighpassTwo_bypass);
                dsphighpassTwo->setBypass(!dsphighpassTwo_bypass);
                cout<<"pushed on/off button for highpass"<<endl;
                cout<<"bypass is"<<dsphighpassTwo_bypass<<endl;
            }
        }
        else if(message->at(1)==10) //Turns on/off the  right channel echo
        {
            if(message->at(2)==127)
            {
                dspechoTwo->getBypass(&dspechoTwo_bypass);
                dspechoTwo->setBypass(!dspechoTwo_bypass);
                cout<<"pushed on/off button for echo"<<endl;
                cout<<"bypass is"<<dspechoTwo_bypass<<endl;
            }
        }
        
    }
    else if(message->at(0)==145)
    {
        if(message->at(1)==7)//Turns left channel lowpass filter on/off
        {
            if(message->at(2)==127)
            {
                dsplowpassOne->getBypass(&dsplowpassOne_bypass);
                dsplowpassOne->setBypass(!dsplowpassOne_bypass);
                cout<<"pushed on/off button for lowpass"<<endl;
                cout<<"bypass is"<<dsplowpassOne_bypass<<endl;
            }
        }
        else if(message->at(1)==8) //Turns left channel flange filter on/off
        {
            if(message->at(2)==127)
            {
                dspflangeOne->getBypass(&dspflangeOne_bypass);
                dspflangeOne->setBypass(!dspflangeOne_bypass);
                cout<<"pushed on/off button for flange"<<endl;
                cout<<"bypass is"<<dspflangeOne_bypass<<endl;
            }
        }
        else if(message->at(1)==9) //Turns left channel highpass filter on/off
        {
            if(message->at(2)==127)
            {
                dsphighpassOne->getBypass(&dsphighpassOne_bypass);
                dsphighpassOne->setBypass(!dsphighpassOne_bypass);
                cout<<"pushed on/off button for highpass"<<endl;
                cout<<"bypass is"<<dsphighpassOne_bypass<<endl;
            }
        }
        else if(message->at(1)==10) //Turns left channel echo filter on/off
        {
            if(message->at(2)==127)
            {
                dspechoOne->getBypass(&dspechoOne_bypass);
                dspechoOne->setBypass(!dspechoOne_bypass);
                cout<<"pushed on/off button for echo"<<endl;
                cout<<"bypass is"<<dspechoOne_bypass<<endl;
            }
        }
        else if(message->at(1)==(12) &&message->at(2)==127) // when the right wheel is touched the sound should stop
        {
            channel->setFrequency(0);
            wheel_touched = true;
         }
         
        else if(message->at(1)==(12) &&message->at(2)==0) //once you let go of the right wheel it should resume playing at its speed
         {
             channel->setFrequency(lastFaderFreq);
             wheel_touched = false;
         }
        else if (message->at(1)==6 and message->at(2)==127) //when you touch the wheel button it turns the scratching on and off
        {
            wheel =!wheel;
        }
        else if (message->at(1)==27&&message->at(2)==127)//Left channel pad 1
        {
           
            channel->stop();//if a pad is pressed then all previous sounds on that channel will stop
            soundsystem->playSound(sound,0, true, &channel); //song starts paused
            channel->setVolume(lastLChannelVol*(1-lastxfadervalue)); //song play at the volume it should account for the channel vol and x fader
            channel->setFrequency(lastFaderFreq); //it should also play at the speed set by the pitch fader
            channel->setPaused(false);
            int numDSPonChan1=0;
            channel->getNumDSPs(&numDSPonChan1);
            cout<<"This is the number of DSP on this channel "<<numDSPonChan1<<endl;
        }
        else if (message->at(1)==28&&message->at(2)==127)//Left channel pad 2
        {
            channel->stop();
            soundsystem->playSound(sound2,0, true, &channel);
            channel->setVolume(lastLChannelVol*(1-lastxfadervalue));
            channel->setFrequency(lastFaderFreq);
            channel->setPaused(false);
        }
        else if (message->at(1)==29&&message->at(2)==127)//Left channel pad 3
        {
            channel->stop();
            soundsystem->playSound(sound3,0, true, &channel);
            channel->setVolume(lastLChannelVol*(1-lastxfadervalue));
            channel->setFrequency(lastFaderFreq);
            channel->setPaused(false);
        }
        else if (message->at(1)==30&&message->at(2)==127)//Left channel pad  4
        {
            channel->stop();
            soundsystem->playSound(sound4,0, true, &channel);
            channel->setVolume(lastLChannelVol*(1-lastxfadervalue));
            channel->setFrequency(lastFaderFreq);
            channel->setPaused(false);
        }
    }
    else if(message->at(0)==176)//CROSSFADER
    {
        if(message->at(1)==7)
        {   lastxfadervalue = message->at(2)/127.0;
            cout<<"last xfade val"<<lastxfadervalue<<endl;
            float volume = lastLChannelVol-lastLChannelVol*(lastxfadervalue);
            channel->setVolume(volume);
            channelTwo->setVolume((message->at(2)/127.0)*lastRChannelVol);
        }
    }
    else if (message->at(0)==178)
    {
        if(message->at(1)==(5))//Right Channel Volume
        {   float volR =message->at(2)/127.0;
            lastRChannelVol = volR;
            channelTwo->setVolume(volR*(lastxfadervalue));
        }
        else if(message->at(1)==26) //lets the slider control the effect
        {
            if(ReffectSelector==1 || ReffectSelector==0)
            {
                float value;
                value = message->at(2)/127.0;
                value = (value*21990.0) + 10.0;
                dsplowpassTwo->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, value);
            }
            else if(ReffectSelector==2)
            {
                float value;
                value = message->at(2)/127.0;
                value = (value*0.99) + 0.01;
                dspflangeTwo->setParameterFloat(FMOD_DSP_FLANGE_DEPTH,value);
            }
            else if(ReffectSelector==3)
            {
                float value;
                value = message->at(2)/127.0;
                value = (value*21990.0) + 10.0;
                dsphighpassTwo->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, value);
            }
            else if(ReffectSelector==4)
            {
                float value;
                value = message->at(2)/127.0;
                value = (value*4950.0) + 10.0;
                dspechoTwo->setParameterFloat(FMOD_DSP_ECHO_DELAY, value);
                cout<<"set value of delay to "<<value<<endl;
                
            }
        }
        else if(message->at(1)==(17))//Right JOG WHEEL FOR SCRATCHING
        {
            unsigned int pos=0;
            double calls_per_rev=600.0;
            double revs_per_min =(33.0 +(1.0/3.0));
            double revs_per_sec = revs_per_min/60.0;
            
            
            if(Rwheel && Rwheel_touched)// if wheel is pressed then it should scratch
            {
                if(message->at(2)>65) //left jog wheel
                {
                    double secs_per_call = 1.0/(calls_per_rev *revs_per_sec); //calculates seconds per increment
                    rjogFreq = (secs_per_call/delta) *44100.0 *(log(127.0-message->at(2)+20.0)/log(20.0)); //data sent by jog wheel on byte 2 only used to deteremine direction of spin and the difference between the number and 127 is used to indicate velocity past delta
                    //log is used because linear growth was too fast
                    //jogFreq=((secs_per_call/delta)+((127.0-message->at(2))/scratch_scaling_factor))*44100.0; //scratch scaling factor controls the impact of the jogwheel data
                    channelTwo->setFrequency(rjogFreq);
                    cout<<rjogFreq<<endl;
                    channelTwo->getPosition(&pos, FMOD_TIMEUNIT_PCM);
                    Rincrement+=(128.0-message->at(2))*(secs_per_call*1000.0);
                    if(Rbackward)
                    {
                        channelTwo->getPosition(&pos, FMOD_TIMEUNIT_PCM);// USE FMOD_TIMEUNIT_PCM which measures in samples
                        channelTwo->setPosition(pos +((Rincrement*44100.0)/1000.0) ,FMOD_TIMEUNIT_PCM);
                        
                        cout<<"position changed by " <<Rincrement <<"millisecs"<<endl;
                        Rincrement=0.0;
                    }
                    Rfwd=true;
                    Rbackward=false;
                }
                
                else
                {
                    double secs_per_call = (1.0)/(calls_per_rev*revs_per_sec);
                    rjogFreq=(secs_per_call/delta) *(log(message->at(2)+19.0)/log(20.0)) *-44100.0; //smallest value sent on wheel is 1 and 1+4=5 log(10)=1
                    // log(x)/log(5) calculates log of base 5  - gives the best scratch sound
                    channelTwo->setFrequency(rjogFreq);
                    cout<<rjogFreq<<endl;
                    
                    Rincrement -= ( message->at(2)*(secs_per_call*1000.0));
                    if(Rfwd)
                    {
                        channelTwo->getPosition(&pos, FMOD_TIMEUNIT_PCM);
                        channelTwo->setPosition(pos-((Rincrement*44100.0)/1000),FMOD_TIMEUNIT_PCM);
                        cout<<"position changed by " <<Rincrement <<"millisecs"<<endl;
                        Rincrement=0.0;
                        
                    }
                    Rfwd=false;
                    Rbackward=true;
                }
            }
        }
            
        else if(message->at(1)==19)
        {
            ReffectSelector++;
            if(ReffectSelector==5||ReffectSelector==1)
            {
                ReffectSelector=1;
                cout<<"right lowpass (FX1) will be changed by the slider"<<endl;
            }
            else if(ReffectSelector==2)
            {
                cout<<"right flange (FX2) will be changed by the slider"<<endl;
            }
            else if(ReffectSelector==3)
            {
                cout<<"right highpass (FX3) will be changed by the slider"<<endl;
            }
            else if(ReffectSelector==4)
            {
                cout<<"right echo will (FX4/TAP) be changed by the slider"<<endl;
            }
        }
        else if(message->at(1)==24)
        {
                float rfader_val= static_cast<float>(message->at(2));
                float rnewFreq=44100.0*((rfader_val/127.0)+0.5);
                result=channelTwo->setFrequency(rnewFreq);
                result= channelTwo->getFrequency(&lastRFaderFreq);
        }
        else if(message->at(1)==1)
        {
            float value;
            if(message->at(2)>=64)
            {
                value =((message->at(2) - 64.0)/63.0)*10.0;
            }
            else
            {
                value= ((message->at(2)/63.0)*80) - 80.0;
            }
            dspthreebandTwo->setParameterFloat(FMOD_DSP_THREE_EQ_HIGHGAIN, value);
            cout<<"Right Chan Treble set to"<<value<<endl;
        }
        else if(message->at(1)==2)//Mid knob
        {
            float value;
            if(message->at(2)>=64)
            {
                value =((message->at(2) - 64.0)/63.0)*10.0;
            }
            else
            {
                value= ((message->at(2)/63.0)*80) - 80.0;
            }
            dspthreebandTwo->setParameterFloat(FMOD_DSP_THREE_EQ_MIDGAIN, value);
            cout<<"Right Chan Mids set to"<<value<<endl;
        }
        else if(message->at(1)==3)//Bass knob
        {
            float value;
            if(message->at(2)>=64)
            {
                value =((message->at(2) - 64.0)/63.0)*10.0;
            }
            else
            {
                value= ((message->at(2)/63.0)*80) - 80.0;
            }
            dspthreebandTwo->setParameterFloat(FMOD_DSP_THREE_EQ_LOWGAIN, value);
            cout<<"Right Chan Bass  set to"<<value<<endl;
        }
    }
    else if(message->at(0)==177)
    {
        
        if(message->at(1)==(5))//Left Channel Volume
        {   float vol =message->at(2)/127.0;
            lastLChannelVol = vol;
            channel->setVolume(vol*(1-lastxfadervalue));
        }
      
        
        else if(message->at(1)==(24))//PITCH FADER
        {
            float fader_val= static_cast<float>(message->at(2));
            float newFreq=44100.0*((fader_val/127.0)+0.5);
            result=channel->setFrequency(newFreq);
            result= channel->getFrequency(&lastFaderFreq);
        }
        else if(message->at(1)==3)//Left channel Bass knob
        {
            float value;
            if(message->at(2)>=64)
            {
                value =((message->at(2) - 64.0)/63.0)*10.0;
            }
            else
            {
                value= ((message->at(2)/63.0)*80) - 80.0;
            }
            dspthreebandOne->setParameterFloat(FMOD_DSP_THREE_EQ_LOWGAIN, value);
             cout<<"Bass set to"<<value<<endl;
        }
        else if(message->at(1)==2)//Left channel Mid knob
        {
            float value;
            if(message->at(2)>=64)
            {
                value =((message->at(2) - 64.0)/63.0)*10.0;
            }
            else
            {
                value= ((message->at(2)/63.0)*80) - 80.0;
            }
            dspthreebandOne->setParameterFloat(FMOD_DSP_THREE_EQ_MIDGAIN, value);
             cout<<"Mids set to"<<value<<endl;
        }
        else if(message->at(1)==1)//Left channel Treble Knob
        {
            float value;
            if(message->at(2)>=64)
            {
                value =((message->at(2) - 64.0)/63.0)*10.0;
            }
            else
            {
                value= ((message->at(2)/63.0)*80) - 80.0;
            }
            dspthreebandOne->setParameterFloat(FMOD_DSP_THREE_EQ_HIGHGAIN, value);
            cout<<"Treble set to"<<value<<endl;
        }
        
        else if(message->at(1)==(17))//LEFT JOG WHEEL FOR SCRATCHING
        {
            unsigned int pos=0;
            double calls_per_rev=600.0;
            double revs_per_min =(33.0 +(1.0/3.0));
            double revs_per_sec = revs_per_min/60.0;
           
           
            if(wheel && wheel_touched)// if wheel is pressed then it should scratch
            {
                if(message->at(2)>65) //left jog wheel
                {
                    double secs_per_call = 1.0/(calls_per_rev *revs_per_sec); //calculates seconds per increment
                    rjogFreq = (secs_per_call/delta) *44100.0 *(log(127.0-message->at(2)+20.0)/log(20.0)); //data sent by jog wheel on byte 2 only used to deteremine direction of spin and the difference between the number and 127 is used to indicate velocity past delta
                    //log is used because linear growth was too fast
                    //jogFreq=((secs_per_call/delta)+((127.0-message->at(2))/scratch_scaling_factor))*44100.0; //scratch scaling factor controls the impact of the jogwheel data
                    channel->setFrequency(jogFreq);
                    cout<<rjogFreq<<endl;
                    channelTwo->getPosition(&pos, FMOD_TIMEUNIT_PCM);
                    increment+=(128.0-message->at(2))*(secs_per_call*1000.0);
                    if(backward)
                    {
                        channel->getPosition(&pos, FMOD_TIMEUNIT_PCM);// USE FMOD_TIMEUNIT_PCM which measures in samples
                        channel->setPosition(pos +((increment*44100.0)/1000.0) ,FMOD_TIMEUNIT_PCM);
                     
                        cout<<"position changed by " <<increment <<"millisecs"<<endl;
                        increment=0.0;
                    }
                    Rfwd=true;
                    Rbackward=false;
                }
            
                else
                {
                    double secs_per_call = (1.0)/(calls_per_rev*revs_per_sec);
                    jogFreq=(secs_per_call/delta) *(log(message->at(2)+19.0)/log(20.0)) *-44100.0; //smallest value sent on wheel is 1 and 1+4=5 log(10)=1
                    // log(x)/log(5) calculates log of base 5  - gives the best scratch sound
                    channel->setFrequency(jogFreq);
                    cout<<jogFreq<<endl;
                   
                    increment -= ( message->at(2)*(secs_per_call*1000.0));
                    if(fwd)
                    {
                        channel->getPosition(&pos, FMOD_TIMEUNIT_PCM);
                        channel->setPosition(pos-((increment*44100.0)/1000),FMOD_TIMEUNIT_PCM);
                        cout<<"position changed by " <<increment <<"millisecs"<<endl;
                        increment=0.0;
                        
                    }
                    fwd=false;
                    backward=true;
                }
            }
        
        }
    }
}

void automat(std::vector< unsigned char > *message)
{
    if(message->at(0)==176)
    {
        if(message->at(1)==36)
        {
            quit=true;
        }
        if(message->at(1)==29)//Automat5 layout
        {
            dsplowpassOne->getBypass(&dsplowpassOne_bypass);
            dsplowpassOne->setBypass(!dsplowpassOne_bypass);
        }
        if(message->at(1)==30)
        {
            dspflangeOne->getBypass(&dspflangeOne_bypass);
            dspflangeOne->setBypass(!dspflangeOne_bypass);
        }
        if(message->at(1)==31)
        {
            dspechoOne->getBypass(&dspechoOne_bypass);
            dspechoOne->setBypass(!dspechoOne_bypass);
        }
        if(message->at(1)==32)
        {
            dsphighpassOne->getBypass(&dsphighpassOne_bypass);
            dsphighpassOne->setBypass(!dsphighpassOne_bypass);
        }
        if(message->at(1)==8)
        {
            float range = (((message->at(2))/127.0)*(22000.0-10))+10.0;
            dsplowpassOne->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF,range);
            
        }
        if(message->at(1)==0)
        {
            float depth = (((message->at(2))/127.0)*(1.0-0.01))+0.01;
            dspflangeOne->setParameterFloat(FMOD_DSP_FLANGE_DEPTH,depth);
            cout<<"This is the depth"<<depth<<endl;
            
        }
        if(message->at(1)==28)
        {
            float volume = 1.0-(message->at(2)/127.0);
            channel->setVolume(volume);
        }
       
    }
}


void mycallback( double deltatime, std::vector< unsigned char > *message, void */*userData*/ )
{
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
        cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        cout << "stamp = " << deltatime << std::endl;
    //automat(message);
    numark(message,deltatime);
    
}

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort( RtMidiIn *rtmidi );


int main()
{
    
 
        //Create a System object and initialize
 
    result = FMOD::System_Create(&soundsystem);
   // ERRCHECK(result);

    result = soundsystem->getVersion(&version);
  //  ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
       // Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
        cout<<"Fmod lib version does not match header version"<<endl;
        return 0;
    }

    result = soundsystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
 //   ERRCHECK(result);
  //  ERRCHECK(result);
 /*   int numDrivers = 0;
    int system_rate;
    int driver_id=0;
    int native_rate,native_channels;
    char driver_name [18]= "BLEH";
    result = soundsystem->getRecordNumDrivers(NULL, &numDrivers); //number of audio drivers
    cout<<"Please select which audio device you would like to record"<<endl;
    for (int i=0;i<numDrivers;i++)
    {
        
        //soundsystem->getRecordDriverInfo(i, driver_name, 18, NULL, &system_rate, NULL, NULL, NULL);
        cout<<i<<". "<< driver_name<<endl;
    }
    cin>> driver_id;
    soundsystem->getRecordDriverInfo(driver_id, driver_name, 18, NULL, &native_rate, NULL, &native_channels, NULL);
    FMOD_CREATESOUNDEXINFO exinfo = {0};
    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = native_channels;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM32; // 32 bit pcm
    exinfo.defaultfrequency = native_rate;
    exinfo.length           = native_rate * sizeof(int) * native_channels; // 1 second buffer, size here doesn't change latency
 
    result = soundsystem->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &turntableAudio);
  */
    result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/Verse1.wav", FMOD_LOOP_NORMAL, 0, &sound);
    result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/Verse2.wav", FMOD_LOOP_NORMAL, 0, &sound2);
    result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/LocationVerse1NoBass.wav", FMOD_LOOP_NORMAL, 0, &sound3);
result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/LocationChorusWithBass.wav", FMOD_LOOP_NORMAL, 0, &sound4);
    result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/Guitar.wav", FMOD_LOOP_NORMAL, 0, &sound5);
    result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/DooDooDo.wav", FMOD_LOOP_NORMAL, 0, &sound6);
    result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/DooDooDo2.wav", FMOD_LOOP_NORMAL, 0, &sound7);
    result = soundsystem->createSound("/Users/djames18/Documents/FMOD_Programmers_API/api/lowlevel/examples/xcode32/BasslineAndDrums.wav", FMOD_LOOP_NORMAL, 0, &sound8);
  //  ERRCHECK(result);
    
    result = soundsystem->playSound(sound, 0, true, &channel);
    result = soundsystem->playSound(sound4, 0, true, &channelTwo);
   
  //  ERRCHECK(result);

 
        //Create some effects to play with
    
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpassOne);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpassTwo);
 //   ERRCHECK(result);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpassOne);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpassTwo);
 //   ERRCHECK(result);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspechoOne);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspechoTwo);
 //   ERRCHECK(result);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspflangeOne);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspflangeTwo);
 //   ERRCHECK(result);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_THREE_EQ, &dspthreebandOne);
    result = soundsystem->createDSPByType(FMOD_DSP_TYPE_THREE_EQ, &dspthreebandTwo);
   
        //Add them to the master channel group.
        //Each time an effect is added (to position 0) it pushes the others down the list.
    result = channel->addDSP(0, dsplowpassOne);
    result = channel->addDSP(1,dsphighpassOne);
    result = channel->addDSP(2, dspechoOne);
    result = channel->addDSP(3, dspflangeOne);
    result = channel->addDSP(4,dspthreebandOne);
    
  //  ERRCHECK(result);
    result = channelTwo->addDSP(0, dsplowpassTwo);
    result = channelTwo->addDSP(1, dsphighpassTwo);
    result = channelTwo->addDSP(2, dspechoTwo);
    result = channelTwo->addDSP(3, dspflangeTwo);
    result = channelTwo->addDSP(4,dspthreebandTwo);
    cout<<"Added DSP for both channels"<<endl;
    //    By default, bypass all effects.  This means let the original signal go through without processing.
    //    It will sound 'dry' until effects are enabled by the user.
 
    result = dsplowpassOne->setBypass(false);
    result = dsphighpassOne->setBypass(false);
    result = dspechoOne->setBypass(false);
    result = dspflangeOne->setBypass(false);
    result = dsplowpassTwo->setBypass(false);
    result = dsphighpassTwo->setBypass(false);
    result = dspechoTwo->setBypass(false);
    result = dspflangeTwo->setBypass(false);
    result = dspthreebandOne->setBypass(false);
    result = dspthreebandTwo->setBypass(false);
    
    int numDSPonChanOne=0;
    channel->getNumDSPs(&numDSPonChanOne);
    cout<<"This is the number of DSP on this channel "<<numDSPonChanOne<<endl;
    cout<<"Set bypass for all channels"<<endl;
    
    // RtMidiIn constructor
    midiin = new RtMidiIn();
    try
    {
    
    // Call function to select port.
        if ( chooseMidiPort( midiin ) == false )
        {
            cout<<"Midi not connected"<<endl;
        }
        else
        {
    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue instead of sent to the callback function.
        midiin->setCallback( &mycallback );
    
    // Don't ignore sysex, timing, or active sensing messages.
        midiin->ignoreTypes( false, false, false );
    
        std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
    //char input;
    //std::cin.get(input);
        }
    }
    catch ( RtMidiError &error )
    {
        error.printMessage();
    }

 
        //Main loop
 

    //ERRCHECK(result);
    bool paused = 0;
   
    
    dsplowpassOne   ->getBypass(&dsplowpassOne_bypass);
    dsphighpassOne  ->getBypass(&dsphighpassOne_bypass);
    dspechoOne      ->getBypass(&dspechoOne_bypass);
    dspflangeOne    ->getBypass(&dspflangeOne_bypass);
    dsplowpassTwo   ->getBypass(&dsplowpassTwo_bypass);
    dsphighpassTwo  ->getBypass(&dsphighpassTwo_bypass);
    dspechoTwo      ->getBypass(&dspechoTwo_bypass);
    dspflangeTwo    ->getBypass(&dspflangeTwo_bypass);
    
    cout<<"Saved bypass state of effects to all variables"<<endl;
    
    if (channel)
    {
        result = channel->getPaused(&paused);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            //ERRCHECK(result);
            return 0;
        }
    }
   // std::thread t1(osc_server);
    channel->getNumDSPs(&numDSPonChanOne);
    cout<<"This is the number of DSP on this channel before the main loop "<<numDSPonChanOne<<endl;
    //void * theobj= MPVT2_CreateNew(1024, native_rate);
    //MPVT2_SetCutoffSharpness(theobj, 1.0L);
    //MPVT2_SetVinylGeneration(theobj, 4l);
    std::thread turntable1(osc_server2);
   // std::thread turntable2(osc_server2);
    unsigned int buffer_length=0;
    unsigned int record_pos=0;
    unsigned last_record_pos=0;
    unsigned int delta = 0;
    void * data = NULL;
   // soundsystem->recordStart(driver_id,turntableAudio, true); //start recording into our user defined sound
    unsigned int soundLength = 0;
    result = turntableAudio->getLength(&soundLength, FMOD_TIMEUNIT_PCM);
    soundsystem->playSound(sound, NULL, false, &channel);
    while(!quit)
    {
        result = soundsystem->update();
  /*      soundsystem->getRecordPosition(driver_id,&record_pos);
        if(record_pos-last_record_pos>=0)
        {
            delta =record_pos-last_record_pos;
        }
        else
        {
            delta =record_pos + soundLength - last_record_pos;
        }
        long num_measurements=0;
        last_record_pos = record_pos;
        turntableAudio->lock(record_pos-delta, delta, &data, NULL, &buffer_length, NULL);// from https://qa.fmod.com/t/pcm-of-microphone-input/12943/3
       // MPVT2_ProcessBuffer(theobj, <#float *inBufferLeft#>, <#float *inBufferRight#>, buffer_length, <#double *velocityVals#>, <#double *powerVals#>, double *positionVals, &num_measurements);//figure out how to split the interleaved RAW PCM data to left and right channels - https://stackoverflow.com/questions/24265532/deinterleaving-pcm-wav-stereo-audio-data
        //use a for loop to run each velocity and position through a callback function
        turntableAudio->unlock(data, NULL, buffer_length, 0);
        */
        usleep(50);
    }
 
        //Shut down
    
    

    result = channel->removeDSP(dsplowpassOne);
    result = channel->removeDSP(dsphighpassOne);
    result = channel->removeDSP(dspechoOne);
    result = channel->removeDSP(dspflangeOne);
    result = channelTwo->removeDSP(dsplowpassTwo);
    result = channelTwo->removeDSP(dsphighpassTwo);
    result = channelTwo->removeDSP(dspechoTwo);
    result = channelTwo->removeDSP(dspflangeTwo);
    
    cout<<"removed DSP from both channels"<<endl;
    result = dsplowpassOne->release();
    result = dsphighpassOne->release();
    result = dspechoOne->release();
    result = dspflangeOne->release();
    result = dsplowpassTwo->release();
    result = dsphighpassTwo->release();
    result = dspechoTwo->release();
    result = dspflangeTwo->release();
    cout<<"released all resources for DSP"<<endl;

    result = sound->release();
    result = sound2->release();
    result = sound3->release();
    result = sound4->release();
    result = sound5->release();
    result = sound6->release();
    result = sound7->release();
    result = sound8->release();
    
    cout<<"Released all memory used to store sounds"<<endl;
    result = soundsystem->close();
    result = soundsystem->release();
    
    delete midiin;
    return 0;
}



bool chooseMidiPort( RtMidiIn *rtmidi )
{
    std::cout << "\nWould you like to open a virtual input port? [y/N] ";
    
    std::string keyHit;
    std::getline( std::cin, keyHit );
    if ( keyHit == "y" ) {
        rtmidi->openVirtualPort();
        return true;
    }
    
    std::string portName;
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if ( nPorts == 0 ) {
        std::cout << "No input ports available!" << std::endl;
        return false;
    }
    
    if ( nPorts == 1 ) {
        std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
    }
    else {
        for ( i=0; i<nPorts; i++ ) {
            portName = rtmidi->getPortName(i);
            std::cout << "  Input port #" << i << ": " << portName << '\n';
        }
        
        do {
            std::cout << "\nChoose a port number: ";
            std::cin >> i;
        } while ( i >= nPorts );
        std::getline( std::cin, keyHit );  // used to clear out stdin
    }
    
    rtmidi->openPort( i );
    
    return true;
}
                        
                        

