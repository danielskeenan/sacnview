// Copyright (c) 2015 Electronic Theatre Controls, http://www.etcconnect.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*defpack.h
  This file defines the big and little endian packing/unpacking utilities that
  common code uses.

  All functions follow the form PackXY and UpackXY, 
  where X denotes endianness (B or L, for big and little, respecively),
  and Y denotes the number of bytes (1, 2, 4, 8)

  Windows version follows, 
  which is a little endian machine with no boundary problems.
  
  I've added explicit masking to this version to allow anyone running with "smaller type"
  checking to not have debug exceptions.  
*/
#ifndef	_DEFPACK_H_
#define	_DEFPACK_H_

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN

#include <QtGlobal>

//Declarations
void  PackBUint8 (quint8* ptr, quint8 val);	//Packs a quint8 to a known big endian buffer
quint8 UpackBUint8 (const quint8* ptr);		//Unpacks a quint8 from a known big endian buffer
void  PackLUint8 (quint8* ptr, quint8 val);	//Packs a quint8 to a known little endian buffer
quint8 UPackLUint8 (const quint8* ptr);		//Unpacks a quint8 from a known little endian buffer
void  PackBUint16 (quint8* ptr, quint16 val);	//Packs a quint16 to a known big endian buffer
quint16 UpackBUint16 (const quint8* ptr);		//Unpacks a quint16 from a known big endian buffer
void  PackLUint16 (quint8* ptr, quint16 val);	//Packs a quint16 to a known little endian buffer
quint16 UpackLUint16 (const quint8* ptr);		//Unpacks a quint16 from a known little endian buffer
void  PackBUint32 (quint8* ptr, quint32 val);	//Packs a quint32 to a known big endian buffer
quint32 UpackBUint32 (const quint8* ptr);		//Unpacks a quint32 from a known big endian buffer
void  PackLUint32 (quint8* ptr, quint32 val);	//Packs a quint32 to a known little endian buffer
quint32 UpackLUint32 (const quint8* ptr);		//Unpacks a quint32 from a known little endian buffer
void  PackBUint56 (quint8* ptr, quint64 val);	//Packs a quint56 to a known big endian buffer
quint64 UpackBUint56 (const quint8* ptr);		//Unpacks a quint56 from a known big endian buffer
void  PackLUint56 (quint8* ptr, quint64 val);	//Packs a quint56 to a known little endian buffer
quint64 UpackLUint56 (const quint8* ptr);		//Unpacks a quint56 from a known little endian buffer
void  PackBUint64 (quint8* ptr, quint64 val);	//Packs a quint64 to a known big endian buffer
quint64 UpackBUint64 (const quint8* ptr);		//Unpacks a quint32 from a known big endian buffer
void  PackLUint64 (quint8* ptr, quint64 val);	//Packs a quint64 to a known little endian buffer
quint64 UpackLUint64 (const quint8* ptr);		//Unpacks a quint64 from a known little endian buffer

//---------------------------------------------------------------------------------
//implementation

//Packs a quint8 to a known big endian buffer
inline void	PackBUint8(quint8* ptr, quint8 val)
{
	*ptr = val;
}

//Unpacks a quint8 from a known big endian buffer
inline quint8 UpackBUint8(const quint8* ptr)
{
	return *ptr;
}

//Packs a quint8 to a known little endian buffer
inline void	PackLUint8(quint8* ptr, quint8 val)
{
	*ptr = val;
}

//Unpacks a quint8 from a known little endian buffer
inline quint8 UPackLUint8(const quint8* ptr)
{
	return *ptr;
}

//Packs a quint16 to a known big endian buffer
inline void PackBUint16(quint8* ptr, quint16 val)
{
    ptr[1] = (quint8)(val & 0xff);
    ptr[0] = (quint8)((val & 0xff00) >> 8);
}

//Unpacks a quint16 from a known big endian buffer
inline quint16 UpackBUint16(const quint8* ptr)
{
    return (quint16)(ptr[1] | ptr[0] << 8);
}

//Packs a quint16 to a known little endian buffer
inline void PackLUint16(quint8* ptr, quint16 val)
{
    *((quint16*)ptr) = val;
}

//Unpacks a quint16 from a known little endian buffer
inline quint16 UpackLUint16(const quint8* ptr)
{
    return *((quint16*)ptr);
}

