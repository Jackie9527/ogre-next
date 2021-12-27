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
#ifndef __GL3PlusHARDWARESHADERSTORAGEBUFFER_H__
#define __GL3PlusHARDWARESHADERSTORAGEBUFFER_H__

#include "OgreGL3PlusPrerequisites.h"
#include "OgreHardwareUniformBuffer.h"
//#include "OgreHardwareShaderStorageBuffer.h"

namespace Ogre {
namespace v1 {

    /// Specialisation of HardwareUniformBuffer for the OpenGL Shader Storage Buffer
    class _OgreGL3PlusExport GL3PlusHardwareShaderStorageBuffer final : public HardwareUniformBuffer
    {
    private:
        GLuint mBufferId;
        GLint mBinding;

    protected:
        /** See HardwareBuffer. */
        void* lockImpl(size_t offset, size_t length, LockOptions options) override;
        /** See HardwareBuffer. */
        void unlockImpl() override;

    public:
        GL3PlusHardwareShaderStorageBuffer(HardwareBufferManagerBase* mgr, size_t bufferSize, HardwareBuffer::Usage usage,
                                           bool useShadowBuffer, const String& name);
        ~GL3PlusHardwareShaderStorageBuffer() override;

        /** See HardwareBuffer. */
        void readData(size_t offset, size_t length, void* pDest) override;

        /** See HardwareBuffer. */
        void writeData(size_t offset, size_t length,
                       const void* pSource, bool discardWholeBuffer = false) override;

        /** See HardwareBuffer. */
        void copyData(HardwareBuffer& srcBuffer, size_t srcOffset,
                      size_t dstOffset, size_t length, bool discardWholeBuffer = false) override;

        inline GLuint getGLBufferId() const { return mBufferId; }
        void setGLBufferBinding(GLint binding);
        inline GLint getGLBufferBinding() const { return mBinding; }
    };
}
}
#endif // __GL3PlusHARDWARESHADERSTORAGEBUFFER_H__
