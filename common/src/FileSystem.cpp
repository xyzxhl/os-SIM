#include "../include/FileSystem.h"
#include <sstream>
#include <string.h>

using namespace std;

FileSystem::FileSystem()
{
    bIsInodeFree.set();
    bIsBlockFree.set();
    bIsInodeFree.reset(0);
    bIsBlockFree.reset(0);
    bRead.reset();
    bWrite.reset();

    memset(InodeMemory, 0, sizeof(Inode) * INODE_NUMBER);
    memset(BlockMemory, 0, sizeof(FileBlock) * BLOCK_NUMBER);
    InodeMemory[0].bIsDir = true;
}

vector<string> FileSystem::ProcessDir(string path)
{
    vector<string> result;
    istringstream iss(path);
    string token;

    while (getline(iss, token, '/'))
    {
        if (token != "")
            result.push_back(token);
    }

    return result;
}

int FileSystem::GetInodeFromDir(string name, int curDirInodeIdx)
{
    if (InodeMemory[curDirInodeIdx].bIsDir == false)
        return 0;

    int curBlockIdx = InodeMemory[curDirInodeIdx].BlockID;
    DirectoryBlock *curBlock = reinterpret_cast<DirectoryBlock *>(&BlockMemory[curBlockIdx]);

    for (int i = 0; i < ENTRY_NUMBER; i++)
    {
        string fileName = string(curBlock->FileName[i]);
        if (fileName == name && curBlock->InodeID[i])
        {
            return curBlock->InodeID[i];
        }
    }

    return 0;
}

int FileSystem::SearchParentInode(vector<string> epath)
{
    int curDir = 0;
    for (int i = 0; i < epath.size() - 1; i++)
    {
        curDir = GetInodeFromDir(epath[i], curDir);
        if (curDir == 0)
            return 0;
    }
    return curDir;
}

void FileSystem::Delete_k2(int InodeIdx)
{
    if (InodeMemory[InodeIdx].bIsDir)
    {
        DeleteDir_k2(InodeIdx);
    }
    else
    {
        DeleteFile_k2(InodeIdx);
    }
}

void FileSystem::DeleteDir_k2(int InodeIdx)
{
    int BlockIdx = InodeMemory[InodeIdx].BlockID;
    DirectoryBlock *Block = reinterpret_cast<DirectoryBlock *>(&BlockMemory[BlockIdx]);

    for (int i = 0; i < ENTRY_NUMBER; i++)
    {
        if (Block->InodeID[i])
        {
            Delete_k2(Block->InodeID[i]);
        }
    }

    memset(&InodeMemory[InodeIdx], 0, sizeof(Inode));
    memset(&BlockMemory[BlockIdx], 0, sizeof(DirectoryBlock));
    bIsInodeFree.set(InodeIdx);
    bIsBlockFree.set(BlockIdx);
}

void FileSystem::DeleteFile_k2(int InodeIdx)
{
    int BlockIdx = InodeMemory[InodeIdx].BlockID;
    memset(&InodeMemory[InodeIdx], 0, sizeof(Inode));
    memset(&BlockMemory[BlockIdx], 0, sizeof(FileBlock));
    bIsInodeFree.set(InodeIdx);
    bIsBlockFree.set(BlockIdx);
    bRead.reset(InodeIdx);
    bWrite.reset(InodeIdx);
}

int FileSystem::Create_k2(string path, bool bIsDir)
{
    vector<string> epath = ProcessDir(path);
    if (epath.back().size() >= MAX_FILENAME_LENGTH || epath.back().size() == 0)
        return 0;

    int InodeIdx = SearchParentInode(epath);
    if (InodeIdx == 0 && epath.size() != 1)
        return 0;

    int BlockIdx = InodeMemory[InodeIdx].BlockID;
    DirectoryBlock *Block = reinterpret_cast<DirectoryBlock *>(&BlockMemory[BlockIdx]);

    for (int i = 0; i < ENTRY_NUMBER; i++)
    {
        string fileName = string(Block->FileName[i]);
        if (fileName == epath.back() && Block->InodeID[i])
        {
            return 0;
        }
    }

    int NewInodeIdx = bIsInodeFree._Find_first();
    int NewBlockIdx = bIsBlockFree._Find_first();
    if (NewInodeIdx == INODE_NUMBER || NewBlockIdx == BLOCK_NUMBER)
        return 0;

    for (int i = 0; i < ENTRY_NUMBER; i++)
    {
        if (Block->InodeID[i] == 0)
        {
            Block->InodeID[i] = NewInodeIdx;
            strcpy(Block->FileName[i], epath.back().data());

            bIsInodeFree.reset(NewInodeIdx);
            bIsBlockFree.reset(NewBlockIdx);

            if (bIsDir == false)
            {
                bRead.set(NewInodeIdx);
                bWrite.set(NewInodeIdx);
            }

            Inode &NewInode = InodeMemory[NewInodeIdx];
            NewInode.bIsDir = bIsDir;
            NewInode.BlockID = NewBlockIdx;

            return NewInodeIdx;
        }
    }

    return 0;
}

void FileSystem::CreateDirectory(string path)
{
    Create_k2(path, true);
}

