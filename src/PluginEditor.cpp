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
#include <cmath>
#include <numbers>
#include "BinaryData.h"


//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    vapp.setTitle("Kitty Advance");
    setSize (800, 600);
    vapp.onDraw() = [this](visage::Canvas& canvas) {
        draw(canvas);
    };
//     startTimerHz(60);
}    

void AudioPluginAudioProcessorEditor::draw(visage::Canvas& canvas) {
    double t = canvas.time();
    
    auto vw = vapp.width();
    auto vh = vapp.height();

    canvas.setColor(0xFF1F0034);
    canvas.fill(0, 0, vw, vh);
    
//     float circle_radius = vh * 0.1f + (20.0f * std::sin(std::numbers::pi * 2.0f * t));
//     float x = vapp.width() * 0.5f - circle_radius;
//     float y = vh * 0.5f - circle_radius;
//     canvas.setColor(0xff002222);
//     canvas.circle(x, y, 2.0f * circle_radius);    

    canvas.setColor(0xFFBBBBBB);
    visage::Font font(16, (const unsigned char*) BinaryData::oaknut_ttf, BinaryData::oaknut_ttfSize);

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
        std::string voices = std::format("Voices: {} / {}", totalVoices, vceCount);
        std::string cgb = std::format("CGB: {} / 4", totalCgb);
        canvas.setColor(0xFFFAC800);
        canvas.text(voices, font, visage::Font::kCenter, vw - 120 - 100 - 120, vh - 80, 120, 40);
        canvas.text(cgb, font, visage::Font::kCenter, vw - 120 - 100 - 120, vh - 80 + 40, 120, 40);
    } else {
        canvas.setColor(0xFFFAC800);
        canvas.text("initialising…", font, visage::Font::kCenter, vw - 120 - 90 - 120, vh - 80, 120, 80);
    }
    
    canvas.setColor(0xFFFAC800);
    canvas.text("Kitty Advance", font, visage::Font::kCenter, vw - 120, vh - 80, 120, 26);
    canvas.text("Plugin v0.01", font, visage::Font::kCenter, vw - 120, vh - 80 + 26, 120, 26);
    canvas.text("Driver v0.01", font, visage::Font::kCenter, vw - 120, vh - 80 + 52, 120, 26);

    canvas.setColor(0xFFFFFFFF);
    canvas.image((const unsigned char*) BinaryData::kitty_png, BinaryData::kitty_pngSize, vw - 90 - 120, vh - 80, 80, 80);
    vapp.redraw();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(31U, 0, 52U));
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const auto bounds = getLocalBounds();
    vapp.setWindowDimensions(bounds.getWidth(), bounds.getHeight());
}

void AudioPluginAudioProcessorEditor::parentHierarchyChanged() {
    if (getPeer()) {
        vapp.show(0, 0, getPeer()->getNativeHandle());
        triggerAsyncUpdate();
    }
}