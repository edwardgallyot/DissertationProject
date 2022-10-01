/*
  ==============================================================================

    EdPF_DelayLine.h
    Created: 3 Aug 2022 2:54:28pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_DELAY_LINE_H
#define EDPF_DELAY_LINE_H

#include <JuceHeader.h>
#include "EdPF_WaveTableInterpolation.h"

namespace EdPF
{
    namespace DSP
    {
        // This is a one-dimensional delay line for DSP operations.
        // It can take any interpolation algorithm that has a GetSample(FloatType) function.
        template<typename FloatType, typename InterpolationType>
        class DelayLine
        {
        public:
            DelayLine(int size) :
                m_size(size),
                m_data(1, size),
                m_writeIndex(0)
            {
                m_data.clear();
            }
            void WriteBlock(int blockSize, juce::AudioBuffer<FloatType>& data)
            {
                m_blockSize = blockSize;
                // We can copy a normal block if we aren't wrapping around
                if (m_writeIndex + blockSize < m_data.getNumSamples())
                {
                    juce::FloatVectorOperations::copy
                    (
                        m_data.getWritePointer(0, m_writeIndex),
                        data.getReadPointer(0, 0),
                        blockSize
                    );
                }
                // Otherwise we can copy two blocks, the rest to fill the buffer and the start of the new lot
                else
                {
                    int remainder = (m_writeIndex + blockSize) % m_data.getNumSamples();
                    int blockSize1 = blockSize - remainder;
                    int blockSize2 = remainder;

                    juce::FloatVectorOperations::copy
                    (
                        m_data.getWritePointer(0, m_writeIndex),
                        data.getReadPointer(0, 0),
                        blockSize1
                    );

                    juce::FloatVectorOperations::copy
                    (
                        m_data.getWritePointer(0, 0), 
                        data.getReadPointer(0, blockSize1 - 1),
                        blockSize2
                    );
                }

                m_writeIndex = (m_writeIndex + blockSize) % m_data.getNumSamples();

            };

            FloatType ReadSample(FloatType index)
            {
                return m_interpolator.template GetSample<FloatType>(index, m_data);
            }

            inline int GetSize() const { return m_size; };
            inline int GetWriteIndex() const { return m_writeIndex; };
            inline int GetBlockSize() const { return m_blockSize; }

            void SetSize(int size)
            {
                m_data.setSize(1, size);
                m_data.clear();
                m_size = size;
            };

        private:
            int m_size;
            int m_writeIndex;
            int m_blockSize;
            juce::AudioBuffer<FloatType> m_data;
            InterpolationType m_interpolator;
        };
    }
}

using LinearDelayLine = EdPF::DSP::DelayLine<float, EdPF::DSP::LinearInterpolation>;

#endif //!EDPF_DELAY_LINE_H
