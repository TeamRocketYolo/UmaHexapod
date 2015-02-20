/**************************************************************************
*  Workfile		: Object.h
*  Author		: Martin Schwarzbauer
*  Date			: 2007-09-24
*  Description	: abstract base class
*  Remarks      : -
*  Revision		: 1
**************************************************************************/
#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public:
	//D-Tor
	virtual ~Object ();

protected:
	//C-Tor
	Object ();
};

#endif