#ifndef MPParticleAffectorFactory_H
#define MPParticleAffectorFactory_H

#include "MPEffPrerequisites.h"

class MPParticleAffectorFactory {
protected:
	std::vector<MPParticleAffector*> m_Affectors;

public:
	MPParticleAffectorFactory() {
	};
	virtual ~MPParticleAffectorFactory();
	/** ,  */
	virtual string getName() const = 0;

	/** .*/
	virtual MPParticleAffector* createAffector(MPParticleSystem* psys) = 0;

	/** . */
	virtual void destroyAffector(MPParticleAffector* e);
};
#endif
