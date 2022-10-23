#include "stdafx.h"

namespace xlsBin2
{
    char* filebuf;
    long filesize;
    long headersize, footersize;
    std::string path;
    std::string filename;

    std::string scriptfile;

    std::vector<StructItem> FileStruct;

    lua_State* LuaHandle;


    void Init(char* fname)
    {
        path = std::string(fname);
        filename = Functions::GetFilename(fname);
        headersize = 0;

        LuaHandle = luaL_newstate();
        luaopen_base(LuaHandle);

        lua_register(LuaHandle, "toCSV", toLua::toCSV);
        lua_register(LuaHandle, "toBIN", toLua::toBIN);
        lua_register(LuaHandle, "loadFileBuf", toLua::loadFileBuf);
        lua_register(LuaHandle, "saveFileBuf", toLua::saveFileBuf);
        lua_register(LuaHandle, "getInput", toLua::getInput);

        if (Functions::FileExists(fname))
        {
            if (Functions::FileExists(Functions::GetFileDir(Functions::Format("data\\%s.st", filename.c_str()).c_str())))
                Interpreter();
            else
                printf("Struct file not found.\n");
        }
        else
            printf("%s not found.\n", fname);

        lua_close(LuaHandle);
    }

    void ProcessStructLine(std::string line)
    {
        auto explodedline = Functions::explode(line, " ");

        if (explodedline.size() >= 2)
        {
            StructItem Item;

            if (explodedline.at(0) == "char")
                Item.itemtype = ITEM_TYPE::CHAR;
            else if (explodedline.at(0) == "byte")
                Item.itemtype = ITEM_TYPE::BYTE;
            else if (explodedline.at(0) == "short")
                Item.itemtype = ITEM_TYPE::SHORT;
            else if (explodedline.at(0) == "int")
                Item.itemtype = ITEM_TYPE::INT;
            else if (explodedline.at(0) == "long")
                Item.itemtype = ITEM_TYPE::LONG;
            else if (explodedline.at(0) == "uchar")
                Item.itemtype = ITEM_TYPE::UCHAR;
            else if (explodedline.at(0) == "ushort")
                Item.itemtype = ITEM_TYPE::USHORT;
            else if (explodedline.at(0) == "uint")
                Item.itemtype = ITEM_TYPE::UINT;
            else if (explodedline.at(0) == "ulong")
                Item.itemtype = ITEM_TYPE::ULONG;
            else if (explodedline.at(0) == "double")
                Item.itemtype = ITEM_TYPE::DOUBLE;
            else if (explodedline.at(0) == "float")
                Item.itemtype = ITEM_TYPE::FLOAT;
            else 
                return;


            for (int i = 1; i < explodedline.size(); i++)
            {

                auto str = explodedline.at(i);

                str.erase(std::remove(str.begin(), str.end(), ','), str.end());

                if ((str.find("[") != std::string::npos) && (str.find("]") != std::string::npos))
                {
                    Item.name = str.substr(0, str.find("["));
                    Item.length = atoi(str.substr(str.find("[") + 1, str.find("]")).c_str());
                }
                else
                {
                    Item.name = str;
                    Item.length = 1;
                }

                FileStruct.push_back(Item);
            }
        }
    }

    bool ExecScript()
    {

        luaL_dostring(LuaHandle, Functions::Format("filename = \"%s\"", filename.c_str()).c_str());
        luaL_dostring(LuaHandle, Functions::Format("fileextension = \"%s\"", Functions::GetExtension(path.c_str()).c_str()).c_str());
        luaL_dostring(LuaHandle, Functions::Format("headersize = %d", headersize).c_str());
        luaL_dostring(LuaHandle, Functions::Format("footersize = %d", footersize).c_str());

        auto ret = luaL_dofile(LuaHandle, Functions::GetFileDir(Functions::Format("data\\%s", scriptfile.c_str()).c_str()));

        if (ret != 0)
        {
            printf("Script execution error, please check. (errorcode = %d).\n", ret);
            lua_close(LuaHandle);
            return 0;
        }

        printf("\n\n\nScript executed sucessfully.\n");

        return true;
    }

