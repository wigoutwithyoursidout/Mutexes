#using <System.dll>

using namespace System;
using namespace System::Threading;

ref class Mutexes
{
public:
	static Object^ lockObj;
	static Mutex^ ourMutex; // static objects make all instances of this class chare the same variables
	static Barrier^ threadBarrier;
	int numThreads;
	static bool Quit;

public:
	Mutexes()
	{ 

		//ourMutex = gcnew Mutex;
		lockObj = gcnew Object;
		numThreads = 3;
		threadBarrier = gcnew Barrier(numThreads);
		Quit = false;
	}

	static void Lidar()
	{
		Console::WriteLine("Lidar Thread has started");
		Thread::Sleep(5000);
		Console::WriteLine("Lidar	Thread Ready");
		threadBarrier->SignalAndWait();

		while (!Quit)
		{
			Console::WriteLine("Lidar Scan Completed");
			CriticalSection();
			Thread::Sleep(50);
		}
	}

	static void GNSS()
	{

		Console::WriteLine("GNSS Thread Started");
		Thread::Sleep(10000);
		Console::WriteLine("GNSS	Thread Ready");
		threadBarrier->SignalAndWait();
		while (!Quit)
		{
			Console::WriteLine("GNSS Scan Completed");
			CriticalSection();
			Thread::Sleep(50);
		}
	}

	static void Vehicle()
	{

		Console::WriteLine("Vehicle Thread Started");
		Thread::Sleep(10000);
		Console::WriteLine("Vehicle	Thread Ready");
		threadBarrier->SignalAndWait();
		while (!Quit)
		{
			Console::WriteLine("Vehicle Scan Completed");
			CriticalSection();
			Thread::Sleep(50);
		}
	}

	static void CriticalSection()
	{

		Console::WriteLine(Thread::CurrentThread->Name + " is requesting the mutex");
		Monitor::Enter(lockObj); 
		//ourMutex->WaitOne();
		Console::WriteLine(Thread::CurrentThread->Name + " is in CS");
		Thread::Sleep(300);
		//ourMutex->ReleaseMutex();
		Monitor::Exit(lockObj);
		Console::WriteLine(Thread::CurrentThread->Name + " has released mutex\n");

	}


};

int main() {
		

	Mutexes^ mutex1 = gcnew Mutexes;

	Thread^ lidarThread = gcnew Thread(gcnew ThreadStart(mutex1->Lidar));
	lidarThread->Name = "Lidar Thread";

	Thread^ GNSSThread = gcnew Thread(gcnew ThreadStart(mutex1->GNSS));
	GNSSThread->Name = "GNSS Thread";

	Thread^ vehicleThread = gcnew Thread(gcnew ThreadStart(mutex1->Vehicle));
	vehicleThread->Name = "Vehicle Thread";

	lidarThread->Start();
	GNSSThread->Start();
	vehicleThread->Start();


	Console::ReadKey();
	mutex1->Quit = true;

	Console::ReadKey();
	return 0; 
	
}