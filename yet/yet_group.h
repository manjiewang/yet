/**
 * @file   yet_group.h
 * @author pengrl
 * @date   20190127
 *
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "yet.hpp"
#include "yet_http_flv/yet_http_flv_buffer_t.hpp"
#include "yet_rtmp/yet_rtmp_chunk_op.h"

namespace yet {

class Group : public std::enable_shared_from_this<Group> {
  public:
    explicit Group(const std::string &live_name);
    ~Group();

    void dispose();

    void set_http_flv_pull(HttpFlvPullPtr pull);
    void reset_http_flv_pull();
    HttpFlvPullPtr get_http_flv_pull();

    void add_http_flv_sub(HttpFlvSubPtr sub);
    void del_http_flv_sub(HttpFlvSubPtr sub);

    void set_rtmp_pub(RtmpSessionPtr pub);
    void reset_rtmp_pub();

    void add_rtmp_sub(RtmpSessionPtr sub);
    void del_rtmp_sub(RtmpSessionPtr sub);

    BufferPtr get_metadata();
    BufferPtr get_video_seq_header();

    void on_rtmp_publish();
    void on_rtmp_publish_stop();

  public:
    void on_http_flv_pull_connected();
    void on_http_flv_data(BufferPtr buf, const std::vector<FlvTagInfo> &tis);

  public:
    void on_rtmp_data(RtmpSessionPtr pub, BufferPtr msg, const RtmpHeader &h);
    void on_rtmp_session_close(RtmpSessionPtr session);

  private:
    void cache_avc_header(BufferPtr msg, const RtmpHeader &h);
    void cache_aac_header(BufferPtr msg, const RtmpHeader &h);

  private:
    Group(const Group &) = delete;
    Group &operator=(const Group &) = delete;

  private:
    const std::string                  live_name_;
    HttpFlvPullPtr                     http_flv_pull_;
    RtmpSessionPtr                     rtmp_pub_;
    std::unordered_set<HttpFlvSubPtr>  http_flv_subs_;
    std::unordered_set<RtmpSessionPtr> rtmp_subs_;
    RtmpHeader                         *prev_audio_header_=nullptr;
    RtmpHeader                         *prev_video_header_=nullptr;
    BufferPtr                          avc_header_;
    BufferPtr                          aac_header_;
};

}
