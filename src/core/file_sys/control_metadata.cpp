// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/string_util.h"
#include "common/swap.h"
#include "core/file_sys/control_metadata.h"

namespace FileSys {

const std::array<const char*, 15> LANGUAGE_NAMES{{
    "AmericanEnglish",
    "BritishEnglish",
    "Japanese",
    "French",
    "German",
    "LatinAmericanSpanish",
    "Spanish",
    "Italian",
    "Dutch",
    "CanadianFrench",
    "Portuguese",
    "Russian",
    "Korean",
    "Taiwanese",
    "Chinese",
}};

std::string LanguageEntry::GetApplicationName() const {
    return Common::StringFromFixedZeroTerminatedBuffer(application_name.data(),
                                                       application_name.size());
}

std::string LanguageEntry::GetDeveloperName() const {
    return Common::StringFromFixedZeroTerminatedBuffer(developer_name.data(),
                                                       developer_name.size());
}

NACP::NACP() = default;

NACP::NACP(VirtualFile file) {
    file->ReadObject(&raw);
}

NACP::~NACP() = default;

const LanguageEntry& NACP::GetLanguageEntry(Language language) const {
    if (language != Language::Default) {
        return raw.language_entries.at(static_cast<u8>(language));
    }

    for (const auto& language_entry : raw.language_entries) {
        if (!language_entry.GetApplicationName().empty())
            return language_entry;
    }

    // Fallback to English
    return GetLanguageEntry(Language::AmericanEnglish);
}

std::string NACP::GetApplicationName(Language language) const {
    return GetLanguageEntry(language).GetApplicationName();
}

std::string NACP::GetDeveloperName(Language language) const {
    return GetLanguageEntry(language).GetDeveloperName();
}

u64 NACP::GetTitleId() const {
    return raw.save_data_owner_id;
}

u64 NACP::GetDLCBaseTitleId() const {
    return raw.dlc_base_title_id;
}

std::string NACP::GetVersionString() const {
    return Common::StringFromFixedZeroTerminatedBuffer(raw.version_string.data(),
                                                       raw.version_string.size());
}

u64 NACP::GetDefaultNormalSaveSize() const {
    return raw.user_account_save_data_size;
}

u64 NACP::GetDefaultJournalSaveSize() const {
    return raw.user_account_sava_data_journal_size;
}

std::vector<u8> NACP::GetRawBytes() const {
    std::vector<u8> out(sizeof(RawNACP));
    std::memcpy(out.data(), &raw, sizeof(RawNACP));
    return out;
}
} // namespace FileSys