void FileSystem::CreateFile(string path)
{
    Create_k2(path, false);
}

void FileSystem::Delete(string path)
{
    vector<string> epath = ProcessDir(path);
    int InodeIdx = SearchParentInode(epath);
    if (InodeIdx == 0 && epath.size() != 1)
        return;

    int BlockIdx = InodeMemory[InodeIdx].BlockID;
    DirectoryBlock *Block = reinterpret_cast<DirectoryBlock *>(&BlockMemory[BlockIdx]);

    int InodeIdx2 = GetInodeFromDir(epath.back(), InodeIdx);
    if (InodeIdx2 == 0)
        return;

    Delete_k2(InodeIdx2);
    for (int i = 0; i < ENTRY_NUMBER; i++)
    {
        string fileName = string(Block->FileName[i]);
        if (fileName == epath.back() && Block->InodeID[i])
        {
            Block->InodeID[i] = 0;
            memset(&(Block->FileName[i]), 0, MAX_FILENAME_LENGTH);
        }
    }
}

vector<string> FileSystem::List(string path)
{
    vector<string> res;
    vector<string> epath = ProcessDir(path);

    int BlockIdx;
    if (epath.size() == 0)
    {
        BlockIdx = 0;
    }
    else
    {
        int _InodeIdx = SearchParentInode(epath);
        if (_InodeIdx == 0 && epath.size() != 1)
            return res;

        int InodeIdx = GetInodeFromDir(epath.back(), _InodeIdx);
        if (InodeIdx == 0 || InodeMemory[InodeIdx].bIsDir == false)
            return res;

        BlockIdx = InodeMemory[InodeIdx].BlockID;
    }
    DirectoryBlock *Block = reinterpret_cast<DirectoryBlock *>(&BlockMemory[BlockIdx]);

    for (int i = 0; i < ENTRY_NUMBER; i++)
    {
        if (Block->InodeID[i])
        {
            res.push_back(string(Block->FileName[i]));
        }
    }

    return res;
}

void FileSystem::Copy(string sourcePath, string targetDir)
{
    vector<string> srcPath = ProcessDir(sourcePath);
    int _InodeIdx = SearchParentInode(srcPath);
    if (_InodeIdx == 0 && srcPath.size() != 1)
        return;

    int InodeIdx = GetInodeFromDir(srcPath.back(), _InodeIdx);
    if (InodeIdx == 0)
        return;

    string newPath = targetDir + "/" + srcPath.back();
    if (InodeMemory[InodeIdx].bIsDir)
    {
        CreateDirectory(newPath);
        vector<string> list = List(sourcePath);
        for (int i = 0; i < list.size(); i++)
            Copy(sourcePath + "/" + list[i], newPath);
    }
    else
    {
        CreateFile(newPath);
        string content = ReadFile(sourcePath);
        WriteFile(newPath, content);
    }
}

string FileSystem::ReadFile(string path)
{
    vector<string> epath = ProcessDir(path);
    int _InodeIdx = SearchParentInode(epath);
    if (_InodeIdx == 0 && epath.size() != 1)
        return "";

    int InodeIdx = GetInodeFromDir(epath.back(), _InodeIdx);
    if (InodeIdx == 0 || InodeMemory[InodeIdx].bIsDir || bRead[InodeIdx] == 0)
        return "";

    int BlockIdx = InodeMemory[InodeIdx].BlockID;
    FileBlock *Block = reinterpret_cast<FileBlock *>(&BlockMemory[BlockIdx]);

    return string(Block->Content);
}

void FileSystem::WriteFile(string path, string content)
{
    if (content.size() >= BLOCK_SIZE)
        return;

    vector<string> epath = ProcessDir(path);
    int _InodeIdx = SearchParentInode(epath);
    if (_InodeIdx == 0 && epath.size() != 1)
        return;

    int InodeIdx = GetInodeFromDir(epath.back(), _InodeIdx);
    if (InodeIdx == 0 || InodeMemory[InodeIdx].bIsDir || bWrite[InodeIdx] == 0)
        return;

    int BlockIdx = InodeMemory[InodeIdx].BlockID;
    FileBlock *Block = reinterpret_cast<FileBlock *>(&BlockMemory[BlockIdx]);

    strcpy(Block->Content, content.data());
}

void FileSystem::ChangeMode(std::string path, int mode)
{
    vector<string> epath = ProcessDir(path);
    int _InodeIdx = SearchParentInode(epath);
    if (_InodeIdx == 0 && epath.size() != 1)
        return;

    int InodeIdx = GetInodeFromDir(epath.back(), _InodeIdx);
    if (InodeIdx == 0 || InodeMemory[InodeIdx].bIsDir)
        return;

    if (mode & 1)
        bWrite.set(InodeIdx);
    else
        bWrite.reset(InodeIdx);

    if ((mode >> 1) & 1)
        bRead.set(InodeIdx);
    else
        bRead.reset(InodeIdx);
}

string FileSystem::GetFileName(string path)
{
    vector<string> epath = ProcessDir(path);
    if (epath.size() == 0)
        return "";
    return epath.back();
}