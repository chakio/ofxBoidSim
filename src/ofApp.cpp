#include "ofApp.h"
//--------------------------------------------------------------
boid::boid(int boidSize, double boundary)
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
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	BOUNDARY_DISTANCE = boundary;
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
	ofColor c,color;
	ofVec3f velNormalize=vel.normalized();
	//cout <<velNormalize.length()<<endl;
	color.set((velNormalize.x)*255.0,(velNormalize.y)*255.0,(velNormalize.z)*255.0);
	//cout<<sqrt(pow(velNormalize.x,2)+pow(velNormalize.y,2)+pow(velNormalize.z,2))<<endl;
	//cout<<(double)velNormalize.x<<","<<(double)velNormalize.y<<","<<(double)velNormalize.z<<endl;
	//cout<<(double)color.r<<","<<(double)color.g<<","<<(double)color.b<<","<<sqrt(pow(color.r,2)+pow(color.g,2)+pow(color.b,2))<<endl;
	bool inside,outside;

	
	ofMesh triangle;
	ofVec3f top,left,right,crossed;
	crossed = vel.getRotated(90,ofVec3f(0,0,1))/30;
	top=pos[pos.size()-1]+vel/30;
	left=pos[pos.size()-1] +crossed;
	right=pos[pos.size()-1] - crossed;
	
	mesh.addVertex(left);
	mesh.addVertex(right);
	mesh.addVertex(top);
	mesh.clearColors();
	if(mesh.getNumVertices()>pos.size()*2)
	{
		for(int i=0;i<2;i++)
		{
			mesh.removeVertex(i);
		}
	}
	for(int i=0;i<mesh.getNumVertices();i++)
	{
		c.set(color.r,color.g,color.b,255/(1+pow((double)(mesh.getNumVertices()-i)/mesh.getNumVertices()*3,2)));
		mesh.addColor(c);
	}

	ofPushMatrix();
	mesh.draw();
	ofPopMatrix();

	//頂点だけ削除しておく
	mesh.removeVertex(mesh.getNumVertices()-1);

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

//--------------------------------------------------------------
attractor::attractor()
{

}
void attractor::setup(double size,double Time, double velsize)
{
	boxSize 	= size;
	beforeTime 	= 	Time;
	position 	= ofVec3f(boxSize/2,boxSize/2,boxSize/2);
	velSize		= velsize;
	this->setVelocity();
}
void attractor::setVelocity()
{
	if(position.x > boxSize/2)
	{
		position.x = boxSize/2;
	}
	else if(position.x < -boxSize/2)
	{
		position.x = -boxSize/2;
	}
	if(position.y > boxSize/2)
	{
		position.y = boxSize/2;
	}
	else if(position.y < -boxSize/2)
	{
		position.y = -boxSize/2;
	}
	if(position.z > boxSize/2)
	{
		position.z = boxSize/2;
	}
	else if(position.z < -boxSize/2)
	{
		position.z = -boxSize/2;
	}

	int normalizePos[3] = {(int)position.x/(boxSize/2),(int)position.y/(boxSize/2),(int)position.z/(boxSize/2)};
	int randomIndex = ofRandom(0,60)/20;
	if(randomIndex==0)
	{
		velocity=ofVec3f(-normalizePos[0]*velSize,0,0);
	}
	else if(randomIndex==1)
	{
		velocity=ofVec3f(0,-normalizePos[0]*velSize,0);
	} 
	else if(randomIndex==2)
	{
		velocity=ofVec3f(0,0,-normalizePos[0]*velSize);
	} 
}
void attractor::update(double Time)
{
	double dTime 	= 	Time-beforeTime;
	beforeTime 		= 	Time; 
	position		+=  dTime*velocity;

	if(abs(position.x) >boxSize/2 || abs(position.y) >boxSize/2 || abs(position.z) >boxSize/2)
	{
		this->setVelocity();
	}
}
void attractor::draw()
{
	ofSetLineWidth(0.5);
	ofSetColor(0,0,255,255);
	ofSetBoxResolution(1);
	ofNoFill();
	ofDrawBox(boxSize);
	ofFill();
	ofSetColor(255,0,0,255);
	ofDrawSphere(position,boxSize/30);
	//cout<<position<<endl;
}
ofVec3f attractor::getPos()
{
	return position;
}


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
	ofBackground(0);
	for(int num=0;num<VOLUME_OF_BOIDS ;num++)
	{
		boid Boid(VOLUME_OF_BOIDS,BOUNDARY_DISTANCE);
		boids.push_back(Boid);
	}
	Attractor.setup(BOX_SIZE,ofGetElapsedTimef(),ATTRACTOR_VEL);
	ofEnableAlphaBlending();
	ofEnableDepthTest();
	
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
		ofVec3f cohesion_velocity;
		ofVec3f separation_velocity;
		ofVec3f alignment_velocity;
		ofVec3f boundary_velocity;
		ofVec3f noise_velocity;
		ofVec3f attractor_velocity;

		ofVec3f average_point;
		ofVec3f average_direct;
		ofVec3f average_vel_direct;

		int count = 0;


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

		//ATTRACTOR
		attractor_velocity = - ATTRACTOR_FORCE*(boids[i].getPos(0)-Attractor.getPos()).normalize()*(boids[i].getPos(0)-Attractor.getPos()).length();

		//NOISE
		noise_velocity.set(0.5-ofNoise(i,0,ofGetElapsedTimef()*1),0.5-ofNoise(i,1,ofGetElapsedTimef()*1),0.5-ofNoise(i,2,ofGetElapsedTimef()*1));
		noise_velocity *= NOISE_FORCE;

		//SUMMARY
		addVelocity = cohesion_velocity+separation_velocity+alignment_velocity+boundary_velocity+noise_velocity+attractor_velocity;

		boids[i].addVel(addVelocity);

	}
	for(int i=0;i<boids.size();i++)
	{
		boids[i].update(ofGetElapsedTimef());
		boids[i].setTarget(false);
	}
	boids[targetNum].setTarget(true);
	Attractor.update(ofGetElapsedTimef());
}
void ofApp::draw(){
	
	//cam.setDistance(300.0f);  
	
	cam.begin();
	ofDrawAxis(100);
	
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
	Attractor.draw();
	cam.end();
	cam.setTarget(boids[targetNum].getPos(0));
	cam.setPosition(boids[targetNum].getPos(boids[targetNum].getPosSize()-1));
}
void ofApp::mousePressed(int x, int y, int button)
{
	targetNum=(int)ofRandom(0,boids.size()-1);
}

