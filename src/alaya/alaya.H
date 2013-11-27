#ifndef _ALAYA_H__
#define _ALAYA_H__

#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace Alaya {

using std::string;
using boost::shared_ptr;

string url();

class _Guard {
  public:
    _Guard();
  private:
    boost::lock_guard<boost::recursive_mutex> guard;
};

class _Widget {
  public:
    virtual void _html(std::stringstream &s) = 0;
    virtual ~_Widget() = 0;
};

void _add(shared_ptr<_Widget> widget);


}  // namespace


#endif
