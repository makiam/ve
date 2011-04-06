/************************************************************************
*
* vapor3D Server � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

#include <dirent.h>

NAMESPACE_BEGIN

//-----------------------------------//

#ifdef VAPOR_ARCHIVE_DIR

static bool    DirArchiveOpen(Archive*, const String&);
static bool    DirArchiveClose(Archive*);
static Stream* DirArchiveOpenFile(Archive*, const Path&, Allocator*);
static void    DirArchiveEnumerateFiles(Archive*, std::vector<Path>&);
static void    DirArchiveEnumerateDirectories(Archive*, std::vector<Path>&);
static bool    DirArchiveExistsFile(Archive*, const Path&);
static bool    DirArchiveExistsDir(Archive*, const Path&);
static bool    DirArchiveMonitor(Archive*); 

static ArchiveFuncs gs_DirArchiveFuncs =
{
	DirArchiveOpen,
	DirArchiveClose,
	DirArchiveOpenFile,
	DirArchiveExistsFile,
	DirArchiveExistsDir,
	DirArchiveEnumerateFiles,
	DirArchiveEnumerateDirectories,
	DirArchiveMonitor,
};

//-----------------------------------//

Archive* ArchiveCreateFromDirectory(Allocator* alloc, const Path& path)
{
	Archive* archive = Allocate<Archive>(alloc);
	
	archive->Path = path;
	archive->Handle = nullptr;
	archive->Scheme = "fs";
	archive->fn = &gs_DirArchiveFuncs;

	if( !ArchiveOpen(archive, path) )
	{
		//LogWarn("Error opening archive: %s", path.c_str());
		Deallocate(alloc, archive);
		return nullptr;
	}

	return archive;
}

//-----------------------------------//

static bool DirArchiveOpen(Archive* archive, const String& path)
{
	if( !archive ) return false;
	return true;
}

//-----------------------------------//

static bool DirArchiveClose(Archive* archive)
{
	if( !archive ) return false;
	return true;
}

//-----------------------------------//

static Stream* DirArchiveOpenFile(Archive* archive, const Path& file, Allocator* alloc)
{
	if( !archive ) return nullptr;
	Path path = StringFormat("%s%s%s", archive->Path.c_str(), PathGetSeparator().c_str(), file.c_str());
	Stream* stream = StreamCreateFromFile(alloc, path, StreamMode::Read);
	return stream;
}

//-----------------------------------//

static void DirArchiveEnumerate(std::vector<String>&, Path, Path, bool);

static void DirArchiveEnumerateFiles(Archive* archive, std::vector<Path>& paths)
{
	if( !archive ) return;
	DirArchiveEnumerate(paths, archive->Path, "", false);
}

//-----------------------------------//

static void DirArchiveEnumerateDirectories(Archive* archive, std::vector<Path>& paths)
{
	if( !archive ) return;
	DirArchiveEnumerate(paths, archive->Path, "", true);
}

//-----------------------------------//

static bool DirArchiveExistsFile(Archive* archive, const Path& file)
{
	if( !archive ) return false;
	const Path& filePath = archive->Path + PathGetSeparator() + file;
	return FileExists(filePath);
}

//-----------------------------------//

static bool DirArchiveExistsDir(Archive* archive, const Path& path)
{
	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);

	for(size_t i = 0; i < dirs.size(); i++)
	{
		Path normalized = PathNormalize(dirs[i]);
		Path dir = StringTrim(normalized, "/");
		if(dir == path) return true;
	}

	return false;
}

//-----------------------------------//

static bool DirArchiveMonitor(Archive*)
{
	return false;
}

#endif

//-----------------------------------//

static void DirArchiveEnumerate(std::vector<String>& paths, Path dirPath, Path filePath, bool dirs)
{
	// Open directory stream.
	DIR *dir = opendir( dirPath.c_str() );
   	if( !dir ) return;

	dirent* entry = nullptr;
	
	// Get all the files and directories within directory.
	while(entry = readdir(dir))
	{
		const Path& name = entry->d_name;
		
		switch(entry->d_type) {
		case DT_REG:
		{
			Path sep = filePath.empty() ? "" : PathGetSeparator();
			Path path = StringFormat("%s%s%s", filePath.c_str(), sep.c_str(), name.c_str() );
			if(!dirs) paths.push_back(path);
			break;
		}
		case DT_DIR:
		{
			if(!name.empty() && name[0] == '.') continue;
		
			Path sep = filePath.empty() ? "" : PathGetSeparator();
			Path _dirPath = StringFormat("%s%s%s", dirPath.c_str(), PathGetSeparator().c_str(), name.c_str());
			Path _filePath = StringFormat("%s%s%s", filePath.c_str(), sep.c_str(), name.c_str());

			if(dirs) paths.push_back(_filePath);
			DirArchiveEnumerate(paths, _dirPath, _filePath, dirs);
			
			break;
		} }
	}

	closedir(dir);
}

//-----------------------------------//

void FileEnumerateFiles(const Path& path, std::vector<Path>& files)
{
	DirArchiveEnumerate(files, path, "", false);
}

//-----------------------------------//

void FileEnumerateDirectories(const Path& path, std::vector<Path>& dirs)
{
	DirArchiveEnumerate(dirs, path, "", true);
}

//-----------------------------------//

bool FileExists(const Path& path)
{
#ifdef VAPOR_COMPILER_MSVC
	return _access(path.c_str(), F_OK) == 0;
#else
	return access(path.c_str(), F_OK) == 0;
#endif
}

//-----------------------------------//

NAMESPACE_END