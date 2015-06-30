#ifndef _TEST_APP
#define _TEST_APP
#include "ofMain.h"
#include "ofxOpenNI.h"
#include "Joya.h"
#include "WakeUp.h"
class testApp : public ofBaseApp {
private:
public:
    void setup();
    void update();
    void draw();
    void keyPressed (int key);
    void keyReleased (int key);
    void mouseMoved (int x, int y);
    void mouseDragged (int x, int y, int button);
    void mousePressed (int x, int y, int button);
    void mouseReleased (int x, int y, int button);
    void windowResized (int w, int h);
    void dragEvent (ofDragInfo dragInfo);
    void gotMessage (ofMessage msg);
	void audioIn(float * input, int bufferSize, int nChannels); 


	ofxJoya Joya;
	WakeUp Wakeup;
	bool b_op, b_wakeup, b_explain, b_joya, b_ending;
	ofVideoPlayer op;
	ofVideoPlayer explain;
	ofVideoPlayer ending;
	
	ofSoundStream soundStream;
	vector <float> left;
	vector <float> right;
	vector <float> volHistory;
	int 	bufferCounter;
	int 	drawCounter;
	
	float smoothedVol;
	float scaledVol;
};

#endif