Once you’ve unzipped go to  /api/lowlevel/examples/xcode32/RnBSampleProj.xcodeproj 
Set /FMODwithRnBOSX/api/lowlevel/examples/xcode32 as your custom working directory.
1. On XCode click on Product->Scheme->Edit Scheme
2. Click on “Use custom working directory” and set it to the path above
3. Run rnbsample.cpp




Using RnB with OSC 
If you would like to connect with another device using osc - add /api/lowlevel/examples/xcode32/oscdatasample.cpp to your project and remove rnbsample.cpp


Using RnB with to get MIDI Hardware data in
add /api/lowlevel/examples/xcode32/midiInSampleOSX.cpp to your project and remove rnbsample.cpp


Using RnB to send out MIDI data 
add /api/lowlevel/examples/xcode32/midiOutSample.cpp to your project and remove rnbsample.cpp