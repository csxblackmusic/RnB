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
vector<double> deltas;
class osc_rec;
void osc_server2(osc_rec &osc_obj);
/*
void mycallback( double deltatime, std::vector< unsigned char > *message, void *)
{

    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
        cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        cout << "stamp = " << deltatime << std::endl;
    //numark(message,deltatime);
    
}
*/
// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort( RtMidiIn *rtmidi );

typedef void(*boolfunc)(const string,std::vector<bool>);
typedef void(*intfunc)(const string,std::vector<int>);
typedef void(*int64func)(const string,std::vector<long>);
typedef void(*floatfunc)(const string,std::vector<float>);
typedef void(*doublefunc)(const string,std::vector<double>);
typedef void(*stringfunc)(const string, std::vector<string>);

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
            if( instance==nullptr)
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
    FMOD::System* soundsystem= nullptr;
	FMOD::Channel** chan = nullptr;
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
        result = soundsystem->getVersion(&version);
        soundsystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
		if (!loop)
		{
			result=soundsystem->createSound(filename, FMOD_LOOP_OFF, 0, &snd);
			if (result != FMOD_OK)
			{
				cerr << "Your sound file was not found" << endl;
			}
		}
		else
		{
			result = soundsystem->createSound(filename, FMOD_LOOP_NORMAL, 0, &snd);
			if (result != FMOD_OK)
			{
				cerr << "Your sound file "<<filename<<" was not found" << endl;
			}
		}
		if (result == FMOD_OK)
		{
			result = soundsystem->playSound(snd, 0, true, &(chan[chan_index]));
		}
		else
		{
			cerr << "couldnt construct Sample object properly" << endl;
		}
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
		
        result = soundsystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &echoptr);
		if (result != FMOD_OK)
		{
			cerr << "There was an error creating the echo effect" << endl;
		}
        effects.insert(pair<string,FMOD::DSP*>("echo",echoptr));
        result= echoptr->setBypass(false);
		if (result != FMOD_OK)
		{
			cerr << "There was an errormaking the echo effect active" << endl;
		}
	    result=(chan[chan_index])->addDSP(0, (effects.find("echo"))->second);
		if (result != FMOD_OK)
		{
			cerr << "There was an error adding the echo effect to the sample" << endl;
		}
    }
    void add_flange()
    {
        FMOD::DSP * flangeptr;
        soundsystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &flangeptr);
        effects.insert(pair<string,FMOD::DSP*>("flange",flangeptr));
        flangeptr->setBypass(false);
        (chan[chan_index])->addDSP(0, (effects.find("flange"))->second);
    }

	void add_bass_mid_treble_ctrl()
	{
		FMOD::DSP * bassmidstreb;
		soundsystem->createDSPByType(FMOD_DSP_TYPE_THREE_EQ, &bassmidstreb);
		effects.insert(pair<string, FMOD::DSP*>("eq", bassmidstreb));
		bassmidstreb->setBypass(false);
		(chan[chan_index])->addDSP(0, (effects.find("eq"))->second);
	}
	void bypass_bass_mid_treb_ctrl()
	{
		FMOD::DSP* eq = (effects.find("eq"))->second;
		eq->setBypass(false);
	}
	void set_bass(float level)
	{
		FMOD::DSP* eq = (effects.find("eq"))->second;
		if(level<=80 && level>=-10)
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_LOWGAIN, level);
		}
		else if (level < -10)
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_LOWGAIN, -10);
		}
		else
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_LOWGAIN, 80);
		}
	}
	void set_mids(float level)
	{
		FMOD::DSP* eq = (effects.find("eq"))->second;
		if (level <= 80 && level >= -10)
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_MIDGAIN, level);
		}
		else if (level < -10)
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_MIDGAIN, -10);
		}
		else
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_MIDGAIN, 80);
		}
	}
	void set_highs(float level)
	{
		FMOD::DSP* eq = (effects.find("eq"))->second;
		if (level <= 80 && level >= -10)
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_HIGHGAIN, level);
		}
		else if (level < -10)
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_HIGHGAIN, -10);
		}
		else
		{
			eq->setParameterFloat(FMOD_DSP_THREE_EQ_HIGHGAIN, 80);
		}
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
class osc_rec
{
public:
    unsigned int osc_port;
    void(*bool_callback)(const string,vector<bool>);
    void(*int_callback)(const string,vector<int>);
    void(*float_callback)(const string,vector<float>);
    void(*int64_callback)(const string,vector<long>);
    void(*double_callback)(const string,vector<double>);
	void(*string_callback)(const string, vector<string>);
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
	osc_rec(unsigned int port, const string search_address = "", void(*func)(const string, std::vector<string>) = nullptr)
	{
		osc_port = port;
		functype = "string";
		string_callback = func;
		address = search_address;
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

	stringfunc get_string_callback()
	{
		return string_callback;
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
	void set_callback(void(*func)(const string, std::vector<float>))
	{
		functype = "float";
		float_callback = func;
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
/*
void process_gyro(const string path,vector<float> data)
{
    cout<<"recieved "<<data.at(0)<<" as the x rotation"<<endl;
    cout<<"recieved "<<data.at(1)<<" as the y rotation"<<endl;
    cout<<"recieved "<<data.at(2)<<" as the z rotation"<<endl;
}
*/
void osc_server2(osc_rec &osc_obj)
{
	UdpSocket sock;
	bool showalldata = false;
	if (osc_obj.get_address() == "")
	{
		showalldata = true;
	}
	sock.bindTo(osc_obj.get_port());
	if (!sock.isOk())
	{
		cerr << "Error opening port " << osc_obj.get_port() << ": " << sock.errorMessage() << "\n";
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
					vector<string> stringdata;
					if (addr == osc_obj.get_address() || showalldata == true)
					{
						while (arg.nbArgRemaining())
						{
							if (arg.isInt32() && osc_obj.get_functype() == "int")
							{
								int a;
								arg.popInt32(a);
								if (showalldata)
								{
									cout << a << " received from " << addr << endl;
								}
								intdata.push_back(a);
							}
							else if (arg.isInt64() && osc_obj.get_functype() == "int64")
							{
								int64_t b;
								
								arg.popInt64(b);
								if (showalldata)
								{
									cout << b << " received from " << addr << endl;
								}
								int64data.push_back(static_cast<long>(b));
							}
							else if (arg.isFloat() && osc_obj.get_functype() == "float")
							{
								float dec;
								arg.popFloat(dec);
								if (showalldata)
								{
									cout << dec << " received from " << addr << endl;
								}
								floatdata.push_back(dec);
							}
							else if (arg.isBool() && osc_obj.get_functype() == "bool")
							{
								bool tf;
								arg.popBool(tf);
								if (showalldata)
								{
									cout << tf << " received from " << addr << endl;
								}
								booldata.push_back(tf);
							}
							else if (arg.isStr())
							{
								string s;
								arg.popStr(s);
								if (showalldata)
								{
									cout << s << " received from " << addr << endl;
								}
								stringdata.push_back(s);
							}
						}
						if (osc_obj.get_functype() == "bool")
						{
							osc_obj.get_bool_callback()(addr, booldata);
						}
						else if (osc_obj.get_functype() == "int")
						{
							osc_obj.get_int_callback()(addr, intdata);
						}
						else if (osc_obj.get_functype() == "float")
						{
							osc_obj.get_float_callback()(addr, floatdata);
						}
						else if (osc_obj.get_functype() == "int64")
						{
							osc_obj.get_int64_callback()(addr, int64data);
						}
						else if (osc_obj.get_functype() == "double")
						{
							osc_obj.get_double_callback()(addr, doubledata);
						}
						else if (osc_obj.get_functype() == "string")
						{
							osc_obj.get_string_callback()(addr, stringdata);
						}

					}
				}
			}
		}
	}
}

bool chooseMidiPort( RtMidiIn *rtmidi )
{
    std::cout << "\nWould you like to open a virtual input port? [y/N] ";
    
    std::string keyHit;
    std::getline( std::cin, keyHit );
    if ( keyHit == "y" ) 
	{
        rtmidi->openVirtualPort();
        return true;
    }
    
    std::string portName;
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if ( nPorts == 0 ) 
	{
        std::cout << "No input ports available!" << std::endl;
        return false;
    }
    
    if ( nPorts == 1 ) 
	{
        std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
    }
    else 
	{
        for ( i=0; i<nPorts; i++ ) 
		{
            portName = rtmidi->getPortName(i);
            std::cout << "  Input port #" << i << ": " << portName << '\n';
        }
        
        do 
		{
            std::cout << "\nChoose a port number: ";
            std::cin >> i;
        } while ( i >= nPorts );
        std::getline( std::cin, keyHit );  // used to clear out stdin
    }
    
    rtmidi->openPort( i );
    
    return true;
}


                        
                        

