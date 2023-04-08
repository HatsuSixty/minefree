#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <urlmon.h>

#include <io.h>
#define F_OK 0
#define access _access

void execute_command(const char* command) {
    switch (system(command)) {
    case E2BIG:
        fprintf(stderr, "ERROR: Argument list too big\n");
        exit(1);
    case ENOENT:
        fprintf(stderr, "ERROR: Command interpreter could not be found\n");
        exit(1);
    case ENOEXEC:
        fprintf(stderr, "ERROR: Command interpreter file could not be executed because the format is invalid\n");
        exit(1);
    case ENOMEM:
        fprintf(stderr, "ERROR: Memory error while executing command `%s`\n", command);
        exit(1);
    }
}

void exitprg(int c) {
    execute_command("pause");
    exit(c);
}

int main(int argc, const char** argv) {
    const char* removedfile = "C:\\Windows\\System32\\Windows.ApplicationModel.Store.dll";

    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0) {
            printf("Usage: %s [FLAGS] [INPUTFILE]\n", argv[0]);
            printf("  FLAGS:\n");
            printf("    --help   Prints this help\n");
            printf("  INPUTFILE: The file that is going to be replaced in order to crack Minecraft\n");
            exitprg(1);
        } else {
            removedfile = argv[1];
        }
    }

    if (access(removedfile, F_OK) != 0) {
        fprintf(stderr, "ERROR: The provided file does not exist\n");
        exitprg(1);
    }

    const char* downloaded_file = basename((char*) removedfile);

    if (URLDownloadToFile(NULL,
                          "https://archive.org/download/freeminecraftbedrock/Windows.ApplicationModel.Store.dll",
                           downloaded_file,
                           0,
                           NULL))
    {
        fprintf(stderr, "ERROR: Could not download file `%s`\n", downloaded_file);
        exitprg(1);
    }

    char takeown_cmd[256] = {0};
    char icacls_cmd[256] = {0};

    snprintf(takeown_cmd, 256, "takeown /F %s", removedfile);
    snprintf(icacls_cmd, 256, "icacls %s /grant %%username%%:F", removedfile);

    execute_command(takeown_cmd);
    execute_command(icacls_cmd);

    if (remove(removedfile) != 0) {
        fprintf(stderr, "ERROR: Could not delete file `%s`\n", removedfile);
        exitprg(1);
    }

    if (!CopyFile(downloaded_file, removedfile, FALSE)) {
        fprintf(stderr, "ERROR: Could not copy file `%s` into `%s`\n", downloaded_file, removedfile);
        exitprg(1);
    }

    MessageBox(NULL, "Minecraft cracked successfully!", "minefree.exe", MB_OK | MB_ICONINFORMATION);

    exitprg(0);
}
