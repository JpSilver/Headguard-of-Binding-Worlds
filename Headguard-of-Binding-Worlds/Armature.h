#ifndef ARMATURE_H
#define ARMATURE_H

#include "Animation.h"

using std::string;


class Armature{
public:
	Armature();
	Armature(float, int, vector<Animation>, vector<Joint>, string);
	string getName();
	void addAnimation(Animation);
	Animation *getAnimation();
	Animation *getPrevAnimation();
	vector<Joint> *getBaseJoints();
	float getFrame();
	int getAnimationIndex();
	int getPrevAnimationIndex();
	void setAnimation(string);
	void setAnimation(int);
	void addFrame(double);
	void addFrame(float, double);
	void removeFrame(double);
	void removeFrame(float, double);

private:
	float m_fps;
	int m_numAnimation;
	string m_name;
	vector<Joint> m_baseJoints;
	vector<Animation> m_animations;
	int m_animationIndex;
	int m_prevAnimationIndex;
	static unsigned int ID;
	static double _DeltaTime;
};

#endif //ARMATURE_H