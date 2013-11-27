#ifndef _LOG_H__
#define _LOG_H__

#include <deque>
#include <string>

#include <boost/shared_ptr.hpp>

#include "alaya.H"

namespace Alaya {

using std::string;
using boost::shared_ptr;

struct LogRecord {
    int severity;
    string msg;
    time_t time;
};

class Log : public _Widget {
  public:
    Log(const string &title, int64_t max_size);
    static shared_ptr<Log> create(const string &title, int64_t max_size = 1000);
    void log(const string &msg);
    void log(int severity, const string &msg);
    
    void _html(std::stringstream &s);

  private:
    string title;
    std::deque<LogRecord> data;
    int64_t max_size;
    int64_t total_logged;
};

}  // namespace

#endif
