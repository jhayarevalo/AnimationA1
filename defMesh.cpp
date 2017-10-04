#include "defMesh.h"

DefMesh::DefMesh()
{
    pmodel = NULL;
    if (!pmodel) {	/* load up the model */

    char meshFile[] = "./model/cheb.obj";
    pmodel = glmReadOBJ(meshFile);
    if (!pmodel) {
        return;
    }
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 0);
        glmFacetNormals(pmodel);
    }
    mySkeleton.loadSkeleton("./model/skeleton.out");
	loadWeights("./model/weights.out");
}

void DefMesh::loadWeights(std::string weightsFileName)
{
	std::string strWeight;
	std::ifstream weightsFile(weightsFileName.c_str());
	if (weightsFile.is_open())
	{
		while (std::getline(weightsFile, strWeight)) { //Read a line to build a bone
			std::vector<std::string> weightParams;
			splitstring splitStr(strWeight);
			weightParams = splitStr.split(' ');

			std::vector<float> weightsOfVertex;

			for (int i = 0; i < weightParams.size(); i++) {
				weightsOfVertex.push_back(std::atof(weightParams[i].c_str())); //Stores a vertex for each bone
			}

			weights.push_back(weightsOfVertex); //Will have vectors of floats where each vector = 1 vertex with weights of each 17 bones
		}
	}
}

void DefMesh::copyVertices() {
	GLfloat* orignalVertices = pmodel->vertices;

	for (int i = 0; i < (3 * pmodel->numvertices) + 3; i++) {
		vertices.push_back(orignalVertices[i]);
	}
}


void DefMesh::updateVertices() {
//newVertexPosition = sum of all (weights of bone * transformation of bone) * currentVertexPosition

	GLfloat* meshVertices = pmodel->vertices;
	int verticesSize = pmodel->numvertices;

	std::vector<Eigen::Matrix4f, Eigen::aligned_allocator<Eigen::Matrix4f>> jointGlobalTrans = mySkeleton.getGlobalTransformationsOfJoints();//All 17 global transformations	

	for (int i = 3; i < (3*verticesSize)+3; i += 3) {
		Eigen::Matrix4f sumWeightTransform = Eigen::Matrix4f::Zero();

		for (int j = 1; j < jointGlobalTrans.size(); j++) {
			float weight = getWeight((i-3)/3,j-1);

			if(weight != 0)
				sumWeightTransform += (jointGlobalTrans[j] * weight);
		}
		Eigen::Vector4f vertexPosition(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
		vertexPosition = sumWeightTransform * vertexPosition;

		meshVertices[i] = vertexPosition.x();
		meshVertices[i + 1] = vertexPosition.y();
		meshVertices[i + 2] = vertexPosition.z();
	}
}

void DefMesh::glDraw(int type)
{
    
    switch(type){
    case 0:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    case 2:
        mySkeleton.glDrawSkeleton(); return;
    
    }
    glColor3f(0.5, 0.5, 0.5);
    mode = GLM_NONE;
    mode = mode | GLM_SMOOTH;
    glPushMatrix();
    glScalef(2,2,2);
    glTranslatef(-0.5, -0.5, -0.5);
    glmDraw(pmodel, mode);
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    mySkeleton.glDrawSkeleton();
}
