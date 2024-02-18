#include "StorageHandler.h"

StorageHandler::StorageHandler()
{
}

StorageHandler::~StorageHandler()
{
}

bool StorageHandler::checkDir(const char *path)
{
    if (LittleFS.exists(path))
    {
        DEBUG_PRINTF("Dir %s exists\r\n", path);
        return true;
    }
    else
    {
        DEBUG_PRINTF("Dir %s does not exist\r\n", path);
        return false;
    }
}

bool StorageHandler::checkFile(const char *path)
{
    if (LittleFS.exists(path))
    {
        DEBUG_PRINTF("File %s exists\r\n", path);
        return true;
    }
    else
    {
        DEBUG_PRINTF("File %s does not exist\r\n", path);
        return false;
    }
}

bool StorageHandler::testFileIO(const char *path)
{
    DEBUG_PRINTF("Test file IO: %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to open file for writing");
        return false;
    }

    size_t i;
    DEBUG_PRINTLN("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
        {
            DEBUG_PRINT(".");
        }
        file.write(buf, 512);
    }
    DEBUG_PRINTLN("");
    uint32_t end = millis() - start;
    DEBUG_PRINTF("- %u bytes written in ", 2048 * 512);
    DEBUG_PRINTF("%u ms\r\n", end);
    file.close();

    file = LittleFS.open(path);
    start = millis();
    end = start;
    i = 0;
    if (file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        DEBUG_PRINT("- reading");
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
            {
                Serial.print(".");
            }
            len -= toRead;
        }
        DEBUG_PRINTLN("");
        end = millis() - start;
        DEBUG_PRINTF("- %u bytes read in ", flen);
        DEBUG_PRINTF("%u ms\r\n", end);
        file.close();
    }
    else
    {

        DEBUG_PRINTLN("- failed to open file for reading");
        return false;
    }
    return true;
}

void StorageHandler::deleteFile(const char *path)
{
    DEBUG_PRINTF("Deleting file: %s\r\n", path);
    if (LittleFS.remove(path))
    {
        DEBUG_PRINTLN("- file deleted");
    }
    else
    {
        DEBUG_PRINTLN("- delete failed");
    }
}

void StorageHandler::renameFile(const char *path1, const char *path2)
{
    DEBUG_PRINTF("Renaming file %s ", path1);
    DEBUG_PRINTF("to %s\r\n", path2);
    if (LittleFS.rename(path1, path2))
    {
        DEBUG_PRINTLN("- file renamed");
    }
    else
    {
        DEBUG_PRINTLN("- rename failed");
    }
}

void StorageHandler::appendFile(const char *path, const char *message)
{
    DEBUG_PRINTF("Appending to file: %s\r\n", path);

    File file = LittleFS.open(path, FILE_APPEND);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to open file for appending");
        return;
    }
    if (file.println(message))
    {
        DEBUG_PRINTLN("- message appended");
    }
    else
    {
        DEBUG_PRINTLN("- append failed");
    }
    file.close();
}

void StorageHandler::writeFile(const char *path, const char *message)
{
    DEBUG_PRINTF("Writing file: %s\r\n", path);

    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to open file for writing");
        return;
    }
    if (file.println(message))
    {
        DEBUG_PRINTLN("- file written");
    }
    else
    {
        DEBUG_PRINTLN("- write failed");
    }
    file.close();
}

void StorageHandler::begin()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        DEBUG_PRINTLN("LittleFS Mount Failed");
        return;
    }
    DEBUG_PRINTLN("LittleFS Mount Success");
}

vector<String> *StorageHandler::readFile(const char *path)
{
    DEBUG_PRINTF("Reading file: %s\r\n", path);

    File file = LittleFS.open(path);
    if (!file || file.isDirectory())
    {
        DEBUG_PRINTLN("- failed to open file for reading");
        return new vector<String>(); // Fix: Return an empty vector
    }

    DEBUG_PRINTLN("- read from file:");
    vector<String> *data = new vector<String>();
    int i = 0;
    while (file.available())
    {
        String temp = file.readStringUntil('\n');
        temp.remove(int(temp.indexOf('\r')));
        data->push_back(temp);
        DEBUG_PRINTLN(data->at(i++));
    }
    file.close();

    return data;
}

void StorageHandler::removeDir(const char *path)
{
    DEBUG_PRINTF("Removing Dir: %s\r\n", path);
    if (LittleFS.rmdir(path))
    {
        DEBUG_PRINTLN("Dir removed");
    }
    else
    {
        DEBUG_PRINTLN("rmdir failed");
    }
}

void StorageHandler::createDir(const char *path)
{
    DEBUG_PRINTF("Creating Dir: %s\r\n", path);
    if (LittleFS.mkdir(path))
    {
        DEBUG_PRINTLN("Dir created");
    }
    else
    {
        DEBUG_PRINTLN("mkdir failed");
    }
}

void StorageHandler::createFile(const char *path)
{
    DEBUG_PRINTF("Creating file: %s\r\n", path);
    if (checkFile(path))
    {
        DEBUG_PRINTLN("- file already exists");
        return;
    }
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        DEBUG_PRINTLN("- failed to create file");
        return;
    }
    DEBUG_PRINTLN("- file created");
    file.close();
}

void StorageHandler::listDir(const char *dirname, uint8_t levels)
{
    DEBUG_PRINTF("Listing directory: %s\r\n", dirname);

    File root = LittleFS.open(dirname);
    if (!root)
    {
        DEBUG_PRINTLN("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        DEBUG_PRINTLN(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            DEBUG_PRINT("  DIR : ");
            DEBUG_PRINTLN(file.name());
            if (levels)
            {
                listDir(file.path(), levels - 1);
            }
        }
        else
        {
            DEBUG_PRINT("  FILE: ");
            DEBUG_PRINT(file.name());
            DEBUG_PRINT("\tSIZE: ");
            DEBUG_PRINTLN(file.size());
        }
        file = root.openNextFile();
    }
}