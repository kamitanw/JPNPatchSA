#include "JPNPatchSA.h"
#include <Windows.h>
#include <array>
#include <string>
#include <iomanip>

//plugin-sdkから流用
GameVersion GetGameVersion()
{
    const int val = *reinterpret_cast<unsigned int*>(0x401000);
    if (val == 0x53EC8B55)
        return GameVersion::v10US_COMPACT;
    if (val == 0x16197BE9)
        return GameVersion::v10US_HOODLUM;
    if (val == 0x94BF)
        return GameVersion::v10EU;

    if (*reinterpret_cast<unsigned int*>(0x8252FC) == 0x94BF)
        return GameVersion::v11US;
    if (*reinterpret_cast<unsigned int*>(0x82533C) == 0x94BF)
        return GameVersion::v11EU;
    if (*reinterpret_cast<unsigned int*>(0x858D51) == 0x3539F633)
        return GameVersion::STEAM;
    if (*reinterpret_cast<unsigned int*>(0x858C61) == 0x3539F633)
        return GameVersion::STEAM_LV;

    return GameVersion::UNKNOWN;
}

std::wstring GetGameVersionName(GameVersion version)
{
    switch (version) {
    case GameVersion::v10US_COMPACT:
        return L"1.0 US 'Compact'";
    case GameVersion::v10US_HOODLUM:
        return L"1.0 US 'HoodLum'";
    case GameVersion::v10EU:
        return L"1.0 EU";
    case GameVersion::v11US:
        return L"1.01 US";
    case GameVersion::v11EU:
        return L"1.01 EU";
    case GameVersion::STEAM:
        return L"新Steam R2";
    case GameVersion::STEAM_LV:
        return L"新Steam R2-LV'";
    default:
        return L"不明";
    }
}

void ShowErrorBox(GameVersion gameVersion)
{
    const auto message = L"現在起動しているバージョンでは日本語化できません。\n"
        L"v1.0(Compact以外)かv1.01のgta_sa.exeで起動してください。\n"
        L"現在起動中のexeのバージョン: " + GetGameVersionName(gameVersion);

    MessageBox(
        nullptr,
        message.c_str(),
        L"日本語化非対応のバージョンです",
        MB_OK | MB_ICONSTOP);
}

