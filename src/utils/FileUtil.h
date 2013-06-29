#ifndef __FILEUTIL_H_
#define __FILEUTIL_H_

#include <utils/Types.h>
#include <utils/StringPiece.h>
#include <boost/noncopyable.hpp>

namespace dbdky
{

namespace FileUtil
{
    class SmallFile : boost::noncopyable
    {
    public:
        SmallFile(StringPiece filename);
        ~SmallFile();

        template<typename String>
       	int readToString(int maxSize, String* content,
			int64_t* fileSize,
			int64_t* modifyTime,
			int64_t* createTime);

        int readToBuffer(int* size);

        const char* buffer() const { return buf_;}
   
        static const int kBufferSize = 65536;

    private:
        int fd_;
        int err_;
        char buf_[kBufferSize];
    };

    template<typename String>
    int readFile(StringPiece filename,
		int maxSize,
		String* content,
		int64_t* fileSize = NULL,
		int64_t* modifyTime = NULL,
		int64_t* createTime = NULL)
    {
        SmallFile file(filename);
        return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
    }
}

}
#endif
