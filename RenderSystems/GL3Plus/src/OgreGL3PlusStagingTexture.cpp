/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2017 Torus Knot Software Ltd

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

#include "OgreGL3PlusStagingTexture.h"
#include "OgreGL3PlusTextureGpu.h"
#include "OgreGL3PlusMappings.h"

#include "Vao/OgreGL3PlusVaoManager.h"

#include "OgrePixelFormatGpuUtils.h"

namespace Ogre
{
    GL3PlusStagingTexture::GL3PlusStagingTexture( size_t internalBufferStart,
                                                  GL3PlusDynamicBuffer *dynamicBuffer,
                                                  VaoManager *vaoManager, size_t size ) :
        StagingTextureBufferImpl( vaoManager, size ),
        mInternalBufferStart( internalBufferStart ),
        mDynamicBuffer( dynamicBuffer ),
        mUnmapTicket( std::numeric_limits<size_t>::max() ),
        mMappedPtr( 0 )
    {
        const bool canPersistentMap = static_cast<GL3PlusVaoManager*>( mVaoManager )->
                supportsArbBufferStorage();

        if( canPersistentMap )
            mMappedPtr = mDynamicBuffer->map( mInternalBufferStart, mSize, mUnmapTicket );
    }
    //-----------------------------------------------------------------------------------
    GL3PlusStagingTexture::~GL3PlusStagingTexture()
    {
        if( mUnmapTicket != std::numeric_limits<size_t>::max() )
        {
            mDynamicBuffer->unmap( mUnmapTicket );
            mUnmapTicket = std::numeric_limits<size_t>::max();
            mMappedPtr = 0;
        }
    }
    //-----------------------------------------------------------------------------------
    bool GL3PlusStagingTexture::belongsToUs( const TextureBox &box )
    {
        return box.data >= mMappedPtr && box.data <= static_cast<uint8*>( mMappedPtr ) + mCurrentOffset;
    }
    //-----------------------------------------------------------------------------------
    DECL_MALLOC void* GL3PlusStagingTexture::mapRegionImpl(void)
    {
        return static_cast<uint8*>( mMappedPtr ) + mCurrentOffset;
    }
    //-----------------------------------------------------------------------------------
    void GL3PlusStagingTexture::startMapRegion(void)
    {
        const bool canPersistentMap = static_cast<GL3PlusVaoManager*>( mVaoManager )->
                supportsArbBufferStorage();

        if( !canPersistentMap )
            mMappedPtr = mDynamicBuffer->map( mInternalBufferStart, mSize, mUnmapTicket );
    }
    //-----------------------------------------------------------------------------------
    void GL3PlusStagingTexture::stopMapRegion(void)
    {
        const bool canPersistentMap = static_cast<GL3PlusVaoManager*>( mVaoManager )->
                supportsArbBufferStorage();

        if( !canPersistentMap )
        {
            mDynamicBuffer->unmap( mUnmapTicket );
            mUnmapTicket = std::numeric_limits<size_t>::max();
            mMappedPtr = 0;
        }
    }
    //-----------------------------------------------------------------------------------
    void GL3PlusStagingTexture::uploadCubemap( const TextureBox &srcBox, PixelFormatGpu pixelFormat,
                                               GLuint texName, uint8 mipLevel,
                                               GLenum format, GLenum type,
                                               GLint xPos, GLint yPos, GLint slicePos,
                                               GLsizei width, GLsizei height, GLsizei numSlices )
    {
        const size_t distToStart = static_cast<uint8*>( srcBox.data ) -
                                    static_cast<uint8*>( mMappedPtr );
        uint8 *offsetPtr = reinterpret_cast<uint8*>( mInternalBufferStart + distToStart );

        const GLsizei sizeBytes = static_cast<GLsizei>(
                PixelFormatGpuUtils::getSizeBytes( srcBox.width, srcBox.height, 1u, 1u, pixelFormat ) );

        for( size_t i=0; i<numSlices; ++i )
        {
            const GLenum targetGl = static_cast<GLenum>( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i + slicePos );
            OCGE( glBindTexture( targetGl, texName ) );
            if( type != GL_NONE )
            {
                OCGE( glTexSubImage2D( targetGl, mipLevel, xPos, yPos, width, height,
                                       format, type, offsetPtr ) );
            }
            else
            {
                OCGE( glCompressedTexSubImage2D( targetGl, mipLevel, xPos, yPos, width, height,
                                                 format, sizeBytes, offsetPtr ) );
            }
            offsetPtr += srcBox.bytesPerImage;
        }
    }
    //-----------------------------------------------------------------------------------
    void GL3PlusStagingTexture::upload( const TextureBox &srcBox, TextureGpu *dstTexture,
                                        uint8 mipLevel, const TextureBox *dstBox )
    {
        size_t bytesPerPixel = PixelFormatGpuUtils::getBytesPerPixel( dstTexture->getPixelFormat() );

        assert(( !srcBox.bytesPerRow || (srcBox.bytesPerImage % srcBox.bytesPerRow)) &&
               "srcBox.bytesPerImage must be a multiple of srcBox.bytesPerRow!" );

        OCGE( glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
        if( bytesPerPixel > 0 )
            OCGE( glPixelStorei( GL_UNPACK_ROW_LENGTH, srcBox.bytesPerRow / bytesPerPixel ) );
        if( srcBox.bytesPerRow > 0 )
            OCGE( glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, srcBox.bytesPerImage / srcBox.bytesPerRow ) );

        const TextureTypes::TextureTypes textureType = dstTexture->getTextureTypes();
        const PixelFormatGpu pixelFormat = dstTexture->getPixelFormat();

        assert( dynamic_cast<GL3PlusTextureGpu*>( dstTexture ) );
        GL3PlusTextureGpu *dstTextureGl = static_cast<GL3PlusTextureGpu*>( dstTexture );

        const GLenum targetGl   = dstTextureGl->getGlTextureTarget();
        const GLuint texName    = dstTextureGl->getGlTextureName();

        if( textureType != TextureTypes::TypeCube )
            OCGE( glBindTexture( targetGl, texName ) );

        OCGE( glBindBuffer( GL_PIXEL_UNPACK_BUFFER, mDynamicBuffer->getVboName() ) );

        GLint xPos      = static_cast<GLint>( dstBox ? dstBox->x : 0 );
        GLint yPos      = static_cast<GLint>( dstBox ? dstBox->y : 0 );
        GLint zPos      = static_cast<GLint>( dstBox ? dstBox->z : 0 );
        GLint slicePos  = static_cast<GLint>( dstBox ? dstBox->sliceStart : 0 );
        GLsizei width   = static_cast<GLsizei>( dstBox ? dstBox->width  : dstTexture->getWidth() );
        GLsizei height  = static_cast<GLsizei>( dstBox ? dstBox->height : dstTexture->getHeight() );
        GLsizei depth   = static_cast<GLsizei>( dstBox ? dstBox->depth  : dstTexture->getDepth() );
        GLsizei numSlices=static_cast<GLsizei>( dstBox ? dstBox->numSlices :
                                                         dstTexture->getNumSlices() );

        const size_t distToStart = static_cast<uint8*>( srcBox.data ) -
                                    static_cast<uint8*>( mMappedPtr );
        const void *offsetPtr = reinterpret_cast<void*>( mInternalBufferStart + distToStart );

        if( !PixelFormatGpuUtils::isCompressed( pixelFormat ) )
        {
            GLenum format, type;
            GL3PlusMappings::getFormatAndType( pixelFormat, format, type );

            switch( textureType )
            {
            case TextureTypes::Unknown:
                OGRE_EXCEPT( Exception::ERR_INVALID_STATE, "Ogre should never hit this path",
                             "GL3PlusStagingTexture::upload" );
                break;
            case TextureTypes::Type1D:
                OCGE( glTexSubImage1D( targetGl, mipLevel, xPos, width,
                                       format, type, offsetPtr ) );
                break;
            case TextureTypes::Type1DArray:
                OCGE( glTexSubImage2D( targetGl, mipLevel, xPos, slicePos, width, numSlices,
                                       format, type, offsetPtr ) );
                break;
            case TextureTypes::Type2D:
                OCGE( glTexSubImage2D( targetGl, mipLevel, xPos, yPos, width, height,
                                       format, type, offsetPtr ) );
                break;
            case TextureTypes::TypeCube:
                uploadCubemap( srcBox, pixelFormat, texName, mipLevel, format, type,
                               xPos, yPos, slicePos, width, height, numSlices );
                break;
            case TextureTypes::Type2DArray:
            case TextureTypes::TypeCubeArray:
                OCGE( glTexSubImage3D( targetGl, mipLevel, xPos, yPos, slicePos,
                                       width, height, numSlices,
                                       format, type, offsetPtr ) );
                break;
            case TextureTypes::Type3D:
                OCGE( glTexSubImage3D( targetGl, mipLevel, xPos, yPos, zPos,
                                       width, height, depth,
                                       format, type, offsetPtr ) );
                break;
            }
        }
        else
        {
            GLenum format = GL3PlusMappings::get( pixelFormat );

            const GLsizei sizeBytes = static_cast<GLsizei>(
                    PixelFormatGpuUtils::getSizeBytes( srcBox.width, srcBox.height, srcBox.depth,
                                                       srcBox.numSlices, pixelFormat ) );

            switch( textureType )
            {
            case TextureTypes::Unknown:
                OGRE_EXCEPT( Exception::ERR_INVALID_STATE, "Ogre should never hit this path",
                             "GL3PlusStagingTexture::upload" );
                break;
            case TextureTypes::Type1D:
                OCGE( glCompressedTexSubImage1D( targetGl, mipLevel, xPos, width,
                                                 format, sizeBytes, offsetPtr ) );
                break;
            case TextureTypes::Type1DArray:
                OCGE( glCompressedTexSubImage2D( targetGl, mipLevel, xPos, slicePos, width, numSlices,
                                                 format, sizeBytes, offsetPtr ) );
                break;
            case TextureTypes::Type2D:
                OCGE( glCompressedTexSubImage2D( targetGl, mipLevel, xPos, yPos, width, height,
                                                 format, sizeBytes, offsetPtr ) );
                break;
            case TextureTypes::TypeCube:
                uploadCubemap( srcBox, pixelFormat, texName, mipLevel, format, GL_NONE,
                               xPos, yPos, slicePos, width, height, numSlices );
                break;
            case TextureTypes::Type2DArray:
            case TextureTypes::TypeCubeArray:
                OCGE( glCompressedTexSubImage3D( targetGl, mipLevel, xPos, yPos, slicePos,
                                                 width, height, numSlices,
                                                 format, sizeBytes, offsetPtr ) );
                break;
            case TextureTypes::Type3D:
                OCGE( glCompressedTexSubImage3D( targetGl, mipLevel, xPos, yPos, zPos,
                                                 width, height, depth,
                                                 format, sizeBytes, offsetPtr ) );
                break;
            }
        }

        OCGE( glBindTexture( targetGl, 0 ) );
        OCGE( glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 ) );
    }
}