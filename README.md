# RnB
RnB is short for Rhythm and Bytes. RnB is a C++ API that allows you  to play and apply effects to audio, while adding MIDI and OSC as interfaces to your software. It does all this by managing memory and references to audio so that beginning C++ programmers can create audio software. Programmers using the API can play and manipulate audio by creating Sample objects, and calling member functions to accomplish their goals.

The code for the api is in the /api/lowlevel/inc/rnb.h

Instructions on running a sample project that will play/manipulate audio on   Mac

1. Once you have pulled the repository go to /api/lowlevel/examples/xcode32/
2. Open randbSampleProj.xcodeproj and build the project with rnbsample.cpp
3. You will hear two samples playing at the same time with effects applied to them. 
4. Code for the example is in /api/lowlevel/examples/xcode32/randbsample.cpp and demonstrates the constructor and other methods furnished by the Sample Object

Instructions on running a sample project that will play/manipulate audio on Windows

1. Once you have pulled the repository go to /api/lowlevel/examples/vs2012/
2. Open RnBSampleProj.vcxproj and build the project with rnbsample.cpp
3. You will hear two samples playing at the same time with effects applied to them. 
4. Code for the example is in /api/lowlevel/examples/vs2012/randbsample.cpp

