#include "JStateManager.h"

JStateManager::JStateManager(AudioProcessor& audioProcessor) : 
    apvt(audioProcessor, nullptr, "DEFAULT", JStateManager::getParameterLayout())
{
    apvt.state = ValueTree("PRESETS");
    presetDirectory = getPresetDirectory();
}

void JStateManager::writeState(juce::MemoryBlock& destData) 
{
    // Copy the state to new variable
    auto state = apvt.copyState();
    // Create XML object
    std::unique_ptr<XmlElement> xml(state.createXml());

    // Write to binary just so readState is called on startup.
    AudioProcessor::copyXmlToBinary(*xml, destData);
}

// Read state from memory
void JStateManager::readState(const void* data, int sizeInBytes) 
{
    // Get XML document
    XmlDocument myDocument(File(presetDirectory.getFullPathName() + File::getSeparatorString() + "preset.xml"));
    // Parse the document
    std::unique_ptr<XmlElement> xmlState(myDocument.getDocumentElement());
    
    // If it is not a nullptr (parsing succeeded) then continue, otherwise get next value
    if (xmlState != nullptr) { apvt.state = ValueTree::fromXml(*xmlState); }

    JStateManager::getPresets();
}

void JStateManager::readPreset(String presetName) 
{
    XmlDocument myDocument(File(presetDirectory.getFullPathName() + File::getSeparatorString() + presetName));
    std::unique_ptr<XmlElement> xmlState(myDocument.getDocumentElement());;
    DBG(myDocument.getLastParseError());
    if (xmlState != nullptr) { apvt.state = ValueTree::fromXml(*xmlState); }
}

void JStateManager::writePreset(String presetName) 
{
    // Copy the state to new variable
    auto state = apvt.copyState();
    // Create XML object
    std::unique_ptr<XmlElement> xml(state.createXml());
	// Write XML to file
    xml->writeTo(File(presetDirectory.getFullPathName() + File::getSeparatorString() + presetName));
}

StringArray JStateManager::getPresets() 
{
    filenames.clear();
	
    for (DirectoryEntry entry : RangedDirectoryIterator(presetDirectory, true)) {
		filenames.add(entry.getFile().getFileName());
    }
	return(filenames);
}

AudioProcessorValueTreeState::ParameterLayout JStateManager::getParameterLayout()
{
        AudioProcessorValueTreeState::ParameterLayout params;
        params.add(std::make_unique<AudioParameterInt>("TIME", "TIME", 1, 44000, 22000));
        params.add(std::make_unique<AudioParameterFloat>("FEEDBACK", "FEEDBACK", NormalisableRange<float>(0, 1), 0.5f));

        return params;
}