void Patch(void* targetAddress, void const* valueAddress, size_t valueSize)
{
    DWORD dwProtect[2];

    VirtualProtect(targetAddress, valueSize, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
    memcpy(targetAddress, valueAddress, valueSize);
    VirtualProtect(targetAddress, valueSize, dwProtect[0], &dwProtect[1]);
}

void Main()
{
#pragma region Instructions 
#pragma warning(push)
#pragma warning(disable:4309)
    constexpr std::array<char, 5> instrForCFont_PrintChar1 = { 0xE9, 0x9A, 0xE3, 0x13, 0x00 };
    constexpr std::array<char, 5> instrForCFont_PrintChar2 = { 0xE9, 0xA8, 0xE3, 0x13, 0x00 };
    constexpr std::array<char, 5> instrForCFont_RenderFontBuffer1 = { 0xE9, 0x50, 0xD4, 0x13, 0x00 };
    constexpr std::array<char, 4> instrForCFont_RenderFontBuffer2 = { 0x89, 0x4C, 0x24, 0x10 };
    constexpr std::array<char, 5> instrForCFont_RenderFontBuffer3 = { 0xE9, 0x7E, 0xD4, 0x13, 0x00 };
    constexpr std::array<char, 5> instrFor719750 = { 0xE9, 0x89, 0xD7, 0x13, 0x00 };
    constexpr std::array<char, 5> instrFor71A150 = { 0xE9, 0xFB, 0xCD, 0x13, 0x00 };
    constexpr std::array<char, 398> instrFor856E00 = { 0x31, 0xC9, 0x8A, 0x0D, 0xC9, 0x1A, 0xC7, 0x00, 0x31, 0xC0, 0x8A, 0x03, 0x3C, 0x81, 0x72, 0x0C, 0x3C, 0x9F, 0x76, 0x37,
        0x3C, 0xE0, 0x72, 0x04, 0x3C, 0xFC, 0x76, 0x2F, 0xC6, 0x05, 0xE0, 0xDF, 0xC9, 0x00, 0x00, 0x52, 0xBA, 0xF0, 0xDF, 0xC9, 0x00, 0xC7, 0x02, 0x00, 0x00, 0x80, 0x3C,
        0xBA, 0xF4, 0xDF, 0xC9, 0x00, 0xC7, 0x02, 0x00, 0x00, 0x20, 0x3D, 0xBA, 0xFC, 0xDF, 0xC9, 0x00, 0xC7, 0x02, 0x00, 0x00, 0xA4, 0x3F, 0x5A, 0xE9, 0x68, 0x2B, 0xEC, 0xFF,
        0x88, 0xC4, 0x8A, 0x43, 0x01, 0x3C, 0x40, 0x72, 0xC8, 0x3C, 0xFC, 0x77, 0xC4, 0x80, 0xFC, 0x98, 0x72, 0x06, 0x77, 0x18, 0x3C, 0x72, 0x77, 0x14, 0x80, 0xEC, 0x81,
        0xD0, 0xE4, 0x3C, 0xA0, 0x73, 0x04, 0x2C, 0x20, 0xEB, 0x0B, 0x80, 0xC4, 0x01, 0x2C, 0x80, 0xEB, 0x04, 0xB4, 0x00, 0xB0, 0x37, 0x43, 0xC6, 0x05, 0xE0, 0xDF, 0xC9, 0x00, 0x01,
        0x52, 0xBA, 0xF0, 0xDF, 0xC9, 0x00, 0xC7, 0x02, 0x00, 0x00, 0x00, 0x3C, 0xBA, 0xF4, 0xDF, 0xC9, 0x00, 0xC7, 0x02, 0x00, 0x00, 0xA0, 0x3C, 0xBA, 0xFC, 0xDF, 0xC9, 0x00,
        0xC7, 0x02, 0x00, 0x00, 0x94, 0x3F, 0x5A, 0x89, 0xC1, 0xE9, 0x20, 0x2B, 0xEC, 0xFF, 0x00, 0x00, 0x00, 0x80, 0x3D, 0xE0, 0xDF, 0xC9, 0x00, 0x01, 0x75, 0x15, 0x83, 0xEC, 0x04,
        0xB8, 0x20, 0x00, 0x00, 0x00, 0x89, 0x44, 0x24, 0x04, 0xDB, 0x44, 0x24, 0x04, 0x83, 0xC4, 0x04, 0xEB, 0x05, 0xE8, 0x9D, 0x18, 0xEC, 0xFF, 0xE9, 0x3E, 0x1C, 0xEC, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3D, 0xE0, 0xDF, 0xC9, 0x00, 0x01, 0x75, 0x15, 0x8B, 0x5C, 0x24, 0x30, 0x89, 0xDA, 0xC1, 0xEA, 0x08, 0x89, 0xD8,
        0x25, 0xFF, 0x00, 0x00, 0x00, 0xE9, 0x41, 0x1C, 0xEC, 0xFF, 0x88, 0xD9, 0xC0, 0xE9, 0x04, 0xE9, 0x30, 0x1C, 0xEC, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80, 0x3D, 0xE0, 0xDF, 0xC9, 0x00, 0x01, 0x75, 0x07, 0xB8, 0x20, 0x00, 0x00, 0x00, 0xEB, 0x08, 0x0F, 0xB6, 0x84, 0x07, 0xB0, 0x18, 0xC7, 0x00, 0xE9, 0x68, 0x2B, 0xEC, 0xFF,
        0x00, 0x00, 0x00, 0x80, 0x3D, 0xE0, 0xDF, 0xC9, 0x00, 0x01, 0x75, 0x07, 0xB8, 0x20, 0x00, 0x00, 0x00, 0xEB, 0x08, 0x0F, 0xB6, 0x84, 0x11, 0xB0, 0x18, 0xC7, 0x00,
        0xE9, 0x5D, 0x28, 0xEC, 0xFF, 0x00, 0x00, 0x00, 0x8A, 0x06, 0x3C, 0x81, 0x72, 0x0C, 0x3C, 0x9F, 0x76, 0x11, 0x3C, 0xE0, 0x72, 0x04, 0x3C, 0xFC, 0x76, 0x09,
        0xC6, 0x05, 0xE0, 0xDF, 0xC9, 0x00, 0x00, 0xEB, 0x15, 0x8A, 0x66, 0x01, 0x80, 0xFC, 0x40, 0x72, 0xEF, 0x80, 0xFC, 0xFC, 0x77, 0xEA,
        0xC6, 0x05, 0xE0, 0xDF, 0xC9, 0x00, 0x01, 0x46, 0x3C, 0x20, 0x8A, 0x8C, 0x24, 0xA8, 0x01, 0x00, 0x00, 0xE9, 0xCD, 0x31, 0xEC, 0xFF };

    constexpr std::array<char, 6> instrForFMulDWordC9DFF0 = { 0xD8, 0x0D, 0xF0, 0xDF, 0xC9, 0x00 };
    constexpr std::array<char, 6> instrForFCompDWordC9DFF0 = { 0xD8, 0x1D, 0xF0, 0xDF, 0xC9, 0x00 };
    constexpr std::array<char, 6> instrForFAddDWordC9DFF0 = { 0xD8, 0x05, 0xF0, 0xDF, 0xC9, 0x00 };
    constexpr std::array<char, 6> instrForFMulDWordC9DFF4 = { 0xD8, 0x0D, 0xF4, 0xDF, 0xC9, 0x00 };
    constexpr std::array<char, 6> instrForFAddDWordC9DFF4 = { 0xD8, 0x05, 0xF4, 0xDF, 0xC9, 0x00 };
    constexpr std::array<char, 6> instrForFMulQWordC9DFF8 = { 0xDC, 0x0D, 0xF8, 0xDF, 0xC9, 0x00 };
#pragma warning(pop)

    //0x6D40AB(GetFiringRateMultiplier)と0x6D40C7(GetFiringRateMultiplier)と0x6D40FB+0x6D4117(GetPlaneGunsRateOfFire)と
    //0x6D45AB+0x6D45C7(GetPlaneOrdnanceRateOfFire)と0x6EFCF6+0x6EFD22+0x6EFD51(RenderWater?)は書き換える必要がないので除外済み
    //日本語用の表示に関係ないことが確認できたアドレスは除外済み
    //callやpushなどがあったアドレスの書き換えはゲームが落ちる原因となったのでそれらのアドレスはすべて除外済み
    constexpr std::array<int, 3> AddressesForFMulDWordC9DFF0 = { 0x718B5A, 0x718B74, 0x718BE9 };
    constexpr std::array<int, 3> AddressesForFCompDWordC9DFF0 = { 0x685404, 0x685462, 0x6A6FD9 };
    constexpr std::array<int, 4> AddressesForFAddDWordC9DFF0 = { 0x718BD7, 0x718CB6, 0x718D42, 0x718DE0 };
    constexpr std::array<int, 4> AddressesForFMulDWordC9DFF4 = { 0x5765C0, 0x576689, 0x57672F, 0x5767F8 };
    constexpr std::array<int, 3> AddressesForFAddDWordC9DFF4 = { 0x718CAC, 0x718D2A, 0x718DD6 };
    constexpr int AddressForFMulQWordC9DFF8 = 0x718C37;
#pragma endregion

    const auto version = GetGameVersion();
    //正常に日本語化できない（と思われる）バージョンでは日本語化できない旨を知らせてexeの起動を止める
    if (version == GameVersion::v10US_COMPACT || version == GameVersion::STEAM || version == GameVersion::STEAM_LV || version == GameVersion::UNKNOWN)
    {
        ShowErrorBox(version);
        ExitProcess(0);
    }

    Patch(reinterpret_cast<void*>(0x7199AB), instrForCFont_RenderFontBuffer1.data(), 0x5);
    Patch(reinterpret_cast<void*>(0x718B11), instrForCFont_PrintChar1.data(), 0x5);
    Patch(reinterpret_cast<void*>(0x718B33), instrForCFont_PrintChar2.data(), 0x5);
    Patch(reinterpret_cast<void*>(0x719A8D), instrForCFont_RenderFontBuffer3.data(), 0x5);
    Patch(reinterpret_cast<void*>(0x7197A2), instrFor719750.data(), 0x5);
    Patch(reinterpret_cast<void*>(0x71A150), instrFor71A150.data(), 0x5);
    Patch(reinterpret_cast<void*>(0x7199CD), instrForCFont_RenderFontBuffer2.data(), 0x4);
    Patch(reinterpret_cast<void*>(0x856E00), instrFor856E00.data(), 0x18E);

    for (auto address : AddressesForFMulDWordC9DFF0)
    {
        if (address != 0)
        {
            Patch(reinterpret_cast<void*>(address), instrForFMulDWordC9DFF0.data(), 0x6);
        }
    }

    for (auto address : AddressesForFCompDWordC9DFF0)
    {
        if (address != 0)
        {
            Patch(reinterpret_cast<void*>(address), instrForFCompDWordC9DFF0.data(), 0x6);
        }
    }

    //Pricedownで','と'.'が出るようにする
    auto comp = static_cast<unsigned char>(0xFF);
    Patch(reinterpret_cast<void*>(0x718C6E), &comp, 0x1);

    for (auto address : AddressesForFAddDWordC9DFF0)
    {
        if (address != 0)
        {
            Patch(reinterpret_cast<void*>(address), instrForFAddDWordC9DFF0.data(), 0x6);
        }
    }

    for (auto address : AddressesForFMulDWordC9DFF4)
    {
        if (address != 0)
        {
            Patch(reinterpret_cast<void*>(address), instrForFMulDWordC9DFF4.data(), 0x6);
        }
    }

    for (auto address : AddressesForFAddDWordC9DFF4)
    {
        if (address != 0)
        {
            Patch(reinterpret_cast<void*>(address), instrForFAddDWordC9DFF4.data(), 0x6);
        }
    }

    Patch(reinterpret_cast<void*>(AddressForFMulQWordC9DFF8), instrForFMulQWordC9DFF8.data(), 0x6);
}
