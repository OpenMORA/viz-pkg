#ifndef _INTERFAZSANCHO_DIBUJABLE_H
#define _INTERFAZSANCHO_DIBUJABLE_H

#include <wx/dc.h>
#include <wx/gdicmn.h>
#include <string>
#include <sstream>
#include <mrpt/utils/CObject.h>
#include <mrpt/gui/WxUtils.h>

namespace InterfazSancho	{
/**
  * Search function inside a smart pointer container.
  */
template<typename SP_CONTAINER,typename SP> static inline typename SP_CONTAINER::iterator findSP(SP_CONTAINER &cont,const SP &smartPtr)	{
	return findSP(cont.begin(),cont.end(),smartPtr);
	/*for (typename SP_CONTAINER::iterator it=cont.begin();it!=cont.end();++it) if (it->pointer()==smartPtr.pointer()) return it;
	return cont.end();*/
}
/**
  * Search function inside a smart pointer const container.
  */
template<typename SP_CONTAINER,typename SP> static inline typename SP_CONTAINER::const_iterator findSP(const SP_CONTAINER &cont,const SP &smartPtr)	{
	return findSP(cont.begin(),cont.end(),smartPtr);
	/*for (typename SP_CONTAINER::const_iterator it=cont.begin();it!=cont.end();++it) if (it->pointer()==smartPtr.pointer()) return it;
	return cont.end();*/
}
/**
  * Search function between a pair of iterators to smart pointers.
  */
template<typename SP_ITERATOR,typename SP> static inline SP_ITERATOR findSP(const SP_ITERATOR &begin,const SP_ITERATOR &end,const SP &smartPtr)	{
	for (SP_ITERATOR it=begin;it!=end;++it) if (it->pointer()==smartPtr.pointer()) return it;
	return end;
}

DEFINE_MRPT_OBJECT_PRE_NO_LINKAGE(Dibujable)
/**
  * Basic drawable object. States and transitions derive from this class, and all of them are used as smart pointers. This is mostly an abstract class.
  * \sa Estado, Transicion
  */
class Dibujable:public mrpt::utils::CObject	{
	DEFINE_VIRTUAL_MRPT_OBJECT(Dibujable)
public:
	virtual void activar()=0;	//!< This function must be called whenever the object is selected.
	virtual void cuadroDeDialogo()=0;	//!< This function calls a dialog box to configure this object. The function should perform all the configuration.
	virtual void desactivar()=0;	//!< This function must be called whenever the object is deselected.
	virtual void desaparecer()=0;	//!< Signal the object to disappear.
	virtual void dibujar(wxDC &)=0;	//!< Draw the object using the given draw context.
	virtual bool esPunto(const wxPoint &) const=0;	//!< Returns whether the given point is inside this object.
	virtual void generarSCXML(std::ostringstream &) const=0;	//!< Writes into the given stream the code which creates this object.
	inline void generarSCXML(std::string &res) const	{	//!< Wrapper function for the homonym. Stores the SCXML code of this object in the given string.
		std::ostringstream oss;
		generarSCXML(oss);
		res=oss.str();
	}
};
}	//End of namespace
#endif
