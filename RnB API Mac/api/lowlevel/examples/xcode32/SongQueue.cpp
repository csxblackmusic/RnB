//
//  SongQueue.cpp
//  effects
//
//  Created by David James on 11/13/19.
//  Copyright © 2019 Firelight Technologies. All rights reserved.
//
#ifndef SongQueue_h
#define SongQueue_h
#include "Song.cpp"
class SongQueue
{
private:
    ListNode * head;
    ListNode * tail;
    int size;
    
public:
    SongQueue()
    {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    
    void push(Song song_to_add)
    {
        ListNode * newSongNode  = new ListNode(song_to_add);
        if(size==0)
        {
            head =newSongNode;
            tail = newSongNode;
            size++;
        }
        else
        {
            tail->next = newSongNode;
            tail = newSongNode;
            size++;
        }
    }
    void pop()
    {
        if(size>0)
        {
            ListNode* term = head;
            head=head->next;
            delete term;
            size--;
        }
    }
    
    Song front()
    {
        Song first = head->track;
        return first;
    }
    

    
}

#endif
