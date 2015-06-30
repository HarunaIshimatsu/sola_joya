#include "testApp.h"
#define resize 0.9375//1280*800 0.83 1440*900 0.9375
#define trans 120 //1280*800 1440*900 120
void testApp::setup () {
	HideCursor();
	//  window
	ofSetFrameRate(30);
	ofBackground(0, 0, 0);
	ofEnableAlphaBlending();//ALPHA値を使う場合は必ず指定
	ofSetVerticalSync(true);
	Joya.setup();
	b_op=false;
	b_wakeup=false;
	b_explain=false;
	b_joya=false;
	b_ending = false;
	op.loadMovie("joya/op.mov");
	op.setLoopState(OF_LOOP_NONE);
	explain.loadMovie("joya/explain.mp4");
	explain.setLoopState(OF_LOOP_NONE);
	ending.loadMovie("joya/ending.mp4");
	soundStream.listDevices();
	int bufferSize = 256;
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	Wakeup.setup();
}
void testApp::update () {
	HideCursor();
	if (b_op) {
		op.update();
	}
	if (b_wakeup) {
		//lets scale the vol up to a 0-1 range 
		scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
		
		//lets record the volume into an array
		volHistory.push_back( scaledVol );
		
		//if we are bigger the the size we want to record - lets drop the oldest value
		if( volHistory.size() >= 400 ){
			volHistory.erase(volHistory.begin(), volHistory.begin()+1);
		}
		Wakeup.update(scaledVol);
	}
	if (b_explain) {
		explain.update();
	}
	if (b_joya) {
		Joya.update();
	}
	if (b_ending) {
		ending.update();
	}
}

void testApp::draw () {
	HideCursor();
	//draw image
	ofScale(resize, resize, 1);
	ofTranslate(trans, 0, 0);
	ofSetColor(255, 255, 255);
	if (b_op) {
		op.draw(0, 0);
	}
	if (b_wakeup) {
		Wakeup.draw();
	}
	if (b_explain) {
		explain.draw(0, 0);
	}
	if (b_joya) {
		Joya.draw();
	}
	if (b_ending) {
		ending.draw(0, 0);
	}
}
void testApp::keyPressed (int key) {
	HideCursor();
	switch (key) {
		case '1':
			b_op=true;
			b_wakeup=false;
			b_explain=false;
			b_joya=false;
			b_ending=false;
			ofSetFrameRate(30);
			op.firstFrame();
			op.play();
			break;
		case '2':
			b_op=false;
			b_wakeup=true;
			b_explain=false;
			b_joya=false;
			b_ending=false;
			ofSetFrameRate(15);
			if (b_wakeup) {
				Wakeup.keyInput(false, false, true);
			}
			break;

		case '3':
			b_op=false;
			b_wakeup=false;
			b_explain=true;
			b_joya=false;
			b_ending=false;
			ofSetFrameRate(30);
			explain.firstFrame();
			explain.play();
			
			break;
		case '4':
			b_op=false;
			b_wakeup=false;
			b_explain=false;
			b_joya=true;
			b_ending=false;
			ofSetFrameRate(15);
			break;

		case '5':
			b_op=false;
			b_wakeup=false;
			b_explain=false;
			b_joya=false;
			b_ending=true;
			ofSetFrameRate(30);
			ending.firstFrame();
			ending.play();
			break;
		case '6':
			b_op=false;
			b_wakeup=false;
			b_explain=false;
			b_joya=false;
			b_ending=false;
			break;


		case ' ':
			if (b_joya) {
				Joya.joya_key(true,false);
			}
			if (b_wakeup) {
				Wakeup.keyInput(false, true,false);
			}
			break;
		case 'c':
			if (b_joya) {
				Joya.joya_key(false,true);
			}
			if (b_wakeup) {
				Wakeup.keyInput(true, false, false);
			}
			break;
		default:
			break;
	}
	if (!b_op) {
		op.stop();
	}
	if (!b_explain) {
		explain.stop();
	}
	if (!b_ending) {
		ending.stop();
	}

}
void testApp::keyReleased (int key) {}
void testApp::mouseMoved (int x, int y) {}
void testApp::mouseDragged (int x, int y, int button){}
void testApp::mousePressed (int x, int y, int button) {}
void testApp::mouseReleased (int x, int y, int button){}
void testApp::windowResized (int w, int h){}
void testApp::dragEvent (ofDragInfo dragInfo) {}
void testApp::gotMessage (ofMessage msg) {}
//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	HideCursor();
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	
	
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
		
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}