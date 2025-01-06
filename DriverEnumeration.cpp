#include <windows.h>
#include <stdio.h>

#pragma once
#pragma warning(disable : 4996)
#define MAXIMUM_FILENAME_LENGTH 255

typedef struct _SYSTEM_MODULE {
    ULONG Reserved1;
    ULONG Reserved2;
#ifdef _WIN64
    ULONG Reserved3;
#endif
    PVOID ImageBaseAddress;
    ULONG ImageSize;
    ULONG Flags;
    WORD Id;
    WORD Rank;
    WORD w018;
    WORD NameOffset;
    CHAR Name[MAXIMUM_FILENAME_LENGTH];
} SYSTEM_MODULE, * PSYSTEM_MODULE;

typedef struct _SYSTEM_MODULE_INFORMATION {
    ULONG ModulesCount;
    SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemModuleInformation = 0xb
} SYSTEM_INFORMATION_CLASS;

typedef NTSTATUS(WINAPI* PNtQuerySystemInformation)(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
    );

int main()
{
    ULONG size = 0;
    HMODULE ntdll = GetModuleHandle(TEXT("ntdll"));
    PNtQuerySystemInformation Systeminfo = (PNtQuerySystemInformation)GetProcAddress(ntdll, "NtQuerySystemInformation");

    NTSTATUS status = Systeminfo(SystemModuleInformation, NULL, 0, &size);
    if (status != 0xC0000004) {
        printf("Failed to retrieve buffer size. Error code: 0x%X\n", status);
        return 1;
    }

    PSYSTEM_MODULE_INFORMATION moduleInfo = (PSYSTEM_MODULE_INFORMATION)GlobalAlloc(GMEM_ZEROINIT, size);
    if (!moduleInfo) {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    status = Systeminfo(SystemModuleInformation, moduleInfo, size, &size);
    if (status != 0x0) {
        printf("Failed to retrieve system module information. Error code: 0x%X\n", status);
        GlobalFree(moduleInfo);
        return 1;
    }

    FILE* outputFile = fopen("output.txt", "w");
    if (!outputFile) {
        printf("Failed to create output file.\n");
        GlobalFree(moduleInfo);
        return 1;
    }

    for (ULONG i = 0; i < moduleInfo->ModulesCount; i++) {
        PVOID kernelImageBase = moduleInfo->Modules[i].ImageBaseAddress;
        PCHAR kernelImageName = (PCHAR)moduleInfo->Modules[i].Name;
        ULONG imageSize = moduleInfo->Modules[i].ImageSize;
        ULONG flags = moduleInfo->Modules[i].Flags;

        fprintf(outputFile, "\n****************************************************\n");
        fprintf(outputFile, "Driver module name: %s\n", kernelImageName);
        fprintf(outputFile, "Base address: 0x%p\n", kernelImageBase);
        fprintf(outputFile, "Image size: 0x%lx\n", imageSize);
        fprintf(outputFile, "Flags: 0x%lx\n", flags);
        fprintf(outputFile, "Id: %hu\n", moduleInfo->Modules[i].Id);
        fprintf(outputFile, "\n****************************************************\n");
    }

    fclose(outputFile);
    GlobalFree(moduleInfo);
    return 0;
}