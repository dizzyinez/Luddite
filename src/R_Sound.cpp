#include "data/SoundAllocator.hpp"

#include <memory>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>

template <>
std::shared_ptr<Sound> ResourceAllocator<Sound>::Allocate(std::string file_path)
{
        struct WavRead
        {
                unsigned int channels = 0;
                unsigned int sampleRate = 0;
                drwav_uint64 totalPCMFrameCount = 0;
                std::vector<uint16_t> pcmData;
                drwav_uint64 getTotalSamples() const {return totalPCMFrameCount * channels;}
        } wav_read;

        drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16(file_path.c_str(), &wav_read.channels, &wav_read.sampleRate, &wav_read.totalPCMFrameCount, nullptr);
        if (pSampleData == NULL)
        {
                std::cout << "failed to load audio file " << file_path << std::endl;
                // return sound;
        }
        if (wav_read.getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
        {
                std::cout << "audio file to big for 32 bit addressing" << file_path << std::endl;
                // return sound;
        }
        wav_read.pcmData.resize(size_t(wav_read.getTotalSamples()));
        memcpy(wav_read.pcmData.data(), pSampleData, wav_read.pcmData.size() * 2);
        drwav_free(pSampleData, nullptr);

        ALuint soundBuffer;
        alGenBuffers(1, &soundBuffer);
        alBufferData(soundBuffer, wav_read.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, wav_read.pcmData.data(), wav_read.pcmData.size() * 2, wav_read.sampleRate);

        return std::make_shared<Sound>(soundBuffer);
}
