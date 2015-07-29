#ifndef I_DATASOURCE_HPP
#define I_DATASOURCE_HPP
#include "reflection/reflected_object.hpp"
#include "reflection/generic/generic_object.hpp"
#include "generic_plugin_system/interfaces/i_component_context.hpp"
class TestPolyStruct;

typedef ObjectHandleT< TestPolyStruct >  ReflectedPolyStructPtr;

class BinaryBlock;
class TestPage;
class TestPage2;

class IDataSource
{
public:
	virtual void init( IComponentContext & contextManager ) = 0;
	virtual void fini( IComponentContext & contextManager ) = 0;
	virtual const ObjectHandleT< TestPage > & getTestPage() const = 0;
	virtual const ObjectHandleT< TestPage2 > & getTestPage2() const = 0;
	virtual std::shared_ptr< BinaryBlock > getThumbnailImage() = 0;
};

#endif // I_DATASOURCE_HPP