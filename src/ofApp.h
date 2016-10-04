#pragma once

#include "ofMain.h"
#include <vector>
#include "Cell.h"
#include <string>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        int static const dimension = 50;
        Cell cells[dimension][dimension];
        int totalSum;
    
        //GUI Parameters
        float spacing = 10.F;
        float radius = 3.F;
        ofTrueTypeFont font;
    
        //Sound variables
        double phase;
        float frequency;
        float sampleRate = 44100;
        std::vector<float> waveform;

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void updateWaveform(int waveformResolution);
        void audioOut( float * output, int bufferSize, int nChannels );
    
        ofMutex waveformMutex;
};
