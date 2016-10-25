#include "Keyframe.h"

Keyframe::Keyframe()
{
	this->m_joints = vector<Joint>();
	this->m_frame = 0;
};

Keyframe::Keyframe(vector<Joint> joints, int frame)
{
	this->m_joints = joints;
	this->m_frame = frame;
};

Joint * Keyframe::getJoint(string name)
{
	int numJoints = this->m_joints.size();
	for (int i = 0; i < numJoints; i++) {
		if (m_joints[i].getName() == name) {
			return &m_joints[i];
		}
	}
	return nullptr;
};

Joint * Keyframe::getJoint(int ID)
{
	return &m_joints[ID];
};

vector<Joint> *Keyframe::getJoints()
{
	return &m_joints;
}
int Keyframe::getFrame()
{
	return this->m_frame;
};
