/*
 * Mesh with skeleton attached
 * You could put attachment here, or create an attachment class to manage weights
 */
#ifndef MESH_H
#define MESH_H
#include "./glm.h"
#include "./skeleton.h"
#include "splitstring.h"

#include <Eigen/Eigen>
class DefMesh
{
private:
	//std::vector<std::vector<float>> vertexWeights;
	std::vector<std::vector<float>> weights;
public:
    Skeleton mySkeleton;
    GLMmodel * pmodel;
    GLuint mode;
    DefMesh(); 
    void glDraw(int type);

	/*
	* Load Weights file
	*/
	void DefMesh::loadWeights(std::string weightsFileName);

	void DefMesh::updateVertices();

	float getWeight(int i, int j) {
		return weights[i][j];
	}
};
#endif
