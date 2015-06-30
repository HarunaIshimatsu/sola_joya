/*
 *  WakeUp.cpp
 *  openNiSample007
 *
 *  Created by Haruna Ishimatsu on 13/12/25.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "WakeUp.h"
#define BAI 50
#define img_h 700
#define img_w 1150
#define COUNT_MAX 50
//--------------------------------------------------------------
WakeUp::WakeUp(){
	
}
void WakeUp::setup(){	 
	ofSetFrameRate(30);
	ofSetCircleResolution(80);
	
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	
	
	//if you want to set a different device id 
	//soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
	scale=0;
	for (int i=0; i<2; i++) {
		img_wakeup[i].loadImage("wakeup/img/wakeup"+ofToString(i)+".png");
	}
	for (int i=0; i<NUM; i++) {
		b_video[i]=false;
	}
	//b_video=false;
	//translate
	pos=ofPoint(img_w/2, img_h);
	img_num=0;
	v_num=-1;
	//sleep.play();
	for (int i=0; i<NUM; i++) {
		b_video[NUM]=false;
	}
	count = 0;
}

//--------------------------------------------------------------
void WakeUp::update(float scaledVol){
	//video
	if (b_video[0] || b_video[2]) {
		sleep.update();
	} else if (b_video[1]) {
		getup.update();
	}
	//speed
	scale=scaledVol*BAI;
	if (scale > 1.0 && b_video[0]) {
		scale=1.0;
		count = count + 1;
	}
	if (count == COUNT_MAX ) {
		b_video[1]=true;
		b_video[0]=false;
		getup.setFrame(1);
		getup.loadMovie("wakeup/video/getup"+ofToString(v_num)+".mov");
		getup.setLoopState(OF_LOOP_NONE);
		sleep.stop();
		getup.play();
		count = 0;
	}
	if (img_num==0) {
		img_num=1;
	} else if (img_num==1) {
		img_num=0;
	}
}

//--------------------------------------------------------------
void WakeUp::draw(){
	ofSetColor(255, 255, 255);
	if (b_video[0] || b_video[2]) {
		sleep.draw(ofGetWidth()/2-200, 100);
	} else if (b_video[1]) {
		getup.draw(ofGetWidth()/2-200, 100);
	}
	if (b_video[0]) {
		ofSetColor(255, 255, 255);
		ofTranslate(pos.x+80, pos.y+300, 0);
		ofScale(scale, scale, 1);
		img_wakeup[img_num].draw(-pos.x, -pos.y);
		ofScale(-scale, -scale, 1);
		ofTranslate(-(pos.x+80), -(pos.y+300), 0);
	}
}

//--------------------------------------------------------------
void WakeUp::keyInput(bool key1, bool key2, bool key3){
	getup.setFrame(1);
	if (key3) {
		v_num=0;
		b_video[2]=true;
		sleep.loadMovie("wakeup/video/sleep"+ofToString(v_num)+".mov");
		sleep.firstFrame();
		sleep.play();
	}
	if (key1) {
		b_video[2]=true;
		getup.stop();
		v_num=v_num+1;
		sleep.loadMovie("wakeup/video/sleep"+ofToString(v_num)+".mov");
		sleep.firstFrame();
		sleep.play();
	}
	if (key2) {
		if (b_video[0]==false && b_video[1]==false && b_video[2] == true) {
			b_video[2]=false;
			b_video[0]=true;
		} else if (b_video[0]==true && b_video[1]==false) {
			b_video[0]=false;
			b_video[1]=true;
			b_video[2]=false;
		} else if (b_video[0]==false && b_video[1]==true) {
			b_video[0]=true;
			b_video[1]=false;
			b_video[2]=false;
		}
		if (v_num > 3) {
			b_video[0] = false;
			b_video[1] = false;
			b_video[2]=false;
		}
	}
}