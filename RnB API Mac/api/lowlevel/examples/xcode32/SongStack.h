//
//  SongStack.h
//  stackQueue
//

#include <list>
#include "Song.cpp"
using namespace std;
class SongStack
{
private:
    list<Song> playlist;
    
public:
    void push(Song track)
    {
        playlist.push_front(track);
    }
    
    void pop()
    {
        if(size>=0)
            playlist.pop_front();
    }
    
    Song top()
    {
        return playlist.front();
    }
}
