#include "serial.h"
#include <stdio.h>
#include <stdbool.h>
#include <3ds.h>
#include <string.h>
#include <stdlib.h>

static char secinfoserial[0x10];
static char twlnserial[0x10];
static char essentialserial[0x10];

char* getSecinfoSerial() {
    return secinfoserial;
}

char* getTWLNSerial() {
    return twlnserial;
}

char* getEssentialSerial() {
    return essentialserial;
}

void setEssentialSerial() {
    FILE *f = fopen("sdmc:/gm9/out/essential.exefs", "rb");
    if (!f) {
        essentialserial[0] = '\0';
        return;
    }
    fseek(f, 0x502, SEEK_SET);
    fread(essentialserial, 0xF, 1, f);
    for (int i = 0;i < 0xF;i++) {
        if (essentialserial[i] == 0) {
            essentialserial[i] = '\0';
            break;
        }
    }
    fclose(f);
    return;
}

void setSecinfoSerial() {
    cfguInit();
    CFGI_SecureInfoGetSerialNumber((u8*)secinfoserial);
    for (int i = 0;i < 0xF;i++) {
        if (secinfoserial[i] == 0) {
            secinfoserial[i] = '\0';
            break;
        }
    }
    cfguExit();
}

void setTWLNSerial() {
    Handle file;

    Result ret = FSUSER_OpenFileDirectly(&file, ARCHIVE_NAND_TWL_FS, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, "/sys/log/inspect.log"), FS_OPEN_READ, 0);

    if (R_FAILED(ret)) {
        sprintf(twlnserial, "1. ret %08lx", ret);
        return;
    }
    u64 size;
    ret = FSFILE_GetSize(file, &size);

    if (R_FAILED(ret)) {
        sprintf(twlnserial, "2. ret %08lx", ret);
        FSFILE_Close(file);
        return;
    }
    char *log = malloc(size);
    ret = FSFILE_Read(file, NULL, 0, log, size);

    if (R_FAILED(ret)) {
        sprintf(twlnserial, "3. ret %08lx", ret);
        FSFILE_Close(file);
        free(log);
        return;
    }
    ret = FSFILE_Close(file);
    if (R_FAILED(ret)) {
        sprintf(twlnserial, "4. ret %08lx", ret);
        FSFILE_Close(file);
        free(log);
        return;
    }

    char* serialentry = strstr(log, "SerialNumber=") + 13;
    for (int i = 0;i<0xF;i++) {
        if (serialentry[i] == '\n') {
            serialentry[i] = '\0';
            strncpy(twlnserial, serialentry, i+1);
            free(log);
            return;
        }
    }
    free(log);
    twlnserial[0] = '\0';
    return;
}