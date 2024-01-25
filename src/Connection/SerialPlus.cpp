#include "SerialPlus.h"

SerialPlus::SerialPlus(uint8_t port) : serialPort(port)
{
}

bool SerialPlus::begin(uint8_t RX, uint8_t TX, int baudrate, bool isAddressMode, int address)
{
    this->baudrate = baudrate;
    this->RX = RX;
    this->TX = TX;
    this->address = address;
    serialPort.begin(baudrate, SERIAL_8N1, RX, TX);
    isBegin = true;
    return true;
}

void SerialPlus::write(int desAddress, String message)
{
    serialPort.print(desAddress);
    serialPort.print(" ");
    serialPort.print(message);
}

void SerialPlus::write(String message)
{
    serialPort.print(message);
}

void SerialPlus::print(int desAddress, String message)
{
    serialPort.print(desAddress);
    serialPort.print(" ");
    serialPort.print(message);
}

void SerialPlus::print(String message)
{
    serialPort.print(message);
}

void SerialPlus::println(int desAddress, String message)
{
    serialPort.print(desAddress);
    serialPort.print(" ");
    serialPort.println(message);
}

void SerialPlus::println(String message)
{
    serialPort.println(message);
}

String SerialPlus::getLatestMsg()
{
    isLatestMsg = false;
    String temp = latest_msg;
    latest_msg = "";
    return temp;
}

String SerialPlus::getSourceAddressLatestMsg()
{
    return sourceAddressLatestMsg;
}

String SerialPlus::getPreviousMsg()
{
    return previous_msg;
}

String SerialPlus::getSourceAddressPreviousMsg()
{
    return sourceAddressPreviousMsg;
}

bool SerialPlus::isMsgAvailable()
{
    return isLatestMsg;
}

void SerialPlus::pauseListen()
{
    isListen = false;
}

void SerialPlus::resumeListen()
{
    isListen = true;
}

void SerialPlus::end()
{
    serialPort.end();
    isBegin = false;
}

void SerialPlus::loop()
{
    if (isBegin)
    {
        if (isListen)
        {
            listen();
        }
    }
}

void SerialPlus::listen()
{
    if (serialPort.available())
    {
        char aChar = serialPort.read();
        if (aChar != terminator)
        {
            buffer += aChar;
        }
        else
        {
            if (isListen)
            {
                previous_msg = latest_msg;
                if (isAddressMode)
                {
                    sourceAddressPreviousMsg = sourceAddressLatestMsg;
                    sourceAddressLatestMsg = buffer.substring(0, buffer.indexOf(" "));
                    latest_msg = buffer.substring(buffer.indexOf(" ") + 1);
                    /* code */
                }
                else
                {
                    sourceAddressPreviousMsg = sourceAddressLatestMsg;
                    sourceAddressLatestMsg = "";
                    latest_msg = buffer;
                }

                isLatestMsg = true;
                buffer = "";
            }
            else
            {
                buffer = "";
            }
        }
    }
}

void SerialPlus::setAddressMode(bool isAddressMode)
{
    this->isAddressMode = isAddressMode;
}

void SerialPlus::setTerminator(char terminator)
{
    this->terminator = terminator;
}

char SerialPlus::getTerminator()
{
    return terminator;
}

SerialPlus::~SerialPlus()
{
}
