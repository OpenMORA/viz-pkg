#ifndef __CVIRTUALWORLD_H
#define __CVIRTUALWORLD_H

#include <mrpt/utils/CSimpleDatabase.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/opengl/C3DSScene.h>

using namespace mrpt::utils;
using namespace std;

class CVirtualWorld
{
public:
	CVirtualWorld();
	~CVirtualWorld();
	bool LoadVirtualWorld(const std::string file);
	bool SaveVirtualWorld(const std::string file);

	void GetWorld(mrpt::opengl::CSetOfObjectsPtr &world);

	void LocateRobot(mrpt::poses::CPose2D &location);
	void SaveAs2DMap(std::string file);

	bool ListofObjects(std::string &objects);

	void Draw(mrpt::opengl::C3DSScenePtr obj);

	typedef struct
	{
		std::string id;
		enum
        {
        	TWall = 0,
        	TBlock,
			TSphere
        }type;
		mrpt::poses::CPose3DPtr pose;

		std::string size;        //Wall [width height] Block [width length height ] Sphere [radius]
		std::string texture;
	}Object;

	typedef struct
	{
		std::string id;
		std::vector<Object> components;
	}CompoundObject;

private:

	CSimpleDatabase		vw;
	std::vector<Object> list_objects;
	std::vector<CompoundObject> list_compound_objects;

	mrpt::opengl::COpenGLScenePtr scene1;
	mrpt::opengl::C3DSScenePtr robot;
	mrpt::gui::CDisplayWindow3D win;

};
#endif
