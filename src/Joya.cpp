/*
 *  Joya.cpp
 *  openNiSample007
 *
 *  Created by Haruna Ishimatsu on 13/12/25.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Joya.h"
ofxJoya::ofxJoya(){
}
void ofxJoya::setup () {
	ofSetFrameRate(15);
    w=1280;
	h=960;
	k_w = 640;
	k_h = 480;
    //  setup ofxOpenNI
    context.setup();
    depth.setup(&context);
	user.setup(&context);
	user.setMaxNumberOfUsers(4);
	user.setUseMaskPixels(true);
	color.setup(&context);
	context.toggleMirror();
	context.toggleRegisterViewport();
	//shadow image
	shadow.allocate(k_w, k_h,OF_IMAGE_GRAYSCALE);//RGBを入れられるpixelで扱うときはk*3,k*3+1,k*3+2
	//GL_ALPHA:透明度だけ GL_RGBA:RGBと透明度
	//入れたい色が単色ならばGL_ALPHAで透明度だけいじって描画の直前に色を指定したらいいかも？
	texture.allocate(w,h,GL_ALPHA);//ALPHA値のみいじる pixelsで扱うときはkのみ　GL_RGBAを使うときはk*4,k*4+1,k*4+2,k*4+3
	b_photo = false;
	alphaPixels = new unsigned char [w*h];//後で使う
	alpha=0;
	for (int k=0; k<w*h; k++) {
		alphaPixels[k]=0;
	}
	texture.allocate(w,h,GL_ALPHA);
	texture.loadData(alphaPixels, w, h, GL_ALPHA);
	pos=ofPoint(0,0);
	for (int i=0; i<2; i++) {
		eye[i]=ofPoint(-10,-10);
	}
	m=ofPoint(0,0);
	//frame
	frame.loadMovie("joya/un.mov");
	frame.play();
	j_mask.loadMovie("joya/mask.mov");
	j_mask.play();
	ha.loadImage("joya/ha.png");
	ha.setImageType(OF_IMAGE_COLOR_ALPHA);
	m_num=ofRandom(0,4);
	e_num=ofRandom(0,4);
	if (m_num==5) {
		m_num=4;
	}
	if (e_num==5) {
		e_num=4;
	}
	for (int i=0; i<5; i++) {
		mouth[i].loadImage("joya/face/mouth"+ofToString(m_num)+"/mouth_"+ofToString(i)+".png");
		//mouth[i].loadImage("joya/face/mouth4/mouth_"+ofToString(i)+".png");
		mouth[i].setImageType(OF_IMAGE_COLOR_ALPHA);
		eye1[i].loadImage("joya/face/eye"+ofToString(e_num)+"/eye_"+ofToString(i)+".png");
		//eye1[i].loadImage("joya/face/eye4/eye_"+ofToString(i)+".png");
		eye1[i].setImageType(OF_IMAGE_COLOR_ALPHA);
	}
	
	f_count=0;
	b_ha=false;
	t_count=0;
	count = 0;
	b_snapshot=false;
	SnapCount=0;
	order=false;
	m_bai=300;
	m_pos=0;
}
void ofxJoya::update () {
	//kinect
    context.update();
	user.update();
	frame.update();
	j_mask.update();
	//make shadow
	unsigned char *sdata = shadow.getPixels();//
	//背景との差分でuser検出をしているので、動くものすべてに反応し、kinect自体を動かすと上手く検出できない
	unsigned char *data = user.getUserPixels(0);//　getUserPixels()の数字は0は検出された人全員、他の数字を入れれば検出人数を指定できる
	unsigned char *maskPixels = j_mask.getPixels();
	ofPoint start=ofPoint(1280,960);
	ofPoint end=ofPoint(0,0);
	if (!b_ha) {
		b_user=false;
		for (int j=0; j<h; j++) {
			for (int i=0; i<w; i++) {
				if (maskPixels[3*(i+j*w)]==255 && data[i/2+j/2*w/2]) {
					alphaPixels[i+j*w]=255;
					if (start.x > i) {
						start.x=i;
					}
					if (start.y > j) {
						start.y = j;
					}
					if (end.x < i) {
						end.x=i;
					}
					if (end.y < j) {
						end.y=j;
					}
					b_user=true;
				} else {
					alphaPixels[i+j*w]=0;
				}
			}
		}
	}
	if (b_ha && b_photo) {
		for (int k=0; k<k_w*k_h; k++) {
			if (data[k]) {
				sdata[k]=0;
				b_user=true;
			}else {
				sdata[k]=255;
			}
			
		}
		b_snapshot=true;
		shadow.update();
	}
	if (b_ha) {
		b_photo=false;
	}
	texture.loadData(alphaPixels, w, h, GL_ALPHA);
	if (!b_ha) {
		m.x = (start.x+end.x)/2-150;
		m.y = (start.y+end.y)/2-100;
		eye[0].x = (start.x+end.x)/2-300;
		eye[1].x = (start.x+end.x)/2;
		eye[0].y = (start.y+end.y)/3-100;
		eye[1].y = (start.y+end.y)/3-100;
	}
}

void ofxJoya::draw () {

	ofSetColor(255, 255, 255);
	ofRect(0, 0, w, h);
	if (!b_ha) {
		ofSetColor(255, 255, 255);
		frame.draw(0, 0);
		//	ofSetColor(255, 255, 255);
		//	shadow.draw(0, 0, 640, 480);
		ofSetColor(0, 0, 0);//ここでtextureの色を変えられ
		texture.draw(0, 0, 0);
		if (!order) {
			f_count = f_count+1;
			if (f_count == 4) {
				order = true;
			}
		} else if (order) {
			f_count = f_count-1;
			if (f_count == 0) {
				order = false;
			}
		}
	} else {
		ofSetColor(255, 255, 255);
		shadow.draw(0, count*(-1), w, h);
		ofSetColor(255, 255, 255);
		f_count=0;
		eye[0].y = eye[0].y-10;
		eye[1].y = eye[1].y-10;
		m.y = m.y - 10;
	}
	//目、口
	if (b_user) {
		ofSetColor(0, 0, 0);
		ofFill();
		if (!b_ha) {
			ofSetColor(255, 255, 255, count);
		} else {
			ofSetColor(255, 255, 255, 255);
		}
		eye1[f_count].draw(eye[0].x, eye[0].y,300,300);
		eye1[f_count].draw(eye[1].x+300, eye[1].y,-300,300);

		mouth[f_count].draw(m.x,m.y+m_pos,300,m_bai);
		if (count < 255) {
			count = count + 10;
		}
		if (!b_ha) {
			b_ha = b_time(b_photo);
			if (b_ha) {
				count = 0;
			}
		}
	}
	if (b_ha) {
		ofSetColor(255, 255, 255, count);
		ofRect(0, 0, w, h);
		ofSetColor(255, 255, 255, 255);
		ha.draw(0,0);
	}
	//snapshot
	if (b_snapshot == true && count < 20){
		// grab a rectangle at 200,200, width and height of 300,180
		snap.grabScreen(0,0,1280,900);
		
		string fileName = "joya/snap/snapshot_"+ofToString(SnapCount)+".png";
		snap.saveImage(fileName);
		SnapCount++;
		b_snapshot = false;
	}
}
void ofxJoya::joya_key (bool b_key1, bool b_key2) {
	if (b_key1) {
		b_photo=true;
		alpha=255;
	}
	b_key1=false;
	if (b_key2) {
		f_count=0;
		b_ha=false;
		b_photo=false;
		count=0;
		order=false;
		m_bai=ofRandom(-1,2);
		if (m_bai > 0) {
			m_bai=300;
			m_pos=0;
		} else {
			m_bai=-300;
			m_pos=350;
		}
		m_num=ofRandom(0,5);
		e_num=ofRandom(0,5);
		if (m_num==5) {
			m_num=4;
		}
		if (e_num==5) {
			e_num=4;
		}
		cout << m_num << "," << e_num << endl;
		for (int i=0; i<5; i++) {
			mouth[i].loadImage("joya/face/mouth"+ofToString(m_num)+"/mouth_"+ofToString(i)+".png");
			eye1[i].loadImage("joya/face/eye"+ofToString(e_num)+"/eye_"+ofToString(i)+".png");
		}
	}
	b_key2=false;
}
bool ofxJoya::b_time(bool b_photo){
	if (b_photo) {
		if (t_count==0) {
			time=ofGetSeconds();
			t_count=4;
		}
		if (time!=ofGetSeconds()) {
			t_count=t_count-1;
			time=ofGetSeconds();
		}
		if (t_count != 0) {
			if (t_count < 4) {
				i_time.loadImage("joya/"+ofToString(t_count)+".png");
				i_time.draw(0, 0);
			}
			return false;
		} else if (t_count==0) {
			return true;
		}
	}
}
