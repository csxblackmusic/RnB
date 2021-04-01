#include "rnb.h"
using namespace std;
Sample samp1 = Sample("/Users/djames18/Documents/FMOD_Programmers_APIwRnB /api/lowlevel/examples/media/Verse1.wav", 0, true); //Provide the full path to a wav file that youd want to play - the second argument says the sound will play on channel 0 - Only one sound can play on a channel at a time - The third argunment specifies whether the sample will loop continuously or not

void process_muse(const string path, vector<float> data)//since no address is specified on line 23 all float data regardless of address will go here - non float data wont be picked up here.
{
	if(path =="/muse/algorithm/concentration")
    {
        cout<<"Received a value of: "<<data.at(0)<<"for concentration"<<endl;
        samp1.set_volume(data.at(0)/100);//sets the volume of the sample to the concentration level
    }
    else if(path=="/muse/algorithm/mellow")
    {
        cout<<"Received a value of: "<<data.at(0)<<"for mellow"<<endl;
    }
    else if(path=="/muse/gyro")
    {
         cout<<"Received a value of: "<<data.at(0)<<"for x" <<path<<endl;
        cout<<"Received a value of: "<<data.at(1)<<"for y" <<path<<endl;
        cout<<"Received a value of: "<<data.at(2)<<"for z" <<path<<endl;
    }
    else if(path=="/muse/acc")
    {
        cout<<"Received a value of: "<<data.at(0)<<"for x" <<path<<endl;
        cout<<"Received a value of: "<<data.at(1)<<"for y" <<path<<endl;
        cout<<"Received a value of: "<<data.at(2)<<"for z" <<path<<endl;
    }
        
}
int main()
{
    osc_rec osc_receive(8000,"", process_muse); //routes the data to the process muse function above
    osc_receive.start();    //sets the computer up to receive the data on port 8000
	while (true)
	{
	}
}
