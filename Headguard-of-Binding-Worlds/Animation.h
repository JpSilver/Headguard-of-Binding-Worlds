#pragma once

#include <string>
#include <map>
#include "Keyframe.h"

using std::string;
using std::map;
using std::pair;

typedef pair<int, Keyframe> keyframe;

class Animation {
public:
	Animation();
	Animation(vector<Keyframe>, string , int);
	void addKeyframe(Keyframe key);
	Keyframe *getKeyframe(int);
	Keyframe *getKeyframeStart();
	Keyframe *getKeyframeEnd();
	float getFrame();
	void addFrame(float fps, double deltaTime);
	void addFrame(float frame);
	void removeFrame(float fps, double deltaTime);
	void removeFrame(float frame);
	string getName();
private:
	string m_name;
	map<int, Keyframe> m_keyframes;
	float frame;
	int endFrame;
	static unsigned int ID;
};