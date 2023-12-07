#pragma once

#define BLOCK_SIZE 1024
#define MAX_FILENAME_LENGTH 28
#define ENTRY_NUMBER 32
#define BLOCK_NUMBER 1024
#define INODE_NUMBER 1024

struct Inode
{
    int BlockID;
    bool bIsDir;
};

struct DirectoryBlock
{
    char FileName[ENTRY_NUMBER][MAX_FILENAME_LENGTH];
    int InodeID[ENTRY_NUMBER];
};

struct FileBlock
{
    char Content[BLOCK_SIZE];
};
