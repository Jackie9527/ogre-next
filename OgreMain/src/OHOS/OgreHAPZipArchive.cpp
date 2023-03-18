#include "OHOS/OgreHAPZipArchive.h"

#include <OgreLogManager.h>
#include <OgreStringConverter.h>

namespace Ogre
{
    //-----------------------------------------------------------------------
    const String &HAPZipArchiveFactory::getType() const
    {
        static String type = "HAPZip";
        return type;
    }
    //-----------------------------------------------------------------------
}  // namespace Ogre
