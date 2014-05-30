/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2008  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */

//#include <mrpt/maps.h>
#include <mrpt/gui.h>
#include <mrpt/opengl/CTexturedPlane.h>

#include "CVirtualWorld.hpp"
#include <mrpt/opengl/CTexturedPlane.h>
#include <mrpt/opengl/CGridPlaneXY.h>
#include <mrpt/opengl/CAxis.h>

using namespace mrpt;
using namespace mrpt::opengl;
using namespace mrpt::poses;



CVirtualWorld::CVirtualWorld()
{
	/*CSimpleDatabaseTablePtr tableobjects, tablecompountobjects;

	tableobjects = vw.createTable("Objects");
	tableobjects->addField("id");
	tableobjects->addField("type");
	tableobjects->addField("pose");
	tableobjects->addField("size");
	tableobjects->addField("texture");

	tablecompountobjects=vw.createTable("CompoundObjects");
	tablecompountobjects->addField("id");
	tablecompountobjects->addField("objects");


	tableobjects->appendRecord();
	tablecompountobjects->appendRecord();
*/
//	LoadVirtualWorld("vw.xml");
//	SaveAs2DMap("newmapa.png");



}
CVirtualWorld::~CVirtualWorld()
{

}
bool CVirtualWorld::LoadVirtualWorld(const std::string file)
{
	CSimpleDatabaseTablePtr table;

	vw.loadFromXML(file);
	table=vw.getTable("Objects");
	size_t numobj=table->getRecordCount();

	for (size_t j=0;j<numobj;j++)
	{
		Object o;
		o.id=table->get(j,"id");
	//	o.type=table->get(j,"type");
		o.texture=table->get(j,"texture");
		o.size=table->get(j,"size");

		CPose3DPtr pose3D=CPose3D::Create();
		printf("%s\n",table->get(j,"pose").c_str());
		pose3D->fromString(table->get(j,"pose"));


		o.pose=pose3D;

		list_objects.push_back(o);
	}



	return true;
}
bool CVirtualWorld::SaveVirtualWorld(const std::string file)
{
	vw.saveAsXML(file);
	return true;
}
void CVirtualWorld::SaveAs2DMap(std::string file)
{
	using namespace mrpt::gui;


	CImagePtr map=CImage::Create();

	map->resize(800,600,3,true);

	map->filledRectangle(0,0,800,600,TColor(0xFFFFFF));
	double offx=400,offy=300;
	double scale=10;

	for (size_t i=0;i<list_objects.size();i++)
	{


		double longitud;
		//double sizex,sizey;

		std::deque<std::string> tok;
		mrpt::system::tokenize(list_objects[i].size,"[ ",tok);

		//printf("%f,%f\n",atof(tok[0].c_str()),atof(tok[1].c_str()));
		// sizex=atof(tok[1].c_str());  // JL: Not used??
		longitud=atof(tok[0].c_str());

		mrpt::math::CVectorDouble pose;
		list_objects[i].pose->getAsVector(pose);
		//printf("longitud %f(%f)\n",pose[3],sin(DEG2RAD(pose[3])));
		//printf("x2,y2,%f,%f\n",longitud*cos(DEG2RAD(pose[4])),pose[1]+longitud*sin(DEG2RAD(pose[4])));

		double x0,y0,x1,y1;

		x0=offx+(scale*pose[0]);
		y0=offy-(scale*pose[1]);
		x1=offx+(scale*pose[0])-(scale*longitud*sin(pose[3]));
		y1=offy-(scale*pose[1])-(scale*longitud*cos(pose[3]));
		map->line(x0,y0,x1,y1,TColor(0x000000),10);

	}


	map->saveToFile(file);







}
void CVirtualWorld::GetWorld(mrpt::opengl::CSetOfObjectsPtr &world)
{
	using namespace mrpt::gui;
	world->clear();
	for (size_t i=0;i<list_objects.size();i++)
	{

		double sizex,longitud;

		std::deque<std::string> tok;
		mrpt::system::tokenize(list_objects[i].size,"[ ",tok);

		//printf("%f,%f\n",atof(tok[0].c_str()),atof(tok[1].c_str()));
		sizex=atof(tok[1].c_str());
		longitud=atof(tok[0].c_str());
		CTexturedPlanePtr plane=CTexturedPlane::Create(0,sizex,0,longitud);


		plane->setPose(*list_objects[i].pose.pointer());

		CImagePtr image=CImage::Create();
		if (image->loadFromFile( list_objects[i].texture))
			plane->assignImage(*image.pointer());
		else
			printf("Texture file is missing or there was an error while loading\n");
		world->insert(plane);


	}



}


void CVirtualWorld::Draw(mrpt::opengl::C3DSScenePtr obj)
{
	using namespace mrpt::gui;

	win.resize(800,600);


//just for playing
//	win.setCameraPointingToPoint(1,16,0);
//	win.setCameraAzimuthDeg(90);
//	win.setCameraElevationDeg(9);
//	win.setCameraZoom(7.5);


	scene1=COpenGLScenePtr(new COpenGLScene());
	opengl::CGridPlaneXYPtr plane1=CGridPlaneXY::Create(-20,20,-20,20,0,1);
	CTexturedPlanePtr suelo=CTexturedPlane::Create(-20,20,-20,20);
	suelo->setColor(0.2,0.5,0.8);
	suelo->setLocation(0,0,0);
	scene1->insert(suelo);





	CAxisPtr axis=CAxis::Create(-10,-10,0,10,10,5,5,3,true);


	scene1->insert(plane1);
	scene1->insert(axis);




	for (size_t i=0;i<list_objects.size();i++)
	{


		double sizex,longitud;

		std::deque<std::string> tok;
		mrpt::system::tokenize(list_objects[i].size,"[ ",tok);

		//printf("%f,%f\n",atof(tok[0].c_str()),atof(tok[1].c_str()));
		sizex=atof(tok[1].c_str());
		longitud=atof(tok[0].c_str());
		CTexturedPlanePtr plane=CTexturedPlane::Create(0,sizex,0,longitud);


		plane->setPose(*list_objects[i].pose.pointer());

		CImagePtr image=CImage::Create();
		if (image->loadFromFile( list_objects[i].texture))
			plane->assignImage(*image.pointer());
		else
			printf("Texture file is missing or there was an error while loading\n");
		scene1->insert(plane);


	}

	robot=obj;

	robot->setLocation(0,0,0);
	scene1->insert( robot );


	win.get3DSceneAndLock()=scene1;
	win.unlockAccess3DScene();



	win.waitForKey();



}
void CVirtualWorld::LocateRobot(CPose2D &location)
{
	location.phi_incr(3.14159/2);

	robot->setPose(location);

	double x=location.x();
	double y=location.y();
	win.setCameraPointingToPoint(x,y,0);

	win.repaint();



}
