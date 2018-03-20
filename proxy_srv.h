#pragma once

#ifndef PROXY_SRV_H
#define PROXY_SRV_H

#include <boost/asio.hpp>
#include <boost/asio/raw_socket_service.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/dump.hpp>

#include "tds_proto.h"

//#include <boost/program_options.hpp>

//sqlcmd -S tcp:127.0.0.1,1321 -Usa -Pmasterkey

#include <array>
#include <string>

#include <codecvt>
#include <locale>

namespace proxy {

using port_t = std::uint16_t;

constexpr auto buf_size { 8192 };

namespace bas = boost::asio;
namespace bs  = boost::system;

namespace bip = bas::ip;

using socket_t = bas::ip::tcp::socket;

using data_buf_t = std::array<uint8_t,buf_size>;

class proxy_link: public std::enable_shared_from_this<proxy_link>
{
    bas::io_context& m_io_ctx;

    socket_t m_usckt;
    socket_t m_dsckt;

    data_buf_t m_udatabuf;
    data_buf_t m_ddatabuf;

    std::mutex m_mtx;

    void close_sockets()
    {
        if (m_usckt.is_open())
            m_usckt.close();
        if (m_dsckt.is_open())
            m_dsckt.close();
    }

    void handle_dreceive(const bs::error_code& error,
                         std::size_t bytes_transferred)
    {
        if (error)
        {
            BOOST_LOG_TRIVIAL(error) <<__FUNCSIG__ << " error: " << error.message();
            close_sockets();
            return;
        }

        tds::packet pdu(m_ddatabuf.data(),bytes_transferred);
        auto* query_pdu = pdu.find_pdu<tds::query_pdu>();

        if (query_pdu != nullptr) {
            auto text = query_pdu->text();
            //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            //std::string str = converter.to_bytes(text);

            BOOST_LOG_TRIVIAL(error) << text;
        }

        BOOST_LOG_TRIVIAL(info) << "\t\t\t\t\tReceived data from downstream link";

        BOOST_LOG_TRIVIAL(info) << boost::log::dump(m_ddatabuf.data(),bytes_transferred);

        bas::async_write(m_usckt,
                          boost::asio::buffer(m_ddatabuf.data(),bytes_transferred),
                          boost::bind(&proxy_link::handle_usend,
                                shared_from_this(),
                                boost::asio::placeholders::error));

        m_dsckt.async_read_some(
                    boost::asio::buffer(m_ddatabuf.data(),m_ddatabuf.size()),
                    boost::bind(&proxy_link::handle_dreceive,
                                shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    }

    void handle_dsend(const bs::error_code& error)
    {
        if (error)
        {
            BOOST_LOG_TRIVIAL(error) <<__FUNCSIG__ << " error: " << error.message();
            close_sockets();
            return;
        }

        BOOST_LOG_TRIVIAL(info) << "\t\t\t\t\tDownstream send was successful";
    }

    void handle_ureceive(const bs::error_code& error,
                         std::size_t bytes_transferred)
    {
        if (error)
        {
            BOOST_LOG_TRIVIAL(error) <<__FUNCSIG__ << " error: " << error.message();
            close_sockets();
            return;
        }

        BOOST_LOG_TRIVIAL(info) << "\t\t\t\t\tReceived data from upstream link";

        BOOST_LOG_TRIVIAL(info) << boost::log::dump(m_udatabuf.data(),bytes_transferred);

        bas::async_write(m_dsckt,
                         boost::asio::buffer(m_udatabuf.data(),bytes_transferred),
                         boost::bind(&proxy_link::handle_dsend,
                              shared_from_this(),
                              boost::asio::placeholders::error));

        m_usckt.async_read_some(
                    boost::asio::buffer(m_udatabuf.data(),m_udatabuf.size()),
                    boost::bind(&proxy_link::handle_ureceive,
                                shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    }

    void handle_usend(const bs::error_code& error)
    {
        if (error)
        {
            BOOST_LOG_TRIVIAL(error) <<__FUNCSIG__ << " error: " << error.message();
            close_sockets();
            return;
        }

        BOOST_LOG_TRIVIAL(info) << "\t\t\t\t\tUpstream send was successful";
    }

    void handle_uaddr_connect(const boost::system::error_code& error)
    {
        if (error)
        {
            std::stringstream os;
            os << __FUNCSIG__ << " error: " << error.message();
            throw std::runtime_error(os.str());
        }

        BOOST_LOG_TRIVIAL(info) << "\t\t\tConnection with upstream host was established";

        BOOST_LOG_TRIVIAL(info) << "\t\t\t\tStart listening downstream socket";

        m_dsckt.async_read_some(
                    boost::asio::buffer(m_ddatabuf.data(),m_ddatabuf.size()),
                    boost::bind(&proxy_link::handle_dreceive,
                                shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));


        BOOST_LOG_TRIVIAL(info) << "\t\t\t\tStart listening upstream socket";

        m_usckt.async_read_some(
                    boost::asio::buffer(m_udatabuf.data(),m_udatabuf.size()),
                    boost::bind(&proxy_link::handle_ureceive,
                                shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    }
public:
    using pointer = std::shared_ptr<proxy_link>;

    proxy_link(bas::io_context& _io_ctx)
        : m_io_ctx{_io_ctx}
        , m_dsckt{m_io_ctx}
        , m_usckt{m_io_ctx}
    {
    }

    ~proxy_link() {}

    socket_t& dsckt() { return m_dsckt; }
    socket_t& usckt() { return m_usckt; }

    void connect(const boost::asio::ip::address& _uaddr, port_t _uport)
    {
        BOOST_LOG_TRIVIAL(info) << "\t\tStart establishing link to upstream host";

        m_usckt.async_connect(
                    bip::tcp::endpoint(
                        _uaddr,
                        _uport),
                    boost::bind(&proxy_link::handle_uaddr_connect,
                        shared_from_this(),
                        boost::asio::placeholders::error));
    }

    static pointer create(bas::io_context& io_context)
    {
        return std::make_shared<proxy_link>(io_context);
    }
};

class proxy_srv
{
    bas::io_context&            m_io_ctx;
    bip::tcp::acceptor            m_acceptor;
    socket_t                    m_asckt;

    using link_ptr_t = std::shared_ptr<proxy_link>;

    bip::address    m_daddr;
    port_t            m_dport;

    bip::address    m_uaddr;
    port_t            m_uport;

    void handle_accept(proxy_link::pointer new_link, const boost::system::error_code& error)
    {
        if (error)
        {
            std::stringstream os;
            os << __FUNCSIG__ << " error: " << error.message();
            throw std::runtime_error(os.str());
        }

        BOOST_LOG_TRIVIAL(info) << "\tDownstream connection was accepted";

        new_link->connect(m_uaddr, m_uport);

        start_accept();
    }
public:
    void start_accept()
    {
        auto new_link = proxy_link::create(m_acceptor.get_executor().context());

        BOOST_LOG_TRIVIAL(info) << "Start accepting downstream connections";

        m_acceptor.async_accept(new_link->dsckt(), boost::bind(&proxy_srv::handle_accept, this, new_link, bas::placeholders::error));
    }

    proxy_srv(bas::io_context& _io_ctx, const std::string& daddr, port_t dport, const std::string& uaddr, port_t uport)
        : m_io_ctx {_io_ctx}
        , m_uaddr {bip::address::from_string(uaddr)}
        , m_uport {uport}
        , m_asckt {m_io_ctx}
        , m_acceptor{m_io_ctx,
                     bip::tcp::endpoint(
                         bip::address::from_string(daddr),
                         dport)}
    {
        start_accept();
    }

};
}

#endif /* PROXY_SRV_H */
