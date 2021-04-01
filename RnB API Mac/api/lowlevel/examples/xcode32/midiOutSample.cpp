#include "rnb.h"
#include <vector>
Sample samp1 = Sample("Verse1.wav", 0); //The second argument indicates the channel - only one sample can be played per channel - you can have a max of 32 channels
											  //the third argument indicates that the sample should play in loop mode where it loops until you stop the program
											  //Put the full file path to your sound for the first argument - for now it only plays wav files
Sample samp2 = Sample("Verse2.wav", 1);
vector<Sample> samples;


class Song
{
private:
	string artist;
	Sample stuff;

public:
	Song(string a_name, const char * filename, int chan)
	{
		artist = a_name;
		stuff = Sample(filename, chan);
	}
	void play()
	{
		cout<<"Now playing "<<artist<<endl;
		stuff.play();
		Sleep(stuff.get_length());
	}

};
int main()
{
	Song location = Song("Khalid", "Verse1.wav", 0);
	location.play();
	midOut ms50 = midOut();
	vector<unsigned char> msg = { 192,45 };
	ms50.send_message(msg);
	sleep(500);
}
