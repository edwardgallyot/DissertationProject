/*
  ==============================================================================

    EdPF_GrainScheduler.cpp
    Created: 22 Aug 2022 10:40:01pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_GrainScheduler.h"

EdPF::Grains::Scheduler::Scheduler() :
    m_shouldActivateGrain(false),
    m_nextOnset(0)
{
}

void EdPF::Grains::Scheduler::IncrementSample()
{
    if (m_nextOnset == 0)
    {
        m_nextDuration = m_sequenceStrategy->GetNextDuration();
        m_nextOnset = m_sequenceStrategy->GetNextInterOnset();
        m_shouldActivateGrain = true;
    }
    else
    {
        m_shouldActivateGrain = false;
        --m_nextOnset;
    }
}
