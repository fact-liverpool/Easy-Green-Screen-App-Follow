#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    bShowGui = true;
    bUpdateBgColor = false;
    

    camW = 1280; camH = 720;
    ofSetWindowShape(camW, camH);
    
    chromakey = new ofxChromaKeyShader(camW, camH);

#ifdef WEBCAM
    //webcam
    camera.setDeviceID(0);
    camera.setDesiredFrameRate(60);
    camera.initGrabber(camW, camH);
#endif
    //canon EDSDK
#ifdef CANONEDSDK
    camera.setup();
#endif
    //balckmagic
#ifdef BLACKMAGIC
    camera.setup(1920, 1080, 59.94f);
#endif
    
    // maskee
    bgImage.load("bg.jpg");
    isImage = true; //start with an Image
    isMovie = false; //start with an Image

    // GUI
    chromaGui.setup();
    ofParameter<bool> reset = false;
    reset.addListener(this, &ofApp::resetParams);
    chromaGui.add(reset.set("RESET", false));
    
    chromaGui.add(chromakey->positionParams);
    chromaGui.add(chromakey->generalParams);
    chromaGui.setDefaultHeight(15);
    chromaGui.setDefaultWidth(camW/5);
    chromaGui.setPosition(0, 0);
    chromaGui.setName("press G toggle MENU");
    
    chromaGui.loadFromFile("settings.xml");
    chromaGui.minimizeAll();
    
    textureRawSyphonServer.setName("EasyGreenScreen - Raw Output");
    textureFinalSyphonServer.setName("EasyGreenScreen - Final Output");


    
}

void ofApp::resetParams(bool & b){
    chromakey->clippingMaskBR.set(ofVec2f(0,0));
    chromakey->clippingMaskTL.set(ofVec2f(camW,camH));
    chromakey->photoRotation.set(0.0f);
    chromakey->photoZoom.set(1.0f);
    chromakey->photoOffset.set(ofVec2f(0,0));
    
}
//--------------------------------------------------------------
void ofApp::exit() {
    delete chromakey;
}

//--------------------------------------------------------------
void ofApp::update(){
    
  
    if(!isImage){
        bgMovie.update();
    }
    
#ifdef CANONEDSDK
    //camera.update();
    if(camera.isFrameNew()) {
        if(bUpdateBgColor)
            chromakey->updateBgColor(camera.getLivePixels());
        if(!isImage){
            chromakey->updateChromakeyMask(camera.getLiveTexture(), bgMovie.getTexture(), camW, camH);
        }else{
            chromakey->updateChromakeyMask(camera.getLiveTexture(), bgImage.getTexture(), camW, camH);
        }
    }
#endif
#ifdef WEBCAM
    camera.update();
    if(camera.isFrameNew()) {
        if(bUpdateBgColor)
            chromakey->updateBgColor(camera.getPixels());
        if(!isImage){
            chromakey->updateChromakeyMask(camera.getTexture(), bgMovie.getTexture(), camW, camH);
        }else{
            chromakey->updateChromakeyMask(camera.getTexture(), bgImage.getTexture(), camW, camH);
        }
    }
#endif
#ifdef BLACKMAGIC
    //balckmagic
    if(camera.update()) {
        if(bUpdateBgColor)
            chromakey->updateBgColor(camera.getColorPixels());
        if(!isImage){
            chromakey->updateChromakeyMask(camera.getColorTexture(), bgMovie.getTexture(), camW, camH);
        }else{
            chromakey->updateChromakeyMask(camera.getColorTexture(), bgImage.getTexture(), camW, camH);
        }
    }
#endif

    

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    ofBackground(0);
    // draw Cam mask
    chromakey->drawFinalImage(0, 0,camW, camH);
    
    // GUI
    if(bShowGui) {
        chromaGui.draw();
    }
        // draw bg color's reference Rect
    if(bUpdateBgColor) {
            ofPushStyle();
            ofNoFill();
            ofSetLineWidth(3);
            ofSetColor(255);
            ofVec2f bgColorPos = chromakey->bgColorPos.get();
            ofDrawRectangle(bgColorPos.x, bgColorPos.y, chromakey->bgColorSize.get(), chromakey->bgColorSize.get());
            ofDrawBitmapString("bgColor", bgColorPos.x, bgColorPos.y - 5);
            ofPopStyle();
    
    }
    
#ifdef CANONEDSDK
    textureRawSyphonServer.publishTexture(&camera.getColorTexture());
#endif
#ifdef WEBCAM
    textureRawSyphonServer.publishTexture(&camera.getTexture());
#endif
#ifdef BLACKMAGIC
    textureRawSyphonServer.publishTexture(&camera.getColorTexture());
#endif
    textureFinalSyphonServer.publishTexture(&chromakey->fbo_final.getTexture());

    
}

void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch(key){
        case ' ':
            
            break;
        case 'a':
            bUpdateBgColor = !bUpdateBgColor;
            break;
        case 'g':
            bShowGui = !bShowGui;
            break;
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    if( dragInfo.files.size() > 0 ){
        ofPoint dragPt = dragInfo.position;
        string filePath = dragInfo.files[dragInfo.files.size()-1]; // for now get only one, the last if you drag many files
        string ext = filePath.substr(filePath.find_last_of('.')+1); //get the extension
        ext = ofToLower(ext); //lowerCase
        cout <<  ext << endl;
        if(ext == "jpg" || ext == "png" || ext == "gif"){
            bgImage.load(filePath);
            isImage = true;
            
        }else if(ext == "mov" | ext == "avi" | ext == "mp4"){
            
            bgMovie.load(filePath);
            bgMovie.setLoopState(OF_LOOP_NORMAL);
            bgMovie.play();
            isImage = false;
        }
        
        

    }
    
}