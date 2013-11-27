import warnings

# ignore warnings for multiple to-Python converters.
warnings.filterwarnings("ignore", "to-Python converter", RuntimeWarning)

from _pyalaya import *

def add_extras():
    global Log, Table

    old_log = Log
    def new_log(title = ""):
        return old_log.create(title)
    Log = new_log

    old_table_update = Table.update
    def new_table_update(self, key, pairs):
        m = StringMap()
        for (k,v) in pairs.iteritems(): m[str(k)] = str(v) 
        return old_table_update(self, str(key), m)
    Table.update = new_table_update

    old_table = Table
    def new_table(title = "", headers = []):
        t = old_table.create(title)
        _headers = StringList()
        for header in headers: _headers.append(header)
        t.set_headers(_headers)
        return t
    Table = new_table

add_extras()
