#include "ofApp.h"
//--------------------------------------------------------------
boid::boid(int boidSize, double boundary, double obstacle)
{
	pos.resize(1);
	pos[0].set(ofRandom(-500,500),ofRandom(-500,500),ofRandom(-500,500));
	vel.set(ofRandom(-MAX_VEL,MAX_VEL),ofRandom(-MAX_VEL,MAX_VEL),ofRandom(-MAX_VEL,MAX_VEL));
	vel			= vel.normalize()*ofRandom(MIN_VEL,MAX_VEL);
	beforeTime 	= ofGetElapsedTimef();
	for(int i=0;i<boidSize;i++)
	{
		distances.push_back(0);
		angles.push_back(0);
	}
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);

	BOUNDARY_DISTANCE = boundary;
	OBSTACLE_DISTANCE = obstacle;
}
void boid::update(double time)
{
	double dTime 	= 	time-beforeTime;
	beforeTime 		= 	time; 

	//posの一番最後のインデックスが最新の位置
	if(pos.size()==HISTORY_SIZE)
	{
		pos.erase(pos.begin());
	}
	pos.push_back(pos[pos.size()-1]+(dTime*vel));
}
void boid::draw()
{
	int count=0;
	for(int i=0;i<distances.size();i++)
	{
		if(NEAR_DISTANCE>distances[i])
		{
			count++;
		}
	}
	//ofSetColor(255,255,255,count*0.5);
	
	//ofDrawLine(pos,pos+vel/40);
	mesh.clear();

	ofColor c,color;
	ofVec3f velNormalize=vel.normalized();
	//cout <<velNormalize.length()<<endl;
	color.set((velNormalize.x)*255.0,(velNormalize.y)*255.0,(velNormalize.z)*255.0);
	//cout<<sqrt(pow(velNormalize.x,2)+pow(velNormalize.y,2)+pow(velNormalize.z,2))<<endl;
	//cout<<(double)velNormalize.x<<","<<(double)velNormalize.y<<","<<(double)velNormalize.z<<endl;
	//cout<<(double)color.r<<","<<(double)color.g<<","<<(double)color.b<<","<<sqrt(pow(color.r,2)+pow(color.g,2)+pow(color.b,2))<<endl;
	bool inside,outside;
	/*if(pos[pos.size()-1].length()>OBSTACLE_DISTANCE)
	{
		inside = false;
	}
	else
	{
		inside = true;
	}
	if(pos[pos.size()-1].length()<BOUNDARY_DISTANCE)
	{
		outside = false;
	}
	else
	{
		outside = true;
	}*/
	if(enemy)
	{
		ofSetLineWidth(10);
	}
	else
	{
		ofSetLineWidth(2);
	}

	for(int i=0;i<pos.size();i++)
	{
		mesh.addVertex(pos[i]);
		/*if(inside)
		{
			c.set(0,255,255,255/(pos.size()-i));
		}
		else if(outside)
		{
			c.set(255,255,0,255/(pos.size()-i));
		}
		else if(target)
		{	
			c.set(255,0,0,255/(pos.size()-i));
		}
		else
		{
			c.set(255,255,255,255/(pos.size()-i));
		}*/
		if(enemy)
		{
			c.set(255,0,0,255/(pow((double)(pos.size()-i),0.3)));
		}
		else
		{
			c.set(color.r,color.g,color.b,255/(pow((double)(pos.size()-i),1)));
			//c.set(255,255,255,255/(pow((double)(pos.size()-i),0.7)));
		}
		mesh.addColor(c);
	}
	ofPushMatrix();
	mesh.drawWireframe();
	ofPopMatrix();
}
void boid::setDistances(int index,double distance)
{
	distances[index]=distance;
}
void boid::setAngles(int index,double angle)
{
	angles[index]=angle;
}
void boid::setTarget(bool Target)
{
	target = Target;
}
void boid::setEnemy(bool Enemy)
{
	enemy = Enemy;
}
void::boid::setEnemyIndex(int EnemyIndex)
{
	enemyIndex = EnemyIndex;
}
void boid::addVel(ofVec3f vec)
{
	vel+=vec;
	//もし範囲外にいってしまったら、ベクトルの向きは維持しながら縮小、拡大する
	
	
	if(vel.length()<MIN_VEL)
	{
		vel=vel.normalize()*MIN_VEL;
	}
	else if(vel.length()>MAX_VEL)
	{
		vel=vel.normalize()*MAX_VEL;
	}
	
}
ofVec3f boid::getPos(int index)
{
	return pos[index];
}
ofVec3f boid::getVel()
{
	return vel;
} 
double boid::getDistance(int index)
{
	return distances[index];
}
double boid::getAngle(int index)
{
	return angles[index];
}
int boid::getPosSize()
{
	return pos.size();
}
bool boid::getEnemy()
{
	return enemy;
}

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
	ofBackground(0);
	for(int num=0;num<VOLUME_OF_BOIDS ;num++)
	{
		boid Boid(VOLUME_OF_BOIDS,BOUNDARY_DISTANCE,OBSTACLE_DISTANCE);
		boids.push_back(Boid);
	}
	ofEnableAlphaBlending();
	//ofEnableDepthTest();
	
}
void ofApp::update(){
	//それぞれのboid間の距離を求める
	for(int i=0;i<boids.size()-1;i++)
	{
		for(int j=i+1;j<boids.size();j++)
		{
			double distance=(boids[i].getPos(0)-boids[j].getPos(0)).length();
			boids[i].setDistances(j,distance);
			boids[j].setDistances(i,distance);
			//cout<<i<<":"<<distance<<endl;
			//cout<<i<<":"<<boids[i].getDistance(j)<<endl;
			
			double angle=boids[i].getVel().angleRad(boids[j].getVel());
			boids[i].setAngles(j,angle);
			boids[j].setAngles(i,angle);
		}
	}

	for(int i=0;i<boids.size();i++)
	{
		ofVec3f addVelocity;
		addVelocity.set(0,0,0);

		ofVec3f cohesion_velocity;
		cohesion_velocity.set(0,0,0);
		ofVec3f separation_velocity;
		separation_velocity.set(0,0,0);
		ofVec3f alignment_velocity;
		alignment_velocity.set(0,0,0);
		ofVec3f boundary_velocity;
		boundary_velocity.set(0,0,0);
		ofVec3f obstacle_velocity;
		obstacle_velocity.set(0,0,0);
		ofVec3f enemy_velocity;
		enemy_velocity.set(0,0,0);
		ofVec3f noise_velocity;
		noise_velocity.set(0,0,0);


		ofVec3f average_point;
		average_point.set(0,0,0);
		ofVec3f average_direct;
		average_direct.set(0,0,0);
		ofVec3f average_vel_direct;
		average_vel_direct.set(0,0,0);

		int count=0;

		//COHESION
		for(int j=0;j<boids.size();j++)
		{
			if(j != i && boids[i].getDistance(j) < COHESION_DISTANCE && boids[i].getAngle(j) < COHESION_ANGLE)
			{
				count++;
				average_point += boids[j].getPos(0);
			}
		}
		average_point /= count;
		cohesion_velocity = COHESION_FORCE*(average_point-boids[i].getPos(0));

		//SEPARATION
		count=0;
		for(int j=0;j<boids.size();j++)
		{
			if(j != i && boids[i].getDistance(j) < SEPARATION_DISTANCE && boids[i].getAngle(j) < SEPARATION_ANGLE)
			{
				count++;
				average_direct += boids[i].getPos(0)-boids[j].getPos(0);
			}
		}
		average_direct /= count;
		separation_velocity = SEPARATION_FORCE*average_direct;

		//ALIGNMENT
		count=0;
		for(int j=0;j<boids.size();j++)
		{
			if(j != i && boids[i].getDistance(j) < ALIGNMENT_DISTANCE && boids[i].getAngle(j) < ALIGNMENT_ANGLE)
			{
				count++;
				average_vel_direct += boids[j].getVel();
			}
		}
		average_vel_direct /= count;
		alignment_velocity = ALIGNMENT_FORCE*average_vel_direct;

		//BAUNDARY
		if(boids[i].getPos(0).length()>BOUNDARY_DISTANCE)
		{
			boundary_velocity = - BOUNDARY_FORCE*boids[i].getPos(0).normalize()*(boids[i].getPos(0).length()-BOUNDARY_DISTANCE);
		}

		//NOISE
		noise_velocity.set(0.5-ofNoise(i,0,ofGetElapsedTimef()*10),0.5-ofNoise(i,1,ofGetElapsedTimef()*10),0.5-ofNoise(i,2,ofGetElapsedTimef()*10));
		noise_velocity *= NOISE_FORCE;
		//SUMMARY

		addVelocity = cohesion_velocity+separation_velocity+alignment_velocity+boundary_velocity+obstacle_velocity+noise_velocity + enemy_velocity ;
		//OBSTACLE
		if(boids[i].getPos(0).length()<OBSTACLE_DISTANCE)
		{
			obstacle_velocity =  OBSTACLE_FORCE*boids[i].getPos(0).normalize()*(OBSTACLE_DISTANCE-boids[i].getPos(0).length());
		}

		//ENEMY
		if(boids[i].getDistance(enemyNum)<ENEMY_DISTANCE && !boids[i].getEnemy())
		{
			enemy_velocity =  ENEMY_FORCE*(boids[i].getPos(0)-boids[enemyNum].getPos(0)).normalize()*(ENEMY_DISTANCE-(boids[enemyNum].getPos(0)-boids[i].getPos(0).length()));
		}
		boids[i].addVel(addVelocity);

	}
	for(int i=0;i<boids.size();i++)
	{
		boids[i].update(ofGetElapsedTimef());
		boids[i].setTarget(false);
		boids[i].setEnemy(false);
		boids[i].setEnemyIndex(enemyNum);
	}
	boids[targetNum].setTarget(true);
	boids[enemyNum].setEnemy(true);
}
void ofApp::draw(){
	
	//cam.setDistance(300.0f);  
	
	cam.begin();
	ofDrawAxis(100);
	ofSetLineWidth(0.5);
	ofSetColor(0,0,255,255);
	ofSetSphereResolution(10);
	ofFill();
	//ofDrawSphere(0,0,OBSTACLE_DISTANCE*0.8);
	for(int i=0;i<boids.size()-1;i++)
	{
		boids[i].draw();
		/*ofSetColor(0,50,255,200);
		ofDrawSphere(boids[i].getPos(),5);
		ofSetColor(255,255,255,250);
		for(int j=i+1;j<boids.size();j++)
		{
			if(boids[i].getDistance(j)<50)
			{
				ofDrawLine(boids[i].getPos(),boids[j].getPos());
			}
		}*/
	}
	
	cam.end();
	//cam.setTarget(boids[targetNum].getPos(0));
	//cam.setPosition(boids[targetNum].getPos(boids[targetNum].getPosSize()-1));
}
void ofApp::mousePressed(int x, int y, int button)
{
	targetNum=(int)ofRandom(0,boids.size()-1);
}

