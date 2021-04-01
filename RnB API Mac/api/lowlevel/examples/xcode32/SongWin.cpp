//
//  Song.cpp
//  effects
//
//  Created by David James on 10/28/19.
//  Copyright © 2019 Firelight Technologies. All rights reserved.
//
#include "rnb.h"
using namespace std;

class Song:public Sample
{
private:
    string artist;
    string song_title;
    string album_title;
    int duration; //in seconds
    const char * filename;
    int channel;
public:
    Song():Sample ("",0) //default constructor for blank songs
    {
        artist="";
        album_title="";
        song_title="";
        duration =0;
        channel =0;
        filename="";
    }
    Song(string s_artist, string s_song_title,string s_album_title, int s_duration, const char * s_filename, int s_channel) : Sample(s_filename,s_channel) //creates a SOng obj with the argument you put in]]
    {
        artist = s_artist;
        song_title =s_song_title;
        album_title = s_album_title;
        duration = s_duration;
        filename = s_filename;
        channel =s_channel;
        
    }
    
    Song(const Song & old_song):Sample(old_song.filename,old_song.channel) //copy constructor
    {
        artist = old_song.artist;
        song_title = old_song.song_title;
        album_title = old_song.album_title;
        duration = old_song.duration;
        filename  = old_song.filename;
        channel = old_song.channel;
        
    }
    string song_string()
    {
        return "Artist: "+artist+'\n' +"Song Title: "+song_title+'\n'+"Album Title: "+album_title+'\n'+"Song Duration: "+std::to_string(duration)+'\n';
    }
};
int main()
{
    Song locationVerse1 = Song ("Khalid","Location","American Teen", 350,"Verse1.wav",0);
    Song locationVerse2 = Song ("Khalid","Location","American Teen", 350,"Verse2.wav",1);
    for (int i =0;i<3;i++)
    {
        locationVerse1.play();
        Sleep(36.029);
        locationVerse2.play();
        Sleep(21.012);
    }
    while(true)
    {
        locationVerse1.upd();
        Sleep(50);
    }
}
