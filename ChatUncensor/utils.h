#pragma once

#include <format>
#include <string>
#include <vector>
#include <unordered_map>
#include <Windows.h>

// hippity hoppity i copyti this from https://github.com/JulienML/BetterChat/commit/dedb44e8e543eb4b40e404257b4ef123404617f1?diff=split#diff-c2a4ceabc49cd85b048435a7850490d7fab95a64ffc33a813b02b6eebf8b14b4
// gracie mille
// And I copied this from https://github.com/0xleft/trnslt cheers pal

std::wstring utf8ToWstring(const std::string& str);
std::string wToString(wchar_t* string);
std::string toLower(std::string str);

struct FString
{
public:
    using ElementType = const wchar_t;
    using ElementPointer = ElementType*;

private:
    ElementPointer ArrayData;
    int32_t ArrayCount;
    int32_t ArrayMax;

public:
    FString()
    {
        ArrayData = nullptr;
        ArrayCount = 0;
        ArrayMax = 0;
    }

    FString(ElementPointer other)
    {
        ArrayData = nullptr;
        ArrayCount = 0;
        ArrayMax = 0;

        ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

        if (ArrayCount > 0)
        {
            ArrayData = other;
        }
    }

    ~FString() {}

public:
    std::wstring ToWideString() const
    {
        if (!IsValid())
        {
            return std::wstring(ArrayData);
        }
        return std::wstring();
    }

    std::string ToString() const
    {
        if (!IsValid())
        {
            std::wstring wideStr(ArrayData);
            std::string str(wideStr.begin(), wideStr.end());
            return str;
        }

        return std::string();
    }

    bool IsValid() const
    {
        return !ArrayData;
    }

    FString operator=(ElementPointer other)
    {
        if (ArrayData != other)
        {
            ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

            if (ArrayCount > 0)
            {
                ArrayData = other;
            }
        }

        return *this;
    }

    // Added this as an easier way to "invalidate" / null out an FString, which is essentially the same thing
    // as creating a new FS(""), but without the memory leak
    // Credit to soup for this method!
    void Clear() { ArrayData = nullptr; ArrayCount = 0; ArrayMax = 0; }

    bool operator==(const FString& other)
    {
        return (!wcscmp(ArrayData, other.ArrayData));
    }
};

FString FS(const std::string& s);

struct FSceNpOnlineId
{
    uint64_t                                           Data[0x2];                                        		// 0x0000 (0x0010) [0x0000000000000000]               
    uint8_t                                            Term;                                             		// 0x0010 (0x0001) [0x0000000000000000]               
    uint8_t                                            Dummy[0x3];                                       		// 0x0011 (0x0003) [0x0000000000000000]               
};
struct FSceNpId
{
    struct FSceNpOnlineId                              Handle;                                           		// 0x0000 (0x0018) [0x0000000000000002] (CPF_Const)   
    uint64_t                                           Opt;                                              		// 0x0018 (0x0008) [0x0000000000000002] (CPF_Const)   
    uint64_t                                           Reserved;                                         		// 0x0020 (0x0008) [0x0000000000000002] (CPF_Const)   
};
struct FUniqueNetId
{
    uint64_t                                           Uid;                                              		// 0x0000 (0x0008) [0x0000000000000000]               
    struct FSceNpId                                    NpId;                                             		// 0x0008 (0x0028) [0x0000000000000000]               
    struct FString                                     EpicAccountId;                                    		// 0x0030 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
    uint8_t                                            Platform;                                         		// 0x0040 (0x0001) [0x0000000000000000]               
    uint8_t                                            SplitscreenID;                                    		// 0x0041 (0x0001) [0x0000000000000000]               
};
struct FGFxChatMessage {
    int32_t Team;
    class FString PlayerName;
    class FString Message;
    uint8_t ChatChannel;
    bool bLocalPlayer : 1;
    struct FUniqueNetId SenderID;
    uint8_t MessageType;
    class FString TimeStamp;
};

struct ChatMessage1
{
    void* PRI;
    void* Team;
    wchar_t* PlayerName;
    uint8_t PlayerNamePadding[0x8];
    wchar_t* Message;
    uint8_t MessagePadding[0x8];
    uint8_t ChatChannel;
    unsigned long bPreset : 1;
};

struct LogMessage {
    std::string Message;
    uint8_t ChatChannel;
    std::string PlayerName;
    uint8_t Team = 0;
    std::string TimeStamp = "0:00";
};