// SerialComms.cpp : Defines the entry point for the console application.
//
#using <System.dll>
#define CRC32_POLYNOMIAL			0xEDB88320L
#include <conio.h>

using namespace System;
using namespace System::IO::Ports;
using namespace Text;

void OnDataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs ^e);
double Northing;
double Easting;
double Height;
int ErrorCode = 0;
unsigned int CalculatedCRC;

struct GPS//112 bytes
{
	unsigned int Header;
	unsigned char Discards1[40];
	double Northing;
	double Easting;
	double Height;
	unsigned char Discards2[40];
	unsigned int Checksum;
};

GPS NovatelGPS;


unsigned long CRC32Value(int i)
{
	int j;
unsigned long ulCRC;
ulCRC = i;
for (j = 8; j > 0; j--)
{
	if (ulCRC & 1)
		ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
	else
		ulCRC >>= 1;
}
return ulCRC;
}
/* --------------------------------------------------------------------------
Calculates the CRC-32 of a block of data all at once
-------------------------------------------------------------------------- */
unsigned long CalculateBlockCRC32(unsigned long ulCount, /* Number of bytes in the data block */
	unsigned char *ucBuffer) /* Data block */
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}

int main()
{
	//declarations
	SerialPort^ Port = nullptr;
	String^ PortName = nullptr;

	//instantiations
	Port = gcnew SerialPort;

	//configurations
	Port->PortName = PortName;
	Port->BaudRate = 115200;
	Port->StopBits = StopBits::One;
	Port->DataBits = 8;
	Port->Parity = Parity::None;
	Port->Handshake = Handshake::None;

	// Set the read/write timeouts & buffer size
	Port->ReadTimeout = 500;
	Port->WriteTimeout = 500;
	Port->ReadBufferSize = 128;
	Port->WriteBufferSize = 16;

	try
	{
		Port->Open();
	}
	catch (System::IO::IOException^)
	{
		Console::WriteLine("IOExeption");
		ErrorCode = 1;
		return 1;
	}

	Port->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(&OnDataReceived);

	Port->DiscardInBuffer();

	while (1) 
	{
		if (!ErrorCode)
		{
			//setGPS Shared memory to Northing easting and height;
		}
		if (_kbhit) break;
		System::Threading::Thread::Sleep(20);
	}


	return 0;
}

void OnDataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs ^e)
{
	ErrorCode = 0;
	//do GPS calculations for Northing, Easting and height
	SerialPort^ ePort = (SerialPort^)sender;
	unsigned char Buffer[sizeof(GPS)];
	array<unsigned char>^ RecvData = gcnew array<unsigned char>(128);
	while (ePort->BytesToRead < 112)
	{
		//put timeout condition

	}
	ePort->Read(RecvData, 0, sizeof(GPS));
	if (*(unsigned int*)(&RecvData) != 0x1c1244aa)
	{
		ErrorCode = 4;
		return;
	}
	unsigned char* BytePtr = (unsigned char*)&NovatelGPS;
	for (int i = 0; i < sizeof(GPS); i++)
	{
		*(BytePtr + i) = Buffer[i] = RecvData[i];
	}
	CalculatedCRC = CalculateBlockCRC32(108, Buffer);
	if (CalculatedCRC == NovatelGPS.Checksum)
	{
		Height = NovatelGPS.Height;
		Northing = NovatelGPS.Northing;
		Easting = NovatelGPS.Easting;
	}
	else
	{
		ErrorCode = 5;
		return;
	}
}