#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetBackgroundColor(0);
    phase = 0;
    totalSum = 0;
    ofSoundStreamSetup(2, 0); // 2 output channels (stereo), 0 input channels
    updateWaveform(32);
    ofstream myfile;
    myfile.open ("verdana.ttf");
    font.load("verdana.ttf", 14, true, true);
    
    for(int i = 0 ; i < dimension ; i++){
        for(int j = 0 ; j < dimension ; j++){
            totalSum += i*j;
            cells[i][j] = Cell::Cell();
        }
    }
    
    for(int i = 0 ; i < dimension ; i++){
        for(int j = 0 ; j < dimension ; j++){
            if(i > 0 && j > 0)
                cells[i][j].addNeighbour(&cells[i-1][j-1]);
            if(i > 0)
                cells[i][j].addNeighbour(&cells[i-1][j]);
            if(i > 0 && j < dimension)
                cells[i][j].addNeighbour(&cells[i-1][j+1]);
            if(j > 0)
                cells[i][j].addNeighbour(&cells[i][j-1]);
            if(j < dimension)
                cells[i][j].addNeighbour(&cells[i][j+1]);
            if(i < dimension && j > 0)
                cells[i][j].addNeighbour(&cells[i+1][j-1]);
            if(i < dimension)
                cells[i][j].addNeighbour(&cells[i+1][j]);
            if(i < dimension && j < dimension)
                cells[i][j].addNeighbour(&cells[i+1][j+1]);
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    float sum = 0;
    float nCellsAlive = 0;
    for(int i = 0 ; i < dimension ; i++){
        for(int j = 0 ; j < dimension ; j++){
            cells[i][j].update();
        }
    }
    for(int i = 0 ; i < dimension ; i++){
        for(int j = 0 ; j < dimension ; j++){
            cells[i][j].Cell::updateStatus();
            nCellsAlive++;
            sum += float(i*j) * float(cells[i][j].getStatus());
        }
    }

    frequency = ofMap(sum, 0, totalSum, 200, 2000, true);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    int color = ofMap(frequency, 200, 2000, 100, 255);
    ofSetColor(color);
    for(int i = 1 ; i <= dimension ; i++){
        for(int j = 1 ; j <= dimension ; j++){
            if(cells[i-1][j-1].getStatus() == 1){
                ofDrawCircle(i*spacing, j*spacing, radius);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    for(int i = 0 ; i < dimension ; i++){
        for(int j = 0 ; j < dimension ; j++){
            cells[i][j].randomizeStatus();
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::updateWaveform(int waveformResolution) {
    waveform.resize(waveformResolution);
    
    // "waveformStep" maps a full oscillation of sin() to the size
    // of the waveform lookup table
    float waveformStep = (M_PI * 2.) / (float) waveform.size();
    
    for(int i = 0; i < waveform.size(); i++) {
        waveform[i] = sin(i * waveformStep);
    }
}

void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {
    ofScopedLock waveFormLock(waveformMutex);
    float phaseStep = frequency/sampleRate;
    
    for(int i = 0; i < bufferSize * nChannels; i += 2) {
        phase += phaseStep;
        int waveformIndex = (int)(phase * waveform.size()) % (waveform.size());
        output[i] = waveform[waveformIndex];
        output[i+1] = waveform[waveformIndex];
    }
}