    void xlsBin2::Interpreter()
    {
        fstream stfile;
        stfile.open(Functions::GetFileDir(Functions::Format("data\\%s.st", filename.c_str()).c_str()), ios::in);

        string line;

        bool compilingstruct = false;

        while (getline(stfile, line))
        {
            if (compilingstruct)
            {
                ProcessStructLine(line);
                continue;
            }

            if (line.find("##scriptfile=") != std::string::npos)
            {
                scriptfile = line.substr(std::string("##scriptfile=").length());
                continue;
            }

            if (line.find("##headersize=") != std::string::npos)
            {
                headersize = atoi(line.substr(std::string("##headersize=").length()).c_str());
                continue;
            }

            if (line.find("##footersize=") != std::string::npos)
            {
                footersize = atoi(line.substr(std::string("##footersize=").length()).c_str());
                continue;
            }

            if (line.find("##beginstruct") != std::string::npos)
            {
                compilingstruct = true;
                continue;
            }

            if (line.find("##endstruct") != std::string::npos)
            {
                compilingstruct = false;
                continue;
            }
        }

        ExecScript();
    }

#pragma region toLua

    namespace toLua
    {
        int ITEMTYPE_SIZES[] = { 0, sizeof(char), sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(unsigned char), sizeof(unsigned short), sizeof(unsigned int), sizeof(unsigned long), sizeof(double), sizeof(float) };

