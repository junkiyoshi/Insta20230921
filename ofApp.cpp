#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofNoFill();
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 150, true, true, true);

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0);

	auto word = "RAIN";
	this->font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word) - 240);

	fbo.end();
	auto span = 1;
	ofPixels pixels;
	fbo.readToPixels(pixels);
	for (int x = 0; x < 700; x += span) {

		for (int y = 0; y < 720; y += span) {

			ofColor color = pixels.getColor(x, y);
			if (color != ofColor(0, 0)) {

				this->font_location_list.push_back(glm::vec3(x - ofGetWidth() * 0.5, y - ofGetHeight() * 0.25, 0));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->location_list.size() - 1; i >= 0; i--) {

		if (this->location_list[i].z < 0) {

			this->location_list[i].z += 20;
			continue;
		}

		this->radius_list[i] += this->speed_list[i];

		if (this->radius_list[i] > this->max_radius_list[i]) {

			this->location_list.erase(this->location_list.begin() + i);
			this->radius_list.erase(this->radius_list.begin() + i);
			this->speed_list.erase(this->speed_list.begin() + i);
			this->max_radius_list.erase(this->max_radius_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}

	for(int i = 0; i < 40; i++) {
		
		int rnd_index = ofRandom(this->font_location_list.size());

		auto location = glm::vec3(this->font_location_list[rnd_index], -300);
		this->location_list.push_back(location);
		this->radius_list.push_back(1);
		this->speed_list.push_back(ofRandom(0.1, 0.3));
		this->max_radius_list.push_back(ofRandom(2, 5));
		this->color_list.push_back(ofColor(0));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(150);

	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->radius_list[i] < this->max_radius_list[i] * 0.5) {

			ofSetColor(this->color_list[i]);
		}
		else {

			ofSetColor(ofColor(this->color_list[i], ofMap(this->radius_list[i], this->max_radius_list[i] * 0.5, this->max_radius_list[i], 255, 0)));
		}

		if (this->location_list[i].z < 0) {

			ofSetColor(this->color_list[i], 128);
			ofDrawLine(this->location_list[i], this->location_list[i] - glm::vec3(0, 0, 30));
			continue;
		}

		ofDrawCircle(glm::vec2(this->location_list[i]), this->radius_list[i]);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}