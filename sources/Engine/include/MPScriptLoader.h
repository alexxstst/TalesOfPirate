#ifndef MPScriptLoader_h
#define MPScriptLoader_h

#include "MPEffPrerequisites.h"

class MINDPOWER_API MPScriptLoader
{
public:
	virtual ~ScriptLoader();

	virtual const MPStringVector& getScriptPatterns(void) const = 0;

	virtual void parseScript(MPDataStreamPtr& stream, const string& groupName) = 0;

	virtual float getLoadingOrder(void) const  = 0;
};

#endif
