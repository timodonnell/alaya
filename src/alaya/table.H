#ifndef _TABLE_H__
#define _TABLE_H__

#include <string>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include "alaya.H"

namespace Alaya {

using std::string;
using boost::shared_ptr;

typedef boost::unordered_map<string, string> StringMap;

class Table : public _Widget {
  public:
    static shared_ptr<Table> create(const string &title);
    void set_headers(const std::vector<string> &headers);
    void update(const string &key, const StringMap &pairs);
    void remove(const string &key);

    void _html(std::stringstream &s);

  private:
    Table(const string &title);
    string title;
    std::vector<string> headers;
    boost::unordered_set<string> headers_set;

    typedef boost::unordered_map<string, StringMap> data_t;
    data_t data;
};

}  // namespace

#endif
