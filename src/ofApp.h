#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxChromaKeyShader.h"
#include "ofxGui.h"
#include "ofxEdsdk.h"
#include "ofxBlackMagic.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void drawCheckerboard(float x, float y, int width, int height, int size);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // === Variables =============================
    
    ofImage bgImage;
    ofVideoPlayer bgMovie;
    
    bool isImage;
    bool isMovie;

    ofxChromaKeyShader *chromakey;
    ofVideoGrabber webcam;
    int camW, camH;
    
    ofFbo checkerboardTex;
    
    ofxPanel chromaGui;
    bool bShowGui;
    bool bUpdateBgColor;
    
    ofxSyphonServer textureFinalSyphonServer;
    ofxSyphonServer textureRawSyphonServer;

    ofxEdsdk::Camera camera;

    ofxBlackMagic cam;

};