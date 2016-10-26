#ifndef ARMATURE_H
#define ARMATURE_H

#include "Animation.h"

using std::string;


class Armature{
public:
	Armature() {};
	Armature(const Armature &other);
	Armature(float, int, vector<Animation>, vector<Joint>, string);
	string getName();
	void addAnimation(Animation);
	Animation &getAnimation();
	Animation &getBaseAnimation();
	Animation &getPrevAnimation();
	vector<Joint> &getBaseJoints();
	float getFrame();
	int getAnimationIndex();
	int getPrevAnimationIndex();
	vector<Joint> getStartJoints();
	void setAnimation(string);
	void setStartJoints(vector<Joint> startJoints);
	void resetPrevAnimation();
	void setAnimation(int);
	void addFrame(double);
	void addFrame(double, int);
	void addFrame(float, double);
	void removeFrame(double);
	void removeFrame(float, double);

private:
	float m_fps;
	int m_numAnimation;
	string m_name;
	vector<Joint> m_baseJoints;
	Joint *m_startJoints;
	vector<Animation> m_animations;
	int m_startJointSize = 0;
	int m_animationIndex;
	int m_prevAnimationIndex;
	static unsigned int ID;
	static double _DeltaTime;
};

#endif //ARMATURE_H