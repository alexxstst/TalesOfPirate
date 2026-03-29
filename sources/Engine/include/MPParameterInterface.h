#ifndef MPParameterInterface_H
#define MPParameterInterface_H

#include "MPEffPrerequisites.h"

/// 
enum MPParameterType
{
	PT_BOOL,
	PT_REAL,
	PT_INT,
	PT_UNSIGNED_INT,
	PT_SHORT,
	PT_UNSIGNED_SHORT,
	PT_LONG,
	PT_UNSIGNED_LONG,
	PT_STRING,
	PT_VECTOR3,
	PT_MATRIX3,
	PT_MATRIX4,
	PT_QUATERNION,
	PT_COLOURVALUE
};

/// ,MPParameterInteface
class MINDPOWER_API MPParameterDef
{
public:
	String name;
	String description;
	MPParameterType paramType;
	MPParameterDef(const String& newName, const String& newDescription, MPParameterType newType)
		: name(newName), description(newDescription), paramType(newType) {}
};
typedef std::vector<MPParameterDef> MPParameterList;

/** /.*/
class MINDPOWER_API MPParamCommand
{
public:
	virtual String doGet(const void* target) const = 0;
	virtual void doSet(void* target, const String& val) = 0;

	virtual ~MPParamCommand() { }
};
typedef std::map<String, MPParamCommand* > ParamCommandMap;

class MINDPOWER_API MPParamDictionary
{
	friend class MPParameterInterface;
protected:
	/// 
	MPParameterList m_ParamDefs;

	/// get/set
	ParamCommandMap m_ParamCommands;

	/** . */
	MPParamCommand* getParamCommand(const String& name)
	{
		ParamCommandMap::iterator i = m_ParamCommands.find(name);
		if (i != m_ParamCommands.end())
		{
			return i->second;
		}
		else
		{
			return 0;
		}
	}

	const MPParamCommand* getParamCommand(const String& name) const
	{
		ParamCommandMap::const_iterator i = m_ParamCommands.find(name);
		if (i != m_ParamCommands.end())
		{
			return i->second;
		}
		else
		{
			return 0;
		}
	}
public:
	MPParamDictionary()  {}

	/** . 
	@param paramDef MPParameterDef
	@param paramCmd getting/setting.
	NB: 

	*/
	void addParameter(const MPParameterDef& paramDef, MPParamCommand* paramCmd)
	{
		m_ParamDefs.push_back(paramDef);
		m_ParamCommands[paramDef.name] = paramCmd;
	}

	const MPParameterList& getParameters(void) const
	{
		return m_ParamDefs;
	}
};

typedef std::map<String, MPParamDictionary> MPParamDictionaryMap;

/** .
@remarks
	-mapMPParamDictionary. 
@remarks
	createParamDictionary
	.
*/
class MINDPOWER_API MPParameterInterface
{
public:

	virtual ~StringInterface() {}

	MPParamDictionary* getParamDictionary(void)
	{
		MPParamDictionaryMap::iterator i = m_sDictionary.find(m_ParamDictName);
		if (i != m_sDictionary.end())
		{
			return &(i->second);
		}
		else
		{
			return 0;
		}
	}

	const MPParamDictionary* getParamDictionary(void) const
	{
		MPParamDictionaryMap::const_iterator i = m_sDictionary.find(m_ParamDictName);
		if (i != m_sDictionary.end())
		{
			return &(i->second);
		}
		else
		{
			return 0;
		}
	}

	const MPParameterList& getParameters(void) const;

	virtual bool setParameter(const String& name, const String& value);
	
	virtual void setParameterList(const NameValuePairList& paramList);

	virtual String getParameter(const String& name) const
	{
		// 
		const MPParamDictionary* dict = getParamDictionary();

		if (dict)
		{
			// 
			const MPParamCommand* cmd = dict->getParamCommand(name);

			if (cmd)
			{
				return cmd->doGet(this);
			}
		}

		// 
		return "";
	}

	virtual void copyParametersTo(MPParameterInterface* dest) const
	{
		// 
		const MPParamDictionary* dict = getParamDictionary();

		if (dict)
		{
			// 
			ParameterList::const_iterator i;

			for (i = dict->mParamDefs.begin(); 
				i != dict->mParamDefs.end(); ++i)
			{
				dest->setParameter(i->name, getParameter(i->name));
			}
		}


	}

	static void cleanupDictionary () ;

protected:
	/** .
	@remarks
		,.NB:
	(RTTI)
	@param
	className ()
	@returns
		true , false 
	*/
	bool createParamDictionary(const String& className)
	{
		m_ParamDictName = className;
		if (m_sDictionary.find(className) == m_sDictionary.end())
		{
			m_sDictionary[className] = ParamDictionary();
			return true;
		}
		return false;

	}

protected:

	/// 
	static MPParamDictionaryMap m_sDictionary;
	///  ()
	String m_ParamDictName;

};

#endif
