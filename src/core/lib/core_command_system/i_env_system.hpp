#ifndef I_ENV_SYSTEM
#define I_ENV_SYSTEM

#include <memory>

struct ECGUID
{
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;

	bool operator == (const ECGUID& other) const
	{
		return a == other.a && b == other.b && c == other.c && d == other.d;
	}
};

class IEnvComponent
{
public:
	virtual const ECGUID& getGUID() const = 0;
};

class IEnvState
{
public:
	typedef std::unique_ptr<IEnvComponent> IEnvComponentPtr;
	virtual void add( IEnvComponentPtr ec ) = 0;
	virtual IEnvComponentPtr remove( const ECGUID& guid ) = 0;
	virtual IEnvComponent* query( const ECGUID& guid ) const = 0;
};

class IEnvEventListener
{
public:
	virtual void onAddEnv( IEnvState* state ) = 0;
	virtual void onRemoveEnv( IEnvState* state ) = 0;
	virtual void onSelectEnv( IEnvState* state ) = 0;
};

class IEnvManager
{
public:
	virtual void registerListener( IEnvEventListener* listener ) = 0;
	virtual void deregisterListener( IEnvEventListener* listener ) = 0;

	virtual int addEnv() = 0;
	virtual void removeEnv( int id ) = 0;
	virtual void selectEnv( int id ) = 0;
};

#define DEFINE_EC_GUID public: static const ECGUID GUID; virtual const ECGUID& getGUID() const override { return GUID; }
#define DECLARE_EC_GUID(CL, A, B, C, D) const ECGUID CL::GUID = { A, B, C, D }

#define ENV_STATE_ADD( CL, var ) auto var = new CL; state->add( IEnvState::IEnvComponentPtr( var ) )
#define ENV_STATE_REMOVE( CL, var ) auto __u_ptr__ = state->remove( CL::GUID ); auto var = static_cast<CL*>( __u_ptr__.get() )
#define ENV_STATE_QUERY( CL, var ) auto var = static_cast<CL*>(state->query( CL::GUID )); assert(var)


#endif // I_ENV_SYSTEM