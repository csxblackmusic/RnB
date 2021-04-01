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
//#include <unistd.h>
#include "windows.h"
#include <cstdlib>
#include <thread>
#define OSCPKT_OSTREAM_OUTPUT
//#define PORT_NUM 8000
#include "oscpkt.hh"
#include "math.h"
#include <iterator>
#include <iostream>
#include <map>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace oscpkt;

FMOD_RESULT result;
/*

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
*/
unsigned int num_ticks = 0;
time_t start;
time_t end_time;
unsigned char prev_dir=0;
unsigned char curr_dir=0;
unsigned int prev_pitch=1;
unsigned int curr_pitch=1;
double last_velocity=0;
char backcount=0;
char fwdcount =0;
vector<double> deltas;
class osc_rec;
void osc_server2(osc_rec &osc_obj);
void mycallback( double deltatime, std::vector< unsigned char > *message, void */*userData*/ )
{

    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
        cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        cout << "stamp = " << deltatime << std::endl;
   // automat(message);
    //numark(message,deltatime);
    
}

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort( RtMidiIn *rtmidi );

typedef void(*boolfunc)(const string,std::vector<bool>);
typedef void(*intfunc)(const string,std::vector<int>);
typedef void(*int64func)(const string,std::vector<long>);
typedef void(*floatfunc)(const string,std::vector<float>);
typedef void(*doublefunc)(const string,std::vector<double>);
vector<unsigned int> time_diffs;
class midIn
{
private:
    RtMidiIn * min;
    
public:
    midIn(void(*func)(double,std::vector<unsigned char>*,void *))
    {
        min = new RtMidiIn();
        try
        {
            
            // Call function to select port.
            if ( chooseMidiPort( min ) == false )
            {
                cout<<"Midi not connected"<<endl;
            }
            else
            {
                // Set our callback function.  This should be done immediately after
                // opening the port to avoid having incoming messages written to the
                // queue instead of sent to the callback function.
                min->setCallback( func);
                
                // Don't ignore sysex, timing, or active sensing messages.
                min->ignoreTypes( false, false, false );
                
                std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
            }
        }
        catch ( RtMidiError &error )
        {
            error.printMessage();
        }
    }
    ~midIn()
    {
        delete min;
    }
    
};

class syst
{
private:
    static FMOD::System* instance;
    syst()
    {
        FMOD::System_Create(&instance);
    }
public:
        static FMOD::System* get_syst()
        {
            if( instance==0)
            {
                syst();
            }
            return instance;
        }
};

class chans
{
private:
    static FMOD::Channel** channels;
    chans()
    {
            channels =new FMOD::Channel*[32];
      
    }
public:
    static FMOD::Channel** get_chans()
    {
        if (channels==nullptr)
        {
            chans();
        }
        return channels;
    }
};

