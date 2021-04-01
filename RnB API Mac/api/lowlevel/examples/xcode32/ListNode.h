//
//  ListNode.h
//  stackQueue
//
//  Created by David James on 11/13/19.
//  Copyright © 2019 Firelight Technologies. All rights reserved.
//

#ifndef ListNode_h
#define ListNode_h
#include "Song.cpp"
class ListNode
{
public:
    Song track;
    ListNode * next;
    
    ListNode(Song song_to_add)
    {
        track = song_to_add;
        next = nullptr;
    }
    
    
}
#endif /* ListNode_h */
