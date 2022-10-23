#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

namespace Functions
{
	std::vector<std::string> explode(std::string str, const char* delimiter);
	char* GetFileDir(const char* filename);
	bool FileExists(const char* name);
	std::string GetFilename(const char* path);
	std::string GetExtension(const char* path);
	std::string Format(const char* fmtstring, ...);
	char* LoadFile(const char* file, long* filesize);

}

#endif