FMOD::System* syst::instance =nullptr;
FMOD::Channel** chans::channels = nullptr;
class Sample
{
private:
    FMOD::System* soundsystem;
    FMOD::Channel** chan;
    static vector<Sample*> Samples;
    FMOD::Sound * snd;
    unsigned int version;
    const char * filename;
    void * extradriverdata=0;
    int chan_index;
    map<string,FMOD::DSP*> effects;
    //insert member variables to keep track of DSP on a sound
public:
    Sample(const char * fname, int channel,bool loop=false)
    {
        if(channel>=0 && channel<=32)
        {
            chan_index = channel;
            chan = chans::get_chans();
            cout<<"here's the memory address of the channel array"<<chan<<endl;
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
            soundsystem = syst::get_syst();
        }
        soundsystem->getVersion(&version);
        soundsystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
		if (!loop)
		{
			result=soundsystem->createSound(filename, FMOD_LOOP_OFF, 0, &snd);
		}
		else
		{
			result = soundsystem->createSound(filename, FMOD_LOOP_NORMAL, 0, &snd);
		}
		result = soundsystem->playSound(snd,0,true,&(chan[chan_index]));
    }
    void play()
    {
        Samples.push_back(this);
        if(snd!=nullptr && chan[chan_index]!=nullptr&&soundsystem!=nullptr)
        {
            cout<<"Here is the address of the channel pointer"<<&(chan[chan_index])<<endl;
            result=soundsystem->playSound(snd,0,false,&(chan[chan_index]));
            cout<<"here's the memory address of the channel array"<<chan<<endl;
            cout<<"playing on channel "<<chan_index<<"on memory address "<<chan[chan_index]<<endl;
        }
        else
        {
            cout<<"Either the sound or the channel hasnt been properly initialized"<<endl;
        }
    }
    void add_echo()
    {
        FMOD::DSP * echoptr;
        soundsystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &echoptr);
        effects.insert(pair<string,FMOD::DSP*>("echo",echoptr));
        echoptr->setBypass(false);
        (chan[chan_index])->addDSP(0, (effects.find("echo"))->second);
    }
    void add_flange()
    {
        FMOD::DSP * flangeptr;
        soundsystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &flangeptr);
        effects.insert(pair<string,FMOD::DSP*>("flange",flangeptr));
        flangeptr->setBypass(false);
        (chan[chan_index])->addDSP(0, (effects.find("flange"))->second);
    }
    void mute_flange()
    {
        map<string,FMOD::DSP*>::iterator itr =effects.find("flange");
        if(itr!=effects.end())
            (itr->second)->setBypass(true);
    }
    void unmute_flange()
    {
        map<string,FMOD::DSP*>::iterator itr =effects.find("flange");
        if(itr!=effects.end())
            (itr->second)->setBypass(false);
    }
    
    void mute_echo()
    {
        map<string,FMOD::DSP*>::iterator itr =effects.find("echo");
        if(itr!=effects.end())
            (itr->second)->setBypass(true);
    }
    void unmute_echo()
    {
        map<string,FMOD::DSP*>::iterator itr =effects.find("echo");
        if(itr!=effects.end())
            (itr->second)->setBypass(false);
    }
    
    void set_speed(float speed)
    {
        if(abs(speed)<=100 && abs(speed)>=0)
        {
            float frequency;
            int priority;
            snd->getDefaults(&frequency, &priority);
            cout<<"default frequency of the sound is: "<<frequency<<endl;
            if(chan!=nullptr&&chan[chan_index]!=nullptr)
            {
                chan[chan_index]->setFrequency(speed*frequency);
                cout<<"setting to rate of "<<speed*frequency<<" samples per sec"<<endl;
            }
            else
            {
                cout<<"either the channel or the array is empty"<<endl;
            }
        }
        else
        {
            
            cout<<"speed not set between 100 and 0"<<endl;
        }
       

    }
    
    void set_volume(float volume)
    {
        if(chan[chan_index]!=nullptr && chan!=nullptr)
        {
            if(volume<=1.0 && volume>=0.0)
            {
                chan[chan_index]->setVolume(volume);
            }
            else
            {
                cout<<"Volume must be set between 0.0 and 1.0"<<endl;
            }
        }
    }
    
    
    FMOD::Channel* get_channel() //  if the api user needs to do anything more advanced they can have direct access to the sound
    {
        if(chan!=nullptr && chan[chan_index]!=nullptr)
            return chan[chan_index];
        else
        {
            cout<<"channel is empty...returning null"<<endl;
            return nullptr;
        }
        
    }
    void upd()
    {
        if(soundsystem!=nullptr)
            soundsystem->update();
    }
    
    float get_samples_per_sec()
    {
        float frequency;
        int priority;
        snd->getDefaults(&frequency, &priority);
        return frequency;
    }
    
    unsigned int get_position()
    {
        if(chan!=nullptr && chan[chan_index]!=nullptr)
        {   unsigned int pos;
            chan[chan_index]->getPosition(&pos,FMOD_TIMEUNIT_PCM);
            return pos;
        }
        else
        {
            cout<<"channel is now empty"<<endl;
        }
    }
    
    void set_position(unsigned int pos)
    {
        if(chan!=nullptr && chan[chan_index]!=nullptr)
        {
            chan[chan_index]->setPosition(pos,FMOD_TIMEUNIT_PCM);
        }
        else
        {
            cout<<"channel is now empty not able to set the position"<<endl;
        }
    }
    
    void close_sound()
    {
        if(soundsystem!=nullptr)
        {
            if(effects.size()>0)
            {
                for(map<string,FMOD::DSP*>::iterator itr = effects.begin();itr!=effects.end();++itr)
                {
                    if(chan[chan_index]!=nullptr)
                    {
                        chan[chan_index]->removeDSP(itr->second);
                        cout<<"removing DSP"<<endl;
                    }
                    (itr->second)->release();//releases the DSP memory
                    cout<<"releasing channel"<<endl;
                    
                }
            }
            snd->release();
            cout<<"releasing sound"<<endl;
        }
    }
    
    
    void close_all ()
    {
		if (Samples.size() > 0)
		{
			for (int i = 0; i < Samples.size(); i++)
			{
				Sample* smp = Samples.at(i);
				smp->close_sound();
			}
		}
		soundsystem->close();
		soundsystem->release();
		cout << "closing and releasing system" << endl;
    }
    ~Sample()
    {
        cout<<"destructor running"<<endl;
    }
};
vector<Sample*> Sample::Samples;
Sample samp1 = Sample("..\\media\\samples\\location\\Verse1.wav",0,false);
const int PORT_NUM = 8000;

void get_velocity(const string address, vector<float> vel)
{
    samp1.set_speed(vel.at(0));
}

