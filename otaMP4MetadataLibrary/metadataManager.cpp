#include "metadataManager.h"

struct MetadataInfo {
    double pts_time;
    std::vector<uint8_t> metadata;
    MetadataInfo(uint8_t* data, int size, double time) : metadata(data, data + size), pts_time(time) {}
};

std::vector<MetadataInfo> allFramMetaData;

//private function
bool areAlmostEqualRelative(double a, double b);

OTAMP4METADATALIB_API uint32_t loadMetadata(const char* input) {
    // Pre process
    AVFormatContext* inputFmtContxt = NULL;
    AVStream* in_stream = NULL;
    AVRational metadata_timebase;
    int ret = 0, data_stream_index = 0;
    int64_t frame_cnt = 0;
    ret = avformat_open_input(&inputFmtContxt, input, NULL, NULL);
    if (ret < 0) {
        std::cout << "Could not open input video" << std::endl;
    }
    ret = avformat_find_stream_info(inputFmtContxt, NULL);
    if (ret < 0) {
        std::cerr << "Could not find the stream info" << std::endl;
    }
    for (int i = 0; i < (int)inputFmtContxt->nb_streams; ++i) {
        in_stream = inputFmtContxt->streams[i];
        AVCodecParameters* in_par = in_stream->codecpar;
        if (in_par->codec_type == AVMEDIA_TYPE_DATA) {
            data_stream_index = i;
            metadata_timebase = in_stream->time_base;
            frame_cnt = in_stream->nb_frames;
        }
    }
    if (in_stream == NULL) {
        return 0;
    }

    //Init process
    int actual_frame_cnt = 0, res = 0;
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    while (true) {
        ret = av_read_frame(inputFmtContxt, packet);
        if (ret < 0) {
            break;
        }
        if (packet->stream_index == data_stream_index) {
            //just in case
            actual_frame_cnt++;
            //size
            int size = packet->size - 8;
            //pts_time
            double pts_time = packet->pts * av_q2d(metadata_timebase);
            //duration
            //double duration_time = packet->duration * av_q2d(metadata_timebase);
            //data
            uint8_t* data = new uint8_t[size];
            memcpy(data, packet->data + 8, size);
            //MetadataInfo作成
            MetadataInfo metadata(data, size, pts_time);
            delete[] data;
            //add Vector
            allFramMetaData.push_back(metadata);
        }
    }
    av_packet_free(&packet);
    av_frame_free(&frame);
    avformat_free_context(inputFmtContxt);
    return frame_cnt;
}

OTAMP4METADATALIB_API uint32_t getBufferSize() {
    uint32_t maxSize = 0;
    for (MetadataInfo metadatainfo : allFramMetaData) {
        if (metadatainfo.metadata.size() <= std::numeric_limits<uint32_t>::max()) {
            maxSize = std::max((uint32_t)metadatainfo.metadata.size(), maxSize);
        }
        else {
            std::cerr << "size_t value too large to fit into uint32_t" << std::endl;
            return 0;
        }
    }
    return maxSize;
}

OTAMP4METADATALIB_API uint32_t peekMetadata(double time, uint8_t*& data) {
    for (MetadataInfo metadatainfo : allFramMetaData) {
        if (areAlmostEqualRelative(time, metadatainfo.pts_time)) {
            uint32_t length = (uint32_t)metadatainfo.metadata.size();
            data = new uint8_t[length];
            memcpy(data, metadatainfo.metadata.data(), metadatainfo.metadata.size());
            return length;
        }
    }
    return 0;
}

static bool areAlmostEqualRelative(double a, double b) {
    double diff = std::fabs(a - b);
    double maxAB = std::max(std::fabs(a), std::fabs(b));
    return diff <= maxAB * std::numeric_limits<double>::epsilon();
}

