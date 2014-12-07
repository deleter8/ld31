#ifndef __FSTREAMs_TO_END_ALL_FSTREAMTs__
#define __FSTREAMs_TO_END_ALL_FSTREAMTs__

#include <fstream>

#if defined(_WIN32) || defined(WIN32)
typedef std::wifstream ifstream_t;
#else
typedef std::ifstream ifstream_t
#endif


#endif //__FSTREAMs_TO_END_ALL_FSTREAMTs__