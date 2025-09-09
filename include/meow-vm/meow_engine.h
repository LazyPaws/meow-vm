#pragma once

struct ObjBytes;
struct ObjString;
struct ObjArray;
struct ObjHash;

struct MeowEngine {
    virtual ~MeowEngine() = default;

    virtual ObjBytes* newBytes() = 0;
    virtual ObjBytes* newString() = 0;
    virtual ObjBytes* newArray() = 0;
    virtual ObjBytes* newObject() = 0;
};