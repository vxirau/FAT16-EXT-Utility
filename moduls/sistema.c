#include "sistema.h"

char* SYS_cleanLabel(char cadena[11]){
    char *token;
    char s[2] = " ";
    token = strtok(cadena, s);
   return token;
}

char* SYS_fileInDirectory(char* directory, char* fileName){
    char *fullPath = malloc(sizeof(char)*(strlen(directory) + strlen(fileName) + 3));;
    strcpy(fullPath, directory);
    strcat(fullPath, "/");
    strcat(fullPath, fileName);
   return fullPath;
}

int SYS_checkOperation(char* operation){
    int op=0;
    for(int i=0; i<ACCEPTED_OPERATIONS ;i++){
        if(strcmp(operation, acceptedOperations[i]) == 0){
            op = i+1;
        }
    }
    return op;
}

int SYS_getSystemType(int fdFitxer, SB **superblock, BootSector **bs){
        int systemType = 0;
        lseek(fdFitxer, SUPERBLOCK_START, SEEK_SET);
        *superblock = malloc(sizeof(SB));
        int bytes = read(fdFitxer, *superblock, sizeof(SB));

        if((*superblock)->s_magic == 0xEF53){
            systemType = 1;
        }else{
            lseek(fdFitxer, 0, SEEK_SET);
            *bs = malloc(sizeof(BootSector));
            read(fdFitxer, (*bs)->BS_jmpBoot, 3);
            read(fdFitxer, (*bs)->BS_OEMName, 8);
            read(fdFitxer, &(*bs)->BPB_BytsPerSec, 2);
            read(fdFitxer, &(*bs)->BPB_SecPerClus, 1);
            read(fdFitxer, &(*bs)->BPB_RsvdSecCnt, 2);
            read(fdFitxer, &(*bs)->BPB_NumFATs, 1);
            read(fdFitxer, &(*bs)->BPB_RootEntCnt, 2);
            read(fdFitxer, &(*bs)->BPB_TotSec16, 2);
            read(fdFitxer, &(*bs)->BPB_Media, 1);
            read(fdFitxer, &(*bs)->BPB_FATSz16, 2);
            read(fdFitxer, &(*bs)->BPB_SecPerTrk, 2);
            read(fdFitxer, &(*bs)->BPB_NumHeads, 2);
            read(fdFitxer, &(*bs)->BPB_HiddSec, 4);
            read(fdFitxer, &(*bs)->BPB_TotSec32, 4);
            read(fdFitxer, &(*bs)->BS_DrvNum, 1);
            read(fdFitxer, &(*bs)->BS_Reserved1, 1);
            read(fdFitxer, &(*bs)->BS_BootSig, 1);
            read(fdFitxer, (*bs)->BS_VolLab, 11);
            read(fdFitxer, (*bs)->BS_FilSysType, 8);

            if(FAT_checkIfFat16(*bs)){
                systemType=2;
            }

        }  
        return systemType;
}

void SYS_fileNotFound(int fdFile, int opType){
    switch(opType){
        case 1:
            write(1, ERR_NO_EXISTEIX, strlen(ERR_NO_EXISTEIX));
            break;
        case 2:
            write(1, ERR_NO_FIND_EXISTEIX, strlen(ERR_NO_FIND_EXISTEIX));       
            break;
        default:
            break;
    }
    
    close(fdFile);
}

void SYS_removeExtension(char* file){
    for(int i=0; file[i]!='\0' ;i++){
        if(file[i]=='.'){
            file[i]='\0';
            break;
        }
    }
}