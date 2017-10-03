#ifndef SKELETON_H
#define SKELETON_H
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include "GL/glut.h"
#else
#include <GL/freeglut.h>
#endif
#endif

#include "simpleMath.h"
#include <Eigen/Eigen>
#include <Eigen/StdVector>
#include <Eigen/Geometry>

using namespace std;

struct Joint
{
	Eigen::Matrix4f localTransformation = Eigen::Matrix4f::Identity();
	Eigen::Matrix4f globalTransformation = Eigen::Matrix4f::Identity();

	int indexParent;
    Vec3 position;
    Vec2 screenCoord;
    bool isHovered;
    bool isPicked;
    
    Joint()
    {
        isHovered = false;
        isPicked = false;
    }

	void updateLocalTransformation(Eigen::Vector3f& axis, double angle) {
		Eigen::Matrix3f rotationMatrix = Eigen::AngleAxisf(angle, axis).matrix();
		//Eigen::Vector3f positionMatrix(position.x, position.y, position.z);

		/*rotationMatrix(0,0) = pow(axis.x(), 2) + cos(angle) * (1 - pow(axis.x(), 2));
		rotationMatrix(0, 1) = (axis.x())*(axis.y())*(1 - cos(angle)) - (axis.z())*sin(angle);
		rotationMatrix(0, 2) = (axis.x())*(axis.z())*(1 - cos(angle)) + (axis.y())*sin(angle);

		rotationMatrix(1, 0) = (axis.x())*(axis.y())*(1 - cos(angle)) + (axis.z())*sin(angle);
		rotationMatrix(1, 1) = pow(axis.y(), 2) + cos(angle) * (1 - pow(axis.y(), 2));
		rotationMatrix(1, 2) = (axis.y())*(axis.z())*(1 - cos(angle)) - (axis.x())*sin(angle);

		rotationMatrix(2, 0) = (axis.x())*(axis.z())*(1 - cos(angle)) - (axis.y())*sin(angle);
		rotationMatrix(2, 1) = (axis.y())*(axis.z())*(1 - cos(angle)) - (axis.x())*sin(angle);
		rotationMatrix(2, 2) = pow(axis.z(), 2) + cos(angle) * (1 - pow(axis.z(), 2));

		rotationMatrix(0, 3) = rotationMatrix(1, 3) = rotationMatrix(2, 3) = 0;
		rotationMatrix(3, 0) = rotationMatrix(3, 1) = rotationMatrix(3, 2) = 0;

		rotationMatrix(3, 3) = 1;*/

		localTransformation.block<3, 3>(0, 0) = rotationMatrix;
		//localTransformation = rotationMatrix;
	}


};

class Skeleton
{
private:
    std::vector<Joint, Eigen::aligned_allocator<Joint>> joints;
    /*Update screen coordinates of joints*/
    void updateScreenCoord();
    
public:
    /*True if the skeleton has a joint selected*/
    bool hasJointSelected;   
    Skeleton(){hasJointSelected = false;};
    /*
     * Load Skeleton file
     */
    void loadSkeleton(std::string skelFileName);

    /*
     * Load animation file
     */
    void loadAnimation(std::string skelFileName);

    /*
     * Draw skeleton with OpenGL
     */
    void glDrawSkeleton();

    /*
     * Check if any joint is hovered by given mouse coordinate
     */
    void checkHoveringStatus(int x, int y);

    void release();
    
    void selectOrReleaseJoint();

	std::vector<Joint, Eigen::aligned_allocator<Joint>>* getJoints() {
		return &joints;
	}

	Joint* getSelectedJoint() {
		for (int i = 0; i < joints.size(); i++) {
			if (joints[i].isPicked) {
				return &joints[i];
				cout << "joint " << i << " was picked.";
				break;
			}
		}
		return nullptr;
	}

	Joint* getParentJoint() {
		for (int i = 0; i < joints.size(); i++) {
			if (joints[i].isPicked) {
				return &joints[joints[i].indexParent];
				break;
			}
		}
		return nullptr;
	}

	void updateGlobalTransformation() {
		for (int i = 1; i < joints.size(); i++) {
			Joint* parent = &joints[joints[i].indexParent];

			Eigen::Matrix4f translateToParent = Eigen::Matrix4f::Identity();
			Eigen::Vector3f parentVector(parent->position.x, parent->position.y, parent->position.z);

			translateToParent.block<3, 1>(0, 3) = parentVector;

			joints[i].globalTransformation = parent->globalTransformation * translateToParent * joints[i].localTransformation * translateToParent.inverse();

			/*translateToParent(0, 3) = parent->position.x;
			translateToParent(1, 3) = parent->position.y;
			translateToParent(2, 3) = parent->position.z;*/

			//joints[i].globalTransformation = parent->globalTransformation  * joints[i].localTransformation;
		}
		
	}
};

#endif
