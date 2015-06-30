/*
 *  WakeUp.h
 *  openNiSample007
 *
 *  Created by Haruna Ishimatsu on 13/12/25.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _WAKE_UP
#define _WAKE_UP
#include "ofMain.h"

class WakeUp {
	
public:
	
	void setup();
	void update(float scaledVol);
	void draw();
	void keyInput(bool key1, bool key2, bool key3);
	vector <float> left;
	vector <float> right;
	vector <float> volHistory;
	
	int 	bufferCounter;
	int 	drawCounter;
	
	float smoothedVol;
	float scaledVol;
	
	float scale;
	ofPoint pos;
	ofImage img_wakeup[2];
	int img_num;
#define NUM 3
	ofVideoPlayer sleep;
	ofVideoPlayer getup;
	bool b_video[NUM];
	int v_num;
	int count;
	WakeUp();
};

#endif

