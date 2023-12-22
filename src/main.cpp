#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <cstdlib>

// Include the FDK-AAC encoder headers
#include <fdk-aac/aacenc_lib.h>

// Constants
#define SAMPLE_RATE 44100
#define CHANNEL_COUNT 2
#define BIT_RATE 128000

// Function to save PCM stereo data as AAC file using FDK-AAC encoder
void savePcmToAac(const char* pcmFile, const char* aacFile) {
    // Open PCM file
    std::ifstream pcm(pcmFile, std::ios::binary);
    if (!pcm) {
        std::cerr << "Failed to open PCM file: " << pcmFile << std::endl;
        return;
    }

    // Get PCM file size
    pcm.seekg(0, std::ios::end);
    std::streampos pcmFileSize = pcm.tellg();
    pcm.seekg(0, std::ios::beg);

    // Allocate memory for PCM data
    float* pcmData = new float[pcmFileSize / sizeof(float)];

    // Read PCM data from file
    pcm.read(reinterpret_cast<char*>(pcmData), pcmFileSize);

    // Close PCM file
    pcm.close();

    // Initialize the AAC encoder
    HANDLE_AACENCODER aacEncoder;
    if (aacEncOpen(&aacEncoder, 0, CHANNEL_COUNT) != AACENC_OK) {
        std::cerr << "Failed to open AAC encoder" << std::endl;
        delete[] pcmData;
        return;
    }

    // Set AAC encoder parameters
    if (aacEncoder_SetParam(aacEncoder, AACENC_AOT, AOT_AAC_LC) != AACENC_OK ||
        aacEncoder_SetParam(aacEncoder, AACENC_SAMPLERATE, SAMPLE_RATE) != AACENC_OK ||
        aacEncoder_SetParam(aacEncoder, AACENC_CHANNELMODE, MODE_2) != AACENC_OK ||
        aacEncoder_SetParam(aacEncoder, AACENC_BITRATE, BIT_RATE) != AACENC_OK) {
        std::cerr << "Failed to set AAC encoder parameters" << std::endl;
        aacEncClose(&aacEncoder);
        delete[] pcmData;
        return;
    }

    // Initialize the AAC encoder
    if (aacEncEncode(aacEncoder, nullptr, nullptr, nullptr, nullptr) != AACENC_OK) {
        std::cerr << "Failed to initialize AAC encoder" << std::endl;
        aacEncClose(&aacEncoder);
        delete[] pcmData;
        return;
    }

    // Open AAC file for writing
    std::ofstream aac(aacFile, std::ios::binary);
    if (!aac) {
        std::cerr << "Failed to open AAC file: " << aacFile << std::endl;
        aacEncClose(&aacEncoder);
        delete[] pcmData;
        return;
    }

    // Allocate buffer for AAC output data
    const int bufferSize = 8192;
    uint8_t* buffer = new uint8_t[bufferSize];
    int bytesWritten = 0;
    int pcmSamplesRead = 0;

    // Encode PCM data and write AAC frames to the file
    while (pcmSamplesRead < pcmFileSize / sizeof(float)) {
        // Encode input PCM samples
        int inputSize = bufferSize / sizeof(float);
        int outputSize = bufferSize;
        int result = aacEncEncode(
            aacEncoder,
            &pcmData[pcmSamplesRead],
            inputSize,
            buffer,
            outputSize
        );

        if (result < 0) {
            std::cerr << "Failed to encode PCM data to AAC" << std::endl;
            break;
        }

        // Write encoded AAC data to the file
        aac.write(reinterpret_cast<char*>(buffer), result);
        bytesWritten += result;

        // Update PCM samples and buffer pointers
        pcmSamplesRead += inputSize;
    }

    // Flush any remaining AAC data to the file
    while (true) {
        int outputSize = bufferSize;
        int result = aacEncEncode(
            aacEncoder,
            nullptr,
            0,
            buffer,
            outputSize
        );

        if (result < 0) {
            std::cerr << "Failed to flush remaining AAC data" << std::endl;
            break;
        }

        if (result == 0) {
            // No more data to flush
            break;
        }

        // Write encoded AAC data to the file
        aac.write(reinterpret_cast<char*>(buffer), result);
        bytesWritten += result;
    }

    // Close AAC file
    aac.close();

    // Printthe number of bytes written
    std::cout << "PCM successfully encoded to AAC. Bytes written: " << bytesWritten << std::endl;

    // Clean up resources
    aacEncClose(&aacEncoder);
    delete[] pcmData;
    delete[] buffer;
}

int main() {
    const char* pcmFile = "input.pcm";
    const char* aacFile = "output.aac";

    savePcmToAac(pcmFile, aacFile);

    return 0;
}
