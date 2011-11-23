#ifndef COCAINE_DRIVERS_LSD_SERVER_HPP
#define COCAINE_DRIVERS_LSD_SERVER_HPP

#include "cocaine/drivers/server+zmq.hpp"

namespace cocaine { namespace engine { namespace drivers {

class lsd_server_t;

class lsd_job_t:
    public zmq_job_t
{
    public:
        lsd_job_t(lsd_server_t* server, const lines::route_t& route);

        virtual void send(error_code code, const std::string& error);
        
        inline zmq::message_t& envelope() {
            return m_envelope;
        }

    private:
        zmq::message_t m_envelope;
};

class lsd_server_t:
    public zmq_server_t
{
    public:
        lsd_server_t(engine_t* engine,
                     const std::string& method, 
                     const Json::Value& args);

        // Driver interface
        virtual Json::Value info() const;

        // Server interface
        virtual void process(ev::idle&, int);
        virtual void send(zmq_job_t* job, zmq::message_t& chunk);
};

}}}

#endif