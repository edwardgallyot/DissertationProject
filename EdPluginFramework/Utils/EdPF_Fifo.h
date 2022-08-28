/*
  ==============================================================================

    EdPF_Fifo.h
    Created: 30 Jul 2022 8:08:38am
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_FIFO_H_INCLUDED
#define EDPF_FIFO_H_INCLUDED

#include <JuceHeader.h>

namespace EdPF
{

    template <typename ValueType>
    class Fifo
    {
    public:
        Fifo(size_t size) :
            m_size(size),
            m_abstractFifo(size),
            m_myBuffer()
        {
            m_myBuffer.resize(static_cast<int>(size));
        };
        ~Fifo()
        {

        };

        void AddToFifo(std::vector<ValueType>& someData)
        {
            const auto scope = m_abstractFifo.write(someData.size());
            int j = 0;
            if (scope.blockSize1 > 0)
            {
                for (int i = 0; i < scope.blockSize1; ++i, j++)
                {
                    m_myBuffer[scope.startIndex1 + i] = someData[j];   
                }

                //copySomeData(myBuffer + scope.startIndex1, someData, scope.blockSize1
            }
            
            if (scope.blockSize2 > 0)
            {
                for (int i = 0; i < scope.blockSize2; ++i, j++)
                {
                    m_myBuffer[scope.startIndex2 + i] = someData[j];
                }
                //copySomeData(someData + scope.blockSize1, myBuffer + scope.startIndex2, scope.blockSize2);
            }
        };

        void ReadFromFifo(std::vector<ValueType>& someData)
        {
            const auto scope = m_abstractFifo.read(someData.size());
            int j = 0;
            if (scope.blockSize1 > 0)
            {
                for (int i = 0; i <  scope.blockSize1; ++i, j++)
                {
                   someData[j] = m_myBuffer[scope.startIndex1 + i];
                }
                //copySomeData(someData, myBuffer + scope.startIndex1, scope.blockSize1);
            }

            if (scope.blockSize2 > 0)
            {
                for (int i = 0; i < scope.blockSize2; ++i, j++)
                {
                    someData[j] = m_myBuffer[scope.startIndex2 + i];
                }
                //copySomeData(someData + scope.blockSize1, myBuffer + scope.startIndex2, scope.blockSize2); scope.blockSize2);
            }
        };

    private:
        size_t m_size;
        juce::AbstractFifo m_abstractFifo;
        std::vector<ValueType> m_myBuffer;
    };
}

#endif //!EDPF_FIFO_H_INCLUDED