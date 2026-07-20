#pragma once
#include <string>

/**
 * Plays a WAV file using ALSA.
 *
 * @param filename Path to the WAV file (must be 16-bit PCM, stereo or mono)
 * @param device   ALSA device name (default = "default")
 * @return true if playback started successfully, false otherwise
 */
bool playAudio(const std::string& filename, const std::string& device = "default");