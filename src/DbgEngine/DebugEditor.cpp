
#include "DebugEditor.h"

DebugEditor::DebugEditor(TestAudioProcessor& audioProcessor)
{
	specHeight = 500;
	specWidth = 700;
	m_AudioProcessor = &audioProcessor;
	startTimerHz(15);

	setSize(700, 500);
}

void DebugEditor::timerCallback()
{

	if (m_AudioProcessor->m_DebugProcessor.nextFFTBlockReady) 
	{
		m_AudioProcessor->m_DebugProcessor.CalcNextFrameOfSpectrum();
		m_AudioProcessor->m_DebugProcessor.nextFFTBlockReady = false;
		repaint();
	}
}

void DebugEditor::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::orange);

	for (int i = 1; i < m_AudioProcessor->m_DebugProcessor.scopeSize; ++i)
	{
		Line<float> line((float)jmap(i - 1, 0, m_AudioProcessor->m_DebugProcessor.scopeSize - 1, 0, specWidth),
			jmap(m_AudioProcessor->m_DebugProcessor.scopeData[i - 1], 0.0f, 1.0f, (float)(specHeight), 0.0f),
			(float)jmap(i, 0, m_AudioProcessor->m_DebugProcessor.scopeSize - 1, 0, specWidth),
			jmap(m_AudioProcessor->m_DebugProcessor.scopeData[i], 0.0f, 1.0f, (float)(specHeight), 0.0f));

		g.drawLine(line, 1.0f);
	}
}