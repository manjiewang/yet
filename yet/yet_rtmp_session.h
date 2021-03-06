/**
 * @file   yet_rtmp_session.h
 * @author pengrl
 * @date   20190127
 *
 */

#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <asio.hpp>
#include "yet.hpp"
#include "chef_base/chef_snippet.hpp"
#include "yet_rtmp/yet_rtmp_chunk_op.h"
#include "yet_rtmp/yet_rtmp.hpp"
#include "yet_rtmp/yet_rtmp_handshake.h"

namespace yet {

enum RtmpSessionType {
  RTMP_SESSION_TYPE_UNKNOWN = 1,
  RTMP_SESSION_TYPE_PUB     = 2,
  RTMP_SESSION_TYPE_SUB     = 3
};

class RtmpSession : public std::enable_shared_from_this<RtmpSession> {
  public:
    explicit RtmpSession(asio::ip::tcp::socket socket);
    ~RtmpSession();

  public:
    typedef std::function<void(RtmpSessionPtr session)> RtmpEventCb;
    typedef std::function<void(RtmpSessionPtr session, BufferPtr buf, const RtmpHeader &header)> RtmpDataCb;

  public:
    void set_rtmp_publish_cb(RtmpEventCb cb);
    void set_rtmp_play_cb(RtmpEventCb cb);
    void set_rtmp_publish_stop_cb(RtmpEventCb cb);
    void set_rtmp_session_close(RtmpEventCb cb);
    void set_rtmp_data_cb(RtmpDataCb cb);

    void start();

    void dispose() {}

  public:
    void async_send(BufferPtr buf);

  private:
    void close();

  private:
    void do_read_c0c1();
    void read_c0c1_cb(ErrorCode ec, std::size_t len);
    void do_write_s0s1();
    void write_s0s1_cb(ErrorCode ec, std::size_t len);
    void do_read_c2();
    void do_write_s2();
    void write_s2_cb(ErrorCode ec, std::size_t len);
    void read_c2_cb(ErrorCode ec, std::size_t len);
    void do_read();
    void read_cb(ErrorCode ec, std::size_t len);
    void do_write_win_ack_size();
    void write_win_ack_size_cb(ErrorCode ec, std::size_t len);
    void do_write_peer_bandwidth();
    void write_peer_bandwidth_cb(ErrorCode ec, std::size_t len);
    void do_write_chunk_size();
    void write_chunk_size_cb(ErrorCode ec, std::size_t len);
    void do_write_connect_result();
    void write_connect_result_cb(ErrorCode ec, std::size_t len);
    void do_write_create_stream_result();
    void write_create_stream_result_cb(ErrorCode ec, std::size_t len);

  private:
    void do_write_on_status_publish();
    void write_on_status_publish_cb(ErrorCode ec, std::size_t len);

  private:
    void do_write_on_status_play();
    void write_on_status_play_cb(ErrorCode ec, std::size_t len);

  private:
    void complete_message_handler();

    void protocol_control_message_handler();
    void set_chunk_size_handler(int val);
    void win_ack_size_handler(int val);

    void command_message_handler();
    void connect_handler(double transaction_id, uint8_t *buf, std::size_t len);
    void create_stream_handler(double transaction_id, uint8_t *buf, std::size_t len);
    void publish_handler(double transaction_id, uint8_t *buf, std::size_t len);
    void play_handler(double transaction_id, uint8_t *buf, std::size_t len);
    void delete_stream_handler(double transaction_id, uint8_t *buf, std::size_t len);
    //void release_stream_handler(double transaction_id, uint8_t *buf, std::size_t len);
    //void fcpublish_handler(double transaction_id, uint8_t *buf, std::size_t len);
    //void fcsubscribe_handler(double transaction_id, uint8_t *buf, std::size_t len);
    //void fcunpublish_handler(double transaction_id, uint8_t *buf, std::size_t len);

    void user_control_message_handler();

    void data_message_handler();

  private:
    void av_handler();

  private:
    void do_send();
    void send_cb(const ErrorCode &ec, std::size_t len);

  private:
    RtmpStreamPtr get_or_create_stream(int csid);

  private:
    RtmpSession(const RtmpSession &) = delete;
    RtmpSession &operator=(const RtmpSession &) = delete;

  public:
    CHEF_PROPERTY(std::string, app);
    CHEF_PROPERTY(std::string, live_name);
    CHEF_PROPERTY_WITH_INIT_VALUE(RtmpSessionType, type, RTMP_SESSION_TYPE_UNKNOWN);

  public:
    CHEF_PROPERTY_WITH_INIT_VALUE(bool, has_sent_audio, false);
    CHEF_PROPERTY_WITH_INIT_VALUE(bool, has_sent_video, false);
    CHEF_PROPERTY_WITH_INIT_VALUE(bool, has_sent_key_frame, false);

  private:
    typedef std::unordered_map<int, RtmpStreamPtr> Csid2Stream;

  private:
    asio::ip::tcp::socket socket_;
    RtmpHandshake         rtmp_handshake_;
    chef::buffer          read_buf_;
    chef::buffer          write_buf_;
    Csid2Stream           csid2stream_;
    int                   curr_csid_;
    RtmpStreamPtr         curr_stream_;
    bool                  header_done_ = false;
    int                   peer_chunk_size_ = RTMP_DEFAULT_CHUNK_SIZE;
    int                   peer_win_ack_size_ = -1;
    double                create_stream_transaction_id_ = -1;
    std::queue<BufferPtr> send_buffers_;
    RtmpEventCb           rtmp_publish_cb_;
    RtmpEventCb           rtmp_play_cb_;
    RtmpEventCb           rtmp_publish_stop_cb_;
    RtmpEventCb           rtmp_session_close_cb_;
    RtmpDataCb            rtmp_data_cb_;
};

}
