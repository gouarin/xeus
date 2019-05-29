#include "xeus/xmessage.hpp"
#include "xeus/xguid.hpp"
#include "xeus/xauthentication.hpp"
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "nlohmann/json.hpp"

namespace nl = nlohmann;

int main()
{
    zmq::context_t context;
    zmq::socket_t talk_controller(context, ZMQ_REQ);;

    auto auth = xeus::make_xauthentication("hmac-sha256", "b285276259d69edc3834ecc73a10e208");
    auto guid = xeus::new_xguid();

    talk_controller.connect("tcp://localhost:51473");

    xeus::xmessage msg = {{guid},
                          xeus::make_header("shutdown_request", "", ""),
                          nl::json::object(),
                          nl::json::object(),
                          nl::json::object(),
                          xeus::buffer_sequence()
    };
    zmq::multipart_t start_msg;
    std::move(msg).serialize(start_msg, *auth);
    start_msg.send(talk_controller);

    return 0;
}
