/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SineWavePlotAudioProcessor::SineWavePlotAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    m_sineWave(2048),
    m_params(*this, nullptr, "MAINTREE", CreateParameterLayout()),
    m_fifo(512)
{
    m_tmpBuffer.resize(2048);
}

SineWavePlotAudioProcessor::~SineWavePlotAudioProcessor()
{
}

//==============================================================================
const juce::String SineWavePlotAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SineWavePlotAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SineWavePlotAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SineWavePlotAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SineWavePlotAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SineWavePlotAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SineWavePlotAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SineWavePlotAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const juce::String SineWavePlotAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void SineWavePlotAudioProcessor::changeProgramName (int /*index*/, const juce::String& /*newName*/)
{
}

//==============================================================================
void SineWavePlotAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_sineWave.PrepareToPlay(sampleRate, samplesPerBlock);
    auto paramFreq = GetParameter(SWP::ParamIds[SWP::Param_Freq]);
    m_sineWave.SetFreq(paramFreq->convertFrom0to1(paramFreq->getValue()));
    // Set the Amplitude
    auto paramAmp = GetParameter(SWP::ParamIds[SWP::Param_Amp]);
    m_sineWave.SetAmp(paramAmp->convertFrom0to1(paramAmp->getValue()));



    m_tmpBuffer.resize(samplesPerBlock);
}

void SineWavePlotAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SineWavePlotAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SineWavePlotAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Set the Frequency 
    auto paramFreq = GetParameter(SWP::ParamIds[SWP::Param_Freq]);
    m_sineWave.SetFreq(paramFreq->convertFrom0to1(paramFreq->getValue()));
    // Set the Amplitude
    auto paramAmp = GetParameter(SWP::ParamIds[SWP::Param_Amp]);
    m_sineWave.SetAmp(paramAmp->convertFrom0to1(paramAmp->getValue()));
     //Process a block of sine wave
    //m_sineWave.ProcessBlock(buffer);

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        m_tmpBuffer[i] = buffer.getSample(0, i);
    }

    //// Cananocalise the data
    //FloatVectorOperations::add(&(m_tmpBuffer[0]), 1.0f, m_tmpBuffer.size());
    //FloatVectorOperations::multiply(&(m_tmpBuffer[0]), 0.5f, m_tmpBuffer.size());
    
    m_fifo.AddToFifo(m_tmpBuffer);
}

//==============================================================================
bool SineWavePlotAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SineWavePlotAudioProcessor::createEditor()
{
    return new SineWavePlotAudioProcessorEditor (*this);
}

//==============================================================================
void SineWavePlotAudioProcessor::getStateInformation (juce::MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SineWavePlotAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

RangedAudioParameter* SineWavePlotAudioProcessor::GetParameter(StringRef ParamId)
{
    return m_params.getParameter(ParamId);
}

AudioProcessorValueTreeState::ParameterLayout SineWavePlotAudioProcessor::CreateParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back
    (
        std::make_unique<AudioParameterFloat>
        (
            SWP::ParamIds[SWP::Param_Freq],
            "Frequency",
            NormalisableRange<float>(20.0f, 20000.0f),
            440.0f
        )
    );

    params.push_back
    (
        std::make_unique<AudioParameterFloat>
        (
            SWP::ParamIds[SWP::Param_Amp],
            "Amplitude",
            NormalisableRange<float>(0.0f, 1.0f),
            0.5f
        )
    );

    return { params.begin(), params.end() };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SineWavePlotAudioProcessor();
}
