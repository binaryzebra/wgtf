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
#include "core_serialization/i_file_info.hpp"

#include <functional>
#include <memory>

class IFileSystem
{
public:
	typedef std::function<bool(IFileInfoPtr&&)> EnumerateCallback;
	typedef std::unique_ptr<IDataStream> IStreamPtr;

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
	virtual IFileInfoPtr	getFileInfo(const char* path) const = 0;
	virtual bool			move(const char* path, const char* new_path) = 0;
	virtual IStreamPtr		readFile(const char* path, std::ios::openmode mode) const = 0;
	virtual bool			writeFile(const char* path, const void* data, size_t len, std::ios::openmode mode) = 0;
};

#endif // I_FILE_SYSTEM_HPP_