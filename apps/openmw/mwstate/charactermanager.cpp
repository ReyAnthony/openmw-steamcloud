#include "charactermanager.hpp"

#include <cctype>
#include <sstream>

#include <boost/filesystem.hpp>

#include <components/misc/utf8stream.hpp>

MWState::CharacterManager::CharacterManager (const boost::filesystem::path& saves,
    const std::vector<std::string>& contentFiles)
: mPath (saves), mCurrent (nullptr), mGame (getFirstGameFile(contentFiles))
{
    if (!boost::filesystem::is_directory (mPath))
    {
        boost::filesystem::create_directories (mPath);
    }
    else
    {
        //TODO migrate old saves with folders and .own ext
        for (boost::filesystem::directory_iterator iter(mPath);
                iter != boost::filesystem::directory_iterator(); ++iter)
        {
            boost::filesystem::path savePath = *iter;
            std::string characterSave = savePath.filename().string();

            if (!boost::filesystem::is_directory(savePath))
            {
                std::string currentPrefix;
                try
                {
                    currentPrefix = characterSave.substr(0, characterSave.find("."));
                }
                catch (...)
                {
                    continue;
                }

                const auto& prefixNotFoundYet = std::find(mPrefixes.begin(), mPrefixes.end(), currentPrefix) == mPrefixes.end();
                if (prefixNotFoundYet)
                {
                    Character character(mPath, currentPrefix, mGame);
                    mCharacters.push_back(character);

                    //TODO do not use prefixes and search directly in mCharacters since they have them too
                    mPrefixes.push_back(currentPrefix);
                }
            }          
        }
    }
}


MWState::Character *MWState::CharacterManager::getCurrentCharacter ()
{
    return mCurrent;
}

void MWState::CharacterManager::deleteSlot(const MWState::Character *character, const MWState::Slot *slot)
{
    std::list<Character>::iterator it = findCharacter(character);

    it->deleteSlot(slot);

    if (character->begin() == character->end())
    {
        // All slots deleted, cleanup and remove this character
        it->cleanup();
        mPrefixes.remove(it->getPrefix());

        if (character == mCurrent)
            mCurrent = nullptr;
        mCharacters.erase(it);
      
    }
}

MWState::Character* MWState::CharacterManager::createCharacter(const std::string& name)
{
    std::string finalName = name;
    int i = 0;

    //Add index to the name if already existing
    while (std::find(mPrefixes.begin(), mPrefixes.end(), finalName) != mPrefixes.end())
    {
        std::ostringstream test;
        test << name << ++i;
        finalName = test.str();
    }

    mCharacters.emplace_back(mPath, finalName, mGame);
    return &mCharacters.back();
}

std::list<MWState::Character>::iterator MWState::CharacterManager::findCharacter(const MWState::Character* character)
{
    std::list<Character>::iterator it = mCharacters.begin();
    for (; it != mCharacters.end(); ++it)
    {
        if (&*it == character)
            break;
    }
    if (it == mCharacters.end())
        throw std::logic_error ("invalid character");
    return it;
}

void MWState::CharacterManager::setCurrentCharacter (const Character *character)
{
    if (!character)
        mCurrent = nullptr;
    else
    {
        std::list<Character>::iterator it = findCharacter(character);

        mCurrent = &*it;
    }
}


std::list<MWState::Character>::const_iterator MWState::CharacterManager::begin() const
{
    return mCharacters.begin();
}

std::list<MWState::Character>::const_iterator MWState::CharacterManager::end() const
{
    return mCharacters.end();
}
