#include "stdafx.h"

namespace Functions
{
	std::vector<std::string> explode(std::string str, const char* delimiter)
	{
		vector<std::string> result;

		std::string atualstr = str;
		int pos = atualstr.find(delimiter);

		while (pos != std::string::npos)
		{
			if (atualstr.length() > pos)
				result.push_back(atualstr.substr(0, pos));

			atualstr = atualstr.substr(pos + strlen(delimiter));
			pos = atualstr.find(delimiter);

		}
		if (atualstr != "")
			result.push_back(atualstr);

		return result;
	}

	char* GetFileDir(const char* filename)
	{
		char* dir = new char[MAX_PATH];
		GetModuleFileNameA(NULL, dir, MAX_PATH);
		*strrchr(dir, '\\') = 0;
		strcpy(dir, Functions::Format("%s\\%s", dir, filename).c_str());
		return dir;
	}

	bool FileExists(const char* name) 
	{
		return filesystem::exists(name);
	}

	std::string GetFilename(const char* path)
	{
		std::filesystem::path p(path);
		
		return p.stem().string();
	}

	std::string GetExtension(const char* path)
	{
		std::filesystem::path p(path);
		return p.extension().string();
	}

	std::string Format(const char* fmtstring, ...)
	{
		char buffer[2048];

		va_list arglist;
		va_start(arglist, fmtstring);
		vsprintf_s(buffer, sizeof(buffer), fmtstring, arglist);
		va_end(arglist);

		return std::string(buffer);
	}


	char* LoadFile(const char* file, long* filesize)
	{
		FILE* il;
		fopen_s(&il, file, "r");

		if (il == NULL)
		{
			printf("%s not found.", file);
			return 0;
		}

		fseek(il, 0, SEEK_END);
		*filesize = ftell(il);
		rewind(il);

		char* buf = new char[*filesize];

		fread(buf, *filesize, 1, il);
		fclose(il);

		return buf;
	}

}