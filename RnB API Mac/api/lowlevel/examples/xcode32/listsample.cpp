//
//  listsample.cpp
//  effects
//
//  Created by David James on 10/23/20.
//  Copyright © 2020 Firelight Technologies. All rights reserved.
//
#include "rnb.h"
#include <list>
#include <iostream>
using namespace std;


int main()
{
    Sample doit = Sample("class_songs/doit.mp3",0);
    Sample ungodlyhour = Sample("class_songs/ungodlyhour.mp3",1);
    Sample eleven = Sample("class_songs/eleven.mp3",2);
    list<Sample> playlist = {doit,ungodlyhour,eleven};
    list<Sample>::iterator i;
    for(i=playlist.begin();i!=playlist.end();i++)
    {
        i->play();
        usleep(i->get_length()*1000);//windows only Sleep(i->get_length());
    }
    
    doit.play();
    usleep(doit.get_length()*1000);//windows only  Sleep(doit.get_length());
    ungodlyhour.play();
    
    
}




