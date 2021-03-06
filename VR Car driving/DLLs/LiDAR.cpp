// LiDAR.cpp : Defines the entry point for the console application.
//
#using <System.dll>
#include <Windows.h>
#include <TlHelp32.h>
#include <SMObject.h>
#include <SMStructs.h>
#include <conio.h>

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace Text;

int main()
{
	//declarations
	TcpClient^ Client = nullptr;
	int PortNumber;
	array<unsigned char>^ SendData = nullptr;
	array<unsigned char>^ RecvData = nullptr;
	System::String ^RequestScan = nullptr;
	System::String ^LaserScan = nullptr;
	array<double>^ Range = nullptr;
	array<double>^ RangeX = nullptr;
	array<double>^ RangeY = nullptr;
	double StartAngle;
	double Resolution;
	int NumRanges;
	array<String^>^Fragments = nullptr;

	//instantiations
	Client = gcnew TcpClient;
	PortNumber = 2112;
	RequestScan = gcnew String("sRN LMDscandata");
	RecvData = gcnew array<unsigned char>(2048);
	//actins
	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;
	Client->SendTimeout = 500;
	Client->SendBufferSize = 32;
	Client->ReceiveBufferSize = 2048;
	Client->Connect("192.168.5.15", PortNumber);
	if (Client->Connected) 
		Console::WriteLine("Connected to LiDAR");

	NetworkStream^ Stream;
	Stream = Client->GetStream();

	SendData = System::Text::Encoding::ASCII->GetBytes(RequestScan);

	//Command to LiDAR
	Stream->WriteByte(0x02); //SOT
	Stream->Write(SendData, 0, SendData->Length);
	Stream->WriteByte(0x03);

	Stream->Read(RecvData, 0, RecvData->Length);
	LaserScan = System::Text::Encoding::ASCII->GetString(RecvData);
	Fragments = LaserScan->Split(' ');

	StartAngle = System::Convert::ToInt32(Fragments[23], 16);
	Resolution = System::Convert::ToInt32(Fragments[24], 16/10000);
	NumRanges = System::Convert::ToInt32(Fragments[25], 16);

	Range = gcnew array<double>(NumRanges);
	RangeX = gcnew array<double>(NumRanges);
	RangeY = gcnew array<double>(NumRanges);

	for (int i = 0; i < NumRanges; i++) {
		Range[i] = System::Convert::ToInt32(Fragments[23], 16);
		Console::Write("{0,10:F3} {1,10:F3}", RangeX[i], RangeY[i]);
		//RangeX[i] = Range[i] * Math::Sin(i*Resolution*Math::PI / 180);
		//RangeY[i] = Range[i] * Math::Cos(i*Resolution*Math::PI / 180);
		//Console::Write("{0,10:F3}", Range[i]);	
	}

	//Console::WriteLine("{0,10:F3} {1,10:F3} {2:D}", StartAngle, Resolution, NumRanges);
	//Console::WriteLine(LaserScan);



	Console::ReadKey();


    return 0;
}

