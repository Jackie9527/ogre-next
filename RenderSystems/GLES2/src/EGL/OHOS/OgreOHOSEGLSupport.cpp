/*
-----------------------------------------------------------------------------
This source file is part of OGRE-Next
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreRoot.h"

#include "OgreGLES2Prerequisites.h"
#include "OgreGLES2RenderSystem.h"

#include "OgreOHOSEGLSupport.h"
#include "OgreOHOSEGLWindow.h"
#include "OgreOHOSEGLContext.h"

namespace Ogre {

    OHOSEGLSupport::OHOSEGLSupport()
    {        
        mNativeDisplay = EGL_DEFAULT_DISPLAY;
        mGLDisplay = getGLDisplay();
        
        mCurrentMode.first.first = 1280; 
        mCurrentMode.first.second = 800; 
        mCurrentMode.second = 0;
        mOriginalMode = mCurrentMode;
        mVideoModes.push_back(mCurrentMode);

        ConfigOption optOrientation;
        optOrientation.name = "Orientation";
        optOrientation.immutable = false;
        optOrientation.possibleValues.push_back("Landscape");
        optOrientation.possibleValues.push_back("Portrait");
        optOrientation.currentValue = optOrientation.possibleValues[0];
        
        mOptions[optOrientation.name] = optOrientation;
    }

    OHOSEGLSupport::~OHOSEGLSupport()
    {
        
    }
    
    void OHOSEGLSupport::switchMode( uint& width, uint& height, short& frequency )
    {
        if (!mRandr)
            return;
    }
    
    RenderWindow* OHOSEGLSupport::newWindow( const String &name, unsigned int width, unsigned int height, bool fullScreen, const NameValuePairList *miscParams)
    {
        OHOSEGLWindow* window = new OHOSEGLWindow(this);
        window->create(name, width, height, fullScreen, miscParams);
        
        return window;
    }
}
