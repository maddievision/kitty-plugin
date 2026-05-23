/* Copyright 2026 Maddie Lim
 *
 * Kitty Advance Plugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Kitty Advance Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with  Kitty Advance Plugin.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <format>
//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    startTimerHz(60);
}    

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    auto core = processorRef.getCore();    

    const size_t vceCount = 32;
    const uint32_t roseMask = 0x65736F00;
    const uint32_t sndArea = 0x3000144;    
    const uint32_t vceArea = sndArea + 0x50;
    const uint32_t cgbArea = sndArea + 0x14B0; 
               
    uint32_t rose = core->rawRead32(core, sndArea, 0);
    bool roseMatch = (rose & 0xFFFFFF00) == roseMask; //0x65736F52;
    if (roseMatch) {
        int totalVoices = 0;
        int totalCgb = 0;
        for (uint32_t i = 0; i < 32; ++i) {
            uint32_t vceptr = vceArea + i * 0x40;
            if (core->rawRead8(core, vceptr, 0)) {
                totalVoices++;
            }
        }
        for (uint32_t i = 0; i < 4; ++i) {
            uint32_t vceptr = cgbArea + i * 0x40;
            if (core->rawRead8(core, vceptr, 0)) {
                totalCgb++;
            }
        }
        std::string status = std::format("s4a ready!\nVoices: {} / {}\nCGB: {} / 4", totalVoices, vceCount, totalCgb);
        g.drawFittedText (status.c_str(), getLocalBounds(), juce::Justification::centred, 3);
    } else {
        g.drawFittedText ("s4a initialising...", getLocalBounds(), juce::Justification::centred, 1);        
    }
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
