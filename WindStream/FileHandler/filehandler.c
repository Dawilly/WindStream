#include "filehandler.h"
#include "wave_filehandler.h"
#include "../WindStream.h"
#include "../helpers.h"

FileHandler* CreateWaveInstance(FileHandler*);

FileHandler* File_CreateInstance(FileType fileType) {
	FileHandler* newHandler = Cmalloc(sizeof(FileHandler), "FileHandler.CreateFileInstance", TRUE);

	switch (fileType) {
		case Wave:
			return CreateWaveInstance(newHandler);
		case Flac:
			return NULL;
		case Ogg:
			return NULL;
		default:
			return NULL;
	}
}

FileHandler* CreateWaveInstance(FileHandler* ptr) {
	ptr->instance = Wave_CreateInstance();
	ptr->Create = Wave_CreateInstance;
	ptr->Open = Wave_OpenFile;
	ptr->Close = Wave_CloseFile;
	ptr->Destroy = Wave_DestroyInstance;
	ptr->Initialize = Wave_SetupInstance;
	ptr->ReadHeader = Wave_ReadHeader;
	ptr->PrintInfo = Wave_PrintInfo;
	ptr->Reset = Wave_ResetPosition;
	return ptr;
}