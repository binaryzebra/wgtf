#include <memory.h>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "core_common/ngt_windows.hpp"

#include "allocator.hpp"
#include <string>

//TODO: Replace with thread_local in C++11 once Visual studio supports it
#define THREADLOCAL( type ) __declspec( thread ) type

// Windows stack helper function definitions
typedef USHORT (__stdcall* RtlCaptureStackBackTraceFuncType)(ULONG FramesToSkip, ULONG FramesToCapture, PVOID* BackTrace, PULONG BackTraceHash);

// DbgHelp functions definitions
typedef BOOL	(__stdcall *SymInitializeFuncType)(HANDLE hProcess, PSTR UserSearchPath, BOOL fInvadeProcess);
typedef BOOL (__stdcall *SymFromAddrFuncType)(HANDLE hProcess, DWORD64 Address, PDWORD64 Displacement, PSYMBOL_INFO Symbol);
typedef DWORD	(__stdcall *SymSetOptionsFuncType)(DWORD SymOptions);	
typedef BOOL (__stdcall *SymSetSearchPathFuncType)(  HANDLE hProcess, PCSTR SearchPath );
typedef BOOL(__stdcall *SymGetLineFromAddr64FuncType)(HANDLE hProcess, DWORD64 qwAddr, PDWORD pdwDisplacement, PIMAGEHLP_LINE64 Line64);

RtlCaptureStackBackTraceFuncType	RtlCaptureStackBackTraceFunc;
SymFromAddrFuncType					SymFromAddrFunc;
SymSetOptionsFuncType				SymSetOptionsFunc;
SymInitializeFuncType				SymInitializeFunc;
SymSetSearchPathFuncType			SymSetSearchPathFunc;
SymGetLineFromAddr64FuncType		SymGetLineFromAddr64Func;

namespace NGTAllocator
{

class MemoryContext
{
	static const size_t numFramesToCapture_ = 25;
	
private:
	template<class T>
	class UntrackedAllocator
	{
	public:
		typedef T				    value_type;
		
		typedef value_type          * pointer;
		typedef value_type          & reference;
		typedef const value_type    * const_pointer;
		typedef const value_type    & const_reference;
		
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		
		template <class Other>
		struct rebind
		{
			typedef UntrackedAllocator<Other> other;
		};
		
		
		UntrackedAllocator()
		{
		}
		
		template <typename Other>
		UntrackedAllocator( const UntrackedAllocator< Other > & )
		{
		}
		
		typename std::allocator<T>::pointer allocate(
			typename std::allocator<T>::size_type n, typename std::allocator<void>::const_pointer = 0 )
		{
			return (typename std::allocator<T>::pointer) ::malloc( n * sizeof( T ) );
		}
		
		void deallocate( typename std::allocator<T>::pointer p, typename std::allocator<T>::size_type n )
		{
			::free( p );
		}
		
		void construct( pointer p, const T & val ) const
		{
			new ((void*)p) T( val );
		}
		
		
		void destroy( pointer p ) const
		{
			p->~T();
		}
		
		size_type max_size() const
		{
			size_type _Count = (size_type)(-1) / sizeof (T);
			return (0 < _Count ? _Count : 1);
		}
	};

public:
	MemoryContext()
		: root_( true )
		, allocId_( 0 )
		, parentContext_( nullptr )
	{
		wcscpy( name_, L"root" );
		HMODULE kernel32 = ::LoadLibraryA( "kernel32.dll" );
		assert( kernel32 );
		RtlCaptureStackBackTraceFunc = ( RtlCaptureStackBackTraceFuncType )
			::GetProcAddress( kernel32, "RtlCaptureStackBackTrace" );
	}

	MemoryContext( const wchar_t * name, MemoryContext * parentContext )
		: root_( false )
		, allocId_( 0 )
		, parentContext_( parentContext )
	{
		parentContext_->childContexts_.push_back( this );
		wcscpy( name_, name );
	}


	~MemoryContext()
	{
	}


	void * allocate( size_t size )
	{
		Allocation * allocation = nullptr;
		{
			std::lock_guard< std::mutex > allocationPoolGuard(allocationPoolLock_);
			if (allocationPool_.size() > 0)
			{
				allocation = allocationPool_.back();
				allocationPool_.pop_back();
			}
		}
		if(allocation == nullptr) 
		{
			allocation =
				static_cast< Allocation * >( ::malloc( sizeof( Allocation ) ) );
		}
		allocation->frames_ =
			RtlCaptureStackBackTraceFunc( 3, numFramesToCapture_, allocation->addrs_, NULL );
		auto ptr = ::malloc( size );
		std::lock_guard< std::mutex > allocationGuard(allocationLock_);
		allocation->allocId_ = allocId_++;
		liveAllocations_.insert( std::make_pair( ptr, allocation ) );
		return ptr;
	}


