//
//  Song.cpp
//  effects
//
//  Created by David James on 10/28/19.
//  Copyright © 2019 Firelight Technologies. All rights reserved.
//

#include <queue>
#include <stack>
#include "rnb.h"
using namespace std;

class Song:public Sample
{
private:
    string artist;
    string song_title;
    string album_title;
    float duration; //in seconds
    const char * filename;
    int channel;
public:
    Song():Sample ("/Users/djames18/Downloads/FMODwithRnBOSX/api/lowlevel/examples/xcode32/media/Verse1.wav",0) //default constructor for blank songs
    {
        artist="";
        album_title="";
        song_title="";
        duration =0;
        channel =0;
        filename="Verse1.wav";
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
    float get_duration()
    {
        return duration;
    }
    
    string song_string()
    {
        return "Artist: "+artist+'\n' +"Song Title: "+song_title+'\n'+"Album Title: "+album_title+'\n'+"Song Duration: "+std::to_string(duration)+'\n';
    }
};

void play_song_list(queue<Song> songlist,stack<Song> & playedSongs)
{
    if(songlist.size()>0)
    {
        queue<Song>songlist_copy=songlist;
        for(int i=0;i<songlist.size();i++)
        {
            Song curr = songlist_copy.front();
            curr.play();
            sleep(curr.get_duration());
            playedSongs.push(curr);
            songlist_copy.pop();
        }
    }
}

void play_prev_song(queue<Song>& songlist,stack<Song> & playedSongs)
{
    Song curr = playedSongs.top();
    curr.play();
    sleep(curr.get_duration());
    playedSongs.pop();
}


int main()
{
    queue<Song> playlist;
    stack<Song> history;
    Song locationVerse1 = Song ("Khalid","Location","American Teen", 36.029,"/Users/djames18/Documents/FMOD_Programmers_APIwRnB /api/lowlevel/examples/media/Verse1.wav",0);
    Song locationVerse2 = Song ("Khalid","Location","American Teen", 21.012,"/Users/djames18/Documents/FMOD_Programmers_APIwRnB /api/lowlevel/examples/media/Verse2.wav",1);
    
    playlist.push(locationVerse1);
    playlist.push(locationVerse2);
    play_song_list(playlist, history);
    play_prev_song(playlist,history);
    play_prev_song(playlist,history);
    while(true)
    {
        locationVerse1.upd();
        sleep(50);
    }
}

