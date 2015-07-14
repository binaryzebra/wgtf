#include "copy_paste_manager.hpp"
#include "i_copyable_object.hpp"
#include "serialization/serializer/i_serialization_manager.hpp"
#include "command_system/i_command_manager.hpp"
#include "serialization/text_stream.hpp"
#include "variant/collection.hpp"


//TODO: Switch to multiplatform clipboard handles
#include "ngt_core_common/ngt_windows.hpp"

namespace
{
	const char * s_ValueHintTag = "DATAHINT";
	const char * s_ValueTag = "COPYDATA";
}


//==============================================================================
CopyPasteManager::CopyPasteManager()
{
}


//==============================================================================
CopyPasteManager::~CopyPasteManager()
{
	
}


//==============================================================================
void CopyPasteManager::onSelect( ICopyableObject* pObject, bool append )
{
	assert( pObject != nullptr );
	if(!append)
	{
		curObjects_.clear();
	}
	curObjects_.push_back( pObject );
}


//==============================================================================
void CopyPasteManager::onDeselect( ICopyableObject* pObject, bool reset )
{
	if(reset)
	{
		curObjects_.clear();
		return;
	}
	curObjects_.erase( 
		std::remove( curObjects_.begin(), curObjects_.end(), pObject ), curObjects_.end() );
}


//==============================================================================
bool CopyPasteManager::copy()
{
	assert( !curObjects_.empty() );
	TextStream stream("", std::ios::out);
	bool ret = true;
	std::vector< ICopyableObject* >::iterator iter;
	for (iter = curObjects_.begin(); iter != curObjects_.end() && ret; ++iter)
	{
		assert( *iter != nullptr );
		const char * hint = (*iter)->getDataHint();
		const Variant & value = (*iter)->getData();
		if(strcmp(hint, "") != 0)
		{
			stream.write(s_ValueHintTag);
			stream.write( hint );
		}
		ret = serializeData( stream, value );
	}
	if(!ret)
	{
		assert( false );
		return false;
	}
	if (!OpenClipboard( NULL ))
	{
		assert( false );
		return false;
	}

	// copy data to clipboard
	std::string data = stream.getData();
	size_t size = data.length();
	EmptyClipboard();
	HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,size + 1);
	if (!hg){
		CloseClipboard();
		assert( false );
		return false;
	}
	memcpy( GlobalLock(hg),data.c_str(),size + 1 );
	GlobalUnlock(hg);
	HANDLE newHandle = SetClipboardData( CF_TEXT,hg );
	int errorcode = ::GetLastError();
	CloseClipboard();
	GlobalFree(hg);

	return ret;
}


//==============================================================================
bool CopyPasteManager::paste()
{
	assert( !curObjects_.empty() );

	if (!OpenClipboard( NULL )) 
	{
		assert( false );
		return false;
	}

	// get data from clipboard
	HANDLE hClipboardData = GetClipboardData(CF_TEXT);
	SIZE_T length = GlobalSize( hClipboardData );
	char * pData = (char*)GlobalLock(hClipboardData);
	assert( pData != nullptr );

	// if nothing is in clipboard, do nothing
	if(length <= 1)
	{
		return false;
	}
	//remove the '\0' appended when copying the data
	std::string str( pData, length-1 );
	TextStream stream( str, std::ios::in );

	GlobalUnlock(hClipboardData);
	CloseClipboard();

	// deserialize values
		std::string tag;
		std::string hint;
	std::string valueTag;
	std::vector<Variant> values;
	while(!stream.eof())
	{
		stream.read( tag );
		if(tag == s_ValueHintTag)
		{
			stream.read(hint);
			stream.read(valueTag);
			assert( valueTag == s_ValueTag );
		}
		else
		{
			assert( tag == s_ValueTag);
		}
		Variant v;
		if (!deserializeData( stream, v ))
		{
			assert( false );
			return false;
		}
		values.emplace_back( std::move( v ) );
	}

	if (values.empty())
	{
		return false;
	}
	// paste value
	commandSystem_->beginBatchCommand();
	bool ret = true;
	std::vector< ICopyableObject* >::iterator iter;
	for (iter = curObjects_.begin(); iter != curObjects_.end() && ret; ++iter)
	{
		assert( *iter != nullptr );
		Variant value = (*iter)->getData();
		
		if (value.typeIs<Collection>())
		{
			Collection collection;
			bool isOk = value.tryCast( collection );
			if (!isOk)
			{
				break;
		}
			size_t i = 0;
			for(auto & v : values)
			{
				assert( i < collection.size() );
				collection[i++] = v;
			}
			value = collection;
		}
		else
		{
			value = values[0];
		}
		ret = (*iter)->setData( value );
	}
	commandSystem_->endBatchCommand();

	return ret;
}


//==============================================================================
bool CopyPasteManager::canCopy() const
{
	if (curObjects_.empty())
	{
		return false;
	}
	return true;
}


//==============================================================================
bool CopyPasteManager::canPaste() const
{
	return (IsClipboardFormatAvailable(CF_TEXT) != 0);
}


//==============================================================================
void CopyPasteManager::init( ISerializationManager * serializationMgr, ICommandManager * commandSystem )
{
	assert( serializationMgr && commandSystem );
	curObjects_.clear();
	serializationMgr_ = serializationMgr;
	commandSystem_ = commandSystem;
}

//==============================================================================
void CopyPasteManager::fini()
{
	curObjects_.clear();
	serializationMgr_ = nullptr;
	commandSystem_ = nullptr;
}


//==============================================================================
bool CopyPasteManager::serializeData( IDataStream& stream, const Variant & value )
{
	
	if (value.typeIs<Collection>())
	{
		Collection collection;
		bool isOk = value.tryCast( collection );
		if (!isOk)
		{
			return false;
		}
		bool br = true;
		for (auto it = collection.begin(), end = collection.end();
			(it != end) && br; ++it )
		{
			auto & v = it.value();
			br = serializeData( stream, v );
		}
		return br;
	}
	else
	{
		stream.write(s_ValueTag);
		stream.write( value.type()->name() );
	return serializationMgr_->serialize( stream, value );
	}
	
}


//==============================================================================
bool CopyPasteManager::deserializeData( IDataStream& stream, Variant& value )
{
	std::string valueType;
	stream.read( valueType );
	const MetaType* metaType = Variant::getMetaTypeManager()->findType( valueType.c_str() );
	Variant variant( metaType );
	bool br = serializationMgr_->deserialize( stream, variant );
	if(br)
	{
		value = variant;
	}
	return br;
}