	bool deallocateInternal( void* ptr )
	{
		std::lock_guard< std::mutex > allocationGuard(allocationLock_);
		auto findIt = liveAllocations_.find( ptr );
		if (findIt != liveAllocations_.end())
		{
			{
				std::lock_guard< std::mutex > allocationPoolGuard(allocationPoolLock_);
				allocationPool_.push_back( findIt->second );
			}
			liveAllocations_.erase( findIt );
			::free( ptr );
			return true;
		}
		return false;
	}


	void deallocate( void* ptr )
	{
		auto success = deallocateInternal( ptr );
		if (success)
		{
			return;
		}
		if (parentContext_ == nullptr)
		{
			assert( false );
			return;
		}
		if (parentContext_->deallocateInternal( ptr ))
		{
			return;
		}

		for (auto context : parentContext_->childContexts_)
		{
			if (context == this)
			{
				continue;
			}
			if(context->deallocateInternal( ptr ))
			{
				return;
			}
		}
#if __DAVAENGINE_NGT__
		assert( success );
#endif //#if __DAVAENGINE_NGT__
	}

	void cleanup()
	{
		HMODULE dbghelp  = ::LoadLibraryA( "dbghelp.dll" );
		assert( dbghelp );
		SymFromAddrFunc = ( SymFromAddrFuncType )
			::GetProcAddress( dbghelp, "SymFromAddr" );
		SymSetOptionsFunc = ( SymSetOptionsFuncType )
			::GetProcAddress( dbghelp, "SymSetOptions" );
		SymInitializeFunc = ( SymInitializeFuncType )
			::GetProcAddress( dbghelp, "SymInitialize" );
		SymSetSearchPathFunc = ( SymSetSearchPathFuncType )
			::GetProcAddress( dbghelp, "SymSetSearchPath" );
		SymGetLineFromAddr64Func = ( SymGetLineFromAddr64FuncType )
			::GetProcAddress( dbghelp, "SymGetLineFromAddr64" );
		auto currentProcess = ::GetCurrentProcess();

		std::basic_string< char, std::char_traits< char >, UntrackedAllocator< char > > builder;

		static bool symbolsLoaded = false;
		if (!symbolsLoaded)
		{
			// build PDB path that should be the same as executable path
			{								
				char path[_MAX_PATH] = { 0 };
				wcstombs(path, name_, wcslen(name_));

				// check that the pdb actually exists and is accessible, if it doesn't then SymInitialize will raise an obscure error dialog
				// so just disable complete callstacks if it is not there
				char* pend = strrchr( path, '.');
				*pend = 0;
				strcat( path, ".pdb" );
				FILE *f = fopen( path, "rb" );
				if ( f == NULL )
				{
					return;
				}
				fclose( f );

				pend = strrchr(path, '\\');
				*pend = 0;
				builder.append( path );
			}

			// append the working directory.
			builder.append( ";.\\" );

			// append %SYSTEMROOT% and %SYSTEMROOT%\system32.
			char * env = getenv( "SYSTEMROOT" );
			if (env) 
			{
				builder.append( ";" );
				builder.append( env );
			}

			// append %_NT_SYMBOL_PATH% and %_NT_ALT_SYMBOL_PATH%.
			if ((env = getenv( "_NT_SYMBOL_PATH" )))
			{
				builder.append( ";" );
				builder.append( env );
			}
			if ((env = getenv( "_NT_ALT_SYMBOL_PATH" )))
			{
				builder.append( ";" );
				builder.append( env );
			}
			builder.append( ";" );
			builder.append( "http://msdl.microsoft.com/download/symbols");

			SymSetOptionsFunc( SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME );
			SymInitializeFunc( currentProcess, (PSTR) builder.c_str(), TRUE );

			builder.clear();

			symbolsLoaded = true;
		}

		wchar_t contextName[ 2048 ]; 
		swprintf( contextName, 2048, L"Destroying memory context for %s\n", name_ );
		::OutputDebugString( contextName );

		for( auto & liveAllocation : liveAllocations_)
		{
			// Allocate a buffer large enough to hold the symbol information on the stack and get 
			// a pointer to the buffer.  We also have to set the size of the symbol structure itself
			// and the number of bytes reserved for the name.
			const int MaxSymbolNameLength = 1024;
			ULONG64 buffer[ (sizeof( SYMBOL_INFO ) + MaxSymbolNameLength + sizeof( ULONG64 ) - 1) / sizeof( ULONG64 ) ] = { 0 };
			SYMBOL_INFO * info = (SYMBOL_INFO *) buffer;
			info->SizeOfStruct = sizeof( SYMBOL_INFO );
			info->MaxNameLen = MaxSymbolNameLength;

			// Attempt to get information about the symbol and add it to our output parameter.
			DWORD64 displacement64 = 0;
			DWORD displacement = 0;

			{
				char allocIdBuffer[ 2048 ] = { 0 };
				sprintf( allocIdBuffer, "Alloc Id: %zu\n", liveAllocation.second->allocId_ );
				builder.append( allocIdBuffer );
			}
			const auto & allocStack = liveAllocation.second;
			for( size_t i = 0; i < allocStack->frames_; ++i )
			{
				char nameBuf[ 1024 ] = { 0 };
				if (SymFromAddrFunc(
					currentProcess,
					(DWORD64) liveAllocation.second->addrs_[ i ],
					&displacement64, info ))
				{
					strncat( nameBuf, info->Name, info->NameLen );
				}
				else
				{
					// Unable to find the name, so lets get the module or address
					MEMORY_BASIC_INFORMATION mbi;
					char fullPath[MAX_PATH];
					if (VirtualQuery( liveAllocation.second->addrs_[ i ], &mbi, sizeof(mbi) ) && 
						GetModuleFileNameA( (HMODULE)mbi.AllocationBase, fullPath, sizeof(fullPath) ))
					{
						// Get base name of DLL
						char * filename = strrchr( fullPath, '\\' );
						strncpy_s( nameBuf, sizeof( nameBuf ), filename == NULL ? fullPath : (filename + 1), _TRUNCATE );
					}
					else
					{
						sprintf_s( nameBuf, sizeof( nameBuf ), "0x%p", liveAllocation.second->addrs_[ i ] );
					}
				}

				IMAGEHLP_LINE64 source_info;
				char lineBuffer[ 1024 ] = { 0 };
				::ZeroMemory( &source_info, sizeof(IMAGEHLP_LINE64) );
				source_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				if(SymGetLineFromAddr64Func(
					currentProcess,
					(DWORD64) liveAllocation.second->addrs_[ i ],
					&displacement, &source_info ))
				{
					sprintf( lineBuffer, "%s(%d)", source_info.FileName, source_info.LineNumber );
				}
				char outputBuffer[ 2048 ] = { 0 };
				sprintf( outputBuffer, "%s : %s\n", lineBuffer, nameBuf );
				builder.append( outputBuffer );
			}
			::free( liveAllocation.second );
			builder.append( "\n\n" );

			::OutputDebugStringA( builder.c_str() );

			builder.clear();
		}
		for( auto allocation : allocationPool_ )
		{
			::free( allocation );
		}
	}

private:
	struct Allocation 
	{
		void *	addrs_[ numFramesToCapture_ ];
		size_t	frames_;
		size_t	allocId_;
	};
	wchar_t name_[ 255 ];
	bool	root_;
	size_t	allocId_;
	MemoryContext * parentContext_;
	std::mutex allocationLock_;
	std::mutex allocationPoolLock_;
	std::vector< Allocation *, UntrackedAllocator< Allocation * > > allocationPool_;
	std::vector< MemoryContext *, UntrackedAllocator< MemoryContext * > > childContexts_;
	std::unordered_map<
		void *,
		Allocation *,
		std::hash< void *>,
		std::equal_to< void * >,
		UntrackedAllocator< std::pair< void * const, Allocation * > > > liveAllocations_;
};


#ifdef WIN32
#pragma warning(disable: 4073)
#pragma init_seg(lib)				// Ensure we get constructed first
#endif // WIN32

MemoryContext					rootContext_;
THREADLOCAL( int )				s_MemoryStackPos = -1;
THREADLOCAL( MemoryContext * )	s_MemoryContext[ 20 ];

//------------------------------------------------------------------------------
MemoryContext * getMemoryContext()
{
	if(s_MemoryStackPos < 0 )
	{
		return &rootContext_;
	}
	return s_MemoryContext[ s_MemoryStackPos ];
}

//------------------------------------------------------------------------------
void * allocate( size_t size )
{
	auto memoryContext = getMemoryContext();
	return memoryContext->allocate( size );
}


//------------------------------------------------------------------------------
void deallocate( void * ptr )
{
	if (ptr == nullptr)
	{
		return;
	}
	auto memoryContext = getMemoryContext();
	memoryContext->deallocate( ptr );
}


//------------------------------------------------------------------------------
void * createMemoryContext( const wchar_t * name )
{
	return new MemoryContext( name, getMemoryContext() );
}


//------------------------------------------------------------------------------
void destroyMemoryContext( void * pContext )
{
	delete static_cast< MemoryContext * >( pContext );
}


//------------------------------------------------------------------------------
void pushMemoryContext( void * pContext )
{
	assert( pContext != nullptr );
	s_MemoryContext[ ++s_MemoryStackPos ] =
		static_cast< MemoryContext * >( pContext );
}


//------------------------------------------------------------------------------
void popMemoryContext()
{
	assert( s_MemoryStackPos >= -1 );
	s_MemoryContext[ s_MemoryStackPos ] = nullptr;
	s_MemoryStackPos--;
}


//------------------------------------------------------------------------------
void cleanupContext( void * pContext )
{
	auto memoryContext = static_cast< MemoryContext * >( pContext );
	memoryContext->cleanup();
}

}
