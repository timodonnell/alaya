#include "alaya/log.H"

namespace Alaya {

// static
shared_ptr<Log> Log::create(const string &title, int64_t max_size) {
    shared_ptr<Log> log(new Log(title, max_size));
    _add(log);
    return log;
}
void Log::log(const string &msg) {
    this->log(0, msg);
}
void Log::log(int severity, const string &msg) {
    _Guard guard();
    this->total_logged++;
    LogRecord record = {severity, msg, time(NULL)};
    this->data.push_front(record);
    if (this->data.size() > (unsigned)this->max_size)
        this->data.pop_back();
}
void Log::_html(std::stringstream &s) {
    s << "<h2>Log: " << this->title
      << " (" << this->data.size() << " of " << this->total_logged << ")</h2>"
      << "<table style=\"min-width: 50%; \">";
    std::deque<LogRecord>::iterator i;
    std::vector<char> buf(32);
    for (i = this->data.begin() ; i < this->data.end(); i++) {
        ctime_r(&i->time, buf.data());
        s << "<tr>"
          << "<td>" << buf.data() << "</td>"
          << "<td>" << i->msg << "</td>"
          << "</tr>";
    }
    s << "</table>";
}

Log::Log(const string &title, int64_t max_size) :
    title(title),
    max_size(max_size),
    total_logged(0)
{}

}  // namespace
