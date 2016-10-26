#include "Armature.h"

unsigned int Armature::ID;

Armature::Armature(const Armature & other)
{
	this->m_fps = other.m_fps;
	this->m_numAnimation = other.m_numAnimation;
	this->m_name = other.m_name;
	this->m_baseJoints = other.m_baseJoints;
	this->m_animations = other.m_animations;
	this->m_animationIndex = other.m_animationIndex;
	this->m_prevAnimationIndex = other.m_prevAnimationIndex;
};

Armature::Armature(float fps, int numAnimations, vector<Animation> animations, vector<Joint> baseJoints, string name)
{
	m_baseJoints = baseJoints;
	m_fps = fps;
	m_numAnimation = numAnimations;
	m_name = name;
	m_animations = animations;
	m_animationIndex = 0;
	m_prevAnimationIndex = 0;
}
string Armature::getName()
{
	return this->m_name;
};

void Armature::addAnimation(Animation anim)
{
	m_animations.push_back(anim);
};

Animation &Armature::getAnimation()
{
	return m_animations[m_animationIndex];
}
Animation &Armature::getBaseAnimation()
{
	return m_animations[0];
};

Animation &Armature::getPrevAnimation()
{
	return m_animations[m_prevAnimationIndex];
}

vector<Joint> &Armature::getBaseJoints()
{
	return this->m_baseJoints;
}

float Armature::getFrame()
{
	return getAnimation().getFrame();
};

int Armature::getAnimationIndex()
{
	return m_animationIndex;
}

int Armature::getPrevAnimationIndex()
{
	return m_prevAnimationIndex;
}

vector<Joint> Armature::getStartJoints()
{
	vector<Joint> retJoints;
	for (int i = 0; i < m_baseJoints.size(); i++) {
		retJoints.push_back(m_startJoints[i]);
	}
	return retJoints;
}

void Armature::setAnimation(string name)
{
	m_prevAnimationIndex = m_animationIndex;
	for (int i = 0; i < m_animations.size(); i++) {
		if (m_animations[i].getName() == name) {
			m_animationIndex = i;
			return;
		}
	}
}
void Armature::setStartJoints(vector<Joint> startJoints)
{
	if (m_startJointSize > 0) {
		delete[] m_startJoints;
	}
	m_startJoints = new Joint[startJoints.size()];
	memcpy(m_startJoints, &startJoints[0], sizeof(Joint)*startJoints.size());
	m_startJointSize = startJoints.size();
};

void Armature::resetPrevAnimation()
{
	m_prevAnimationIndex = m_animationIndex;
};

void Armature::setAnimation(int ID)
{
	m_prevAnimationIndex = m_animationIndex;
	m_animationIndex = ID;
};

void Armature::addFrame(double deltaTime)
{
	m_animations[m_animationIndex].addFrame(m_fps, deltaTime);
}
void Armature::addFrame(double deltaTime, int animID)
{
	m_animations[animID].addFrame(m_fps, deltaTime);
};

void Armature::addFrame(float frame, double deltaTime)
{
	m_animations[m_animationIndex].addFrame(m_fps*frame, deltaTime);
};

void Armature::removeFrame(double deltaTime)
{
	m_animations[m_animationIndex].removeFrame(m_fps, deltaTime);
};

void Armature::removeFrame(float frame, double deltaTime)
{
	m_animations[m_animationIndex].addFrame(m_fps*frame, deltaTime);
};
