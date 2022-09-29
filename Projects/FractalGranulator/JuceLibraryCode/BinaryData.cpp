/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== EdPF_SequenceStrategy.h ==================
static const unsigned char temp_binary_data_0[] =
"/*\r\n"
"  ==============================================================================\r\n"
"\r\n"
"    EdPF_SequenceStrategy.h\r\n"
"    Created: 27 Aug 2022 6:04:40pm\r\n"
"    Author:  Edward Gallyot\r\n"
"\r\n"
"  ==============================================================================\r\n"
"*/\r\n"
"\r\n"
"#ifndef EDPF_SEQUENCE_STRATEGY_H_INCLUDED\r\n"
"#define EDPF_SEQUENCE_STRATEGY_H_INCLUDED\r\n"
"\r\n"
"namespace EdPF\r\n"
"{\r\n"
"    namespace Grains\r\n"
"    {\r\n"
"        class SequenceStrategy\r\n"
"        {\r\n"
"        public:\r\n"
"            virtual ~SequenceStrategy() = default;\r\n"
"\r\n"
"            virtual float GetNextDuration() = 0;\r\n"
"            virtual int GetNextInterOnset() = 0;\r\n"
"        private:\r\n"
"        };\r\n"
"    }\r\n"
"}\r\n"
"\r\n"
"#endif //!EDPF_SEQUENCE_STRATEGY_H_INCLUDED\r\n";

const char* EdPF_SequenceStrategy_h = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x219f82e7:  numBytes = 709; return EdPF_SequenceStrategy_h;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "EdPF_SequenceStrategy_h"
};

const char* originalFilenames[] =
{
    "EdPF_SequenceStrategy.h"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
