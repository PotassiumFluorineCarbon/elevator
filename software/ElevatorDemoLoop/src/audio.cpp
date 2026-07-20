#include "audio.h"
#include <alsa/asoundlib.h>
#include <cstdio>
#include <cstring>

bool playAudio(const std::string& filename, const std::string& device) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename.c_str());
        return false;
    }

    // Skip standard WAV header (44 bytes)
    if (fseek(fp, 44, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Invalid WAV file\n");
        fclose(fp);
        return false;
    }

    snd_pcm_t* handle = nullptr;
    if (snd_pcm_open(&handle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        fprintf(stderr, "Error: Cannot open audio device %s\n", device.c_str());
        fclose(fp);
        return false;
    }

    snd_pcm_hw_params_t* params = nullptr;
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 2);           // stereo
    unsigned int rate = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, nullptr);

    if (snd_pcm_hw_params(handle, params) < 0) {
        fprintf(stderr, "Error: Failed to set hardware parameters\n");
        snd_pcm_close(handle);
        fclose(fp);
        return false;
    }

    snd_pcm_prepare(handle);

    char buffer[8192];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        int frames = bytes_read / 4;  // 2 channels * 2 bytes
        if (snd_pcm_writei(handle, buffer, frames) < 0) {
            snd_pcm_prepare(handle);  // Recover from underrun
        }
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    fclose(fp);

    return true;
}