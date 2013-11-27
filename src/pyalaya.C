#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/shared_ptr.hpp>

#include "alaya/alaya.H"
#include "alaya/log.H"
#include "alaya/table.H"

namespace Alaya {

using boost::python::def;
using boost::python::class_;
using boost::python::init;
using boost::python::no_init;
using boost::python::return_value_policy;
using boost::python::return_by_value;
using boost::python::optional;
using boost::shared_ptr;

static void log_info(Log &log, const string &msg) {
    log.log(0, msg);
}

static void log_full(Log &log, int severity, const string &msg) {
    log.log(severity, msg);
}

static shared_ptr<Log> create_log(const string &title) {
    return Log::create(title);
}
static shared_ptr<Table> create_table(const string &title) {
    return Table::create(title);
}

BOOST_PYTHON_MODULE(_pyalaya)
{
    if (PyErr_Occurred()) return;

    class_< StringMap, shared_ptr<StringMap> >("StringMap")
        .def(boost::python::map_indexing_suite<StringMap, true>())
    ;
    class_< std::vector<std::string> >("StringList")
        .def(boost::python::vector_indexing_suite<std::vector<std::string> >())
    ;

    def("url", &url);

    class_< Log, shared_ptr<Log> >("Log", no_init)
        .def("create", &create_log)
        .staticmethod("create")
        .def("log", &log_info)
        .def("log", &log_full)
    ; 
    class_< Table, shared_ptr<Table> >("Table", no_init)
        .def("create", &create_table)
        .staticmethod("create")
        .def("set_headers", &Table::set_headers)
        .def("update", &Table::update)
        .def("remove", &Table::remove)
    ; 
}

} // namespace
