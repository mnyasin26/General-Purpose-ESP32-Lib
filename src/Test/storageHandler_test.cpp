#include <main_header.h>

#if TESTING == true

#if ENABLE_DEBUG == true
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(x, y) Serial.printf(x, y)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x, y)
#endif

#include <TaskScheduler.h>
#include "Utility/StorageHandler.h"

void t1Callback();

// Tasks
Task t1(1000, 10, &t1Callback);

Scheduler runner;

void setup()
{
    Serial.begin(115200);

    delay(1000);
    DEBUG_PRINTLN("Hello world");

    DEBUG_PRINTLN("StorageHandler test");

    StorageHandler::begin();

    DEBUG_PRINTLN("=== checkDir /test: ==========================================");
    if (StorageHandler::checkDir("/test"))
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    else
    {
        DEBUG_PRINTLN("[OK]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== checkFile /test/test.txt: ==========================================");
    if (StorageHandler::checkFile("/test/test.txt"))
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    else
    {
        DEBUG_PRINTLN("[OK]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== testFileIO /test/test.txt: ==========================================");
    if (StorageHandler::testFileIO("/test/test.txt"))
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    else
    {
        DEBUG_PRINTLN("[OK]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== createDir /test: ==========================================");
    StorageHandler::createDir("/test");
    if (StorageHandler::checkDir("/test"))
    {
        DEBUG_PRINTLN("[OK]");
    }
    else
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== createFile /test/test.txt: ==========================================");
    StorageHandler::createFile("/test/test.txt");
    if (StorageHandler::checkFile("/test/test.txt"))
    {
        DEBUG_PRINTLN("[OK]");
    }
    else
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== testFileIO /test/test.txt: ==========================================");
    if (StorageHandler::testFileIO("/test/test.txt"))
    {
        DEBUG_PRINTLN("[OK]");
    }
    else
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== writeFile /test/test.txt: ==========================================");
    StorageHandler::writeFile("/test/test.txt", "Hello world");
    DEBUG_PRINTLN("=== readFile /test/test.txt: ==========================================");
    vector<String> *v = StorageHandler::readFile("/test/test.txt");
    for (int i = 0; i < v->size(); i++)
    {
        DEBUG_PRINTF("=== v1->at(%d): ", i);
        DEBUG_PRINTLN(v->at(i));
    }
    if (v->size() == 1 && v->at(0) == "Hello world")
    {
        DEBUG_PRINTLN("[OK]");
    }
    else
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    delete v;
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== appendFile /test/test.txt: ==========================================");
    StorageHandler::appendFile("/test/test.txt", "Hello world2");
    DEBUG_PRINTLN("=== readFile /test/test.txt: ==========================================");
    vector<String> *v1 = StorageHandler::readFile("/test/test.txt");
    for (int i = 0; i < v1->size(); i++)
    {
        DEBUG_PRINTF("=== v1->at(%d): ", i);
        DEBUG_PRINTLN(v1->at(i));
    }
    if (v1->size() == 2 && v1->at(0) == "Hello world" && v1->at(1) == "Hello world2")
    {
        DEBUG_PRINTLN("[OK]");
    }
    else
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    delete v1;
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== renameFile /test/test.txt to /test/test2.txt: ==========================================");
    StorageHandler::renameFile("/test/test.txt", "/test/test2.txt");
    DEBUG_PRINTLN("=== checkFile /test/test.txt: ==========================================");
    if (StorageHandler::checkFile("/test/test.txt"))
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    else
    {
        DEBUG_PRINTLN("[OK]");
    }
    DEBUG_PRINTLN("=== checkFile /test/test2.txt: ==========================================");
    if (StorageHandler::checkFile("/test/test2.txt"))
    {
        DEBUG_PRINTLN("[OK]");
    }
    else
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== deleteFile /test/test2.txt: ==========================================");
    StorageHandler::deleteFile("/test/test2.txt");
    DEBUG_PRINTLN("=== checkFile /test/test2.txt: ==========================================");
    if (StorageHandler::checkFile("/test/test2.txt"))
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    else
    {
        DEBUG_PRINTLN("[OK]");
    }
    DEBUG_PRINTLN();

    DEBUG_PRINTLN("=== removeDir /test: ==========================================");
    StorageHandler::removeDir("/test");
    DEBUG_PRINTLN("=== checkDir /test: ==========================================");
    if (StorageHandler::checkDir("/test"))
    {
        DEBUG_PRINTLN("[FAILED]");
    }
    else
    {
        DEBUG_PRINTLN("[OK]");
    }
    DEBUG_PRINTLN();

    runner.init();
    runner.addTask(t1);
    t1.enable();
}

void loop()
{
    runner.execute();
}

void t1Callback()
{
    DEBUG_PRINT("t1: ");
    DEBUG_PRINTLN(millis());
}

#endif