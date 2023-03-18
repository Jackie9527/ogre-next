#include "OHOS/OgreHAPFileSystemArchive.h"

#include <OgreLogManager.h>
#include <OgreString.h>
#include <OgreStringConverter.h>

namespace Ogre
{
    static std::map<String, std::vector<String> > mFiles;

#if 0
    bool IsFolderParsed( String Folder )
    {
        bool parsed = false;
        std::map<String, std::vector<String> >::iterator iter = mFiles.find( Folder );
        if( iter != mFiles.end() )
            parsed = true;
        return parsed;
    }

    void ParseFolder( AAssetManager *AssetMgr, String Folder )
    {
        std::vector<String> mFilenames;
        AAssetDir *dir = AAssetManager_openDir( AssetMgr, Folder.c_str() );
        const char *fileName = NULL;
        while( ( fileName = AAssetDir_getNextFileName( dir ) ) != NULL )
        {
            mFilenames.push_back( String( fileName ) );
        }
        mFiles.insert( std::make_pair( Folder, mFilenames ) );
    }
#endif

    HAPFileSystemArchive::HAPFileSystemArchive( const String &name, const String &archType) :
        Archive( name, archType )
    {
#if 0
        if( mName.size() > 0 && mName[0] == '/' )
            mName.erase( mName.begin() );

        mPathPreFix = mName;
        if( mPathPreFix.size() > 0 )
            mPathPreFix += "/";

        if( !IsFolderParsed( mName ) )
        {
            ParseFolder( mAssetMgr, mName );
        }
#endif
    }

    HAPFileSystemArchive::~HAPFileSystemArchive()
    {
        std::map<String, std::vector<String> >::iterator iter = mFiles.find( mName );
        iter->second.clear();
        mFiles.erase( iter );
        unload();
    }

    bool HAPFileSystemArchive::isCaseSensitive() const { return true; }

    void HAPFileSystemArchive::load() {}

    void HAPFileSystemArchive::unload() {}

    DataStreamPtr HAPFileSystemArchive::open( const Ogre::String &filename, bool readOnly )
    {
#if 0
        DataStreamPtr stream;
        AAsset *asset =
            AAssetManager_open( mAssetMgr, ( mPathPreFix + filename ).c_str(), AASSET_MODE_BUFFER );
        if( asset )
        {
            const size_t length = static_cast<size_t>( AAsset_getLength( asset ) );
            void *membuf = OGRE_MALLOC( length, Ogre::MEMCATEGORY_GENERAL );
            memcpy( membuf, AAsset_getBuffer( asset ), length );
            AAsset_close( asset );

            stream = Ogre::DataStreamPtr( new Ogre::MemoryDataStream( membuf, length, true, true ) );
        }
        return stream;
#endif
        return nullptr;
    }

    DataStreamPtr HAPFileSystemArchive::create( const Ogre::String &filename )
    {
        return DataStreamPtr();
    }

    void HAPFileSystemArchive::remove( const String &filename ) {}

    StringVectorPtr HAPFileSystemArchive::list( bool recursive, bool dirs )
    {
        StringVectorPtr files( new StringVector );
        std::map<String, std::vector<String> >::iterator iter = mFiles.find( mName );
        std::vector<String> fileList = iter->second;
        for( size_t i = 0; i < fileList.size(); i++ )
        {
            files->push_back( fileList[i] );
        }
        return files;
    }

    FileInfoListPtr HAPFileSystemArchive::listFileInfo( bool recursive, bool dirs )
    {
        FileInfoListPtr files( new FileInfoList );
#if 0
        std::map<String, std::vector<String> >::iterator iter = mFiles.find( mName );
        std::vector<String> fileList = iter->second;
        for( size_t i = 0; i < fileList.size(); i++ )
        {
            AAsset *asset = AAssetManager_open( mAssetMgr, ( mPathPreFix + fileList[i] ).c_str(),
                                                AASSET_MODE_UNKNOWN );
            if( asset )
            {
                FileInfo info;
                info.archive = this;
                info.filename = fileList[i];
                info.path = mName;
                info.basename = fileList[i];
                info.compressedSize = static_cast<size_t>( AAsset_getLength( asset ) );
                info.uncompressedSize = info.compressedSize;
                files->push_back( info );
                AAsset_close( asset );
            }
        }
#endif
        return files;
    }

    StringVectorPtr HAPFileSystemArchive::find( const String &pattern, bool recursive, bool dirs )
    {
        StringVectorPtr files( new StringVector );
        std::map<String, std::vector<String> >::iterator iter = mFiles.find( mName );
        std::vector<String> fileList = iter->second;
        for( size_t i = 0; i < fileList.size(); i++ )
        {
            if( StringUtil::match( fileList[i], pattern ) )
                files->push_back( fileList[i] );
        }
        return files;
    }

    FileInfoListPtr HAPFileSystemArchive::findFileInfo( const String &pattern, bool recursive,
                                                        bool dirs )
    {
        FileInfoListPtr files( new FileInfoList );
#if 0
        std::map<String, std::vector<String> >::iterator iter = mFiles.find( mName );
        std::vector<String> fileList = iter->second;
        for( size_t i = 0; i < fileList.size(); i++ )
        {
            if( StringUtil::match( fileList[i], pattern ) )
            {
                AAsset *asset = AAssetManager_open( mAssetMgr, ( mPathPreFix + fileList[i] ).c_str(),
                                                    AASSET_MODE_UNKNOWN );
                if( asset )
                {
                    FileInfo info;
                    info.archive = this;
                    info.filename = fileList[i];
                    info.path = mName;
                    info.basename = fileList[i];
                    info.compressedSize = static_cast<size_t>( AAsset_getLength( asset ) );
                    info.uncompressedSize = info.compressedSize;
                    files->push_back( info );
                    AAsset_close( asset );
                }
            }
        }
#endif
        return files;
    }

    bool HAPFileSystemArchive::exists( const String &filename )
    {
#if 0
        AAsset *asset =
            AAssetManager_open( mAssetMgr, ( mPathPreFix + filename ).c_str(), AASSET_MODE_UNKNOWN );
        if( asset )
        {
            AAsset_close( asset );
            return true;
        }
#endif
        return false;
    }

    time_t HAPFileSystemArchive::getModifiedTime( const Ogre::String &filename ) { return 0; }

    //////////////////////////////////////////////////////////////////////////////

    const String &HAPFileSystemArchiveFactory::getType() const
    {
        static String type = "HAPFileSystem";
        return type;
    }

    void HAPFileSystemArchiveFactory::convertPath( String &inOutPath ) const
    {
        if( inOutPath.size() > 0 && inOutPath[0] == '/' )
            inOutPath.erase( inOutPath.begin() );
    }

}  // namespace Ogre
