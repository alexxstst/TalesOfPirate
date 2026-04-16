#ifndef MPParticleEmitterFactory_H
#define MPParticleEmitterFactory_H

#include "MPEffPrerequisites.h"

class MPParticleEmitterFactory
{
protected:
	std::vector<MPParticleEmitter*> m_Emitters;
public:
	MPParticleEmitterFactory() {};
	virtual ~MPParticleEmitterFactory();

	virtual string getName() const = 0;

	virtual MPParticleEmitter* createEmitter(MPParticleSystem* psys) = 0;

	virtual void destroyEmitter(MPParticleEmitter* e);
};
#endif
