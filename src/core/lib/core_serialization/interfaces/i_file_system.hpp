//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//  i_file_system.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Copyright (c) Wargaming.net. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef I_FILE_SYSTEM_HPP_
#define I_FILE_SYSTEM_HPP_

#pragma once

#include "core_serialization/i_datastream.hpp"
#include "core_string_utils/file_path.hpp"

#include <functional>
#include <memory>

struct FileInfo;

namespace FileAttributes
{
	enum FileAttribute
	{
		None					= 0x00000000,
		ReadOnly				= 0x00000001,
		Hidden					= 0x00000002,
		System					= 0x00000004,
		Directory				= 0x00000010,
		Archive					= 0x00000020,
		Device					= 0x00000040,
		Normal					= 0x00000080,
		Temporary				= 0x00000100,
		SparseFile				= 0x00000200,
		ReparsePoint			= 0x00000400,
		Compressed				= 0x00000800,
		Offline					= 0x00001000,
		NotContentIndexed		= 0x00002000,
		Encrypted				= 0x00004000,
		IntegrityStream			= 0x00008000,
		Virtual					= 0x00010000,
		NoScrubData				= 0x00020000,

		AppendOnly				= 0x00040000,
		NoDump					= 0x00080000,
		ExtentFormat			= 0x00100000,
		Immutable				= 0x00200000,
		DataJournaling			= 0x00400000,
		SecureDeletion			= 0x00800000,
		NoTailMerging			= 0x01000000,
		Undeletable				= 0x02000000,
		NoAtimeUpdates			= 0x04000000,
		SynchronousDirectory	= 0x08000000,
		Updates					= 0x10000000,
		SynchronousUpdates		= 0x20000000,
		TopOfDirectoryHierarchy	= 0x40000000
	};
};

class IFileSystem
{
public:
	typedef std::function<bool(FileInfo&&)> EnumerateCallback;
	typedef std::unique_ptr<IDataStream> istream_uptr;

	enum FileType
	{
		NotFound,
		Directory,
		File,
		Archive
	};

	virtual ~IFileSystem(){}
	virtual bool			copy(const char* path, const char* new_path) = 0;
	virtual bool			remove(const char* path) = 0;
	virtual bool			exists(const char* path) const = 0;
	virtual void			enumerate(const char* dir, EnumerateCallback callback) const = 0;
	virtual FileType		getFileType(const char* path) const = 0;
	virtual FileInfo		getFileInfo(const char* path) const = 0;
	virtual bool			move(const char* path, const char* new_path) = 0;
	virtual istream_uptr	readFile(const char* path, std::ios::openmode mode) const = 0;
	virtual bool			writeFile(const char* path, const void* data, size_t len, std::ios::openmode mode) = 0;
};

struct FileInfo
{
	FileInfo()
		: size(0)
		, created(0)
		, modified(0)
		, accessed(0)
		, fullPath("")
		, attributes(FileAttributes::None)
	{
	}

	FileInfo(uint64_t size, uint64_t created, uint64_t modified, uint64_t accessed,
		const std::string& fullPath, FileAttributes::FileAttribute attributes)
		: size(size)
		, created(created)
		, modified(modified)
		, accessed(accessed)
		, fullPath(fullPath)
		, attributes(attributes)
	{
	}

	bool isDirectory() const
	{
		return (attributes & FileAttributes::Directory) == FileAttributes::Directory;
	}

	bool isReadOnly() const
	{
		return (attributes & FileAttributes::ReadOnly) == FileAttributes::ReadOnly;
	}

	bool isHidden() const
	{
		return (attributes & FileAttributes::Hidden) == FileAttributes::Hidden;
	}

	bool isDots() const
	{
		return isDirectory() && fullPath.length() > 0 && fullPath.back() == '.';
	}

	const char* name() const
	{
		auto index = fullPath.find_last_of("/\\");
		return &fullPath.c_str()[index != std::string::npos ? index + 1 : 0];
	}

	const char* extension() const
	{
		auto index = fullPath.rfind( FilePath::kExtensionSeparator );
		return index != std::string::npos ? &fullPath.c_str()[index + 1] : "";
	}

	const uint64_t			size;
	const uint64_t			created;
	const uint64_t			modified;
	const uint64_t			accessed;
	const std::string		fullPath;

	const FileAttributes::FileAttribute	attributes;
};

#endif // I_FILE_SYSTEM_HPP_
