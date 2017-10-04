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
	Eigen::Matrix4d localTransformation = Eigen::Matrix4d::Identity();
	Eigen::Matrix4d globalTransformation = Eigen::Matrix4d::Identity();

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

	void updateLocalTransformation(Eigen::Vector3d& axis, double angle) {
		Eigen::Matrix3d rotationMatrix = Eigen::AngleAxisd(angle, axis).matrix();
		localTransformation.block<3, 3>(0, 0) = rotationMatrix;
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

	std::vector<Eigen::Matrix4f, Eigen::aligned_allocator<Eigen::Matrix4f>> getGlobalTransformationsOfJoints() {
		std::vector<Eigen::Matrix4f, Eigen::aligned_allocator<Eigen::Matrix4f>> allGlobalTransformations;

		for (int i = 0; i < joints.size()-1; i++) {
			Eigen::Matrix4d transformationD(joints[i].globalTransformation);
			Eigen::Matrix4f transformationF = transformationD.cast<float>();

			allGlobalTransformations.push_back(transformationF);
		}

		return allGlobalTransformations;
		
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

			Eigen::Matrix4d translateToParent = Eigen::Matrix4d::Identity();
			Eigen::Vector3d parentVector(parent->position.x, parent->position.y, parent->position.z);

			translateToParent.block<3, 1>(0, 3) = parentVector;

			joints[i].globalTransformation = parent->globalTransformation * translateToParent * joints[i].localTransformation * translateToParent.inverse();
		}
		
	}
};

#endif
