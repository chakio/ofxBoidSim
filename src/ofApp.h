#pragma once

#include "ofMain.h"
#include "ofxGui.h"





class boid
{
private:
	vector <ofVec3f> pos;
	ofVec3f vel;
	double beforeTime;
	vector<double> distances;
	vector<double> angles;
	ofMesh mesh;
	bool target;
	int enemyIndex;

	double MAX_VEL 				= 100;
	double MIN_VEL 				= 10;
	double NEAR_DISTANCE		= 300;
	int    HISTORY_SIZE			= 30;

	double BOUNDARY_DISTANCE;

public:
	boid(int boidSize, double boundary);
	void update(double time);
	void draw();

	void setDistances(int index,double distance);
	void setAngles(int index,double angle);
	void setTarget(bool Target);
	void setEnemy(bool Enemy);
	void setEnemyIndex(int EnemyIndex);

	void calVelocity();
	void addVel(ofVec3f vec);

	ofVec3f getPos(int index);
	ofVec3f getVel();
	double getDistance(int index);
	double getAngle(int index);
	int getPosSize();
	bool getEnemy();
};

class attractor
{
private:
	double boxSize;
	ofVec3f velocity;
	ofVec3f position;
	double	velSize;
	double 	beforeTime;
	void setVelocity();

public:
	attractor();
	void setup(double size,double time,double velSize);
	void update(double time);
	void draw();
	ofVec3f getPos();
};

class ofApp : public ofBaseApp{
	public:
	double VOLUME_OF_BOIDS  =400;
	vector<boid> boids;
	attractor Attractor;

	int targetNum=0;
	int enemyNum=0;

	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);

	ofEasyCam cam;

	double COHESION_FORCE 		= 2;
	double SEPARATION_FORCE		= 2;
	double ALIGNMENT_FORCE		= 1.5;
	double BOUNDARY_FORCE       = 0.00;
	double OBSTACLE_FORCE		= 0;
	double NOISE_FORCE 			= 0.4;
	double ATTRACTOR_FORCE		= 0.1;

	double COHESION_DISTANCE	= 500;
	double SEPARATION_DISTANCE	= 450;
	double ALIGNMENT_DISTANCE	= 100;
	double BOUNDARY_DISTANCE	= 500;

	double COHESION_ANGLE		= 3.14/4;
	double SEPARATION_ANGLE		= 3.14/1;
	double ALIGNMENT_ANGLE		= 3.14/5;

	double BOX_SIZE				= 500;
	double ATTRACTOR_VEL		= 300;

};