//Packs a quint32 to a known big endian buffer
inline void PackBUint32(quint8* ptr, quint32 val)
{
    ptr[3] = (quint8) (val & 0xff);
    ptr[2] = (quint8)((val & 0xff00) >> 8);
    ptr[1] = (quint8)((val & 0xff0000) >> 16);
    ptr[0] = (quint8)((val & 0xff000000) >> 24);
}

//Unpacks a quint32 from a known big endian buffer
inline quint32 UpackBUint32(const quint8* ptr)
{
    return (quint32)(ptr[3] | (ptr[2] << 8) | (ptr[1] << 16) | (ptr[0] << 24));
}

//Packs a quint32 to a known little endian buffer
inline void PackLUint32(quint8* ptr, quint32 val)
{
    *((quint32*)ptr) = val;
}

//Unpacks a quint32 from a known little endian buffer
inline quint32 UpackLUint32(const quint8* ptr)
{
    return *((quint32*)ptr);
}

//Packs a quint56 to a known big endian buffer
inline void PackBUint56(quint8* ptr, quint64 val)
{
    ptr[6] = (quint8) (val & 0xff);
    ptr[5] = (quint8)((val & 0xff00) >> 8);
    ptr[4] = (quint8)((val & 0xff0000) >> 16);
    ptr[3] = (quint8)((val & 0xff000000) >> 24);
    ptr[2] = (quint8)((val & 0xff00000000) >> 32);
    ptr[1] = (quint8)((val & 0xff0000000000) >> 40);
    ptr[0] = (quint8)((val & 0xff000000000000) >> 48);
}

//Unpacks a quint56 from a known big endian buffer
inline quint64 UpackBUint56(const quint8* ptr)
{
    return (quint64)ptr[6] |
            (((quint64)ptr[5]) << 8) |
            (((quint64)ptr[4]) << 16) |
            (((quint64)ptr[3]) << 24) |
            (((quint64)ptr[2]) << 32) |
            (((quint64)ptr[1]) << 40) |
            (((quint64)ptr[0]) << 48);
}

//Packs a quint56 to a known little endian buffer
inline void PackLUint56(quint8* ptr, quint64 val)
{
    ptr[0] = (quint8) (val & 0xff);
    ptr[1] = (quint8)((val & 0xff00) >> 8);
    ptr[2] = (quint8)((val & 0xff0000) >> 16);
    ptr[3] = (quint8)((val & 0xff000000) >> 24);
    ptr[4] = (quint8)((val & 0xff00000000) >> 32);
    ptr[5] = (quint8)((val & 0xff0000000000) >> 40);
    ptr[6] = (quint8)((val & 0xff000000000000) >> 48);
}

//Unpacks a quint56 from a known little endian buffer
inline quint64 UpackLUint56(const quint8* ptr)
{
    return *((quint64*)ptr) & 0xFFFFFFFFFFFFFF;
}

//Packs a quint64 to a known big endian buffer
inline void PackBUint64(quint8* ptr, quint64 val)
{
    ptr[7] = (quint8) (val & 0xff);
    ptr[6] = (quint8)((val & 0xff00) >> 8);
    ptr[5] = (quint8)((val & 0xff0000) >> 16);
    ptr[4] = (quint8)((val & 0xff000000) >> 24);
    ptr[3] = (quint8)((val & 0xff00000000) >> 32);
    ptr[2] = (quint8)((val & 0xff0000000000) >> 40);
    ptr[1] = (quint8)((val & 0xff000000000000) >> 48);
    ptr[0] = (quint8)((val & 0xff00000000000000) >> 56);
}

//Unpacks a quint64 from a known big endian buffer
inline quint64 UpackBUint64(const quint8* ptr)
{
    return ((quint64)ptr[7]) | (((quint64)ptr[6]) << 8) | (((quint64)ptr[5]) << 16) |
           (((quint64)ptr[4]) << 24) | (((quint64)ptr[3]) << 32) |
           (((quint64)ptr[2]) << 40) | (((quint64)ptr[1]) << 48) |
           (((quint64)ptr[0]) << 56);
}

//Packs a quint64 to a known little endian buffer
inline void PackLUint64(quint8* ptr, quint64 val)
{
    *((quint64*)ptr) = val;
}

//Unpacks a quint64 from a known little endian buffer
inline quint64 UpackLUint64(const quint8* ptr)
{
    return *((quint64*)ptr);
}

#endif // Q_LITTLE_ENDIAN

#endif	/*_DEFPACK_H_*/

