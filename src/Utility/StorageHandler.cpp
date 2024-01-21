#include "StorageHandler.h"

StorageHandler::StorageHandler()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }
}

StorageHandler::~StorageHandler()
{
}

bool StorageHandler::checkDir(const char *path)
{
    if (LittleFS.exists(path))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StorageHandler::checkFile(const char *path)
{
    if (LittleFS.exists(path))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void StorageHandler::testFileIO(const char *path)
{
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
        {
            Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
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
        Serial.print("- reading");
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
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    }
    else
    {
        Serial.println("- failed to open file for reading");
    }
}

void StorageHandler::deleteFile(const char *path)
{
    Serial.printf("Deleting file: %s\r\n", path);
    if (LittleFS.remove(path))
    {
        Serial.println("- file deleted");
    }
    else
    {
        Serial.println("- delete failed");
    }
}

void StorageHandler::renameFile(const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (LittleFS.rename(path1, path2))
    {
        Serial.println("- file renamed");
    }
    else
    {
        Serial.println("- rename failed");
    }
}

void StorageHandler::appendFile(const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\r\n", path);

    File file = LittleFS.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("- failed to open file for appending");
        return;
    }
    if (file.println(message))
    {
        Serial.println("- message appended");
    }
    else
    {
        Serial.println("- append failed");
    }
    file.close();
}

void StorageHandler::writeFile(const char *path, const char *message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.println(message))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
}

void StorageHandler::begin()
{
}

String *StorageHandler::readFile(const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = LittleFS.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return new String(""); // Fix: Return an empty string
    }

    Serial.println("- read from file:");
    String *data = new String();
    int i = 0;
    while (file.available())
    {
        data[i++] = file.readStringUntil('\n');
        // Serial.write(file.read());
    }
    file.close();

    return data;
}

void StorageHandler::removeDir(const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (LittleFS.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

void StorageHandler::createDir(const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (LittleFS.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void StorageHandler::listDir(const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = LittleFS.open(dirname);
    if (!root)
    {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(file.path(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}