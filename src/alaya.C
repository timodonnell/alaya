#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <vector>

#include <boost/foreach.hpp>
#include <glog/logging.h>

#include "alaya/mongoose.h"
#include "alaya/alaya.H"
#include "alaya/log.H"

namespace Alaya {

struct AlayaGlogSink : public google::LogSink {
    void send(google::LogSeverity severity, const char* full_filename,
              const char* base_filename, int line,
              const struct ::tm* tm_time,
              const char* message, size_t message_len);
};
struct AlayaSingleton {
    AlayaSingleton();
    void start();

    bool started;
    string url;
    boost::recursive_mutex lock;
    std::vector< shared_ptr<_Widget> > widgets;
    struct mg_context* mongoose_context;
    shared_ptr<Log> default_log;
    AlayaGlogSink alaya_glog_sink;

};
static AlayaSingleton SINGLETON;

_Widget::~_Widget() {}

AlayaSingleton::AlayaSingleton() :
        started(false),
        url("not-started"),
        widgets(),
        mongoose_context(NULL),
        default_log(),
        alaya_glog_sink()
{}

static void* handler(enum mg_event event,
              struct mg_connection* connection, 
              const struct mg_request_info* request_info);
void AlayaSingleton::start() {
    _Guard guard();
    if (!this->started) {
        // start mongoose server
        this->started = true;
        const char *options[] = {
            "listening_ports", "0",
            NULL
        };
        this->mongoose_context = mg_start(handler, NULL, options);
        CHECK_NOTNULL(this->mongoose_context);

        // find url
        int sock = get_socket(this->mongoose_context);
        CHECK_GT(sock, 0);
        struct sockaddr_in sa;
        socklen_t sa_len = sizeof(sa);
        PCHECK(getsockname(sock, (sockaddr*)&sa, &sa_len) >= 0);
        char hostname[256];
        PCHECK(gethostname(hostname, 255) >= 0);
        hostname[255] = '\0';
        std::stringstream stream;
        stream << "http://" << hostname
               << ":" << (int)ntohs(sa.sin_port);
        this->url = stream.str();

        // add default log.
        shared_ptr<Log> log(new Log("GLOG", 1000));
        this->default_log = log;
        this->widgets.push_back(log);

        // start glog integration
        google::AddLogSink(&this->alaya_glog_sink);
    }
}

_Guard::_Guard() : guard(SINGLETON.lock) {}

string url() {
    SINGLETON.start();  // no op if already started.
    return SINGLETON.url;
}

void _add(shared_ptr<_Widget> widget) {
    _Guard guard();
    SINGLETON.widgets.push_back(widget);
}

static void make_page(std::stringstream &s);
static void* handler(enum mg_event event,
              struct mg_connection* connection, 
              const struct mg_request_info* request_info) {
    if (event == MG_NEW_REQUEST) {
        std::stringstream page;
        page << "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n\r\n";
        make_page(page);
        string page_str = page.str();
        mg_write(connection, page_str.c_str(), page_str.size());
        return (void*)1;  // Mark as processed
    } else {
        return NULL;
    }
}

static void add_css(std::stringstream &s);
static void make_page(std::stringstream &s) {
    _Guard guard();

    pid_t pid = getpid();
    char hostname[256];
    PCHECK(gethostname(hostname, 255) >= 0);
    hostname[255] = '\0';
    s << "<html>"
      << "<head>"
      << "<title>Alaya on " << hostname << ", pid " << pid << "</title>"
      << "<style>";
    add_css(s);
    s << "</style>"
      << "</head>"
      << "<body>"
      << "<h1>Alaya on " << hostname << ", pid " << pid << "</h1>";

    BOOST_FOREACH(shared_ptr<_Widget> widget, SINGLETON.widgets) {
        s << "<div style=\"max-height: 20em; min-width: 50%; overflow: auto; \">";
        widget->_html(s);
        s << "</div><br />";
    }

    s << "</body>"
      << "</html>";
}

static void add_css(std::stringstream &s) {
    s <<
    "body {"
    "    font-family: sans-serif;"
    "}"
    ""
    "table {"
    "    border: 1px black solid;"
    "    border-spacing: 2px;"
    "    border-collapse: collapse;"
    "    background-color: white;"
    "}"
    "table th {"
    "    padding: 3px 3px 3px 3px;"
    "    border: 1px gray solid;"
    "    background-color: white;"
    "    -moz-border-radius: 0px 0px 0px 0px;"
    "}"
    "table td {"
    "    border-width: 1px 1px 1px 1px;"
    "    padding: 3px 3px 3px 3px;"
    "    border-style: solid solid solid solid;"
    "    border-color: gray gray gray gray;"
    "    background-color: white;"
    "    -moz-border-radius: 0px 0px 0px 0px;"
    "}"
    "div.tasks {"
    "    padding: 0px 10px 10px 10px; "
    "    border: 1px black solid;"
    "    background-color: #FFFFF6;"
    "    max-height: 12em;"
    "    overflow: auto;"
    "    margin: 1em;"
    "}"
    "div.tasks table {"
    "    margin: 1em;"
    "}"
    "div.indent {"
    "    margin-left: 1em;"
    "}"
    ""
    "table.query {"
    "    background: azure;"
    "}"
    "table.query td {"
    "    background: azure;"
    "    border: none;"
    "    text-align: left;"
    "    vertical-align: top;"
    "}"
    "table.query td h2 {"
    "    margin-bottom: 0px;"
    "}";
}

void AlayaGlogSink::send(google::LogSeverity severity, const char* full_filename,
              const char* base_filename, int line,
              const struct ::tm* tm_time,
              const char* message, size_t message_len) {

    string text = google::LogSink::ToString(severity, full_filename, line,
                                            tm_time, message, message_len);
    SINGLETON.default_log->log(severity, text);
}

}  // namespace


