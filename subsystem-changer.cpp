#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>

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
	std::cout << "Current Subsystem: " << ntHeaders.OptionalHeader.Subsystem << std::endl;

	WORD newSubsystem = (ntHeaders.OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI) ?
		IMAGE_SUBSYSTEM_WINDOWS_CUI : IMAGE_SUBSYSTEM_WINDOWS_GUI;

	if (newSubsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI)
		std::cout << "Changing to Console (CLI) subsystem" << std::endl;
	else
		std::cout << "Changing to Windows (GUI) subsystem" << std::endl;

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
