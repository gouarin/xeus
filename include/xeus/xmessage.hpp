/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XEUS_MESSAGE_HPP
#define XEUS_MESSAGE_HPP

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "xauthentication.hpp"
#include "xeus.hpp"
#include "zmq_addon.hpp"

namespace nl = nlohmann;

namespace xeus
{
    using buffer_sequence = std::vector<zmq::message_t>;

    class XEUS_API xmessage_base
    {
    public:

        xmessage_base(const xmessage_base&) = delete;
        xmessage_base& operator=(const xmessage_base&) = delete;

        const nl::json& header() const;
        const nl::json& parent_header() const;
        const nl::json& metadata() const;
        const nl::json& content() const;
        const buffer_sequence& buffers() const;

    protected:

        xmessage_base() = default;
        xmessage_base(nl::json header,
                      nl::json parent_header,
                      nl::json metadata,
                      nl::json content,
                      buffer_sequence buffers);

        ~xmessage_base() = default;

        xmessage_base(xmessage_base&&) = default;
        xmessage_base& operator=(xmessage_base&&) = default;

        bool is_delimiter(zmq::message_t& frame) const;
        void deserialize(zmq::multipart_t& wire_msg, const xauthentication& auth);
        void serialize(zmq::multipart_t& wire_msg, const xauthentication& auth) &&;

        static const std::string DELIMITER;

    private:

        nl::json m_header;
        nl::json m_parent_header;
        nl::json m_metadata;
        nl::json m_content;
        buffer_sequence m_buffers;
    };

    class XEUS_API xmessage : public xmessage_base
    {
    public:

        using base_type = xmessage_base;
        using guid_list = std::vector<std::string>;

        xmessage() = default;
        xmessage(const guid_list& zmq_id,
                 nl::json header,
                 nl::json parent_header,
                 nl::json metadata,
                 nl::json content,
                 buffer_sequence buffers);

        ~xmessage() = default;

        xmessage(xmessage&&) = default;
        xmessage& operator=(xmessage&&) = default;

        xmessage(const xmessage&) = delete;
        xmessage& operator=(const xmessage&) = delete;

        void deserialize(zmq::multipart_t& wire_msg, const xauthentication& auth);
        void serialize(zmq::multipart_t& wire_msg, const xauthentication& auth) &&;

        const guid_list& identities() const;

    private:

        guid_list m_zmq_id;
    };

    class XEUS_API xpub_message : public xmessage_base
    {
    public:

        using base_type = xmessage_base;

        xpub_message() = default;
        xpub_message(const std::string& topic,
                     nl::json header,
                     nl::json parent_header,
                     nl::json metadata,
                     nl::json content,
                     buffer_sequence buffers);

        ~xpub_message() = default;

        xpub_message(xpub_message&&) = default;
        xpub_message& operator=(xpub_message&&) = default;

        xpub_message(const xpub_message&) = delete;
        xpub_message& operator=(const xpub_message&) = delete;

        void deserialize(zmq::multipart_t& wire_msg, const xauthentication& auth);
        void serialize(zmq::multipart_t& wire_msg, const xauthentication& auth) &&;

        const std::string& topic() const;

    private:

        std::string m_topic;
    };

    XEUS_API std::string iso8601_now();

    XEUS_API std::string get_protocol_version();

    XEUS_API nl::json make_header(const std::string& msg_type,
                               const std::string& user_name,
                               const std::string& session_id);
}

#endif
