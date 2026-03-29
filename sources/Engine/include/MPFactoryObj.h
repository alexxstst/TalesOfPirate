#ifndef MPFactoryObj_H
#define MPFactoryObj_H

/** . , .
*/
template< typename T > class FactoryObj
{
public:
	virtual ~FactoryObj() {};

	/** .	*/
	virtual const string& getType() const = 0;

	/** .
	@param name 
	@return .
	*/
	virtual T* createInstance( const string& name ) = 0;    
	/** .
	@param ptr 
	*/
	virtual void destroyInstance( T* ) = 0;    
};

#endif
