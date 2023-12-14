#pragma once
#include "InodeBase.h"

#include <string>
#include <vector>
#include <bitset>

class FileSystem
{
public:
    FileSystem();

private:
    std::vector<std::string> ProcessDir(std::string path);

    int GetInodeFromDir(std::string name, int curDirInodeIdx);
    int SearchParentInode(std::vector<std::string> epath);

    void Delete_k2(int InodeIdx);
    void DeleteDir_k2(int InodeIdx);
    void DeleteFile_k2(int InodeIdx);

    int Create_k2(std::string path, bool bIsDir);

public:
    void CreateDirectory(std::string path);
    void CreateFile(std::string path);

    void Delete(std::string path);

    std::vector<std::string> List(std::string path);

    void Copy(std::string sourcePath, std::string targetDir);

    std::string ReadFile(std::string path);
    void WriteFile(std::string path, std::string content);

    /*
            read    write
    0 = 00  forbid  forbid
    1 = 01  forbid  permit
    2 = 10  permit  forbid
    3 = 11  permit  permit
    */
    void ChangeMode(std::string path, int mode);

    std::string GetFileName(std::string path);

private:
    Inode InodeMemory[INODE_NUMBER];
    FileBlock BlockMemory[BLOCK_NUMBER];
    std::bitset<INODE_NUMBER> bIsInodeFree;
    std::bitset<BLOCK_NUMBER> bIsBlockFree;
    std::bitset<INODE_NUMBER> bRead;
    std::bitset<INODE_NUMBER> bWrite;
};