/**
 * @file   yet.hpp
 * @author pengrl
 * @date   initial 20190127
 *
 */

#pragma once

#include <cinttypes>
#include <cstddef>
#include "yet_common/yet_common.hpp"
#include "yet_common/yet_log.h"

// config
namespace yet {

static constexpr std::size_t BUF_INIT_LEN_HTTP_FLV_PULL_EACH_READ   = 16384;
static constexpr std::size_t BUF_SHRINK_LEN_HTTP_FLV_PULL_EACH_READ = 2147483647;
static constexpr std::size_t BUF_INIT_LEN_RTMP_EACH_READ            = 16384;
static constexpr std::size_t BUF_SHRINK_LEN_RTMP_EACH_READ          = 2147483647;
// @NOTICE len of rtmp-write only for non-av data
static constexpr std::size_t BUF_INIT_LEN_RTMP_WRITE                = 4096;
static constexpr std::size_t BUF_SHRINK_LEN_RTMP_WRITE              = 2147483647;
static constexpr std::size_t BUF_INIT_LEN_RTMP_COMPLETE_MESSAGE     = 16384;
static constexpr std::size_t BUF_SHRINK_LEN_RTMP_COMPLETE_MESSAGE   = 2147483647;

}

// const
namespace yet {

class Server;
class RtmpServer;
class HttpFlvServer;
class Group;
class HttpFlvSub;
class HttpFlvPull;
class RtmpSession;
struct RtmpStream;
typedef std::shared_ptr<Server> ServerPtr;
typedef std::shared_ptr<RtmpServer> RtmpServerPtr;
typedef std::shared_ptr<HttpFlvServer> HttpFlvServerPtr;
typedef std::shared_ptr<Group> GroupPtr;
typedef std::shared_ptr<HttpFlvSub> HttpFlvSubPtr;
typedef std::shared_ptr<HttpFlvPull> HttpFlvPullPtr;
typedef std::shared_ptr<RtmpSession> RtmpSessionPtr;
typedef std::shared_ptr<RtmpStream> RtmpStreamPtr;

}
