#ifndef OPENMW_ESM_CREA_H
#define OPENMW_ESM_CREA_H

#include <array>
#include <string>

#include "aipackage.hpp"
#include "loadcont.hpp"
#include "spelllist.hpp"
#include "transport.hpp"

#include "components/esm/attr.hpp"
#include "components/esm/defs.hpp"
#include "components/esm/refid.hpp"

namespace ESM
{

    class ESMReader;
    class ESMWriter;

    /*
     * Creature definition
     *
     */

    struct Creature
    {
        constexpr static RecNameInts sRecordId = REC_CREA;

        /// Return a string descriptor for this record type. Currently used for debugging / error logs only.
        static std::string_view getRecordType() { return "Creature"; }

        // Default is 0x48?
        enum Flags
        {
            Bipedal = 0x01,
            Respawn = 0x02,
            Weapon = 0x04, // Has weapon and shield
            Base = 0x08, // This flag is set for every actor in Bethesda ESMs
            Swims = 0x10,
            Flies = 0x20, // Don't know what happens if several
            Walks = 0x40, // of these are set
            Essential = 0x80
        };

        enum Type
        {
            Creatures = 0,
            Daedra = 1,
            Undead = 2,
            Humanoid = 3
        };

        struct NPDTstruct
        {
            int32_t mType;
            // For creatures we obviously have to use ints, not shorts and
            // bytes like we use for NPCs.... this file format just makes so
            // much sense! (Still, _much_ easier to decode than the NIFs.)
            int32_t mLevel;
            std::array<int32_t, Attribute::Length> mAttributes;

            int32_t mHealth, mMana, mFatigue; // Stats
            int32_t mSoul; // The creatures soul value (used with soul gems.)
            // Creatures have generalized combat, magic and stealth stats which substitute for
            // the specific skills (in the same way as specializations).
            int32_t mCombat, mMagic, mStealth;
            int32_t mAttack[6]; // AttackMin1, AttackMax1, ditto2, ditto3
            int32_t mGold;
        }; // 96 byte

        NPDTstruct mData;

        int32_t mBloodType;
        unsigned char mFlags;

        float mScale;

        uint32_t mRecordFlags;
        RefId mId, mScript;
        std::string mModel;
        std::string mName;
        ESM::RefId mOriginal; // Base creature that this is a modification of

        InventoryList mInventory;
        SpellList mSpells;

        AIData mAiData;
        AIPackageList mAiPackage;
        Transport mTransport;

        const std::vector<Transport::Dest>& getTransport() const;

        void load(ESMReader& esm, bool& isDeleted);
        void save(ESMWriter& esm, bool isDeleted = false) const;

        void blank();
        ///< Set record to default state (does not touch the ID).
    };

}
#endif