        static int toCSV(lua_State* L)
        {

            if (filesize > 0)
            {
                fstream csvFile;

                csvFile.open(Functions::GetFileDir((filename + ".csv").c_str()), ios::out);


                if (csvFile.is_open())
                {
                    
                    csvFile << Functions::Format("binSize=%lu", filesize) << endl;

                    csvFile << "header=";
                    for (int i = 0; i < headersize; i++)
                        csvFile << Functions::Format("%d ", *(unsigned char*)&filebuf[i]);
                    csvFile << endl;

                    csvFile << "footer=";
                    for (int i = 0; i < footersize; i++)
                        csvFile << Functions::Format("%d ", *(unsigned char*)&filebuf[filesize - footersize + i - 1]);
                    csvFile << endl;
                    

                    for (int i = 0; i < FileStruct.size(); i++)
                        csvFile << FileStruct.at(i).name + ";";
                    csvFile << endl;

                    int i = headersize;

                    while (i < filesize - footersize)
                    {
                        for (int j = 0; (j < FileStruct.size()) && (i < filesize - footersize); j++)
                        {


                            auto Item = FileStruct.at(j);

                            if ((i + (ITEMTYPE_SIZES[(int)Item.itemtype] * Item.length)) <= filesize - footersize)
                            {
                                switch (Item.itemtype)
                                {
                                case ITEM_TYPE::CHAR:
                                {
                                    char* str = new char[Item.length + 1];
                                    ZeroMemory(str, Item.length + 1);

                                    if (i + Item.length <= filesize - footersize)
                                    {
                                        memcpy(str, &filebuf[i], Item.length);
                                        csvFile << "\"" << std::string(str) << "\"" << ";";
                                    }

                                    break;
                                }

                                case ITEM_TYPE::BYTE:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%d", *(char*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::SHORT:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%d", *(short*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::INT:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%d", *(int*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::LONG:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%ld", *(long*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::UCHAR:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%u", *(unsigned char*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::USHORT:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%u", *(unsigned short*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::UINT:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%u", *(unsigned int*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";

                                    break;

                                case ITEM_TYPE::ULONG:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%lu", *(unsigned long*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::DOUBLE:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%f", *(double*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;

                                case ITEM_TYPE::FLOAT:
                                    csvFile << "\"";
                                    for (int x = 0; x < Item.length; x++)
                                        csvFile << Functions::Format("%f", *(float*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]]) << " ";
                                    csvFile << "\"";
                                    csvFile << ";";
                                    break;
                                }
                            }

                            i += Item.length * ITEMTYPE_SIZES[(int)Item.itemtype];
                        }
                        csvFile << endl;
                    }

                    csvFile.close();
                }
            }

            return 1;
        }

        static int toBIN(lua_State* L)
        {
            fstream csvfile;
            csvfile.open(path, ios::in);

            string line;

            int i = headersize;
            filesize = -1;
            
            bool skippedtitle = false, settedheader = false, settedfooter = false;

            while (getline(csvfile, line))
            {
                if (filesize == -1)
                {
                    if (line.find("binSize=") != std::string::npos)
                    {
                        filesize = stol(line.substr(line.find("=") + 1));
                        filebuf = new char[filesize];
                        LSB::Init(LuaHandle, filebuf);
                        luaL_dostring(LuaHandle, Functions::Format("filesize = %lu", filesize).c_str());
                    }

                    continue;
                }


                if (!settedheader)
                {
                    if (line.find("header=") != std::string::npos)
                    {
                        auto explodedheader = Functions::explode(line.substr(line.find("=") + 1), " ");
                        for (int i = 0; (i < explodedheader.size()) && (i < headersize); i++)
                            filebuf[i] = atoi(explodedheader.at(i).c_str());

                        settedheader = true;
                    }
                    continue;
                }

                if (!settedfooter)
                {
                    if (line.find("footer=") != std::string::npos)
                    {
                        auto explodedfooter = Functions::explode(line.substr(line.find("=") + 1), " ");
                        for (int i = 0; (i < explodedfooter.size()) && (i < footersize); i++)
                            filebuf[filesize - footersize + i - 1] = atoi(explodedfooter.at(i).c_str());

                        settedfooter = true;
                    }
                    continue;
                }

                if (!skippedtitle)
                {

                    skippedtitle = true;
                    continue;
                }

                if (i < filesize - footersize)
                {
                    auto explodedline = Functions::explode(line, "\";");

                    for (int j = 0; (j < FileStruct.size()) && (i < filesize - footersize); j++)
                    {

                        auto Item = FileStruct.at(j);

                        switch (Item.itemtype)
                        {
                        case ITEM_TYPE::CHAR:
                            memcpy(&filebuf[i], explodedline.at(j).substr(1).c_str(), explodedline.at(j).length());
                            break;

                        case ITEM_TYPE::BYTE:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(char*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = atoi(explodeditem.at(x).c_str());
                        }
                        break;

                        case ITEM_TYPE::SHORT:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(short*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = atoi(explodeditem.at(x).c_str());
                        }
                        break;

                        case ITEM_TYPE::INT:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(int*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = atoi(explodeditem.at(x).c_str());
                        }
                        break;

                        case ITEM_TYPE::LONG:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(long*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = std::stol(explodeditem.at(x).c_str());
                        }
                        break;

                        case ITEM_TYPE::UCHAR:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(unsigned char*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = atoi(explodeditem.at(x).c_str());
                        }
                        break;

                        case ITEM_TYPE::USHORT:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(unsigned short*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = atoi(explodeditem.at(x).c_str());
                        }
                        break;

                        case ITEM_TYPE::UINT:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(unsigned int*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = strtoul(explodeditem.at(x).c_str(), 0, 10);
                        }
                        break;

                        case ITEM_TYPE::ULONG:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(unsigned long*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = strtoul(explodeditem.at(x).c_str(), 0, 10);
                        }
                        break;

                        case ITEM_TYPE::DOUBLE:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(double*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = stod(explodeditem.at(x).c_str());
                        }
                        break;

                        case ITEM_TYPE::FLOAT:
                        {
                            auto explodeditem = Functions::explode(explodedline.at(j).substr(1), " ");
                            for (int x = 0; x < Item.length; x++)
                                *(double*)&filebuf[i + x * ITEMTYPE_SIZES[(int)Item.itemtype]] = stof(explodeditem.at(x).c_str());
                        }
                        break;
                        }

                        i += Item.length * ITEMTYPE_SIZES[(int)Item.itemtype];
                    }
                }
            }

            csvfile.close();

            return 1;
        }

        static int loadFileBuf(lua_State* L)
        {
            filebuf = Functions::LoadFile(path.c_str(), &filesize);
            luaL_dostring(L, Functions::Format("filesize = %d", filesize).c_str());
            LSB::Init(L, filebuf);
        }

        static int saveFileBuf(lua_State* L)
        {
            string fname(lua_tostring(L, 1));

            FILE* il;
            fopen_s(&il, Functions::GetFileDir(fname.c_str()), "wb");
            fwrite(filebuf, filesize, 1, il);
            fclose(il);

            return 1;
        }

        static int getInput(lua_State* L)
        {
            std::string input;
            getline(cin, input);

            lua_pushstring(L, input.c_str());

            return 1;
        }
    }
#pragma endregion
}