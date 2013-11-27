#include <boost/foreach.hpp>

#include "alaya/table.H"

namespace Alaya {

// static
shared_ptr<Table> Table::create(const string &title) {
    shared_ptr<Table> table(new Table(title));
    _add(table);
    return table;
}
void Table::set_headers(const std::vector<string> &arg_headers) {
    _Guard guard();
    this->headers = arg_headers;
    this->headers_set.clear();
    BOOST_FOREACH(const string &header, this->headers) {
        this->headers_set.insert(header);
    }
}
void Table::update(const string &key, const StringMap &pairs) {
    _Guard guard();
    StringMap &row = this->data[key];
    for (StringMap::const_iterator i = pairs.begin() ; i != pairs.end() ; i++) {
        if (this->headers_set.find(i->first) == this->headers_set.end())
            throw std::runtime_error("Unknown table column: " + i->first);
        row[i->first] = i->second;
    }
}
void Table::remove(const string &key) {
    _Guard guard();
    this->data.erase(key);
}
void Table::_html(std::stringstream &s) {
    s << "<h2>Table: " << this->title << " (" << this->data.size() << ")</h2>";
    s << "<table>";
    s << "<tr><th>Key</th>";
    BOOST_FOREACH(const string &header, this->headers) {
        s << "<th>" << header << "</th>";
    }
    s << "</tr>";
    for (data_t::iterator i = this->data.begin() ; i != this->data.end() ; i++) {
        s << "<tr>";
        s << "<td><b>" << i->first << "</b></td>";
        BOOST_FOREACH(const string &header, this->headers) {
            s << "<td>" << i->second[header] << "</td>";
        }
        s << "</tr>";
    }
    s << "</table>";
}

// private
Table::Table(const string &title) : title(title) {}

}  // namespace
