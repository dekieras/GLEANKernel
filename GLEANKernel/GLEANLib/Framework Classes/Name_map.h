#ifndef NAME_MAP_H
#define NAME_MAP_H

#include "Symbol.h"

#include <map>


// A name map associates physical object names with psychological object names.
// Looking up the name is used to finesse object identity problems for input, 
// and to allow output to pass a physical name to the device for simplicity
// (e.g. when something is pointed to)

// This encapsulated class avoids duplicate functionality and identifies this
// finesse explicitly, and allows it to be repositioned in the architecture as needed.


class Name_map {
public:
	// add a name pair
	void add_names(const Symbol& physical_name, const Symbol& psychological_name);
	// the physical name is the key, the psychological name is the value
	Symbol get_psychological_name(const Symbol& physical_name) const;
	// the psychological name is the key, the physical name is the value
	Symbol get_physical_name(const Symbol& psychological_name) const;
	// replace the physical name for the psychologically-named object, do not check for duplicate physical name values,
	// and overwrite any previous physical_name value.
	void replace_physical_name_for_psychological_name(const Symbol& physical_name, const Symbol& psychological_name);
	// remove both names, given one  of them
	void remove_names_with_psychological_name(const Symbol& psychological_name);
	void remove_names_with_physical_name(const Symbol& physical_name);
	void clear();

private:
	typedef std::map<Symbol, Symbol> Symbol_map_t;
	// the physical name is the key, the psychological name is the value
	Symbol_map_t psychological_names;
	// the psychological name is the key, the physical name is the value
	Symbol_map_t physical_names;
};

#endif