void osc_server()
{
    UdpSocket sock;
    bool showalldata = false;
    sock.bindTo(PORT_NUM);
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
                                samp1.set_speed(velocity);
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
class osc_rec
{
public:
    unsigned int osc_port;
    void(*bool_callback)(const string,vector<bool>);
    void(*int_callback)(const string,vector<int>);
    void(*float_callback)(const string,vector<float>);
    void(*int64_callback)(const string,vector<long>);
    void(*double_callback)(const string,vector<double>);
    string functype; //could be bool, string, int, int64
    string address = "";
	std::thread osc_data_proc;
    osc_rec(unsigned int port,const string search_address="",void(*func)(const string,vector<float>)=nullptr)
    {
        osc_port = port;
        functype = "float";
        float_callback = func;
        address=search_address;
    }
    osc_rec(unsigned int port,const string search_address="",void(*func)(const string,std::vector<int>)=nullptr)
    {
        osc_port = port;
        functype = "int";
        int_callback = func;
        address=search_address;
    }
    
    osc_rec(unsigned int port,const string search_address="",void(*func)(const string,std::vector<double>)=nullptr)
    {
        osc_port = port;
        functype = "double";
        double_callback = func;
        address=search_address;
    }
    
    string get_functype()
    {
        return functype;
    }
    string get_address()
    {
        return address;
    }
    
    unsigned int get_port()
    {
        return osc_port;
    }
    
    osc_rec(unsigned int port,const string address="",void(*func)(const string,std::vector<long>)=nullptr)
    {
        osc_port = port;
        functype = "int64";
        int64_callback  = func;
    }
    int64func get_int64_callback()
    {
        return int64_callback;
    }
    
    floatfunc get_float_callback()
    {
        return float_callback;
    }
    boolfunc get_bool_callback()
    {
        return bool_callback;
    }
    
    intfunc get_int_callback()
    {
        return int_callback;
    }
    
    doublefunc get_double_callback()
    {
        return double_callback;
    }
    
    void set_callback(void(*func)(const string,std::vector<bool> ))
    {
        functype = "bool";
        bool_callback = func;
    }
    void set_callback(void(*func)(const string,std::vector<int> ))
    {
        functype = "int";
        int_callback = func;
    }
    void start()
    {
        
        osc_data_proc = std::thread(osc_server2,std::ref(*this));
		//osc_data_proc.join();
    }
	void end()
	{
		std::terminate();
	}
    
};

void process_gyro(const string path,vector<float> data)
{
    cout<<"recieved "<<data.at(0)<<" as the x rotation"<<endl;
    cout<<"recieved "<<data.at(1)<<" as the y rotation"<<endl;
    cout<<"recieved "<<data.at(2)<<" as the z rotation"<<endl;
}

void osc_server2(osc_rec &osc_obj)
{
    UdpSocket sock;
    bool showalldata = false;
    if( osc_obj.get_address() =="")
    {
        bool showalldata = true;
    }
    sock.bindTo(osc_obj.get_port());
    if (!sock.isOk())
    {
        cerr << "Error opening port " << PORT_NUM << ": " << sock.errorMessage() << "\n";
    }
    else
    {
        cout << "Server started, will listen to packets on port " << osc_obj.get_port() << std::endl;
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
                        vector<int> intdata;
                        vector<long> int64data;
                        vector<float> floatdata;
                        vector<bool> booldata;
                        vector<double> doubledata;
                        if(addr==osc_obj.get_address()||showalldata==true)
                        {
                            while(arg.nbArgRemaining())
                            {
                                if(arg.isInt32() && osc_obj.get_functype()=="int")
                                {
                                    int a;
                                    arg.popInt32(a);
                                    cout<<a<<" received from "<<addr<<endl;
                                    intdata.push_back(a);
                                }
                                else if (arg.isInt64()&& osc_obj.get_functype()=="int64")
                                {
                                    int64_t b;
                                    arg.popInt64(b);
                                    cout<<b<<" received from "<<addr<<endl;
                                    int64data.push_back(b);
                                }
                                else if(arg.isFloat()&& osc_obj.get_functype()=="float")
                                {
                                    float dec;
                                    arg.popFloat(dec);
                                    cout<<dec<<" received from "<<addr<<endl;
                                    floatdata.push_back(dec);
                                }
                                else if(arg.isBool()&& osc_obj.get_functype()=="bool")
                                {
                                    bool tf;
                                    arg.popBool(tf);
                                    cout<<tf<<" received from "<<addr<<endl;
                                    booldata.push_back(tf);
                                }
                                else if(arg.isStr())
                                {
                                    string s;
                                    arg.popStr(s);
                                    cout<<s<<" received from "<<addr<<endl;
                                }
                            }
                            if(osc_obj.get_functype()=="bool")
                            {
                                osc_obj.get_bool_callback()(addr,booldata);
                            }
                            else if(osc_obj.get_functype()=="int")
                            {
                                osc_obj.get_int_callback()(addr,intdata);
                            }
                            else if(osc_obj.get_functype()=="float")
                            {
                                osc_obj.get_float_callback()(addr,floatdata);
                            }
                            else if(osc_obj.get_functype()=="int64")
                            {
                                osc_obj.get_int64_callback()(addr,int64data);
                            }
                            else if(osc_obj.get_functype()=="double")
                            {
                                osc_obj.get_double_callback()(addr,doubledata);
                            }
                        }
                    }
                }
            }
        }
    }
