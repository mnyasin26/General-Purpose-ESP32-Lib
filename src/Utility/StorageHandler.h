#ifndef STORAGE_HANDLER_H
#define STORAGE_HANDLER_H

#include <main_header.h>
#include "FS.h"
#include <LittleFS.h>
#include <vector>

#if ENABLE_DEBUG == true
#if STORAGE_HANDLER_DEBUG == true
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(x, y) Serial.printf(x, y)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x, y)
#endif
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x, y)
#endif

#define FORMAT_LITTLEFS_IF_FAILED true

using namespace std;

class StorageHandler
{
private:

public:
    StorageHandler();
    static void begin();
    static void listDir(const char *dirname, uint8_t levels);
    static bool checkDir(const char * path);
    static bool checkFile(const char * path);
    static void createDir(const char * path);
    static void createFile(const char * path);
    static void removeDir(const char * path);
    static vector<String> *readFile(const char * path);
    static void writeFile(const char * path, const char * message);
    static void appendFile(const char * path, const char * message);
    static void renameFile(const char * path1, const char * path2);
    static void deleteFile(const char * path);
    static bool testFileIO(const char * path);

    ~StorageHandler();
};



#endif