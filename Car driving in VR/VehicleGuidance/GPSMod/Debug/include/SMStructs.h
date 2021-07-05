#pragma once
struct ModuleFlags {
	unsigned char	PM : 1,
		GPS : 1,
		Laser : 1,
		Xbox : 1,
		Vehicle : 1,
		Display : 1,
		Unused : 3;
};

union ExecFlags {
	unsigned char Status;
	ModuleFlags Flags;
};
struct Remote {
	double SetSpeed;
	double SetSteering;
	bool crash;
};

struct PM {

	ExecFlags Heartbeats;
	ExecFlags PMHeartbeats;
	ExecFlags Shutdown;
	double TimeStamp[5];
	//double LaserTimeStamp;
	//double GPSTimeStamp;
	//double VehicleTimeStamp;
	//double RemoteTimeStamp;
	//bool Shutdown;
};


struct Laser {

	double XRange[600];
	double YRange[600];
};

struct GPS//112 bytes
{
	unsigned int Header;
	unsigned char Discards1[40];
	double Northing;
	double Easting;
	double Height;
	unsigned char Discards2[40];
	unsigned long Checksum;
};