void osc_server1()
{
    UdpSocket sock;
    bool showalldata = false;
    sock.bindTo(PORT_NUM);
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
                    
                    if(addr=="/muse/gyro"||showalldata==true)
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
                                float dec;
                                arg.popFloat(dec);
                                cout<<dec<<" received from "<<addr<<endl;
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
                            else if(arg.isFloat())
                            {
                                float f;
                                arg.popFloat(f);
                                cout<<f<<" received from "<<addr<<endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

void othercallback( double deltatime, std::vector< unsigned char > *message, void */*userData*/ )
{
    double velocity=1;
    char sign = 1;
    //short combined = (msb << 8 ) | (lsb & 0xff);
    if(num_ticks==0)
    {
        
        time(&start);
        cout<<"Starting timer"<<endl;
    }
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
       cout <<"2nd dev "<< "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        cout << "stamp = " << deltatime << std::endl;
    if(message->at(0)==224)
    {
        double velocity = 0.000996793/deltatime;
        deltas.push_back(deltatime);
    }
    if(message->at(0)==144 &&message->at(1) ==52)
    {
        samp1.set_speed(0);
    }
    ++num_ticks;

    
    if(message->at(0)==176 && message->at(1)==6)
    {
        prev_dir = curr_dir;
        curr_dir = message->at(2);
        if(curr_dir>=prev_dir)
        {
            ++fwdcount;
            if (fwdcount>1)
            {
            sign = 1;
            //cout<<"FORWARD!!!!!"<<endl;
            fwdcount=0;
            }
        }
        else
        {
            ++backcount;
            if (backcount>1)
            {
                sign =-1;
           // cout<<"BACKWARD!!!!!"<<endl;
                backcount=0;
            }
        }
        
    }
    if(abs(last_velocity-velocity)>.2)
    {
        samp1.set_speed(sign*velocity);
    }
    last_velocity = velocity;
    /*
    if(message->at(0)==224 )
    {
        unsigned int lsb = message->at(1);
        unsigned int msb = message->at(2);
        unsigned int comb = msb<<7|lsb;
        prev_pitch = curr_pitch;
        curr_pitch =comb;
        float ratio_comp= static_cast<float>(comb)/static_cast<float>(prev_pitch);
        float ratio_pitch =static_cast<float>(comb)/16383.0;
        cout<<"The combined pitch bend value is: "<<comb<<endl; //16383 is the max value
        cout<<"The difference between the last two values is "<< comb-prev_pitch<<endl;
        cout<<"The ratio between the last two values is"<<ratio_comp<<endl;
        cout<<"The ratio between the curr value and 16383 is"<<ratio_pitch<<endl;
    }
    */

    // automat(message);
    //numark(message,deltatime);
    
}

int main()
{
   // midIn device = midIn(mycallback);
   // midIn device2 = midIn(othercallback);
    
    samp1.play();
    cout<<"executed play method"<<endl;
    samp1.set_volume(0.9);
	cout << "added echo" << endl;
    //samp1.add_echo();
    FMOD::Channel* ch1 =samp1.get_channel();
    bool running= true;
    cout<<"entering engine loop"<<endl;
	osc_rec osc_receive = osc_rec(8000,"/accxyz",process_gyro);
    osc_receive.start();
    while(running)
    {
        samp1.upd();
        Sleep(50);
        ch1->isPlaying(&running);
    }
    cout<<"ended engine loop"<<endl;
    samp1.close_all();
    cout<<"closed sample"<<endl;
	//int a; 
	//cin >> a;
  /*  time(&end_time);
    double diff =difftime(start, end_time);
    cout<<diff<<" seconds have elapsed"<<endl;
    cout<<num_ticks<<" have occured "<<endl;
    cout<<"turntable is producing "<<num_ticks/diff<<"ticks per second at 33 RPM"<<endl;
    double sum=0;
    for (int i=0;i<deltas.size();i++)
    {
        sum+= deltas.at(i);
    }
    cout<<"the avg delta at 33 and 1/3 rpm is"<< sum/deltas.size()<<endl; //avg delta is 0.000996906
    */ 
	return 0;
}
/*
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

    while(!quit)
    {
        result = soundsystem->update();
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
*/
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


                        
                        

