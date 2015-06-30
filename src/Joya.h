#ifndef _OF_JOYA
#define _OF_JOYA
#include "ofMain.h"
#include "ofxOpenNI.h"
class ofxJoya {
private:
	int k_w, k_h;
    ofxOpenNIContext  context;
    ofxDepthGenerator depth;
	ofxUserGenerator user;
	ofxImageGenerator color;
	ofImage shadow;
	ofImage photo;
	ofImage photo_img;
	ofTexture texture;
	unsigned char * alphaPixels;
	int alpha;
	bool b_photo;
	ofPoint pos;
	ofPoint eye[2];
	ofPoint m;
	
	ofVideoPlayer frame;
	ofPoint center;
	ofVideoPlayer j_mask;
	int w,h;
	ofImage ha;
	
	ofImage mouth[5];
	ofImage eye1[5];
	ofImage i_time;
	bool b_ha;
	
	int f_count;
	int t_count;//timeを数える
	int time;//秒数を入れる
	int count;//だんだん薄くする
	bool b_snapshot;
	int SnapCount;
	bool b_user;
	ofImage snap;
	int m_num;
	int e_num;
	bool order;
	int m_bai;
	int m_pos;
public:
    void setup();
    void update();
    void draw();
	void joya_key(bool b_key1, bool b_key2);
	bool b_time (bool b_photo);
	ofxJoya();
};

#endif