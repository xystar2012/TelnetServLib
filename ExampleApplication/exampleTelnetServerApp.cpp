// TelnetApplication.cpp : Defines the entry point for the console application.
//

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "exampleTelnetServerApp.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <ctime>
#include <sys/timeb.h>
#include <string>

void myConnected(SP_TelnetSession session)
{
    std::cout << "myConnected got called\n";
    session->sendLine("Welcome to the Telnet Server.");
}

void myNewLine(SP_TelnetSession session, std::string line)
{
    std::cout << "myNewLine got called with line: " << line << "\n";
	
	if (line.compare("bye") == 0 || line.compare("quit") == 0)
	{
		session->sendLine("quit bye server ...");
		session->closeClient();
	}
	else
	{
		time_t time(NULL);
		timeb tb;
		ftime(&tb);
		tb.millitm;
		char szTm[64] = { 0 };
		struct tm buf;
		strftime(szTm, 64, "[%Y-%m-%d %H:%M:%S.", localtime(&tb.time));
		//session->sendLine("Copy that.");
		std::string data;
		data.assign(szTm);
		data.append(std::to_string(tb.millitm));
		data.append("]");
		data.append(line);

		session->sendLine(data);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
    // Do unit tests
    TelnetSession::UNIT_TEST();

    // Create a terminal server which
    auto ts = std::make_shared < TelnetServer >();
    
    ts->initialise(27015,"prompt: ");
    ts->connectedCallback(myConnected);
    ts->newLineCallback(myNewLine);

    // Our loop
    do 
    {
        ts->update();
        Sleep(16);
    } 
    while (true);

    ts->shutdown();
    WSACleanup();

    return 0;
}
