#include "Animation.h"

Animation::Animation()
{
	string m_name = "anm_" + (char)(48 + ID);
	vector<Keyframe> m_keyframes = vector<Keyframe>();
	frame = 0;
	ID++;
};

Animation::Animation(vector<Keyframe> keyframes, string name, int endFrame)
{
	for (int i = 0; i < keyframes.size(); i++) {
		addKeyframe(keyframes[i]);
	}
	this->endFrame = endFrame;
	this->m_name = name;
	this->frame = 0;
	this->ID++;
};

void Animation::addKeyframe(Keyframe key)
{
	map<int, Keyframe>::iterator it = m_keyframes.begin();
	this->m_keyframes.insert(it, keyframe(key.getFrame(), key));
};

Keyframe * Animation::getKeyframe(int frame)
{
	return &m_keyframes.at(frame);
};

Keyframe * Animation::getKeyframeStart()
{
	map<int, Keyframe>::iterator it = m_keyframes.lower_bound((int)frame);
	return &m_keyframes.at(it->first);
};

Keyframe * Animation::getKeyframeEnd()
{
	map<int, Keyframe>::iterator it = m_keyframes.upper_bound((int)frame);
	return &m_keyframes.at(it->first);
};

void Animation::addFrame(float fps, double deltaTime)
{
	float am = (fps * deltaTime);
	this->frame = (frame+am < endFrame) ? (frame + am) : 0;
};

void Animation::addFrame(float amount)
{
	this->frame = (frame + amount < endFrame) ? (frame + amount) : 0;
};

void Animation::removeFrame(float fps, double deltaTime)
{
	float am = (fps * deltaTime);
	this->frame = (frame + am < endFrame) ? (frame - am) : 0;
};

void Animation::removeFrame(float amount)
{
	this->frame = (frame-amount > 0) ? (frame - amount) : endFrame;
}
string Animation::getName()
{
	return m_name;
};
