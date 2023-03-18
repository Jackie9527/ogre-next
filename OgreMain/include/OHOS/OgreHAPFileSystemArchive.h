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

#ifndef __HAPFileSystemArchive_H__
#define __HAPFileSystemArchive_H__

#include <OgreArchive.h>
#include <OgreArchiveFactory.h>


namespace Ogre
{
    class HAPFileSystemArchive : public Archive
    {
    private:

        String         mPathPreFix;

    public:
        HAPFileSystemArchive( const String &name, const String &archType);
        ~HAPFileSystemArchive();

        /// @copydoc Archive::isCaseSensitive
        bool isCaseSensitive() const;

        /// @copydoc Archive::load
        void load();
        /// @copydoc Archive::unload
        void unload();

        /// @copydoc Archive::open
        DataStreamPtr open( const String &filename, bool readOnly = true );

        /// @copydoc Archive::create
        DataStreamPtr create( const String &filename );

        /// @copydoc Archive::remove
        void remove( const String &filename );

        /// @copydoc Archive::list
        StringVectorPtr list( bool recursive = true, bool dirs = false );

        /// @copydoc Archive::listFileInfo
        FileInfoListPtr listFileInfo( bool recursive = true, bool dirs = false );

        /// @copydoc Archive::find
        StringVectorPtr find( const String &pattern, bool recursive = true, bool dirs = false );

        /// @copydoc Archive::findFileInfo
        FileInfoListPtr findFileInfo( const String &pattern, bool recursive = true, bool dirs = false );

        /// @copydoc Archive::exists
        bool exists( const String &filename );

        /// @copydoc Archive::getModifiedTime
        time_t getModifiedTime( const String &filename );
    };

    class HAPFileSystemArchiveFactory : public ArchiveFactory
    {
    public:
        HAPFileSystemArchiveFactory() {}
        virtual ~HAPFileSystemArchiveFactory() {}
        /// @copydoc FactoryObj::getType
        const String &getType() const;
        /// @copydoc FactoryObj::createInstance
        Archive *createInstance( const String &name, bool readOnly )
        {
            return OGRE_NEW HAPFileSystemArchive( name, getType());
        }
        /// @copydoc FactoryObj::destroyInstance
        void destroyInstance( Archive *arch ) { OGRE_DELETE arch; }

        virtual void convertPath( String &inOutPath ) const;

    };
}  // namespace Ogre

#endif
