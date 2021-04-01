/*==============================================================================
Effects Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2018.

This example shows how to apply some of the built in software effects to sounds
by applying them to the master channel group. All software sounds played here
would be filtered in the same way. To filter per channel, and not have other
channels affected, simply apply the same functions to the FMOD::Channel instead
of the FMOD::ChannelGroup.
==============================================================================*/
#include "fmod.hpp"
#include "common.h"
#include "RtMidi.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>

using namespace std;

void usage( void ) {
    // Error function in case of incorrect command-line
    // argument specifications.
    cout << "\nuseage: cmidiin <port>\n";
    cout << "    where port = the device to use (default = 0).\n\n";
    exit( 0 );
}

void mycallback( double deltatime, std::vector< unsigned char > *message, void */*userData*/ )
{
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
        cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        cout << "stamp = " << deltatime << std::endl;
}

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort( RtMidiIn *rtmidi );


int FMOD_Main()
{
    FMOD::System       *system        = 0;
    FMOD::Sound        *sound         = 0;
    FMOD::Channel      *channel       = 0;
    FMOD::ChannelGroup *mastergroup   = 0; 
    FMOD::DSP          *dsplowpass    = 0;
    FMOD::DSP          *dsphighpass   = 0;
    FMOD::DSP          *dspecho       = 0;
    FMOD::DSP          *dspflange     = 0;
    FMOD_RESULT         result;
    unsigned int        version;
    void               *extradriverdata = 0;
    bool quit = false;
    RtMidiIn *midiin = 0;
    
    /*
        Create a System object and initialize
    */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
    }

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);

    result = system->getMasterChannelGroup(&mastergroup);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("drumloop.wav"), FMOD_DEFAULT, 0, &sound);
    ERRCHECK(result);

    result = system->playSound(sound, 0, false, &channel);
    ERRCHECK(result);

    /*
        Create some effects to play with
    */
    result = system->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpass);
    ERRCHECK(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpass);
    ERRCHECK(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspecho);
    ERRCHECK(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspflange);
    ERRCHECK(result);

    /*
        Add them to the master channel group.  Each time an effect is added (to position 0) it pushes the others down the list.
    */
    result = mastergroup->addDSP(0, dsplowpass);
    ERRCHECK(result);
    result = mastergroup->addDSP(0, dsphighpass);
    ERRCHECK(result);
    result = mastergroup->addDSP(0, dspecho);
    ERRCHECK(result);
    result = mastergroup->addDSP(0, dspflange);
    ERRCHECK(result);

    /*
        By default, bypass all effects.  This means let the original signal go through without processing.
        It will sound 'dry' until effects are enabled by the user.
    */
    result = dsplowpass->setBypass(true);
    ERRCHECK(result);
    result = dsphighpass->setBypass(true);
    ERRCHECK(result);
    result = dspecho->setBypass(true);
    ERRCHECK(result);
    result = dspflange->setBypass(true);
    ERRCHECK(result);
    
    
    // RtMidiIn constructor
    midiin = new RtMidiIn();
    try{
    
    // Call function to select port.
    if ( chooseMidiPort( midiin ) == false ) goto cleanup;
    
    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue instead of sent to the callback function.
    midiin->setCallback( &mycallback );
    
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );
    
    std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
    char input;
    std::cin.get(input);
    
} catch ( RtMidiError &error ) {
    error.printMessage();
}
cleanup:
    delete midiin;
    /*
        Main loop
    */
    ERRCHECK(result);
    bool paused = 0;
    bool dsplowpass_bypass;
    bool dsphighpass_bypass;
    bool dspecho_bypass;
    bool dspflange_bypass;
    
    dsplowpass   ->getBypass(&dsplowpass_bypass);
    dsphighpass  ->getBypass(&dsphighpass_bypass);
    dspecho      ->getBypass(&dspecho_bypass);
    dspflange    ->getBypass(&dspflange_bypass);
    
    if (channel)
    {
        result = channel->getPaused(&paused);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }
    }
    while(!quit)
    {

        result = system->update();
        usleep(50);
    }
    /*
        Shut down
    */
    

    result = mastergroup->removeDSP(dsplowpass);
    ERRCHECK(result);
    result = mastergroup->removeDSP(dsphighpass);
    ERRCHECK(result);
    result = mastergroup->removeDSP(dspecho);
    ERRCHECK(result);
    result = mastergroup->removeDSP(dspflange);
    ERRCHECK(result);
    
    result = dsplowpass->release();
    ERRCHECK(result);
    result = dsphighpass->release();
    ERRCHECK(result);
    result = dspecho->release();
    ERRCHECK(result);
    result = dspflange->release();
    ERRCHECK(result);

    result = sound->release();
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

    return 0;
}
