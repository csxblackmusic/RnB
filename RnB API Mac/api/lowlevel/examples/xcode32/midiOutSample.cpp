#include "rnb.h"
#include <vector>
midOut ms50 = midOut();
vector <unsigned char> msg2= { 0xf0,0x52,0x00,0x58,0x31,0x00,0x04,0x00,0x00,0xf7 }; //7th value changes the knob value
//6th value chooses the selected param
//2 IS THE FIRST PARAM on the first page, 3 IS THE 2ND, 4 IS THE 3RD on the first pg
//5 is the first param on the 2nd pg, 6 is the sex
//Sets the mix knob to 0 bt default

void process_muse(const string path, vector<float> data)//since no address is specified on line 23 all float data regardless of address will go here - non float data wont be picked up here.
{

    if(path=="/muse/algorithm/mellow")
    {
        cout<<"Received a value of: "<<data.at(0)<<"for mellow"<<endl;
        unsigned char knob_val  = static_cast<unsigned char>(data.at(0));
        msg2[7]= knob_val;
        ms50.send_message(msg2);
    }
    if(path=="/muse/algorithm/concentration")
    {
        cout<<"Received a value of: "<<data.at(0)<<"for concentration"<<endl;
        unsigned char knob_val  = static_cast<unsigned char>(data.at(0));
        msg2[7]= knob_val;
        ms50.send_message(msg2);
    }
}

int main()
{
    osc_rec osc_receive(8000,"/muse/algorithm/concentration", process_muse); //muse algorithms only work if you put the exact path in here
    osc_receive.start();    //sets the computer up to receive the data on port 8000
  
	vector<unsigned char> msg = { 0xc0,32 }; //change to  patch 33
    vector<unsigned char> msg1= { 0xf0,0x52,0x00,0x58,0x50,0xf7 }; //change to edit parameter mode
    ms50.send_message(msg);
    ms50.send_message(msg1);
    ms50.send_message(msg2);
   
    while (true)
    {
        sleep(200);
    }
}
