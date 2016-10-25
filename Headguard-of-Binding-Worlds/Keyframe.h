#pragma once
#include <vector>
#include "Joint.h"

using std::vector;

class Keyframe {
public:
	Keyframe();
	Keyframe(vector<Joint> joints, int frame);
	Joint *getJoint(string name);
	Joint *getJoint(int ID);
	vector<Joint> *getJoints();
	int getFrame();

private:
	vector<Joint> m_joints;
	int m_frame;
};