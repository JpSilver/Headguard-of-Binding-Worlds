#include "Armature.h"

Armature::Armature()
{
	m_fps = 0;
	m_numAnimation = 0;
	m_name = "arm_" + (char)(48 + ID);
	m_animations = vector<Animation>();
	m_animationIndex = 0;
};

Armature::Armature(float fps, int numAnimations, vector<Animation> animations, vector<Joint> baseJoints, string name)
{
	m_baseJoints = baseJoints;
	m_fps = fps;
	m_numAnimation = numAnimations;
	m_name = name;
	m_animations = animations;
	m_animationIndex = 0;
};

void Armature::addAnimation(Animation anim)
{
	m_animations.push_back(anim);
};

Animation * Armature::getAnimation()
{
	return &m_animations[m_animationIndex];
};

Animation * Armature::getPrevAnimation()
{
	return &m_animations[m_prevAnimationIndex];
}

int Armature::getAnimationIndex()
{
	return m_animationIndex;
}

int Armature::getPrevAnimationIndex()
{
	return m_prevAnimationIndex;
}

void Armature::setAnimation(string name)
{
	for (int i = 0; i < m_animations.size(); i++) {
		if (m_animations[i].getName() == name) {
			m_prevAnimationIndex = m_animationIndex;
			m_animationIndex = i;
		}
	}
};

void Armature::setAnimation(int ID)
{
	m_prevAnimationIndex = m_animationIndex;
	m_animationIndex = ID;
};

void Armature::addFrame(double deltaTime)
{
	m_animations[m_animationIndex].addFrame(m_fps, deltaTime);
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
