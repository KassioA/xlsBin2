#ifndef XLSBIN2_H_
#define XLSBIN2_H_

enum class ITEM_TYPE : unsigned char
{
	CHAR = 1,
	BYTE,
	SHORT,
	INT,
	LONG,
	UCHAR,
	USHORT,
	UINT,
	ULONG,
	DOUBLE,
	FLOAT
};


namespace xlsBin2
{
	struct StructItem
	{
		ITEM_TYPE itemtype;
		std::string name;
		unsigned int length;
	};

	void Init(char* fname);

	bool ExecScript();


	void Interpreter();
	void ProcessStructLine(std::string line);

	namespace toLua
	{
		static int toCSV(lua_State* L);
		static int toBIN(lua_State* L);

		static int loadFileBuf(lua_State* L);
		static int saveFileBuf(lua_State* L);

		static int getInput(lua_State* L);
	}
};

#endif