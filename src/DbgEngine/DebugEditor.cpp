
#include "DebugEditor.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 830

#define GRAPHS_HEIGHT 800
#define GRAPHS_WIDTH 700

#define SPECTOGRAM_HEIGHT 398
#define SPECTOGRAM_WIDTH 700

using namespace juce;

DebugEditor::DebugEditor(TestAudioProcessor& audioProcessor) : resetBtn("reset", 0.0, Colour(190, 190, 190))
{
	m_audioProcessor = &audioProcessor;
	startTimerHz(15);

	setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	addAndMakeVisible(pauseBtn);
	pauseBtn.onClick = [this] { pauseBtn.getToggleState() ? m_audioProcessor->m_debugProcessor.Pause() : m_audioProcessor->m_debugProcessor.Resume(); };
 
	addAndMakeVisible(muteBtn);
	muteBtn.onClick = [this] { muteBtn.getToggleState() ? m_audioProcessor->mute() : m_audioProcessor->unmute(); };

	addAndMakeVisible(resetBtn);
	resetBtn.onClick = [this] { m_audioProcessor->reset(); };

	addAndMakeVisible(pauseBtnLabel);
	pauseBtnLabel.setFont(juce::Font(12.0f, juce::Font::bold));
	pauseBtnLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	pauseBtnLabel.setText(L"Pause", NotificationType::dontSendNotification); 	
	
	addAndMakeVisible(muteBtnLabel);
	muteBtnLabel.setFont(juce::Font(12.0f, juce::Font::bold));
	muteBtnLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	muteBtnLabel.setText(L"Mute", NotificationType::dontSendNotification);
	
	addAndMakeVisible(resetBtnLabel);
	resetBtnLabel.setFont(juce::Font(12.0f, juce::Font::bold));
	resetBtnLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	resetBtnLabel.setText(L"Reset", NotificationType::dontSendNotification);
}

void DebugEditor::timerCallback()
{
	if (m_audioProcessor->m_debugProcessor.nextFFTBlockReady) 
	{
		m_audioProcessor->m_debugProcessor.CalcNextDebugFrame();
		m_audioProcessor->m_debugProcessor.nextFFTBlockReady = false;
		repaint();
	}
}

void DebugEditor::PaintWaveform(juce::Graphics& g, const int index)
{
	Line<float> line((float)jmap(index - 1, 0, m_audioProcessor->m_debugProcessor.scopeSize - 1, 0, SPECTOGRAM_WIDTH),
		jmap(m_audioProcessor->m_debugProcessor.waveformScopeData[index - 1], -1.0f, 1.0f, 400.0f, 800.0f),
		(float)jmap(index, 0, m_audioProcessor->m_debugProcessor.scopeSize - 1, 0, SPECTOGRAM_WIDTH),
		jmap(m_audioProcessor->m_debugProcessor.waveformScopeData[index], -1.0f, 1.0f, 400.0f, 800.0f));

	g.drawLine(line, 1.0f);
}

void DebugEditor::PaintSpectrum(juce::Graphics& g, const int index)
{
	Line<float> line((float)jmap(index - 1, 0, m_audioProcessor->m_debugProcessor.scopeSize - 1, 0, SPECTOGRAM_WIDTH),
		jmap(m_audioProcessor->m_debugProcessor.spectrumScopeData[index - 1], 0.0f, 1.0f, (float)(SPECTOGRAM_HEIGHT), 0.0f),
		(float)jmap(index, 0, m_audioProcessor->m_debugProcessor.scopeSize - 1, 0, SPECTOGRAM_WIDTH),
		jmap(m_audioProcessor->m_debugProcessor.spectrumScopeData[index], 0.0f, 1.0f, (float)(SPECTOGRAM_HEIGHT), 0.0f));

	g.drawLine(line, 1.0f);
}

void DebugEditor::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::orange);

	for (int i = 1; i < m_audioProcessor->m_debugProcessor.scopeSize; ++i)
	{
		PaintWaveform(g, i);
		PaintSpectrum(g, i);
	}

	g.setColour(juce::Colours::cornsilk);
	Line<float> horizontal(0, 400, SPECTOGRAM_WIDTH, 400);
	g.drawLine(horizontal, 2.0f);

	Line<float> verticalDivider(GRAPHS_WIDTH, 0, GRAPHS_WIDTH, GRAPHS_HEIGHT);
	g.drawLine(verticalDivider, 2.0f);

	g.setColour(juce::Colours::dimgrey);
	Line<float> verticalWaveform(350, GRAPHS_HEIGHT / 2, 350, GRAPHS_HEIGHT);
	g.drawLine(verticalWaveform, 0.5f);
	
	Line<float> hotizontalWaveform(0, 600, SPECTOGRAM_WIDTH, 600);
	g.drawLine(hotizontalWaveform, 0.5f);
}

void DebugEditor::resized()
{
	pauseBtnLabel.setBounds(GRAPHS_WIDTH + 10, 10, 40, 20);
	pauseBtn.setBounds(GRAPHS_WIDTH + 70, 10, 20, 20); 		
	
	muteBtnLabel.setBounds(GRAPHS_WIDTH + 10, 50, 40, 20);
	muteBtn.setBounds(GRAPHS_WIDTH + 70, 50, 20, 20); 	
	
	resetBtnLabel.setBounds(GRAPHS_WIDTH + 10, 90, 40, 20);
	resetBtn.setBounds(GRAPHS_WIDTH + 75, 92, 20, 20);
}
