#include "opusencoder.h"

static constexpr int sampleRate = 48000;
static constexpr int defaultBitRate = 16000;
static constexpr int frameSize = 960;
static constexpr int maxPacketSize = 2048;

static constexpr const char* errorNoFile = "No file opened";
static constexpr const char* errorWrongLength = "Invalid PCM buffer length";

LuaOpusEncoder::LuaOpusEncoder() {
	int error;

	encoder = opus_encoder_create(sampleRate, 1, OPUS_APPLICATION_VOIP, &error);
	if (error != OPUS_OK) {
		throw std::runtime_error(opus_strerror(error));
	}

	setBitRate(defaultBitRate);
}

LuaOpusEncoder::~LuaOpusEncoder() {
	opus_encoder_destroy(encoder);
	close();
}

void LuaOpusEncoder::setBitRate(opus_int32 bitRate) const {
	int error = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitRate));
	if (error != OPUS_OK) {
		throw std::runtime_error(opus_strerror(error));
	}
}

opus_int32 LuaOpusEncoder::getBitRate() const {
	opus_int32 bitRate;
	int error = opus_encoder_ctl(encoder, OPUS_GET_BITRATE(&bitRate));
	if (error != OPUS_OK) {
		throw std::runtime_error(opus_strerror(error));
	}
	return bitRate;
}

void LuaOpusEncoder::close() {
	if (input) {
		fclose(input);
		input = nullptr;
	}
}

void LuaOpusEncoder::open(const char* fileName) {
	close();
	input = fopen(fileName, "rb");
	if (!input) {
		throw std::runtime_error(strerror(errno));
	}
}

void LuaOpusEncoder::rewind() const {
	if (!input) {
		throw std::runtime_error(errorNoFile);
	}
	std::rewind(input);
}

sol::object LuaOpusEncoder::encodeFrame(sol::this_state s) const {
	if (!input) {
		throw std::runtime_error(errorNoFile);
	}

	sol::state_view lua(s);

	opus_int16 pcm[frameSize];
	uint8_t inputBytes[frameSize * sizeof(opus_int16)] = {0};

	size_t samples = fread(inputBytes, sizeof(opus_int16), frameSize, input);
	if (!samples) {
		return sol::make_object(lua, sol::nil);
	}

	// Convert from little-endian
	for (int i = 0; i < frameSize; i++) {
		pcm[i] = inputBytes[sizeof(opus_int16) * i + 1] << 8 |
		         inputBytes[sizeof(opus_int16) * i];
	}

	unsigned char output[maxPacketSize];

	int length = opus_encode(encoder, pcm, frameSize, output, maxPacketSize);
	if (length < 0) {
		throw std::runtime_error(opus_strerror(length));
	}

	return sol::make_object(
	    lua, std::string(reinterpret_cast<const char*>(output), length));
}

int LuaOpusEncoder::encodeFrameStringShorts(std::string_view inputBytes,
                                            unsigned char output[]) const {
	int length = opus_encode(
	    encoder, reinterpret_cast<const opus_int16*>(inputBytes.data()),
	    frameSize, output, maxPacketSize);
	if (length < 0) {
		throw std::runtime_error(opus_strerror(length));
	}
	return length;
}

int LuaOpusEncoder::encodeFrameStringFloats(std::string_view inputBytes,
                                            unsigned char output[]) const {
	int length = opus_encode_float(
	    encoder, reinterpret_cast<const float*>(inputBytes.data()), frameSize,
	    output, maxPacketSize);
	if (length < 0) {
		throw std::runtime_error(opus_strerror(length));
	}
	return length;
}

std::string LuaOpusEncoder::encodeFrameString(
    std::string_view inputBytes) const {
	unsigned char output[maxPacketSize];

	int length;

	switch (inputBytes.size()) {
		case frameSize * sizeof(opus_int16):
			length = encodeFrameStringShorts(inputBytes, output);
			break;

		case frameSize * sizeof(float):
			length = encodeFrameStringFloats(inputBytes, output);
			break;

		default:
			throw std::invalid_argument(errorWrongLength);
			break;
	}

	return std::string(reinterpret_cast<const char*>(output), length);
}
