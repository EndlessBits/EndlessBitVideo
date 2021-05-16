#include "ofApp.h"



//Moloch lomtik:
//3840x2160 -> 1920x1080
//конвертируем цвета в 2*2*2 или 3*3*3 цвета dithering
string folder = "D:\\eamuseum\\Projects\\2020-11-Endless bit\\Endless Instruments\\1bitcovers\\07_moloch_lomtik\\";
string in_folder = folder + "6";
string out_folder = folder + "6_bit";

//diffuse params
float diffuse = 0.7; //0.5; //0.4; //0.5; //1;

//разделение на 2 цвета в канале
int threshold = 150; //128; 
//разделение на 3 цвета в канале
int threshold0_ = threshold / 2;    
int threshold1_ = (threshold + 255) / 2; 



int shrink = 8; 
int zoom = shrink/2;  

//--------------------------------------------------------------
void convert1bit(string filein, string fileout) {

    ofPixels in0;

    //load
	ofLoadImage(in0, filein);       //исходные цвета
    in0.setImageType(OF_IMAGE_COLOR);
    in0.resize(in0.getWidth() / shrink, in0.getHeight() / shrink);
    int w = in0.getWidth();
    int h = in0.getHeight();

    //create 1-bit image
    ofPixels process;

    process = in0;

    //process
    int q1 = int(diffuse * 70);
    int q2 = int(diffuse * 30);
    int q3 = int(diffuse * 50);
    int q4 = int(diffuse * 10);

    //Diffuse dithering
    //https://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering
    unsigned char* ProcessPixels = process.getPixels();

    vector<int> pixels(w * h * 3);  //need use int for avoid overflow
    for (int i = 0; i < w * h * 3; i++) {
        pixels[i] = ProcessPixels[i];
    }

    int w_1 = w - 1;
    int h_1 = h - 1;

    //0,1 - dithering 0, less and more threshold
    //128,129 - dithering 1, less and more threshold

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int chann = 0; chann < 3; chann++) {
                int i = 3*(x + w * y) + chann;
                int oldpixel = pixels[i];

                //2 цвета в канале
                //int newpixel = (oldpixel >= threshold) ? 255 : 0;

                //3 цвета в канале
                int newpixel = (oldpixel >= threshold1_) ? 255 : ((oldpixel >= threshold0_) ? 128 : 0);

                ProcessPixels[i] = newpixel;

                int quant_error = oldpixel - newpixel;
                if (x < w_1) {
                    pixels[3*(x + 1 + w * y) + chann] += quant_error * q1 / 160;
                    if (y < h_1) {
                        pixels[3*(x + 1 + w * (y + 1)) + chann] += quant_error * q4 / 160;
                    }
                }
                if (x >= 1 && y < h_1) {
                    pixels[3*(x - 1 + w * (y + 1))+chann] += quant_error * q2 / 160;
                }
                if (y < h_1) {
                    pixels[3*(x + w * (y + 1))+chann] += quant_error * q3 / 160;
                }
            }
        }
    }

    //There is but in oF resize
    //process.resize(w * zoom, h * zoom);
    //So resize by myself
    int W = w * zoom;
    int H = h * zoom;

    ofPixels out;
    out.allocate(W, H, OF_IMAGE_COLOR);
    unsigned char* OutPixels = out.getPixels();
    for (int Y = 0; Y < H; Y++) {
        for (int X = 0; X < W; X++) {
            int x = X / zoom;
            int y = Y / zoom;
            for (int ch = 0; ch < 3; ch++) {
                OutPixels[3 * (X + W * Y) + ch] = ProcessPixels[3 * (x + w * y) + ch];
            }
        }
    }

    //save
    ofSaveImage(out, fileout);

}

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "1 bit converter" << endl;
	cout << "input folder " << in_folder << endl;
	cout << "output folder " << out_folder << endl;
	ofDirectory dir;
	int n = dir.listDir(in_folder);
	cout << "images: " << n << endl;
	for (int i = 0; i < n; i++) {
		string filein = dir.getPath(i);
		string fileout = out_folder + "\\" + ofToString(i, 5, '0') + ".png";
		cout << i << " / " << n << endl;
		convert1bit(filein, fileout);
	}
	cout << "FINISHED" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
