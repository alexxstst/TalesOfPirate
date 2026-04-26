#ifndef MPParticleAffector_H
#define MPParticleAffector_H

#include "MindPowerAPI.h"

class MPParticleSystem;
class MPParticle;

/** .
@remarks

vector forcefader

@par


ParticleAffector
ParticleAffectorFactory
ParticleSystemManageraddAffectorFactory,

@par
.,
,.
*/
class MPParticleAffector {
public:
	MPParticleAffector(MPParticleSystem* mParticleSystem) : m_ParticleSystem(mParticleSystem) {
	}

	virtual ~MPParticleAffector();

	/** 
	@remarks
	.
	,
	@param
	pParticle .
	*/
	virtual void _initParticle(MPParticle* pParticle) {
		/* by default do nothing */
	}

	/** 
	@remarks
	.
	,
	@param
	pSystem .
	@param
	timeElapsed ().
	*/
	virtual void _affectParticles(MPParticleSystem* pSystem, float timeElapsed) = 0;

	/** . 
	@remarks
	,
	
	*/
	const String& getType(void) const {
		return m_Type;
	}

protected:
	/** 
	@remarks
	StringInterface,
	.,
	,
	@par
	createParamDictionary
	
	*/
	void addBaseParameters(void) {
		/*  -  */
	}

protected:
	/// , 
	string m_Type;

	MPParticleSystem* m_ParticleSystem;
};
#endif
