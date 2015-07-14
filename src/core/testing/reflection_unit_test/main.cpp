#include "pch.hpp"

#include "variant/variant.hpp"
#include "variant/collection.hpp"
#include "variant/default_meta_type_manager.hpp"
#include "test_objects.hpp"

int main( int argc, char* argv[] )
{
	DefaultMetaTypeManager metaTypeManager;
	Variant::setMetaTypeManager( &metaTypeManager );

	std::vector<std::unique_ptr<MetaType>> metaTypes;
	metaTypes.emplace_back(new MetaTypeImpl<BW::Vector3>);
	metaTypes.emplace_back(new MetaTypeImpl<BW::Vector4>);
	metaTypes.emplace_back(new MetaTypeImpl<std::shared_ptr< BinaryBlock >>);
	metaTypes.emplace_back(new MetaTypeImpl<ObjectHandle>);

	for(const auto& m: metaTypes)
	{
		metaTypeManager.registerType(m.get());
	}

	int result = 0;
	result = BWUnitTest::runTest( "", argc, argv );

	return result;
}

// main.cpp