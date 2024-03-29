/*
  ==============================================================================

    EdPF_AudioProcessor.cpp
    Created: 3 Aug 2022 2:51:28pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_AudioProcessor.h"

EdPF::AudioProcessor::AudioProcessor(juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout, int numOfParams)
    : juce::AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    m_apvts(*this, nullptr, "PARAMETERS", std::move(parameterLayout)),
    m_smoothedValues(numOfParams),
    m_smoothedValuesBuffers(numOfParams)
{
}



EdPF::AudioProcessor::~AudioProcessor()
{
}

//==============================================================================
const juce::String EdPF::AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EdPF::AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool EdPF::AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool EdPF::AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double EdPF::AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EdPF::AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EdPF::AudioProcessor::getCurrentProgram()
{
    return 0;
}

void EdPF::AudioProcessor::setCurrentProgram(int /*index*/)
{
}

const juce::String EdPF::AudioProcessor::getProgramName(int /*index*/)
{
    return {};
}

void EdPF::AudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/)
{
}

//==============================================================================
void EdPF::AudioProcessor::prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void EdPF::AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EdPF::AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void EdPF::AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        /*auto* channelData = buffer.getWritePointer(channel);*/

        // ..do something to the data...
    }
}

//==============================================================================
bool EdPF::AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

//==============================================================================
void EdPF::AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = m_apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void EdPF::AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(m_apvts.state.getType()))
            m_apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::Value EdPF::AudioProcessor::GetParameterAsValue(juce::StringRef ID)
{
    // TODO: Add some checks in here to tell if we're using this responsibly

    // TODO: We should probably be able to tell the difference between automated and non-automated parameters
    return m_apvts.getParameterAsValue(ID);
}

juce::RangedAudioParameter* EdPF::AudioProcessor::GetParameter(juce::StringRef ID)
{
    // TODO: Add some checks in here to tell if we're using this responsibly

    // TODO: We should probably be able to tell the difference between automated and non-automated parameters
    return m_apvts.getParameter(ID);
}

void EdPF::AudioProcessor::PrepareSmoothedValues(double sampleRate, int numSamplesExpectedPerBlock)
{
    for (int i = 0; i < m_smoothedValues.size(); ++i)
    {
        m_smoothedValues[i].reset(sampleRate, 0.1);
        m_smoothedValuesBuffers[i].setSize(1, numSamplesExpectedPerBlock);
    }
}

void EdPF::AudioProcessor::CopySmoothedValuesToBuffers()
{
    for (int i = 0; i < m_smoothedValues.size(); ++i)
    {
        for (int j = 0; j < m_smoothedValuesBuffers[i].getNumSamples(); ++j)
        {
            m_smoothedValuesBuffers[i].setSample(0, j, m_smoothedValues[i].getNextValue());
        }
    }
}

void EdPF::AudioProcessor::UpdateSmoothedValues(int index, float valueAtStartOfBlock)
{
    m_smoothedValues[index].setTargetValue(valueAtStartOfBlock);
}



