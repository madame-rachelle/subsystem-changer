#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <ostream>

void changeSubsystem(const char* filePath)
{
	std::ifstream file(filePath, std::ios::binary | std::ios::in);
	if (!file)
	{
		std::cerr << "Cannot open file." << std::endl;
		return;
	}

	// Read DOS header
	IMAGE_DOS_HEADER dosHeader;
	file.read(reinterpret_cast<char*>(&dosHeader), sizeof(dosHeader));
	if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
	{
		std::cerr << "Not a valid PE file." << std::endl;
		return;
	}

	// Move to and read NT headers
	file.seekg(dosHeader.e_lfanew, std::ios::beg);
	IMAGE_NT_HEADERS ntHeaders;
	file.read(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));
	if (ntHeaders.Signature != IMAGE_NT_SIGNATURE)
	{
		std::cerr << "Not a valid PE file." << std::endl;
		return;
	}

	// Display current subsystem
	std::cout << "Current Subsystem: ";

	switch(ntHeaders.OptionalHeader.Subsystem)
	{
	case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		std::cout << "Console (CLI) subsystem\n";
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		std::cout << "Windows (GUI) subsystem\n";
		break;
	default:
		std::cout << ntHeaders.OptionalHeader.Subsystem << std::endl;
		break;
	}

	WORD newSubsystem = (ntHeaders.OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI) ?
		IMAGE_SUBSYSTEM_WINDOWS_CUI : IMAGE_SUBSYSTEM_WINDOWS_GUI;

	if (newSubsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI)
		std::cout << "Change to Console (CLI) subsystem (y/N)? ";
	else
		std::cout << "Change to Windows (GUI) subsystem (y/N)? ";

	// query user response

	char response;
	std::cin >> response;

	std::cout << std::endl;

	if (response != 'Y' && response != 'y')
	{
		std::cout << "Operation cancelled." << std::endl;
		return;
	}

	// Modify subsystem
	ntHeaders.OptionalHeader.Subsystem = newSubsystem;

	// Write changes back to file
	std::ofstream outFile(filePath, std::ios::binary | std::ios::out | std::ios::in);
	outFile.seekp(dosHeader.e_lfanew, std::ios::beg);
	outFile.write(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));
	outFile.close();

	std::cout << "Subsystem changed successfully." << std::endl;
}

int main(int argc, char* argv[])
{
	std::string filePath;

	if (argc > 1 && std::filesystem::exists(argv[1]))
	{
		filePath = argv[1];
	}
	else if (std::filesystem::exists("gzdoom.exe"))
	{
		filePath = "gzdoom.exe";
	}
	else if (std::filesystem::exists("vkdoom.exe"))
	{
		filePath = "vkdoom.exe";
	}
	else
	{
		std::cerr << "Error: No valid file found." << std::endl;
		return 1;
	}
	changeSubsystem(filePath.c_str());

	return 0;
}
