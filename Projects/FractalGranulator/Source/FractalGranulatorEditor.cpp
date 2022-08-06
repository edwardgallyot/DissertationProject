/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "FractalGranulatorProcessor.h"
#include "FractalGranulatorEditor.h"

//==============================================================================
FractalGranulatorAudioProcessorEditor::FractalGranulatorAudioProcessorEditor (FractalGranulatorAudioProcessor& p)
    : EdPF::AudioProcessorEditor (p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 640);
}

FractalGranulatorAudioProcessorEditor::~FractalGranulatorAudioProcessorEditor()
{
}