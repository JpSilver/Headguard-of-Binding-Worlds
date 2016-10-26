#include "Animation.h"

unsigned int Animation::ID = 0;


Animation::Animation(vector<Keyframe> keyframes, string name, int endFrame)
{
	for (int i = 0; i < keyframes.size(); i++) {
		addKeyframe(keyframes[i]);
	}
	this->endFrame = m_keyframes.rbegin()->first;
	this->m_name = name;
	this->frame = 0;
	this->ID++;
};

void Animation::addKeyframe(Keyframe key)
{
	map<int, Keyframe>::iterator it = m_keyframes.begin();
	this->m_keyframes.insert(it, keyframe(key.getFrame(), key));
};

Keyframe &Animation::getKeyframe(int frame)
{
	return m_keyframes.at(frame);
};

Keyframe &Animation::getKeyframeStart()
{
	map<int, Keyframe>::iterator it = m_keyframes.lower_bound((int)frame);
	if (frame < it->first) {
		it--;
	}
	return m_keyframes.at(it->first);
}
Keyframe &Animation::getNextKeyframe(int frame)
{
	map<int, Keyframe>::iterator it = m_keyframes.lower_bound(frame);
	if (it->first == m_keyframes.end()->first) {
		it = m_keyframes.begin();
	}
	return m_keyframes.at(it->first);
};

Keyframe &Animation::getKeyframeEnd()
{
	int max = m_keyframes.rbegin()->first;
	if (frame > max) {
		frame = max;
	}
	map<int, Keyframe>::iterator it = m_keyframes.upper_bound((int)frame);
	int index = it->first;
	while ((index = it->first) == getKeyframeStart().getFrame()) {
		it++;
	}
	if (it->first > m_keyframes.rbegin()->first) {
		it = m_keyframes.begin();
		index = it->first;
	}
	if (index < 0 || index > max) {
		index = m_keyframes.begin()->first;
	}
	return m_keyframes.at(index);
}
float Animation::getFrame()
{
	return this->frame;
}
void Animation::resetFrame()
{
	this->frame = getKeyframeStart().getFrame();
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
