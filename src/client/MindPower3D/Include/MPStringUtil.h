#pragma once

#ifndef MPStringUtil_H
#define MPStringUtil_H

#include "MPEffPrerequisites.h"
#include "MPStringVector.h"

class MINDPOWER_API MPStringUtil
{
public:
	typedef std::stringstream StrStreamType;

public:
	/** , Tabsbe.
	@remarks
	.
	*/
	static void trim( String& str, bool left = true, bool right = true );

	/** MPStringVector.
	@param 
	delims 
	@param 
	maxSplits 00
	.
	*/
	static MPStringVector split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

	/** .
	*/
	static void toLowerCase( String& str );

	/** .
	*/
	static void toUpperCase( String& str );

};
#endif
