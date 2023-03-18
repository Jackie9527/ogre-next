#include "OHOS/OgreOHOSLogListener.h"


#define LOGI( ... ) 
#define LOGE( ... ) 

namespace Ogre
{
    OHOSLogListener::OHOSLogListener() {}

    void OHOSLogListener::messageLogged( const String &message, LogMessageLevel lml, bool maskDebug,
                                            const String &logName, bool &skipThisMessage )
    {
        if( lml < Ogre::LML_CRITICAL )
        {
            LOGI( "%s", message.c_str() );
        }
        else
        {
            LOGE( "%s", message.c_str() );
        }
    }
}  // namespace Ogre
