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
#include "BinaryData.h"

static const juce::FontOptions& baseFontOptions()
{
    static juce::FontOptions fo{juce::Typeface::createSystemTypefaceFor(
        BinaryData::oaknut_ttf, BinaryData::oaknut_ttfSize)};
    return fo;
}

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizeLimits(240, 160, 100000, 100000);
    setResizable(true,
        p.wrapperType != AudioPluginAudioProcessor::wrapperType_AudioUnitv3 &&
        p.wrapperType != AudioPluginAudioProcessor::wrapperType_Standalone);
    setSize(240 * 4, 160 * 4);
    startTimerHz(60);
}    

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1F0034));
    g.setColour(juce::Colour(0xFFFAC800));
    g.setFont(baseFontOptions().withHeight(16));
        
    auto vw = getWidth();
    auto vh = getHeight();

    auto core = processorRef.getCore();    

    const size_t vceCount = 32;
    const uint32_t roseMask = 0x65736F00;
    const uint32_t sndArea = 0x3000144;    
    const uint32_t vceArea = sndArea + 0x50;
    const uint32_t cgbArea = sndArea + 0x14B0; 
               
    uint32_t rose = core->rawRead32(core, sndArea, 0);
    bool roseMatch = (rose & 0xFFFFFF00) == roseMask; //0x65736F52;
    
    // right bottom align
    auto x = vw;
    auto y = vh;
    
    x -= 90;
    y -= 80;
    auto logo = juce::ImageCache::getFromMemory(BinaryData::kitty_png, BinaryData::kitty_pngSize);
    g.drawImage(logo, x, y, 80, 80, 0, 0, logo.getWidth(), logo.getHeight());
      
    x -= 10;
    x -= 120;

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
        std::string voices = std::format("Voices: {} / {}\nCGB: {}  /4", totalVoices, vceCount, totalCgb);
        g.drawFittedText(voices.c_str(), x, y, 120, 80, juce::Justification::centred, 2);
    } else {
        g.drawFittedText("loading...", x, y, 120, 80, juce::Justification::centred, 2);
    }
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
