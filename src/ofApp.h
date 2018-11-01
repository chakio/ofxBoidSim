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
	bool enemy;
	int enemyIndex;

	double MAX_VEL 				= 100;
	double MIN_VEL 				= 10;
	double NEAR_DISTANCE		= 300;
	int    HISTORY_SIZE			= 40;

	double OBSTACLE_DISTANCE;
	double BOUNDARY_DISTANCE;

public:
	boid(int boidSize, double boundary, double obstacle);
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


class ofApp : public ofBaseApp{
	public:
	double VOLUME_OF_BOIDS  =500;
	vector<boid> boids;

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
	double BOUNDARY_FORCE       = 0.0001;
	double OBSTACLE_FORCE		= 0;
	double ENEMY_FORCE			= 15;
	double NOISE_FORCE 			= 10;

	double COHESION_DISTANCE	= 300;
	double SEPARATION_DISTANCE	= 100;
	double ALIGNMENT_DISTANCE	= 200;
	double OBSTACLE_DISTANCE	= 150;
	double ENEMY_DISTANCE		= 200;
	double BOUNDARY_DISTANCE	= 500;

	double COHESION_ANGLE		= 3.14/3;
	double SEPARATION_ANGLE		= 3.14/1;
	double ALIGNMENT_ANGLE		= 3.14/4;


};
