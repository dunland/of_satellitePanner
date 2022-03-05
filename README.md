# of_satellitePanner
panning through moving images of satellites to distort them with sound in openframeworks / pure-data

## required addons

(as listed in `addons.make` have to be cloned to `openframeworks/addons`)

ofxOsc
ofxGui
ofxCv
ofxGui
ofxOpenCv
(ofxFFmpegRecorder)
ofxSimpleSerial https://github.com/LeoColomb/ofxSimpleSerial

## Communication

### teensy

this is complicated.  
the Python sketch is required. you need to `pip3 install python-osc` 

1. upload teensy sketch
2. within only some seconds, start running the python sketch (teensy only has a small time window for the serial port to start)
3. start the openframeworks-app


### MIDI Controller

handled via Pd-sketch

